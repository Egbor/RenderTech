#pragma once

#include "Engine/EngineDef.h"

namespace Engine {
	//template<class ...TArgs>
	//class EventBase {
	//private:
	//	using TEventBase = EventBase<TArgs...>;

	//public:
	//	EventBase() = default;
	//	EventBase(const EventBase&) = default;
	//	~EventBase() = default;

	//	virtual void Invoke(TArgs... args) = 0;
	//	virtual bool IsEqual(const TEventBase& other) const = 0;

	//	bool operator==(const TEventBase& other) const {
	//		return IsEqual(other);
	//	}

	//	bool operator!=(const TEventBase& other) const {
	//		return !(*this == other);
	//	}

	//	static void Free(EventBase<TArgs...>& del) {
	//		EventBase<TArgs...>* temp = &del;
	//		delete temp;
	//	}
	//};

	//template<class TOwner, class ...TArgs>
	//struct Method {
	//	typedef void(TOwner::*TMethodPtr)(TArgs...);

	//	TOwner* owner;
	//	TMethodPtr ptr;

	//	Method(TOwner* _owner, TMethodPtr _ptr) : owner(_owner), ptr(_ptr) {}
	//	Method(const Method<TOwner, TArgs...>&) = default;
	//	~Method() = default;
	//};

	//template<class ...TArgs>
	//class Event {
	//private:
	//	using TEventBase = EventBase<TArgs...>;
	//	using TEventBaseIterator = typename std::list<TEventBase*>::const_iterator;

	//private:
 //       List<TEventBase*> m_delegates;

	//public:
	//	Event() : m_delegates() {}
	//	Event(const Event<TArgs...>&) = default;

	//	~Event() {
	//		for (TEventBase* handler : m_delegates) {
	//			delete handler;
	//		}
	//	}

	//	void operator()(TArgs... args) {
	//		for (TEventBase* handler : m_delegates) {
	//			handler->Invoke(args...);
	//		}
	//	}

	//	bool operator+=(TEventBase& handler) {
	//		if (FindDelegate(handler) == m_delegates.end()) {
	//			m_delegates.push_back(&handler);
	//			return true;
	//		}
	//		return false;
	//	}

	//	bool operator-=(TEventBase& handler) {
	//		auto it = FindDelegate(handler);
	//		if (it != m_delegates.end()) {
	//			TEventBase* handler = *it;
	//			m_delegates.erase(it);
	//			delete handler;
	//			return true;
	//		}
	//		return false;
	//	}

	//private:
	//	inline TEventBaseIterator FindDelegate(TEventBase& handler) const {
	//		return std::find_if(m_delegates.cbegin(), m_delegates.cend(), [&handler](const TEventBase* other) {
	//			return (*other == handler);
	//		});
	//	}
	//};

	//template<class TOwner, class ...TArgs>
	//class Delegate : public EventBase<TArgs...> {
	//	friend TOwner;

	//private:
	//	using TDelegate = Delegate<TOwner, TArgs...>;
	//	using TMethod = Method<TOwner, TArgs...>;

	//private:
	//	TMethod m_method;

	//public:
	//	Delegate(TOwner* owner, typename TMethod::TMethodPtr method) : m_method(owner, method) {}
	//	Delegate(const Delegate<TOwner, TArgs...>&) = default;
	//	~Delegate() = default;

	//	void Invoke(TArgs... args) override {
	//		((m_method.owner)->*(m_method.ptr))(args...);
	//	}

	//	bool IsEqual(const EventBase<TArgs...>& other) const override {
	//		const TDelegate* _other = dynamic_cast<const TDelegate*>(&other);
	//		return (_other != nullptr) && (_other->m_method.owner == this->m_method.owner) && (_other->m_method.ptr == this->m_method.ptr);
	//	}

 //       static Delegate<TOwner, TArgs...>& Allocate(TOwner* owner, typename TMethod::TMethodPtr method) {
 //           return *new Delegate(owner, method);
 //       }
	//};

template<class TOwner, class TSignature>
class Delegate;

template<class TOwner, class TSignature>
class Method;

template<class TSignature>
class Callable;

template<class TOwner, class TReturn, class ...TArgs>
class Method<TOwner, TReturn(TArgs...)> {
public:
	typedef TReturn(TOwner::* TMethodPtr)(TArgs...);

	TOwner* const owner;
	TMethodPtr const ptr;

	Method(TOwner* owner, TMethodPtr ptr) : owner(owner), ptr(ptr) {}
	~Method() = default;
};

template<class TReturn, class ...TArgs>
class Callable<TReturn(TArgs...)> {
private:
	using TCallable = Callable<TReturn(TArgs...)>;

public:
	Callable() = default;
	~Callable() = default;

	virtual TReturn Invoke(TArgs... args) = 0;
	virtual bool IsEqual(const TCallable& other) const = 0;

	bool operator==(const TCallable& other) const {
		return IsEqual(other);
	}

	bool operator!=(const TCallable& other) const {
		return !IsEqual(other);
	}

	template<class TOwner>
	static TCallable* AllocateDelegate(TOwner* owner, typename Method<TOwner, TReturn(TArgs...)>::TMethodPtr method) {
		return new Delegate<TOwner, TReturn(TArgs...)>(owner, method);
	}

	static void Free(Callable* callback) {
		DELETE_OBJECT(callback);
	}
};

template<class TOwner, class TReturn, class ...TArgs>
class Delegate<TOwner, TReturn(TArgs...)> : public Callable<TReturn(TArgs...)> {
	friend TOwner;

private:
	using TCallable = Callable<TReturn(TArgs...)>;
	using TDelegate = Delegate<TOwner, TReturn(TArgs...)>;
	using TMethod = Method<TOwner, TReturn(TArgs...)>;

	TMethod m_method;

public:
	Delegate(TOwner* owner, typename TMethod::TMethodPtr method) : m_method(owner, method) {}
	Delegate(const Delegate& other) : m_method(other.m_method) {};
	~Delegate() = default;

	Delegate& operator=(const Delegate& other) {
		m_method = other.m_method;
		return *this;
	}

	TReturn Invoke(TArgs... args) override {
		return ((m_method.owner)->*(m_method.ptr))(args...);
	}

	bool IsEqual(const TCallable& other) const override {
		const TDelegate& _other = dynamic_cast<const TDelegate&>(other);
		return (_other.m_method.owner == this->m_method.owner) && (_other.m_method.ptr == this->m_method.ptr);
	}
};

	template<class ...TArgs>
	class Event {
	private:
		using TCallable = Callable<void(TArgs...)>;

	public:
		Event() : m_callbackList() {};

		~Event() {
			for (Size i = 0; i < m_callbackList.size(); i++) {
				DELETE_OBJECT(m_callbackList[i]);
			}
		}

		void operator()(TArgs... args) {
			for (Size i = 0; i < m_callbackList.size(); i++) {
				m_callbackList[i]->Invoke(args...);
			}
		}

		void operator+=(const TCallable& callback) {
			auto it = std::find_if(m_callbackList.begin(), m_callbackList.end(), [&](TCallable* item) { item->IsEqual(callback); });
			if (it == m_callbackList.end()) {
				m_callbackList.push_back(new TCallable(callback));
			}
		}

		void operator-=(const TCallable& callback) {
			auto it = std::find_if(m_callbackList.begin(), m_callbackList.end(), [&](TCallable* item) { item->IsEqual(callback); });
			if (it != m_callbackList.end()) {
				DELETE_OBJECT(*it);
				m_callbackList.erase(it);
			}
		}

	private:
		Array<TCallable*> m_callbackList;
	};
}
