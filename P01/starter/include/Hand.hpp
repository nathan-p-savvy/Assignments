#pragma once

#include <vector>
#include <string>

#include "Card.hpp"

class Hand{
private:    
    std::vector<Card> hand;
public:
    void addCard(const Card& c);
    void clear();
    int size() const;
    const Card& at(int i) const;
    int value() const;
    bool isSoft() const;
    bool empty() const;
};