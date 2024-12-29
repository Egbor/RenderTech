#ifndef RUNTIME_PROCESS_H
#define RUNTIME_PROCESS_H

#include "Engine/Core/Core.h"
#include "Engine/Core/System/Time/Time.h"
#include "Engine/Core/Runtime/RuntimeSync.h"

#include <thread>

namespace Engine {
	struct RuntimeProcessArgs {
		const Time* time;
	};

	class RuntimeProcess {
	public:
		using Callback = Callable<void(Float)>;

		RuntimeProcess(Int32 id, RuntimeSync* sync, Callback* callback, const RuntimeProcessArgs& args);
		~RuntimeProcess();

		Int32 GetId() const;

		bool operator==(const RuntimeProcess& process) const;

	private:
		void Launch(RuntimeSync* sync, Callback* callback, const RuntimeProcessArgs& args);

		std::thread m_thread;
		Int32 m_id;
	};
}

#endif // !RUNTIME_PROCESS_H
