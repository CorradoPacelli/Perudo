#include "Bid.hpp"

Bid::Bid(int quantity, short int face) : quantity(quantity), face(face) {
    /** TODO:
     * 1) maybe we want to play with a d4, or a d20...
     */
    if (face < 1 || face > 6) {
        throw std::invalid_argument("The face of a Die must be between 1 and 6");
    }

    if (quantity < 1) {
        throw std::invalid_argument("The quantity must be grather than 1");
    }
}

int Bid::getQuantity() const {
    return quantity;
}

int Bid::getFace() const {
    return face;
}

bool Bid::isValidAfter(const Bid& other) const {
    if (quantity > other.quantity) return true;
    if (quantity == other.quantity) {
        return face > other.face;
    }
    return false;
}