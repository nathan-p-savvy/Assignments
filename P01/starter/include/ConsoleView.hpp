#pragma once

#include "GameView.hpp"
#include "Blackjack.hpp"

// A VIEW: shows the game with cout, reads commands with getline.
class ConsoleView : public GameView {
private:
  Blackjack &game; // uses the game, does not own it

  void showHand() const;
  void showOutcome() const;

public:
  explicit ConsoleView(Blackjack &game);
  void run() override;
};