#pragma once

#include "GameView.hpp"
#include "Blackjack.hpp"

// A VIEW: shows the same game with FTXUI.
class TuiView : public GameView {
private:
  Blackjack &game;

public:
  explicit TuiView(Blackjack &game);
  void run() override;
};