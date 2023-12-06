#include "EngineThread.h"

namespace Engine {
	EngineThreadSync::EngineThreadSync(const Int32 minThreads)
		: m_minThreads(minThreads), m_threads(0), m_cv() {

	}

	void EngineThreadSync::Sync() {
		std::unique_lock lock(m_mutex);

		m_threads++;
		m_cv.wait(lock, [&]{ return m_threads >= m_minThreads; });
	}

	void EngineThreadSync::Allow() {
		m_cv.notify_all();
	}

	EngineThread::EngineThread() 
		: m_hasExit(false) {

	}

	void EngineThread::Start(EngineThreadSync& sync, EventBase<>& callback) {
		m_thread = std::thread([&]() {
			while (m_hasExit) {
				sync.Sync();
				callback.Invoke();
			}
		});
	}

	void EngineThread::Wait() {
		m_hasExit = true;
		m_thread.join();
	}
}