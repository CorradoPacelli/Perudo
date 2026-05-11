#pragma once

enum class ActionType { BID, DUDO, EXACTLY };

class IAction {
protected:
    ActionType type;

public:
    virtual ~IAction() = default;
    IAction();
    virtual ActionType getType() const { return type; };
};