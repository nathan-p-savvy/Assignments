#include "Hand.hpp"

#include <stdexcept>

void Hand::addCard(const Card& c){
    hand.push_back(c);
}

void Hand::clear(){
    hand.clear();
}

int Hand::size() const{
    return (int) hand.size();
}

const Card& Hand::at(int i) const{
if (i < 0 || i > 51){
    throw std::out_of_range("Card value must be 0-51");
    }
    return hand[i];
}

int Hand::value() const{
    int value = 0;
    int aces = 0;
    for (const auto& card : hand){
        if(card.getRank() == 0){
            value += 11;
            aces++;
        }
        else if(card.getRank() > 0 && card.getRank() < 10){
            value += (card.getRank()+1);
        }
        else{
            value += 10;
        }
    }
            

        
        while(value > 21 && aces > 0){
            value -= 10;
            aces--;
        }

        return value;
    }

bool Hand::isSoft() const{
    int hardvalue = 0;
    int aces = 0;
    for (const auto& card : hand){
        if(card.getRank() == 0){
            hardvalue += 1;
            aces++;
        }
        else if(card.getRank() > 0 && card.getRank() < 10){
            hardvalue += (card.getRank()+1);
        }
        else{
            hardvalue += 10;
        }
    }
    if(aces == 0){
        return false;
    }

    return (value() > hardvalue);
}

bool Hand::empty() const { return hand.empty(); }
    

