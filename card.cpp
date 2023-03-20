/*
 * card.cpp
 * Written by: Sammy Kao
 * Date: 12/1/22
 * Purpose: Implement the methods for the Card class in the Splendor game.
 *
 */

#include "card.h"
#include <string>
#include <iostream>

// int main() {
//     int p[5];
//     for (int i=0; i < 5; i++) {
//         p[i] = 2;
//     }
//     Card card1 = Card(1, "green", p);
//     cout << card1.getPrestige();
//     cout << " is the prestige point value" << endl;
//     cout << card1.getGemColor();
//     cout << " is the discount color" << endl;
//     for (int i=0; i < 5; i++) {
//         cout << " The #" << i << " price is: ";
//         cout << card1.getPrice(i) << endl;
//     }
//     return 0;
// }


// Constructor: gets passed in values to create a card
// as well as a price array, to which a pointer of arrays 
// is passed in
Card::Card(int prestige, string gemcolor, int *p) {
    this->prestige = prestige;
    this->gemcolor = gemcolor;
    for(int i=0; i < 5; i++) {
        price[i] = p[i];
    }

}


// getPrestige()
// Input: No input needed
// Description: retrieve our private prestige value
// Output: returns prestige integer
int Card::getPrestige() {
    return prestige;
}

// getGemColor()
// Input: No input needed
// Description: retrieve our private gemcolor string
// Output: returns prestige string
string Card::getGemColor() {
    return gemcolor;
}

// getPrice()
// Input: the color of integer the price we want to get
// Description: retrieve our private price value
// Output: returns price integer, depending on which color
int Card::getPrice(int color) {
    return price[color];
}

