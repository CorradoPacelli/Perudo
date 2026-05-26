#include "GameController.hpp"
#include "GameModel.hpp"
#include "IGameView.hpp"
#include "IAction.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

GameController::GameController(GameModel& m, IGameView& v) : model(m), view(v) {}

void GameController::run() {

    model.startGame();
    model.render(view);

    while (!model.isGameOver()) {
        try {
            if (model.requiresAction()) {
                auto action = view.waitForAction();
                model.handleAction(*action);
            } else {
                //view.waitForContinue(); //TODO: I don't know if I want this wait for continue or not 
                model.step();
            }
            model.render(view);
        } catch (const std::invalid_argument& e) {
            // Recoverable error: inform the user and ask for action again
            view.displayBroadcastMessage(std::string("\n[!] Invalid Action: ") + e.what() + "\nPlease try again.");
        } catch (const std::exception& e) {
            // Unrecoverable error: log the error and stop the process
            view.displayBroadcastMessage(std::string("\n[!] Fatal Error: ") + e.what() + "\nQuitting the game.");
            model.stopGame();
        }
    }
}