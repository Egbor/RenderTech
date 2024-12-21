#ifndef RUNTIME_H
#define RUNTIME_H

#include "Engine/Core/Runtime/RuntimeProcess.h"

namespace Engine {
	class Runtime {
	public:
		Runtime();
		virtual ~Runtime();

		void DelegateProcess(Callable<void(Float)>* callback);

		virtual void Start();
		virtual void Pause();
		virtual void Terminate();

	protected:
		void ExitSafty();

	private:
		void Sync();

		Array<RuntimeProcess*> m_tasks;
		RuntimeSync* m_sync;

		Time* m_time;
	};
}

#endif // !RUNTIME_H
