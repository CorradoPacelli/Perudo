#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <string>

#include "GameController.hpp"
#include "GameModel.hpp"
#include "IGameView.hpp"
#include "ExitAction.hpp"
#include "Player.hpp"
#include "MockGameView.hpp"
#include "BidAction.hpp"
#include "DudoAction.hpp"
#include "GameModelTestAccessor.hpp"

class GameControllerTest : public ::testing::Test {
protected:
    GameModel model;
    MockGameView view;

    void SetUp() override {
        // Ensure the model has enough players to start normally
        model.addPlayer(Player("Saitama", 5));
        model.addPlayer(Player("Genos", 5));
    }
};

TEST_F(GameControllerTest, RunExecutesGameLoopUntilGameOver) {
    // The mock view will provide an "exit" action on the first request.
    GameController controller(model, view);
    
    EXPECT_NO_THROW(controller.run());

    EXPECT_TRUE(model.isGameOver());
    EXPECT_GT(view.messages.size(), 0); // Controller should have rendered to the view at least once.
    EXPECT_EQ(view.actionCallCount, 1); // Controller should have asked for one action, which was "exit".
}

/*
TEST_F(GameControllerTest, FullGameWithTwoPlayers) {
    // Ripetiamo 5 volte per inserire un totale di 20 azioni, coprendo 10 round teorici.
    for (int i = 0; i < 5; ++i) {
        // Round in cui il Giocatore 0 inizia, il Giocatore 1 chiama Dudo e perde
        view.actionQueue.push(std::make_unique<BidAction>(Bid(1,2),0));
        view.actionQueue.push(std::make_unique<DudoAction>(1));
        
        // Round successivo: il Giocatore 1 deve iniziare. Il Giocatore 0 chiama Dudo e perde
        view.actionQueue.push(std::make_unique<BidAction>(Bid(1,2),1));
        view.actionQueue.push(std::make_unique<DudoAction>(0));
    }

    // Hook into the view's wait action to override the randomly rolled dice
    // faces to all 2s every time the game asks for an action.
    view.onWaitForActionCallback = [&]() {
        if (model.isGameOver()) {
            for (const auto& player : model.getPlayers()) {
                if (player.isAlive()) {
                    view.actionQueue.push(std::make_unique<ExitAction>(0));
                }
            }
        }

        for (auto& player : model.getPlayers()) {
            if (player.isAlive()) {
                int currentDiceCount = player.getDiceCount();
                GameModelTestAccessor::setPlayerDice(player, std::vector<short int>(currentDiceCount, 2));
            }
        }
    };

    GameController controller(model, view);
    
    EXPECT_NO_THROW(controller.run());

    // Verify that the game actually ended because someone won, not just because we quit early
    EXPECT_TRUE(model.isGameOver());
    EXPECT_TRUE(model.isOnlyOnePlayerAlive());
    //TODO: here the game ends around action 10, but we still consume the action in the mock because in the EndGameState we are waiting for either play again or quit
    EXPECT_EQ(view.actionCallCount, 21); 
}
*/