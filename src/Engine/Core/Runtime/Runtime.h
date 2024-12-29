#ifndef RUNTIME_H
#define RUNTIME_H

#include "Engine/Core/Core.h"
#include "Engine/Core/Runtime/RuntimeProcess.h"

namespace Engine {
	class Runtime {
	public:
		Runtime(IWindow* window, IContext* context);
		virtual ~Runtime();

		void DelegateProcess(RuntimeProcess::Callback* callback);

		virtual void Start();
		virtual void Pause();
		virtual void Terminate();

	protected:
		virtual void Sync();
		void ExitSafty();

		const _Core* GetCore() const;

	private:
		Array<RuntimeProcess*> m_tasks;
		RuntimeSync* m_sync;

		Time* m_time;
		_Core* m_core;
	};
}

#endif // !RUNTIME_H
