// Starter main. Right now it only proves the classes link together.
// Replace the body as described in the assignment README:
//
//   ./game          -> runs your TuiView (FTXUI)
//   ./game --text   -> runs your ConsoleView (cout/cin)
//
// Final shape (sketch):
//
//   VideoPoker game;            // or Blackjack game;
//   std::unique_ptr<GameView> view;
//   if (textMode) view = std::make_unique<ConsoleView>(game);
//   else          view = std::make_unique<TuiView>(game);
//   view->run();

#include "ConsoleView.hpp"
#include "Blackjack.hpp"
#include "TuiView.hpp"
#include "Tests.cpp"

#include <iostream>
#include <memory>
#include <string>

int main(int argc, char **argv){
   std::string mode = argc > 1 ? argv[1] : "";
    if (mode == "--test") return runTests();

  Blackjack game;

  // Polymorphism: main() never knows which kind of view it has.
  std::unique_ptr<GameView> view;
  if (mode == "--text") view = std::make_unique<ConsoleView>(game);
  else                  view = std::make_unique<TuiView>(game);
  view->run();
  return 0;
}
