#pragma once

enum class ActionType { BID, DUDO, EXACTLY, EXIT, RESET };

class IAction {
protected:
    ActionType type;

public:
    virtual ~IAction() = default;
    IAction(const IAction&) = delete;
    IAction& operator=(const IAction&) = delete;
    IAction(IAction&&) noexcept = default;
    IAction& operator=(IAction&&) noexcept = default;

    IAction(ActionType type) : type(type) {};
    virtual ActionType getType() const { return type; };
};