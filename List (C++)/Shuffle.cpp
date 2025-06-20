/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA5
* File: Shuffle.cpp
* Description: Client for List ADT
***/

#include<stdexcept>
#include<iomanip>
#include "List.h"

using namespace std;

void shuffle(List& source) {
    int totalSize = source.length();
    if (totalSize < 2) return;
    
    List firstHalf, secondHalf;
    
    // Split the source list into two halves
    source.moveFront();
    for (int i = 0; i < totalSize / 2; i++) {
        firstHalf.insertBefore(source.moveNext());
    }
    while (source.position() < totalSize) {
        secondHalf.insertBefore(source.moveNext());
    }

    // Clear the source list for interleaving
    source.clear();
    
    firstHalf.moveFront();
    secondHalf.moveFront();
    
    // Interleave elements from secondHalf and firstHalf into the source
    while (secondHalf.position() < secondHalf.length()) {
        source.insertBefore(secondHalf.moveNext());
        if (firstHalf.position() < firstHalf.length()) {
            source.insertBefore(firstHalf.moveNext());
        }
    }

}

int main(int argc, char* argv[]) {
    int deckSize, shuffleCount, maxCards;
    List originalDeck, shuffledDeck;

    // Check if the correct number of arguments are provided
    if (argc != 2) {
        throw invalid_argument("Usage: Shuffle <int>, incorrect number of arguments");
    }

    // Parse the number of cards to be shuffled
    maxCards = stoi(argv[1]);
    if (maxCards <= 0) {
        throw invalid_argument("Argument must be positive int");
    }

    // Print table header
    cout << "Deck Size\tShuffle Count" << endl;
    string divider(30, '-');
    cout << divider << endl;

    // Loop through different deck sizes and count shuffles needed to return to original order
    for (deckSize = 1; deckSize <= maxCards; deckSize++) {
        shuffleCount = 0;

        // Initialize the deck with cards 0 to deckSize - 1
        originalDeck.clear();
        for (int i = 0; i < deckSize; i++) {
            originalDeck.insertBefore(i);
        }

        // Copy the deck to start shuffling
        shuffledDeck = originalDeck;
        shuffle(shuffledDeck);
        shuffleCount++;

        // Continue shuffling until the shuffled deck matches the original
        while (!(shuffledDeck == originalDeck)) {
            shuffle(shuffledDeck);
            shuffleCount++;
        }

        // Print the results for the current deck size
        cout << " " << left << setw(16) << deckSize << shuffleCount << endl;
    }
    return EXIT_SUCCESS;
}