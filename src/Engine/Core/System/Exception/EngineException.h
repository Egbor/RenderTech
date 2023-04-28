#ifndef ENGINEEXCEPTION_H
#define ENGINEEXCEPTION_H

#include "Engine/EngineDef.h"
#include <exception>

namespace Engine {
    class EngineException : public std::exception {
    public:
        EngineException(const char* const& message);
        EngineException(const String message);
        EngineException(const EngineException&) = default;
        virtual ~EngineException() = default;

        WString WhatW() const;
    };
}

#endif // ENGINEEXCEPTION_H
