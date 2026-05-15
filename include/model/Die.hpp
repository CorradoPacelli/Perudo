#pragma once

#include <random>

class Die {
private:
    short int face;
    static std::mt19937 randomGenerator;

public:
    Die();

    static short int roll();
    short int getFace() const;

    bool operator==(const Die& other) const;
};