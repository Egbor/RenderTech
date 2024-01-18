#ifndef ENGINE_THREAD_H
#define ENGINE_THREAD_H

#include "Engine/Core/Utils/Event.h"
#include "Engine/Core/System/Time/Time.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include <thread>
#include <mutex>
#include <atomic>

#include <Windows.h>

namespace Engine {
	enum class EngineThreadStatus {
		ETS_Terminate,
		ETS_Progress,
		ETS_Pause,
	};

	struct EngineThreadSyncSlot {
		EngineThreadStatus status;
		bool isSynced;
	};

	class EngineThreadSync {
	public:
		EngineThreadSync(EventBase<>& callback, const Int32 syncLimit) 
			: m_syncCallback(callback), m_syncSlots() {

		}

		virtual ~EngineThreadSync() {
			EventBase<>::Free(m_syncCallback);
		}

		bool Sync(EngineThreadStatus status, Int32 slotId) {
			std::unique_lock lock(m_mutex);

			UpdateSyncSlot(slotId, status);

			if (!HasUnsyncedSlot()) {
				m_syncCallback.Invoke();
				UnsyncAllSyncSlots();
				m_cv.notify_all();
			} else {
				m_cv.wait(lock, [&]() { return !m_syncSlots[slotId].isSynced; });
			}

			return !HasProgressSlot();
		}

		Int32 ReserveSyncSlot() {
			m_syncSlots.push_back({});
			return m_syncSlots.size() - 1;
		}

	private:
		void UnsyncAllSyncSlots() {
			for (Int32 i = 0; i < m_syncSlots.size(); i++) {
				m_syncSlots[i].isSynced = false;
			}
		}

		void UpdateSyncSlot(Int32 slotId, EngineThreadStatus status) {
			m_syncSlots[slotId].status = status;
			m_syncSlots[slotId].isSynced = true;
		}

		bool HasProgressSlot() {
			for (Int32 i = 0; i < m_syncSlots.size(); i++) {
				if (m_syncSlots[i].status == EngineThreadStatus::ETS_Progress) {
					return true;
				}
			}
			return false;
		}

		bool HasUnsyncedSlot() {
			for (Int32 i = 0; i < m_syncSlots.size(); i++) {
				if (!m_syncSlots[i].isSynced) {
					return true;
				}
			}
			return false;
		}

		std::condition_variable m_cv;
		std::mutex m_mutex;

		Array<EngineThreadSyncSlot> m_syncSlots;
		EventBase<>& m_syncCallback;
	};

	class EngineThread {
	public:
		EngineThread() 
			: m_status(EngineThreadStatus::ETS_Pause) {

		}

		virtual ~EngineThread() {
			m_thread.join();
		}

		template<class ...TArgs>
		void Start(EngineThreadSync& sync, EventBase<TArgs...>& callback, TArgs... args) {
			m_thread = std::thread([&]() {
				bool isSyncExit = false;
				Int32 syncId = sync.ReserveSyncSlot();

				while (!isSyncExit) {
					if (m_status != EngineThreadStatus::ETS_Pause) {
						isSyncExit = sync.Sync(m_status.load(), syncId);
						callback.Invoke(args...);
					}
				}
				EventBase<TArgs...>::Free(callback);
			});
		}

		void Resume() {
			m_status = EngineThreadStatus::ETS_Progress;
		}

		void Exit() {
			m_status = EngineThreadStatus::ETS_Terminate;
		}

	private:
		std::thread m_thread;
		std::atomic<EngineThreadStatus> m_status;
	};

	class EngineThreadPool {
	public:
		EngineThreadPool(EventBase<>& syncCallback, const Int32 size) {
			m_sync = new EngineThreadSync(syncCallback, size);
			m_pool.reserve(size);
		}

		virtual ~EngineThreadPool() {
			Reset();
			DELETE_OBJECT(m_sync);
		}

		template<class ...TArgs>
		void Append(EventBase<TArgs...>& callback, TArgs... args) {
			if (m_pool.capacity() == m_pool.size()) {
				throw EngineException("[EngineThreadPool] Out of range");
			}

			m_pool.push_back(new EngineThread());
			m_pool[m_pool.size() - 1]->Start(*m_sync, callback, args...);
		}

		void Start() {
			for (Int32 i = 0; i < m_pool.size(); i++) {
				m_pool[i]->Resume();
			}
		}

		void Reset() {
			for (Int32 i = 0; i < m_pool.size(); i++) {
				m_pool[i]->Exit();
			}

			DELETE_ARRAY_OF_OBJECTS(m_pool);
			m_pool.resize(0);
		}

	private:
		EngineThreadSync* m_sync;
		Array<EngineThread*> m_pool;
	};
}

#endif // !ENGINE_THREAD_H
