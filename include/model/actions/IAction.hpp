#pragma once

enum class ActionType { BID, DUDO, EXACTLY, EXIT, RESET };

class IAction {
protected:
    ActionType type_;
    int playerId_;

public:
    virtual ~IAction() = default;
    IAction(const IAction&) = delete;
    IAction& operator=(const IAction&) = delete;
    IAction(IAction&&) noexcept = default;
    IAction& operator=(IAction&&) noexcept = default;

    IAction(ActionType type, int playerId) : type_(type), playerId_(playerId) {};
    virtual ActionType getType() const { return type_; };
    virtual int getPlayerId() const { return playerId_; };
};