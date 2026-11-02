#include "TuiView.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <vector>
#include <string>

using namespace ftxui;

constexpr int CARD_WIDTH = 9;
constexpr int CARD_HEIGHT = 7;


Element DrawFaceDown(){
  return vbox({
    text("░░░░░░░░░"),
    text("░░░░░░░░░"),
    text("░░░░░░░░░"),
    text("░░░░░░░░░"),
    text("░░░░░░░░░"),
    text("░░░░░░░░░"),
    text("░░░░░░░░░"),
    text("░░░░░░░░░"),
  }) | color(Color::Blue)
     | size(WIDTH, EQUAL, CARD_WIDTH)
     | size(HEIGHT, EQUAL, CARD_HEIGHT)
     | border;
}

Element DrawCard(const Card &card) {

  auto suit_color = card.isRed() ? Color::Red : Color::White;

  auto body = vbox({
            hbox({text(card.rankSymbol()), filler(), text(card.suitSymbol())}),

            filler(),

            text(card.rankSymbol()) | bold | center,

            filler(),

            hbox({text(card.suitSymbol()), filler(), text(card.rankSymbol())})
          }) | color(suit_color) 
             | size(WIDTH, EQUAL, CARD_WIDTH) 
             | size(HEIGHT, EQUAL, CARD_HEIGHT)
             | border;
  return body;
}


TuiView::TuiView(Blackjack &game) : game(game) {}

void TuiView::run(){
  int bet = 10;

  auto screen = ScreenInteractive::Fullscreen();

  screen.TrackMouse(false);


  auto renderer = Renderer([&] {
    std::string chips = std::to_string(game.chips());
  
    std::string currentbet = std::to_string(bet);

    std::string controls;

    std::string hiddenvalue = std::to_string(game.dealerhiddenvalue());

    std::string truevalue = std::to_string(game.handValue(game.dealerHand()));

    std::string playervalue = std::to_string(game.handValue(game.playerHand()));

    Elements dealercards;
    Elements playercards;

    if(game.phase() == Phase::Betting){
    controls = "<- ->  Adjust Bet    ENTER  Start Round    Q  Quit";
    }

    if(game.phase() == Phase::PlayerTurn){
    controls = "Space  Hit    ENTER  Stand    Q  Quit";
   }

    if(game.phase() == Phase::RoundOver){
    controls = "Enter  Next Round    Q  Quit";
    }

    if(game.phase() == Phase::GameOver){
    controls = "Enter  Play Again    Q Quit";
   }

    
    if(game.dealerHoleHidden()){
       if (!game.dealerHand().empty()) {
      dealercards.push_back(DrawCard(game.dealerHand().at(0)));
      dealercards.push_back(text(" "));
      dealercards.push_back(DrawFaceDown());
       }
    }
    else{
      if (!game.dealerHand().empty()) {
        for (int i = 0; i < game.dealerHand().size(); ++i) {
          dealercards.push_back(DrawCard(game.dealerHand().at(i)));

      // Space between cards.
        if (i != game.dealerHand().size() - 1)
          dealercards.push_back(text(" "));
        }
      }
    }

    if (!game.dealerHand().empty()) {
      for (int i = 0; i < game.playerHand().size(); ++i) {

        playercards.push_back(DrawCard(game.playerHand().at(i)));

      // Space between cards.
        if (i != game.playerHand().size() - 1)
          dealercards.push_back(text(" "));
      }
    }
    if(game.phase() == Phase::Betting){
      return vbox({

              text("Blackjack") | bold | center,
              hbox({text("Chips: ") | bold, text(chips) | color(Color::Green)}) | center,
              separator(),
              text("Place your bet!") | bold | center,
              hbox({text("<-["), text(std::to_string(bet)) | color(Color::Yellow), text("]->")}) | center,
              separator(),
              paragraph(controls) | dim | center
                }) | border;
    }

    else{
      return vbox({

              text("Blackjack") | bold | center,
              hbox({text("Chips: ") | bold, text(chips) | color(Color::Green)}) | center,
              text(currentbet) | color(Color::Red) | center,
              separator(),
              text("Dealer") | center,
              hbox(std::move(dealercards)) | center,
              hbox({text("Value: "), text(game.dealerHoleHidden() ? hiddenvalue : truevalue)}) | center,
              separator(),
              text("Player") | center,
              hbox(std::move(playercards)) | center,
              hbox({text("Value: "), text(playervalue)}) | center,
              separator(),
              text(game.phase() == Phase::GameOver
                        ? "GAME OVER! The House Always Wins... Play Again?"
                        : game.outcomeName(game.outcome())) | bold | center,
              separator(),
              paragraph(controls) | dim | center
                }) | border;
    }
  });

  auto app = CatchEvent(renderer, [&](Event event) {

    if (event == Event::Character('q') || event == Event::Character('Q')) {

      screen.Exit();
      return true;
    }

    if(game.phase() == Phase::Betting){
      if (event == Event::ArrowLeft) {
        if(bet > 1){
          bet -= 1;
        }
        return true;
    }
      else if (event == Event::ArrowRight){
        if(bet < game.chips()){
        bet += 1;
        }
        return true;
      }

      else if (event == Event::Return){
        if(bet > 0 && bet <= game.chips()){
          game.startRound(bet);
          screen.PostEvent(Event::Custom);
          return true;
        }
       
      }
    }

      if(game.phase() == Phase::PlayerTurn){
        if (event == Event::Character(' ')){
          game.hit();
          screen.PostEvent(Event::Custom);
          return true;
        }
        if (event == Event::Return){
          game.stand();
          screen.PostEvent(Event::Custom);
          return true;
        }
      }

      if(game.phase() == Phase::RoundOver){
        if (event == Event::Return){
          game.nextRound();
          screen.PostEvent(Event::Custom);
          return true;
        }
      }
      if(game.phase() == Phase::GameOver){
        if (event == Event::Return){
          bet = 10;
          game.restart();
          screen.PostEvent(Event::Custom);
          return true;
        }
      }

   return false;
  });

  screen.Loop(app);
}