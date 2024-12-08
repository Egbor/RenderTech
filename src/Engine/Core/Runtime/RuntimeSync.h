#ifndef RUNTIME_SYNC_H
#define RUNTIME_SYNC_H

#include "Engine/Core/Utils/Event.h"

#include <mutex>
#include <atomic>

namespace Engine {
	class RuntimeProcess;

	enum class RuntimeState {
		RPS_PROGRESS,
		RPS_PAUSE,
		RPS_TERMINATE,
	};

	class RuntimeSync {
		using ThreadSafetyRuntimeState = std::atomic<RuntimeState>;
		using ThreadSafetyRuntimeSyncFlags = std::atomic<Int32>;

	public: 
		static constexpr Size MAX_PROCESSES_NUMBER = sizeof(ThreadSafetyRuntimeSyncFlags) * 8;

		RuntimeSync();
		~RuntimeSync() = default;

		RuntimeState SyncProcess(const RuntimeProcess* process);
		void RegisterProcess(const RuntimeProcess* process);
		void RegisterSyncCallback(Callable<void()>* callback);
		void Set(RuntimeState state);

	private:
		std::mutex m_mutex;
		std::condition_variable m_cv;

		Event<> m_syncEvent;

		ThreadSafetyRuntimeState m_state;
		ThreadSafetyRuntimeSyncFlags m_syncs;
		ThreadSafetyRuntimeSyncFlags m_syncsMask;
	};
}

#endif // !RUNTIME_SYNC_H
