/*
 * player.cpp
 * Written by: Sammy Kao
 * Date: 12/1/22
 * Purpose: Implement the methods for the player class in the Splendor game.
 *
 */


#include "player.h"
#include "card.h"
#include <string>
#include <iostream>
//Constructor: creates a player and initializes
// base values for players
Player::Player() {
    prestige = 0;
    purchases = 0;
    for (int i=0; i < 5; i++) {
        gemcount[i] = 0;
        discount[i] = 0;
    }
    for (int i=0; i < 3; i++) {
        reserve[i] = nullptr;
    }
}

//Destructor: deletes the reserve
Player::~Player() {
    for (int i=0; i < 3; i++) {
        if (reserve[i] != nullptr) {
            delete reserve[i];
        }
    }
}



// totalPrestige()
// Input: No input needed
// Description: retrieve our private prestige value
// Output: returns prestige integer
int Player::totalPrestige() {
    return prestige;
}

// getGemCount()
// Input: the integer index of color
// Description: retrieve our private gemcount value
// Output: returns gemcount integer based on color
int Player::getGemCount(int color) {
    return gemcount[color];
}

// getDiscount()
// Input: the integer index of color
// Description: retrieve our private discount value
// Output: returns discount integer based on color
int Player::getDiscount(int color) {
    return discount[color];
}

// getReserve()
// Input: index of what card
// Description: retrieve a card in the reserve
// Output: returns card based on index integer
Card *Player::getReserve(int index) {
    return reserve[index];
}

// setTotalPrestige()
// Input: amt to add
// Description: change our private prestige amt
// Output: does not return anything
void Player::setTotalPrestige(int amt) {
    prestige += amt;
}

// setGemCount()
// Input: amt to add and what color
// Description: change our private gemcount amt
// based on the color and amt
// Output: does not return anything
void Player::setGemCount(int color, int amt) {
    gemcount[color] += amt;
    if (gemcount[color] < 0) {
        gemcount[color] = 0;
    }
}

/// setDiscount()
// Input: amt to add and what color
// Description: change our private discount amt
// based on the color and amt
// Output: does not return anything
void Player::setDiscount(int color, int amt) {
    discount[color] += amt;
}

// emptyReserve()
// Input: integer of index 
// Description: empties that reserve position 
// Output: none
void Player::emptyReserve(int index) {
    reserve[index] = nullptr;
}


// setReserve()
// Input: integer of index, card that player reserves
// Description: makes the reserve at the 
// index point to the card now
// Output: none
void Player::setReserve(int index, Card *reserveCard) {
    reserve[index] = reserveCard;
}

// enoughGems()
/// setDiscount()
// Input: Card that player wants to buy
// Description: Compare the player's gem count
// and determine whether the player can buy the
// card
// Output: Return true if they can, false if not
bool Player::enoughGems(Card *purchaseCard) {
    int difference = 0;
    for(int i=0; i<5; i++) {
        if (getGemCount(i) < purchaseCard->getPrice(i)) {
            if (!(getDiscount(i) >= (purchaseCard->getPrice(i) - getGemCount(i)))) {
                difference += (purchaseCard->getPrice(i) - (getGemCount(i) + getDiscount(i)));
            }
        }
    }
    if (getGemCount(5) < difference) {
        cout << "Invalid move (not Enough Gems) - enter a valid move: ";
        return false;
    }
    return true;
}

// PlayerGemChecker()
// Input: none
// Description: checks to see if player has more 
// than 10 gems
// Output: return false if they do, true otherwise
bool Player::PlayerGemChecker() {
    int total = 0;
    for (int i=0; i < 6; i++) {
        total += getGemCount(i);
    }
    if (total > 10) {
        return false;
    }
    return true;
}

// purchaseCard()
// Input: card being purchased
// Description: add prestige, discount, of card to
// player values, and subtracts the gems from
// player gem count accounting for discount
// Output: none

void Player::purchaseCard(Card *purchasedCard) {
    purchases++;
    setTotalPrestige(purchasedCard->getPrestige());
    string discount = purchasedCard->getGemColor();
    for(int i=0; i<5; i++) {
        if (getDiscount(i) < purchasedCard->getPrice(i)) {
            setGemCount(i, -(purchasedCard->getPrice(i)));
        }
    }
    int difference = 0;
    for(int i=0; i<5; i++) {
        if (getGemCount(i) < purchasedCard->getPrice(i)) {
            if ((getDiscount(i) < (purchasedCard->getPrice(i) - getGemCount(i)))) {
                difference += (purchasedCard->getPrice(i) - (getGemCount(i) + getDiscount(i)));
            }
        }
    }
    setGemCount(5, -(difference));
    if (discount == "white") {
        setDiscount(0, 1);
    } else if (discount == "red") {
        setDiscount(1, 1);
    } else if (discount == "pink") {
        setDiscount(2, 1);
    } else if (discount == "blue") {
        setDiscount(3, 1);
    } else if (discount == "green") {
        setDiscount(4, 1);
    }
}

//getPurchases
//Input: none
//Description: get our private purchases value
//Output: return number of purchases for player
int Player::getPurchases() {
    return purchases;
}