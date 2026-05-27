#include "ActionInterpreter.hpp"

#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <cctype>

#include "DudoAction.hpp"
#include "ExactlyAction.hpp"
#include "BidAction.hpp"
#include "ExitAction.hpp"
#include "ResetAction.hpp"
#include "PlayerMessage.hpp"

std::unique_ptr<IAction> ActionInterpreter::interpret(const PlayerMessage& playerMessage) {
    std::string rawInput = playerMessage.message;
    // Safely cast tolower to avoid ambiguity
    std::transform(rawInput.begin(), 
                rawInput.end(), 
                rawInput.begin(), 
                [](unsigned char c){ 
                    return std::tolower(c); 
                });
    
    std::stringstream ss(rawInput);
    std::string command;
    ss >> command;

    if (command == "dudo") {
        return std::make_unique<DudoAction>(playerMessage.playerId);
    }

    if (command == "exactly") {
        return std::make_unique<ExactlyAction>(playerMessage.playerId);
    }

    if (command == "bid") {
        int qty, val;
        if (ss >> qty >> val) {
            try {
                return std::make_unique<BidAction>(Bid{qty, val}, playerMessage.playerId);
            } catch (const std::invalid_argument& e) {
                return nullptr;
            }
        }
    }

    if (command == "exit" || command == "quit" || command == "q") {
        return std::make_unique<ExitAction>(playerMessage.playerId);
    }

    if (command == "reset") {
        return std::make_unique<ResetAction>(playerMessage.playerId);
    }

    return nullptr;
}