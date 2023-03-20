/*
 * player.h
 * Written by: Sammy Kao 
 * Date: 12/1/2022
 * Purpose: Define a PLayer class to represent a card in splendor game.
 *
 */

#ifndef	PLAYER_H
#define	PLAYER_H
#include "card.h"
using namespace std;


class Player {
public:
    Player();
    ~Player();
    int totalPrestige();
    int getGemCount(int color);
    bool PlayerGemChecker();
    int getDiscount(int color);
    Card *getReserve(int index);
    void setTotalPrestige(int amt);
    void setGemCount(int color, int amt);
    void setDiscount(int color, int amt);
    void emptyReserve(int index);
    void setReserve(int index, Card *reserveCard);
    bool enoughGems(Card *purchaseCard);
    void purchaseCard(Card *purchasedCard);
    int getPurchases();
    
private:
    int purchases;
    int prestige;
    int gemcount[6];
    int discount[5];
    Card *reserve[3];
};



#endif