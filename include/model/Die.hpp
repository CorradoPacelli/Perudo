#pragma once

#include <random>

class Die {
private:
    short int value;
    static std::mt19937 randomGenerator;

public:
    Die();
    ~Die() = default;

    static short int roll();
    short int getValue() const;
};