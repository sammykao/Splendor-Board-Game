/*
 * deck.cpp
 * Written by: Sammy Kao
 * Date: 12/1/22
 * Purpose: Implement the methods for the Card class in the Splendor game.
 *
 */


#include "deck.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Constructor: read in the numbers listed in a file, create a Card on
// the heap for each card, adjusting for deck type
Deck::Deck(string filename, string type) {
    ifstream infile(filename);
    if (infile.fail()) {
        cerr << "Failed to open deck file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    this->type = type;
    string cardtype, gemcolor;
    int prestige, prices[5];
    for (int i=0; i<8; i++) {
        infile >> cardtype;
    }
    int deckindex;
    if (type == "m") { 
        deckindex = 40;
    } else if (type == "t") {
        deckindex = 30;
    } else if (type == "v") {
        deckindex = 20;
    } else if (type == "n") {
        deckindex = 1;
    }
    while (!infile.eof()) {
        infile >> cardtype >> prestige >> gemcolor;
        for (int i=0; i<5; i++) {
            infile >> prices[i];
        }
        if (type == cardtype) {
            deck[deckindex - 1] = new Card(prestige, gemcolor, prices);
            if (type == "n") {
                deckindex += 2;
            }
            deckindex--;
        }
    }
    topCard = 0;
    infile.close();
}


// Destructor: deallocate any Card pointers remaining in the deck
Deck::~Deck() {
    if (type == "m") { 
        for (int i=40; i>0; i--) {
            delete deck[i - 1];
        }
    } else if (type == "t") {
        for (int i=30; i>0; i--) {
            delete deck[i - 1];
        }
    } else if (type == "v") {
        for (int i=20; i>0; i--) {
            delete deck[i - 1];
        }
    } else {
        for (int i=10; i>0; i--) {
            delete deck[i - 1];
        }
    }
}


// Empty()
// Input: No input needed
// Description: Check to see if the deck is empty by using
// top card value, adjusts for deck type
// Output: returns true if empty, else returns false

bool Deck::empty() {
  if (type == "m"){
    if (topCard == 40) {
        return true;
    }
  } else if (type == "t"){
    if (topCard == 30) {
        return true;
    }
  } else if (type == "v"){
    if (topCard == 20) {
        return true;
    }
  } else {
    if (topCard == 10) {
        return true;
    }
  }
  return false; 
}


// draw()
// Input: No input needed
// Description: draw a card by returning the "topCard"
// which is in a deck array. We also move the topCard 
// position one over
// Output: returns the topcard
Card *Deck::draw() {
    if (empty()) {
        cerr << "ERROR: Drawing from an empty deck" << endl;
        exit(EXIT_FAILURE);
    }
    // move the topcard one over so diff topcard
    topCard++;
    // draws the topcard 
    return deck[topCard - 1];
} 

// at()
// Input: Integer for index
// Description: show what is at the specific index given
// Output: returns the card at specific index
Card *Deck::at(int index) {
    return deck[index];
} 

//setNobles()
// Input: none
// Description: shift the cards in the noble
// deck one down
// Output: none
void Deck::setNobles(int i) {
    deck[i] = nullptr;
}