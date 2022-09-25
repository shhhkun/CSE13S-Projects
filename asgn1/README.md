# Assignment 1: Pass the Pigs
Serjo Barron

sepbarro

Fall 2021

## [files]
- pig.c
- Makefile
- names.h

## [description]
This program imitates a game called "Pass the Pigs", where each player rolls a pig (an asymmetrical die) to land side, back, upright, snout, or ear. Each land has its own point allocation, and some of the lands occur more than once.
The roll has a total sample space of 7 with 2 sides, 1 back, 1 upright, 1 snout, and 2 ears. The point system is 15 pts for landing snout, 10 pts for back or upright, 5pts for ears, and a special case for side which rewards 0 pts
and ends the current players turn. The game cycles through its players in a circular fashion and ends when one of the players gets a total of 100 or more points.

## [instructions]
Open virtual box and start your server, next move to the proper directory (cse13s/asgn1) then use the ls command to make sure all the files needed are present. Once you are sure all files are present, use the make command to trigger
the Makefile in the directory. The Makefile will allow to automatically compile and clang-format the required C code to run the game/program. After all files have been properly formatted and compiled as as executable, run the command
./pig to start the game.
Once the game has begun, you will be prompted with the message "How many players? ", the allowed range for the input is 2 to 10 inclusive. If the user-input is out of range the program will instead use the default number of players
(2). The next message asks the user "Random seed: ", which the user will choose what seed the game Pass the Pigs will run on. The seed must be an unsigned integer (32-bit), if the user-input is out of range the program will use
a default seed (2021).
After the number of players and seed for the game has been decided, the game will automatically roll the pig and prompt messages indicating what each player rolled. Once a player has won the game (getting 100 or more total points), 
the program will prompt a message of the player who won and terminate.


