#include "GameController.hpp"
#include "ActionInterpreter.hpp"

#include <iostream>
#include <string>

GameController::GameController(GameModel& m, IGameView& v) : model(m), view(v) {}

void GameController::run() {

    model.startGame();

    while (!model.isGameOver()) {
        model.render(view);

        std::string rawInput = view.waitForInput();
        auto action = ActionInterpreter::interpret(rawInput);
        while (!action) {
            view.displayMessage("Command was not recognized. Try again :)");

            std::string rawInput = view.waitForInput();
            action = ActionInterpreter::interpret(rawInput);
        }
        model.handleAction(*action);
    }
}