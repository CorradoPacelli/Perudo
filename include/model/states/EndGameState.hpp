#pragma once

class EndGameState : public IGameState {
public:
    EndGameState() = default;
    virtual ~EndGameState() = default;

    void onEnter(GameModel& context) override;

    void handleAction(GameModel& context, const IAction& action) override;

    void render(const GameModel& context, IGameView& view) const override;
};