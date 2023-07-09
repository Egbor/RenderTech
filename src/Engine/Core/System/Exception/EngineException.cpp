#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    EngineException::EngineException(const char* const& message)
        : std::exception(message){

    }

    EngineException::EngineException(const String message)
        : std::exception(message.c_str()){

    }

   WString EngineException::WhatW() const {
        String whats(what());
        WString whatw;
        whatw.assign(whats.begin(), whats.end());
        return whatw;
    }

   EngineBadAllocationException::EngineBadAllocationException() 
       : EngineException("Bad allocation") {

   }

   EngineBadPointerException::EngineBadPointerException() 
       : EngineException("Bad pointer") {

   }
}
