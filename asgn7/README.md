# Assignment #7: The Great Firewall of Santa Cruz - Bloom Filters, Binary Search Trees and Hash Tables
Serjo Barron

sepbarro

Fall 2021

## [files]
 - banhammer.c
 - messages.h
 - salts.h
 - speck.h
 - speck.c
 - ht.h
 - ht.c
 - bst.h
 - bst.c
 - node.h
 - node.c
 - bf.h
 - bf.c
 - bv.h
 - bv.c
 - parser.h
 - parser.c
 - Makefile

## [description]
This program implements a scenario in which you are the glorious leader of the Glorious People's Republic of Santa Cruz (GPRSC for short). And as the leader of this nation you must keep what the citizens say in check. To do this, you are using two data structures: a hashtable of binary search trees (BST's) and a bloomfilter.

A list of badspeak words and newspeak words (oldspeak words with their newspeak translations) will be read and stored in these two data structures (hashtable and bloomfilter). After having a reliable way of storing and looking up words, you can then begin reading in words using a parser and search through the bloomfilter and hashtable to see if a person is in need of a reprimanding message to alert them of their crimes.

However, if no words have brought caution then the citizen is all and good and requires no reprimanding to fix their behavior.

## [instructions]
Once all the files are present (as listed above) in the right directory, compile the executable (banhammer) by running the command "make all". Once the executable (banhammer) has been made, run the command "./banhammer -h" to be prompted with a synopsis message of the available CLI options, what they mean, and how to use them.

example:

./banhammer\
(the program will now read from stdin and will do so until specified to stop)\
(you can type any words and it will be read according to the regex and parser ADT)\
a\
b\
c\
d\
(use ctrl + d to specify the end of stdin)

The example above has read words "a, b, c, and d" and will then categorize those words in two piles: bad words and new words if it does exist when looking it up in the bloomfilter and hashtable. Otherwise, the word is not added in either of the piles. Once "ctrl + d" is used to end reading from stdin, the program will then decide to print out the reprimanding message alerting the citizen of their misbehavior if it was conducted.
