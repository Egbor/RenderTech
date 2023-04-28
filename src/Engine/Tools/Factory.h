#ifndef FACTORY_H
#define FACTORY_H

#include "Engine/EngineDef.h"

namespace Engine {
    template<class TReturn>
    class Creator {
    public:
        Creator() = default;
        Creator(const Creator<TReturn>&) = default;
        virtual ~Creator() = default;

        virtual TReturn* Create() = 0;
    };

    template<class TReturn, class ...TArgs>
    class CreatorWithArguments : public Creator<TReturn> {
    public:
        CreatorWithArguments() = default;
        CreatorWithArguments(const CreatorWithArguments&) = default;
        virtual ~CreatorWithArguments() = default;

        virtual void SetArguments(TArgs ...args) = 0;
        virtual TReturn* Create() override = 0;
    };

    template<class TReturn, class TClass>
    class DefaultCreator : public Creator<TReturn> {
    public:
        DefaultCreator() = default;
        DefaultCreator(const DefaultCreator<TReturn, TClass>&) = default;
        virtual ~DefaultCreator() = default;

        TReturn* Create() override {
            return new TClass();
        }
    };

    template<class TReturn, class TClass, class TArg1>
    class DefaultCreatorWith1Args : public CreatorWithArguments<TReturn, TArg1> {
    private:
        TArg1 m_defaultArg1;

    public:
        DefaultCreatorWith1Args() = default;
        DefaultCreatorWith1Args(const DefaultCreatorWith1Args<TReturn, TClass, TArg1>&) = default;
        virtual ~DefaultCreatorWith1Args() = default;

        void SetArguments(TArg1 arg1) override {
            m_defaultArg1 = arg1;
        }

        TReturn* Create() override {
            return new TClass(m_defaultArg1);
        }
    };

    template<class TReturn, class TClass, class TArg1, class TArg2>
    class DefaultCreatorWith2Args : public CreatorWithArguments<TReturn, TArg1, TArg2> {
    private:
        TArg1 m_defaultArg1;
        TArg2 m_defaultArg2;

    public:
        DefaultCreatorWith2Args() = default;
        DefaultCreatorWith2Args(const DefaultCreatorWith2Args<TReturn, TClass, TArg1, TArg2>&) = default;
        virtual ~DefaultCreatorWith2Args() = default;

        void SetArguments(TArg1 arg1, TArg2 arg2) override {
            m_defaultArg1 = arg1;
            m_defaultArg2 = arg2;
        }

        TReturn* Create() override {
            return new TClass(m_defaultArg1, m_defaultArg2);
        }
    };

    template<class TReturn, class TClass, class TArg1, class TArg2, class TArg3>
    class DefaultCreatorWith3Args : public CreatorWithArguments<TReturn, TArg1, TArg2, TArg3> {
    private:
        TArg1 m_defaultArg1;
        TArg2 m_defaultArg2;
        TArg3 m_defaultArg3;

    public:
        DefaultCreatorWith3Args() = default;
        DefaultCreatorWith3Args(const DefaultCreatorWith3Args<TReturn, TClass, TArg1, TArg2, TArg3>&) = default;
        virtual ~DefaultCreatorWith3Args() = default;

        void SetArguments(TArg1 arg1, TArg2 arg2, TArg3 arg3) override {
            m_defaultArg1 = arg1;
            m_defaultArg2 = arg2;
            m_defaultArg3 = arg3;
        }

        TReturn* Create() override {
            return new TClass(m_defaultArg1, m_defaultArg2, m_defaultArg3);
        }
    };

    template<class TReturn, class TClass, class TArg1, class TArg2, class TArg3, class TArg4>
    class DefaultCreatorWith4Args : public CreatorWithArguments<TReturn, TArg1, TArg2, TArg3, TArg4> {
    private:
        TArg1 m_defaultArg1;
        TArg2 m_defaultArg2;
        TArg3 m_defaultArg3;
        TArg4 m_defaultArg4;

    public:
        DefaultCreatorWith4Args() = default;
        DefaultCreatorWith4Args(const DefaultCreatorWith4Args<TReturn, TClass, TArg1, TArg2, TArg3, TArg4>&) = default;
        virtual ~DefaultCreatorWith4Args() = default;

        void SetArguments(TArg1 arg1, TArg2 arg2, TArg3 arg3, TArg4 arg4) override {
            m_defaultArg1 = arg1;
            m_defaultArg2 = arg2;
            m_defaultArg3 = arg3;
            m_defaultArg4 = arg4;
        }

        TReturn* Create() override {
            return new TClass(m_defaultArg1, m_defaultArg2, m_defaultArg3, m_defaultArg4);
        }
    };

    template<class TReturn, class TClass, class TArg1, class TArg2, class TArg3, class TArg4, class TArg5>
    class DefaultCreatorWith5Args : public CreatorWithArguments<TReturn, TArg1, TArg2, TArg3, TArg4, TArg5> {
    private:
        TArg1 m_defaultArg1;
        TArg2 m_defaultArg2;
        TArg3 m_defaultArg3;
        TArg4 m_defaultArg4;
        TArg5 m_defaultArg5;

    public:
        DefaultCreatorWith5Args() = default;
        DefaultCreatorWith5Args(const DefaultCreatorWith5Args<TReturn, TClass, TArg1, TArg2, TArg3, TArg4, TArg5>&) = default;
        virtual ~DefaultCreatorWith5Args() = default;

        void SetArguments(TArg1 arg1, TArg2 arg2, TArg3 arg3, TArg4 arg4, TArg5 arg5) override {
            m_defaultArg1 = arg1;
            m_defaultArg2 = arg2;
            m_defaultArg3 = arg3;
            m_defaultArg4 = arg4;
            m_defaultArg5 = arg5;
        }

        TReturn* Create() override {
            return new TClass(m_defaultArg1, m_defaultArg2, m_defaultArg3, m_defaultArg4, m_defaultArg5);
        }
    };

    template<class TReturn>
    class Factory {
    private:
        Map<String, Creator<TReturn>*> m_map;

    public:
        Factory() : m_map() {}
        Factory(const Factory<TReturn>&) = default;
        ~Factory() {
            UnRegister();
        }

        void Register(const String key, Creator<TReturn>* creator) {
            if (m_map.find(key) == m_map.end()) {
                m_map.insert(std::pair<std::string, Creator<TReturn>*>(key, creator));
            }
        }

        void UnRegister() {
            while (m_map.begin() != m_map.end()) {
                auto it = m_map.begin();
                delete it->second;
                m_map.erase(it);
            }
        }

        template<class ...TArgs>
        TReturn* Create(const String& key, TArgs ...args) {
            if (m_map.find(key) != m_map.end()) {
                if (sizeof...(TArgs) > 0) {
                    reinterpret_cast<CreatorWithArguments<TReturn, TArgs...>*>(m_map[key])->SetArguments(args...);
                }
                return m_map[key]->Create();
            }
            return nullptr;
        }

        template<class TCastReturn, class ...TArgs>
        TCastReturn* CreateAs(const String& key, TArgs ...args)  {
            return dynamic_cast<TCastReturn*>(Create(key, args...));
        }
    };
}

#endif // FACTORY_H
