#pragma once 

#include <memory>
#include <string>

class IAction;

class ActionInterpreter {
public:
    static std::unique_ptr<IAction> interpret(const std::string& rawInput);
};