#pragma once

#include <stdexcept>

class Bid {
private:
    int quantity;
    short int face;

public:
    Bid(int quantity, short int face);
    int getQuantity() const;
    int getFace() const;
    
    bool operator>(const Bid& other) const;
    bool operator==(const Bid& other) const;
};