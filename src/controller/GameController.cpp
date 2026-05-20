#include "GameController.hpp"
#include "GameModel.hpp"
#include "IGameView.hpp"
#include "IAction.hpp"

#include <iostream>
#include <string>

GameController::GameController(GameModel& m, IGameView& v) : model(m), view(v) {}

void GameController::run() {

    model.startGame();

    while (!model.isGameOver()) {
        model.render(view);
        
        if (model.requiresAction()) {
            auto action = view.waitForAction();
            model.handleAction(*action);
        } else {
            // TODO: I don't know if I want this wait for continue or not 
            //view.waitForContinue();
            model.step();
        }
    }
}