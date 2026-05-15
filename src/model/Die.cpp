#include "Die.hpp"

Die::Die() : face(roll()) {};

short int Die::roll() {
    /** TODO:
     * 1) I would like at some point to add a Cheater player with a different distribution XD
     * 2) I would like to have also d4 and d8...
    */
    std::uniform_int_distribution<short int> dist(1, 6);
    return dist(randomGenerator);
}

short int Die::getFace() const { 
    return face; 
}

bool Die::operator==(const Die& other) const{
    return face == other.face;
}

std::mt19937 Die::randomGenerator(std::random_device{}());