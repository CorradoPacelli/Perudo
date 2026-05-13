#pragma once

#include <string>
#include <memory>

class GameModel;
class IGameView;
class IAction;

class IGameState {
public:
    virtual ~IGameState() = default;

    virtual void onEnter(GameModel& context) = 0;

    virtual void handleAction(GameModel& context, const IAction& action) = 0;

    virtual void render(const GameModel& context, IGameView& view) const = 0;
    
protected:
    void requestStateChange(GameModel& context, std::unique_ptr<IGameState> newState);
};