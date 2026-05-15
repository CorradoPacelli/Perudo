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

std::unique_ptr<IAction> ActionInterpreter::interpret(const std::string& rawInput) {
    std::string input = rawInput;

    // Safely cast tolower to avoid ambiguity
    std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c){ return std::tolower(c); });
    
    std::stringstream ss(input);
    std::string command;
    ss >> command;

    if (command == "dudo") {
        return std::make_unique<DudoAction>();
    }

    if (command == "exactly") {
        return std::make_unique<ExactlyAction>();
    }

    if (command == "bid") {
        int qty, val;
        if (ss >> qty >> val) {
            try {
                return std::make_unique<BidAction>(Bid{qty, val});
            } catch (const std::invalid_argument& e) {
                return nullptr;
            }
        }
    }

    if (command == "exit" || command == "quit") {
        return std::make_unique<ExitAction>();
    }

    if (command == "reset") {
        return std::make_unique<ResetAction>();
    }

    return nullptr;
}