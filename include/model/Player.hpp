#pragma once

#include <string>
#include <memory>
#include <optional>

#include "Bid.hpp"

class IStrategy;
class Die;

class Player {
private:
    std::string name;
    std::vector<Die> hand;
    std::vector<Bid> bids;
    std::unique_ptr<IStrategy> strategy;
    bool alive;

public:

    ~Player();
    
    /**
     * @brief Standard real player constructor
     * @param name Name of the player
     * In this variant of the game, the default number of dice is 6. 
     */
    Player(const std::string name);

    /**l
     * @brief Special real player constructorl
     * @param name Name of the player
     * @param numberOfDice Number of dice for this player
     */
    Player(std::string name, int numberOfDice);

    /**
     * @brief Standard non-real player
     * @param name Name of the player
     * @param strategy The strategy used by the artifical player
     */
    Player(std::string name, std::unique_ptr<IStrategy> strategy);

    const std::string getName() const;
    bool isAlive() const;
    bool isAI() const;

    void rollAllDice();
    void loseDie();
    void loseDice(int numberToLose);
    const std::vector<Die>& getHand() const;
    int getDiceCount() const;

    void addBid(const Bid& bid);
    std::optional<Bid> getLastBid() const;
    void clearHistory();
    const std::vector<Bid>& getFullHistory() const;

    bool operator==(const Player& other) const;
    bool operator!=(const Player& other) const;
};