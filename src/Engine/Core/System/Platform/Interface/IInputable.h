#ifndef IINPUTABLE_H
#define IINPUTABLE_H

namespace Engine {
	class Input;

	class IInputable {
	public:
		virtual Input* GetInput() const = 0;
	};
}

#endif // !IINPUTABLE_H
