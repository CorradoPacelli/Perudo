#pragma once

enum class ActionType { BID, DUDO, EXACTLY, EXIT, RESET };

class IAction {
protected:
    ActionType type;

public:
    virtual ~IAction() = default;
    IAction(ActionType type) : type(type) {};
    virtual ActionType getType() const { return type; };
};