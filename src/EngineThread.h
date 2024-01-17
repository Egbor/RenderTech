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
		ETS_Sync		= 0x01,
		ETS_Progress	= 0x02,
		ETS_Ready		= 0x04,
		ETS_Terminate	= 0x08,
	};

#define IS_ETS_FREEZE(status) (((status) == EngineThreadStatus::ETS_Ready) || ((status) == EngineThreadStatus::ETS_Terminate))

	class EngineThreadSync {
	public:
		EngineThreadSync(EventBase<>& callback, const Int32 syncLimit) 
			: m_syncCallback(callback), m_threadSyncLimit(syncLimit - 1) {

		}

		virtual ~EngineThreadSync() {
			EventBase<>::Free(m_syncCallback);
		}

		bool Sync(EngineThreadStatus& status) {
			std::unique_lock lock(m_mutex);

			if (!IS_ETS_FREEZE(status) && (m_syncBatch.size() >= m_threadSyncLimit)) {
				lock.unlock();

				m_syncCallback.Invoke();
				ResumeAllThreads();
			} else {
				PauseThread(lock, status);
			}

			if (status != EngineThreadStatus::ETS_Terminate) {
				status = EngineThreadStatus::ETS_Progress;
			}

			return status == EngineThreadStatus::ETS_Progress;
		}

		void Run() {
			ResumeAllThreads();
		}

	private:
		void ResumeAllThreads() {
			for (Int32 i = 0; i < m_syncBatch.size(); i++) {
				*m_syncBatch[i] = EngineThreadStatus::ETS_Sync;
			}
			m_cv.notify_all();
		}

		void PauseThread(std::unique_lock<std::mutex>& lock, EngineThreadStatus& status) {
			m_syncBatch.push_back(&status);
			m_cv.wait(lock, [&]() { return (status == EngineThreadStatus::ETS_Sync) || (status == EngineThreadStatus::ETS_Terminate); });
			m_syncBatch.pop_back();
		}

		std::condition_variable m_cv;
		std::mutex m_mutex;

		const Int32 m_threadSyncLimit;
		Array<EngineThreadStatus*> m_syncBatch;

		EventBase<>& m_syncCallback;
	};

	class EngineThread {
	public:
		EngineThread() : m_hasExit(false), m_status(EngineThreadStatus::ETS_Ready) {}
		virtual ~EngineThread() = default;

		template<class ...TArgs>
		void Start(EngineThreadSync& sync, EventBase<TArgs...>& callback, TArgs... args) {
			m_thread = std::thread([&]() {
				while (sync.Sync(m_status)) {
					callback.Invoke(args...);
				}
				EventBase<TArgs...>::Free(callback);
			});
		}

		void Exit() {
			//m_hasExit = true;
			m_status = EngineThreadStatus::ETS_Terminate;
			m_thread.join();
		}

	private:
		std::thread m_thread;
		bool m_hasExit;

		EngineThreadStatus m_status;
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
			m_sync->Run();
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
