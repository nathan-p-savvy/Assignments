#include "Blackjack.hpp"

Blackjack::Blackjack(int startingchips){
    chips_ = startingchips;
    currentbet_ = 0;
    deck_.shuffle(); 
}
Blackjack::Blackjack(const Deck &stacked){
    chips_ = 10;
    deck_  = stacked;
}

bool Blackjack::startRound(int bet){
    if (phase_ != Phase::Betting){
        return false;
    }
    if (bet > chips_ || bet <= 0){
        return false;
    }
    currentbet_ = bet;
    chips_ -= bet;

    player.clear();
    dealer.clear();

    dealerHoleHidden_ = true;

    player.addCard(deck_.deal());
    dealer.addCard(deck_.deal());
    player.addCard(deck_.deal());
    dealer.addCard(deck_.deal());

    int rank = dealerHand().at(0).getRank();
    dealerhiddenvalue_ = (rank == 0) ? 11 : (rank < 10 ? rank + 1 : 10);

    if(player.value() == 21 && dealer.value() == 21){
        phase_ = Phase::RoundOver;
        outcome_ = Outcome::Push;
        dealerHoleHidden_ = false;
        chips_ += bet;
    }
    else if(player.value() == 21){
        phase_ = Phase::RoundOver;
        outcome_ = Outcome::PlayerBlackjack;
        dealerHoleHidden_ = false;
        chips_ += currentbet_ + (currentbet_ * 3) / 2;
    }
    else if(dealer.value() == 21){
    phase_ = Phase::RoundOver;
    outcome_ = Outcome::DealerWin;
    dealerHoleHidden_ = false;
    }
    else{
        phase_ = Phase::PlayerTurn;
        outcome_ = Outcome::None;
    }
    return true;
}

bool Blackjack::hit(){
    if(phase_ != Phase::PlayerTurn){
        return false;
    }
    player.addCard(deck_.deal());
    if(player.value() > 21){
        phase_ = Phase::RoundOver;
        outcome_ = Outcome::PlayerBust;
    }
    return true;
}

bool Blackjack::stand(){
    if(phase_ != Phase::PlayerTurn){
        return false;
    }

    dealerHoleHidden_ = false;

    while(dealer.value() < 17){
        dealer.addCard(deck_.deal());
    }

    if(dealer.value() > 21){
        chips_ += currentbet_ * 2;
        outcome_ = Outcome::DealerBust;
    }
    else if(dealer.value() == player.value()){
        chips_ += currentbet_;
        outcome_ = Outcome::Push;
    }
    else if(dealer.value() > player.value()){
        outcome_ = Outcome::DealerWin;
    }
    else if(dealer.value() < player.value()){
        chips_ += currentbet_ * 2;
        outcome_ = Outcome::PlayerWin;
    }

    phase_ = Phase::RoundOver;
    return true;
}

bool Blackjack::nextRound(){
    if(phase_ != Phase::RoundOver){
        return false;
    }

    outcome_ = Outcome::None;

    if(chips_ < 1){ 
        phase_ = Phase::GameOver;
        return true;
    }

    phase_ = Phase::Betting;
    return true;
}

bool Blackjack::restart(){
    if(phase_ != Phase::GameOver){
        return false;
    }

    chips_ = 100;
    phase_ = Phase::Betting;
    outcome_ = Outcome::None;
    return true;
}



Phase Blackjack::phase() const{
    return phase_;
}

int Blackjack::chips() const{
    return chips_;
}

int Blackjack::dealerhiddenvalue() const{
    return dealerhiddenvalue_;
}
const Hand& Blackjack::playerHand() const{
    return player;
}
const Hand& Blackjack::dealerHand() const{
    return dealer;
}
bool Blackjack::dealerHoleHidden() const{
    return dealerHoleHidden_;
}  // the views must respect this!
Outcome Blackjack::outcome() const{
    return outcome_;
}

int Blackjack::handValue(const Hand& h){
    int value = 0;
    int aces = 0;
    for (int i = 0; i < h.size(); i++){
        Card card = h.at(i);
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
}    // pure function: easy to test
bool Blackjack::isSoft(const Hand& h){
    int hardvalue = 0;
    int aces = 0;
    for (int i = 0; i < h.size(); i++){
        Card card = h.at(i);
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

    return (Blackjack::handValue(h) > hardvalue);
}
std::string Blackjack::outcomeName(Outcome o){
    switch (o){
        case Outcome::None: return "Hit or Stand";
        case Outcome::PlayerBlackjack: return "Player Blackjack!";
        case Outcome::PlayerWin: return "Player Win!";
        case Outcome::DealerWin: return "Dealer Win!";
        case Outcome::Push: return "Push";
        case Outcome::PlayerBust: return "Player Bust!";
        case Outcome::DealerBust: return "Dealer Bust";
    }
    return "Unknown";
}


