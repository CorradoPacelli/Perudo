#pragma once

#include <stdexcept>

class Bid {
private:
    int quantity;
    short int face;

public:
    Bid(int quantity, short int face);
    int getQuanmtity() const;
    int getFace() const;
    bool isValidAfter(const Bid& other) const;
};