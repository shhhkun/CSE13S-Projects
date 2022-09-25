#include "names.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// The following two lines of code (enumeration) has been provided from the assignment pdf.
typedef enum { SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER } Position;
const Position pig[7] = { SIDE, SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER, JOWLER };

int main(void) {
    int players;
    int64_t SEED;
    // Ask user for the number of players, if its invalid use the default number of players.
    {
        printf("How many players? ");
        scanf("%d", &players);
    }
    if (players < 2 || players > 10) {
        fprintf(stderr, "Invalid number of players. Using 2 instead.\n");
        players = 2;
    }
    // Ask user for the seed the game will run on, if invalid use the default seed.
    {
        printf("Random seed: ");
        scanf("%" SCNd64, &SEED);
    }
    if (SEED < 0 || SEED > 4294967295) {
        fprintf(stderr, "Invalid random seed. Using 2021 instead.\n");
        SEED = 2021;
    }

    int scores[players];
    for (int c = 0; c < players; c++) {
        scores[c] = 0;
    }
    srandom(SEED);
    int winner = false;

    while (winner == false) {
        for (int i = 0; i < players; i++) {
            printf("%s rolls the pig...", names[i]);
            int side_roll = false;
            while (side_roll == false) {
                int roll = random();
                // Takes the pseudo-randomly generated number (roll) and does a modulus operation on it by 7.
                // The result is the remainder ranging from 0 to 6 inclusive which will represent and map to all the possible ways the pig can land.
                int result = roll % 7;
                // The pig[result] will take the 0 to 6 result and give SIDE, RAZORBACK, TROTTER, SNOUTER, or JOWLER of the enum values based on what pig[result] is.
                switch (pig[result]) {
                case SIDE:
                    printf(" pig lands on side\n");
                    side_roll = true;
                    break;
                case RAZORBACK:
                    printf(" pig lands on back");
                    scores[i] = scores[i] + 10;
                    break;
                case TROTTER:
                    printf(" pig lands upright");
                    scores[i] = scores[i] + 10;
                    break;
                case SNOUTER:
                    printf(" pig lands on snout");
                    scores[i] = scores[i] + 15;
                    break;
                default:
                    printf(" pig lands on ear");
                    scores[i] = scores[i] + 5;
                    break;
                }
                // If the current player has a score equal to or greater than 100 the program/game terminates.
                if (scores[i] >= 100) {
                    printf("\n%s wins with %d points!\n", names[i], scores[i]);
                    return 0;
                }
            }
        }
    }
}
