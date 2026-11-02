#include "ConsoleView.hpp"

#include <iostream>
#include <string>
#include <limits>

ConsoleView::ConsoleView(Blackjack &game) : game(game) {}

void ConsoleView::showHand() const{
    std::cout << "Dealer:  ";
    if(game.dealerHoleHidden()){
        std::cout << "[1] " << game.dealerHand().at(0).toString() << "  ";
        std::cout << "[2] ?  ";
        std::cout << "Value: " << game.dealerhiddenvalue();
    }
    else{
        for(int i = 0; i < game.dealerHand().size(); i++){
            std::cout << "[" << i + 1 << "] " << game.dealerHand().at(i).toString() << "  ";
        }
    std::cout << "Value: " << game.handValue(game.dealerHand());
    }
    std::cout << "\n";
    std::cout << "Player:  ";
        for(int i = 0; i < game.playerHand().size(); i++){
            std::cout << "[" << i + 1 << "] " << game.playerHand().at(i).toString() << "  ";
        }
    std::cout << "Value: " << game.handValue(game.playerHand()) << std::endl;
}

void ConsoleView::showOutcome() const{
    if(game.outcome() != Outcome::None){
        showHand();
    }

    switch (game.outcome()) {
        case Outcome::PlayerBlackjack: std::cout << "Blackjack! You Win!\n"; break;
        case Outcome::DealerWin: std::cout << "The Dealer Wins!\n"; break;
        case Outcome::PlayerWin: std::cout << "The Player Wins!\n"; break;
        case Outcome::DealerBust: std::cout << "The Dealer Busts! You Win!\n"; break;
        case Outcome::PlayerBust: std::cout << "Bust! The Dealer Wins!\n"; break;
        case Outcome::Push: std::cout << "Tie! Your bet returns.\n"; break;
        case Outcome::None: break;
    }
}

void ConsoleView::run(){
    std::string line;
    int num;
    while(true){
        showOutcome();

        if(game.phase() == Phase::Betting){
            std::cout << "Chips: " << game.chips() << "  Bet(1-" << game.chips() << ", q to quit) ";
            if(std::cin >> num){
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if(num > 0 && num <= game.chips()){
                std::cout <<"\nYou have bet " << num << (num == 1 ? " chip." : " chips.") 
                          << " Press t to start the round. ";
                }
                else{
                    std::cout << "Invalid Bet. Your bet must be between one and " << game.chips() << ".\n\n";
                    continue;
                }
            }
            else{
                std::cin.clear(); 
                std::cin >> line; 
        
                if (line == "q" || line == "Q") {
                std::cout << "Exiting game...\n";
                break; // Properly terminates the program (requires #include <cstdlib>)
            }
        
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid Input. Enter a number or 'q'.\n";
                continue;
            }
        }

        if(game.phase() == Phase::PlayerTurn){
            showHand();
            std::cout << "\nWould you like [H]it or [S]tand? (Press h/s to hit or stand) ";
        }

        if(game.phase() == Phase::RoundOver){
            std::cout << "\n[N]ext Round? (Press n to begin betting for the next round) ";
        }

        if(game.phase() == Phase::GameOver){
            std::cout << "\nGame Over! The House always wins... \n";
            std::cout << "\nPlay again? (Press y to play again, q to quit) ";
        }
        
        
        //Player Actions
        std::cin.clear();
        if (!std::getline(std::cin, line) || line == "q" || line == "Q"){
        std::cout << "Exiting game...\n";
        break;
        }

        if(line == "t" || line == "T"){
            game.startRound(num);
            continue;
        }

        if(line == "h" || line == "H"){
            game.hit();
            continue;
        }
        
        if(line == "s" || line == "S"){
            game.stand();
            continue;
        }

        if(line =="n" || line == "N"){
            game.nextRound();
            continue;
        }

        if(line == "y" || line == "Y"){
            game.restart();
            continue;
        }
    }
}
    
 