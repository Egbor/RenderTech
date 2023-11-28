#ifndef FACTORY_H
#define FACTORY_H

#include "Engine/EngineDef.h"

namespace Engine {
	template<class TKey, class TReturn, class ...TArgs>
	class Factory {
	public:
		using FactoryMethod = std::function<TReturn* (TArgs ...)>;

		Factory() = default;
		virtual ~Factory() = default;

		TReturn* Create(TKey key, TArgs ...args) {
			return m_factoryMap[key](args...);
		}

		void Register(TKey key, FactoryMethod method) {
			m_factoryMap[key] = method;
		}

		void Unresgister(TKey key) {
			m_factoryMap.erase(key);
		}

	private:
		Map<TKey, FactoryMethod> m_factoryMap;
	};
}

#endif // !FACTORY_H
