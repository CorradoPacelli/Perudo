#include "GameModel.hpp"
#include "ConsoleView.hpp"
#include "GameController.hpp"
#include "Player.hpp"

int main() {
    GameModel model;
    
    model.addPlayer(Player("Cori"));
    model.addPlayer(Player("Ade"));
    model.addPlayer(Player("Domenico Palmieri"));

    ConsoleView view;
    GameController controller(model, view);

    controller.run();

    return 0;
}