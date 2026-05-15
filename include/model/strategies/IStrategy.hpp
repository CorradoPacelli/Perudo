#pragma once

//TODO: implement strategies for non real players

class IStrategy {
public:
    IStrategy() = default;
    virtual ~IStrategy() = default;
    IStrategy(const IStrategy&) = delete;
    IStrategy& operator=(const IStrategy&) = delete;
    IStrategy(IStrategy&&) noexcept = default;
    IStrategy& operator=(IStrategy&&) noexcept = default;

private:
    int whichStrategy{};
};