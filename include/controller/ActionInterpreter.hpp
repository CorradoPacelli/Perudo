#pragma once 

#include <memory>
#include <string>

#include "IAction.hpp"

class ActionInterpreter {
public:
    static std::unique_ptr<IAction> interpret(const std::string& rawInput);
};