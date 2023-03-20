/*
 * game.h
 * CS11 Splendor
 * Edited by: Sammy Kao
 * Date: 12/8/22
 * Purpose: Define a Game class to represent a Game in splendor game.
 */

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "card.h"
#include "player.h"
#include "deck.h"
using namespace std;

class Game {
  public:
    //constructor
    Game(string filename, string play_nobles);

    ~Game();

    //main gameplay
    void playGame();

    //utility functions
    string indexToColor(int index);
    bool getDiffGems(Player *player);
    bool getSameGems(Player *player);
    void setGemBank(int color, int amount);
    bool SetOverGems(Player *player, int playerNum);
    void drawreset(int vpos, int hpos);
    bool buyDeckCard(Player *player);
    bool buyReserveCard(Player *player);
    bool reserveCard(Player *player);
    bool inputChecker(string move);
    bool colorChecker(string color);
    int typeChecker(string vpos);
    bool FullGemChecker(string color);
    bool GemChecker(string color);
    bool columnChecker(int hpos);
    bool takeTurn(string move, int playerNum);
    void getNobles(Player *player);
    void oneMoreTurn(int playerNum);
    int lessPurchases();

  private:
    //drawing functions
    void drawBoard();
    void drawNoble(int vpos, int hpos, Card *c);
    void drawPlayer(int vpos, int hpos, Player *p);
    void drawDeck(int vpos, int hpos, string type);
    void drawCard(int vpos, int hpos, Card *c);
    void drawGem(int vpos, int hpos, string color);
    void clearBoard();
    void display();
    
    static const int BOARD_HEIGHT = 41;
    static const int BOARD_WIDTH = 120;
    static const int CARD_WIDTH = 13;
    static const int CARD_HEIGHT = 8;
    static const int CARD_ROWS = 3;
    static const int CARD_COLS = 4;
    static const int NUM_NOBLES = 3;
    static const int GEMS = 6;

    //nobles activated true/false
    bool play_nobles;

    //cards in play
    Card *grid[CARD_ROWS][CARD_COLS];

    //display
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    string board_color[BOARD_HEIGHT][BOARD_WIDTH];

    //Gem colors in the game
    string colors[GEMS];
    //all possible decks
    Deck *mines;
    Deck *transport;
    Deck *vendors;
    Deck *nobles;
    //gembanks and players
    int gem_bank[6];
    Player *players[2];
};

#endif
