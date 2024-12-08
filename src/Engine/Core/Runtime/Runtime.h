#ifndef RUNTIME_H
#define RUNTIME_H

#include "Engine/Core/Runtime/RuntimeProcess.h"

namespace Engine {
	class Runtime {
	public:
		Runtime();
		~Runtime();

		void DelegateProcess(Callable<void(Float)>* callback);

		void Start();
		void Pause();
		void Terminate();

	private:
		void Sync();

		Array<RuntimeProcess*> m_tasks;
		RuntimeSync* m_sync;

		Time* m_time;
	};
}

#endif // !RUNTIME_H
