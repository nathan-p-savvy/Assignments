### Nathaniel Pacete

I Chose Blackjack for my game.

To build it:

cmake -S . -B build -G Ninja

cmake --build build

To run it
.\build\game.exe --test (runTests())

.\build\game.exe --text (text version)

.\build\game.exe (FTXUI version)

![Project Screenshot](screenshot.png)

There is a bug in the tuiview where it doesn't show the dealer's second card if you bust. Works fine in the console view. I would fix it, but my brain is actively melting.

I didn't make any changes to the deck or card class files.
