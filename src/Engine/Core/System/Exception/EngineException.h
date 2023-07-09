#ifndef ENGINEEXCEPTION_H
#define ENGINEEXCEPTION_H

#include "Engine/EngineDef.h"
#include <exception>
#include <stdexcept>

namespace Engine {
    class EngineException : public std::exception {
    public:
        EngineException(const char* const& message);
        EngineException(const String message);
        virtual ~EngineException() = default;

        WString WhatW() const;
    };

    class EngineBadAllocationException : public EngineException {
    public:
        EngineBadAllocationException();
        virtual ~EngineBadAllocationException() = default;
    };

    class EngineBadPointerException : public EngineException {
    public:
        EngineBadPointerException();
        virtual ~EngineBadPointerException() = default;
    };
}

#endif // ENGINEEXCEPTION_H
