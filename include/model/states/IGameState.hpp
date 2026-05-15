#pragma once

#include <string>
#include <memory>

class GameModel;
class IGameView;
class IAction;

class IGameState {
public:
    virtual ~IGameState() = default;
    IGameState(const IGameState&) = delete;
    IGameState& operator=(const IGameState&) = delete;
    IGameState(IGameState&&) noexcept = default;
    IGameState& operator=(IGameState&&) noexcept = default;

    /**
     * @brief Called immediately when the game transitions into this state.
     * @param context The main game model.
     */
    virtual void onEnter(GameModel& context) = 0;

    /**
     * @brief Handles a specific user action for this state.
     * @param context The main game model.
     * @param action The action parsed from the user input.
     */
    virtual void handleAction(GameModel& context, const IAction& action) = 0;
    
    /**
     * @brief Defines whether this state needs an explicit user action to proceed.
     * @return true if the state requires an action, false if it just needs to step.
     */
    virtual bool requiresAction() const { return true; }

    /**
     * @brief Advances the state machine if no user action is required.
     * @param context The main game model.
     */
    virtual void step(GameModel& context) = 0;

    /**
     * @brief Displays the state's specific visual information to the user.
     * @param context The main game model.
     * @param view The view used to display the output.
     */
    virtual void render(const GameModel& context, IGameView& view) const = 0;

protected:
    /**
     * @brief Requests the context to transition to a different state.
     * @param context The main game model.
     * @param newState The new state to transition to.
     */
    void requestStateChange(GameModel& context, std::unique_ptr<IGameState> newState);
};