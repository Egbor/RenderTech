#ifndef ENGINE_THREAD_H
#define ENGINE_THREAD_H

#include "Engine/Core/Utils/Event.h"

#include <thread>
#include <mutex>
#include <atomic>

namespace Engine {
	class EngineThreadSync {
	public:
		EngineThreadSync(const Int32 minThreads);
		virtual ~EngineThreadSync() = default;

		void Sync();
		void Allow();

	private:
		std::condition_variable m_cv;
		std::mutex m_mutex;

		std::atomic<Int32> m_threads;
		const Int32 m_minThreads;
	};

	class EngineThread {
	public:
		EngineThread();
		virtual ~EngineThread() = default;

		void Start(EngineThreadSync& sync, EventBase<>& callback);
		void Wait();

	private:
		std::thread m_thread;
		bool m_hasExit;
	};
}

#endif // !ENGINE_THREAD_H
