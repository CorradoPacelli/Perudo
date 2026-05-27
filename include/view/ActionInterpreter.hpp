#pragma once 

#include <memory>
#include <string>
#include "IAction.hpp"

struct PlayerMessage;

class ActionInterpreter {
public:
    static std::unique_ptr<IAction> interpret(const PlayerMessage& playerMessage);
};