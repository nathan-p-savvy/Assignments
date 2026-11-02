#pragma once

#include "Hand.hpp"
#include "Deck.hpp"



enum class Phase   { Betting, PlayerTurn, RoundOver, GameOver };
enum class Outcome { None, PlayerBlackjack, PlayerWin, DealerWin, Push,
                     PlayerBust, DealerBust };

class Blackjack {
    Deck deck_;
    Hand dealer;
    Hand player;
    int chips_;
    int currentbet_;
    int dealerhiddenvalue_;
    bool dealerHoleHidden_;
    Phase phase_ = Phase::Betting;
    Outcome outcome_ = Outcome::None;
public:
  Blackjack(int startingChips = 100);
  explicit Blackjack(const Deck &stacked);

  bool startRound(int bet);      // Betting -> PlayerTurn (or RoundOver on a natural)
  bool hit();                    // only during PlayerTurn; may bust -> RoundOver
  bool stand();                  // dealer plays, round is settled -> RoundOver
  bool nextRound(); 
  bool restart();             // RoundOver -> Betting (or GameOver)

  Phase phase() const;
  int chips() const;
  int dealerhiddenvalue() const;
  const Hand& playerHand() const;
  const Hand& dealerHand() const;
  bool dealerHoleHidden() const;  // the views must respect this!
  Outcome outcome() const;

  static int handValue(const Hand& h);     // pure function: easy to test
  static bool isSoft(const Hand& h);
  static std::string outcomeName(Outcome o);
};