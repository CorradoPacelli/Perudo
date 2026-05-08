#pragma once

#include <string>
#include <memory>

class GameEngine;

class IGameState {
public:
    virtual ~IGameState() = default;

    virtual void onEnter() = 0;

    virtual std::unique_ptr<IGameState> handleInput(GameEngine& context, const std::string& input) = 0;

    virtual void render() const = 0;
};