#ifndef RUNTIME_PROCESS_H
#define RUNTIME_PROCESS_H

#include "Engine/Core/System/Time/Time.h"
#include "Engine/Core/Runtime/RuntimeSync.h"

#include <thread>

namespace Engine {
	class RuntimeProcess {
	public:
		RuntimeProcess(Int32 id, RuntimeSync* sync, const Time* time, Callable<void(Float)>* callback);
		~RuntimeProcess();

		Int32 GetId() const;

		bool operator==(const RuntimeProcess& process) const;

	private:
		void Launch(RuntimeSync* sync, const Time* time, Callable<void(Float)>* callback);

		std::thread m_thread;
		Int32 m_id;
	};
}

#endif // !RUNTIME_PROCESS_H
