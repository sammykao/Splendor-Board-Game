/*
 * deck.h
 * Written by: Sammy Kao 
 * Date: 12/1/2022
 * Purpose: Define a Deck class to represent a card in splendor game.
 *
 */


#ifndef _DECK_H
#define _DECK_H

#include "card.h"
#include <string>

using namespace std;

class Deck {
public:
    Deck(string filename, string type);

    ~Deck();

    bool empty();
    Card *at(int index);
    Card *draw();
    void setNobles(int i);

private:
    int topCard;
    string type;
    Card *deck[40];

};

#endif // _DECK_H