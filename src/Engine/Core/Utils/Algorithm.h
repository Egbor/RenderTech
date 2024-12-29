#ifndef ENGINE_ALGORITHM_H
#define ENGINE_ALGORITHM_H

#include "Engine/EngineDef.h"

#include <algorithm>
#include <locale>

namespace Engine {
	inline String ToLowerCopy(const String& value, const std::locale& loc = std::locale()) {
		auto const& facet = std::use_facet<std::ctype<char>>(loc);

		String out;
		out.reserve(value.size());

		std::transform(value.begin(), value.end(), std::back_inserter(out),
			[&facet](unsigned char c) { return facet.tolower(c); });

		return out;
	}
}

#endif // !ENGINE_ALGORITHM_H
