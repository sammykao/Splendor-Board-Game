/*
 * card.h
 * Written by: Sammy Kao 
 * Date: 12/1/2022
 * Purpose: Define a Card class to represent a card in splendor game.
 *
 */


#ifndef	CARD_H
#define	CARD_H
#include <string>

using namespace std;


class Card {
public:
    Card(int prestige, string gemcolor, int *p);
    int getPrestige();
    int getPrice(int color);
    string getGemColor();
private:
    int prestige;
    string gemcolor;
    int price[5];
};



#endif