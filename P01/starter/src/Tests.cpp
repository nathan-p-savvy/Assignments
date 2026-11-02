#include "Blackjack.hpp"

#include <iostream>

static int runTests() {
  int failures = 0;
  auto check = [&](bool ok, const std::string &name) {
    std::cout << (ok ? "PASS  " : "FAIL  ") << name << '\n';
    if (!ok) failures++;
  };

  // Stacked deck: 5C, then 9C, then 2C, then 2D, ...
  // Card values: Clubs 0-12 (Ace=0), Diamonds 13-25.
  Blackjack g(Deck({Card(0), Card(2), Card(12), Card(1), 
                    Card(13), Card(26), Card(25), Card(38), 
                    Card(12), Card(2), Card(11), Card(3), Card(4),
                    Card(0), Card(0), Card(0), Card(5), Card(8),
                    Card(0), Card(0), Card(5), Card(4), Card(9), Card(2)}));

    check(g.hit() == false, "checking if hitting returns a false in betting phase ");
    g.startRound(10);
    check(g.chips() == 25, "checking natural payout");
    check(g.playerHand().value() == 21, "checking A + K = 21 and is natural");
    g.nextRound();
    g.startRound(25);
    check(g.chips() == 25,  "checking natural vs natural push");
    g.nextRound();
    g.startRound(1);
    g.hit();
    check(g.playerHand().value() == 25 && g.outcome() == Outcome::PlayerBust, "checking K + Q + 5 = 25 and that it busts");
    g.nextRound();
    g.startRound(1);
    g.hit();
    check(g.playerHand().value() == 21, "checking A + A + 5 = 21");
    g.stand();
    check(g.dealerHand().value() == 17 && g.dealerHand().isSoft() == true, "checking A + 6 = 17 soft");
    check(g.outcome() == Outcome::PlayerWin && g.dealerHand().value() == 17, "checking that dealer stands on soft 17");
    g.nextRound();
    g.startRound(1);
    g.hit();
    check(g.playerHand().value() == 17 && g.playerHand().isSoft() == false, "checking A + 6 + 10 = 17 hard");
    g.stand();
    check(g.dealerHand().value() > 16 && g.outcome() == Outcome::DealerWin, "checking that dealer hits on a 16");



    

  return failures == 0 ? 0 : 1;
}