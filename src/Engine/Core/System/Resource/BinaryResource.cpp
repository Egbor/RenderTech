#include "Engine/Core/System/Resource/Resource.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include <fstream>

namespace Engine {
	template<>
	const Array<Int8> Resource::Load(const String& filename) {
		std::ifstream in(filename, std::ios::binary);

		if (!in.is_open()) {
			throw new EngineException("[Resource] Binary file can not be opened");
		}

		in.seekg(0, std::ios::end);
		Size codeLength = in.tellg();
		in.seekg(0, std::ios::beg);

		Array<Int8> code(codeLength);
		in.read(reinterpret_cast<char*>(code.data()), codeLength);

		return code;
	}
}