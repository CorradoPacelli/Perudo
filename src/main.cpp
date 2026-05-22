#include "GameModel.hpp"
#include "ConsoleView.hpp"
#include "GameController.hpp"
#include "Player.hpp"
#include "NetworkView.hpp"

int main() {
    GameModel model;
    
    model.addPlayer(Player("Cori"));
    model.addPlayer(Player("Ade"));
    model.addPlayer(Player("Domenico Palmieri"));
    
    NetworkView view(8080);
    GameController controller(model, view);

    controller.run();

    return 0;
}