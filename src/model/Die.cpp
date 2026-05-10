#include "Die.hpp"

Die::Die() : value(roll()) {};

short int Die::roll() {
    std::uniform_int_distribution<short int> dist(1, 6);
    return dist(randomGenerator);
}

short int Die::getValue() const { 
    return value; 
}

std::mt19937 Die::randomGenerator(std::random_device{}());