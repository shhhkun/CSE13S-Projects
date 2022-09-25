#include "bf.h"
#include "ht.h"
#include "bst.h"
#include "parser.h"
#include "speck.h"
#include "messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define OPTIONS "ht:f:s"

// Regex: This regular expression will handle a single word like "word", hyphenated words
//        such as "one-sided", and contracted words such as "i'm". A word can consist of
//        the alphabetical letters (both lower and uppercase), numbers (0-9), and underscores.
#define WORD "([a-zA-Z0-9_]+)|(([a-zA-Z0-9_]+[-'])+[a-zA-Z0-9_]+)"

static bool header = false;
static bool stats = false;

static uint32_t size_ht = 0x10000; // 2^16
static uint32_t size_bf = 0x100000; // 2^20

// Usage: Helper function that converts the characters of a string 's' to lowercase if
//        it is not already, and will return the lowercase string as a copy.
//
// Parameters: a string 's'.
// Return value: a copy of string 's', which will be all lowercase.
char *stringlower(char *s) {
    char *copy = calloc(strlen(s) + 1, sizeof(char)); // copy of string (in lowercase)
    for (uint32_t i = 0; i < strlen(s); i += 1) {
        copy[i] = tolower(s[i]); // set each character in the string 's' to lowercase
    }
    return copy;
}

int main(int argc, char **argv) {
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': {
            header = true;
            break;
        }
        case 't': {
            size_ht = atoi(optarg);
            break;
        }
        case 'f': {
            size_bf = atoi(optarg);
            break;
        }
        case 's': {
            stats = true;
            break;
        }
        default: {
            header = true;
            break;
        }
        }
    }
    if (header) {
        printf("SYNOPSIS\n"
               "  A word filtering program for the GPRSC.\n"
               "  Filters out and reports bad words parsed from stdin.\n"
               "\n"
               "USAGE\n"
               "  ./banhammer [-hs] [-t size] [-f size]\n"
               "\n"
               "OPTIONS\n"
               "  -h           Program usage and help.\n"
               "  -s           Print program statistics.\n"
               "  -t size      Specify hash table size (default: 2^16).\n"
               "  -f size      Specify Bloom filter size (default: 2^20).\n");
        return 0;
    }

    HashTable *ht = ht_create(size_ht);
    BloomFilter *bf = bf_create(size_bf);
    regex_t re;

    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        ht_delete(&ht);
        bf_delete(&bf);
        regfree(&re);
        return 1;
    }

    FILE *bd_speak = fopen("badspeak.txt", "r");
    char *word = NULL;

    // populate bf and ht with badspeak words
    while ((word = next_word(bd_speak, &re)) != NULL) {
        bf_insert(bf, word);
        ht_insert(ht, word, NULL);
    }
    clear_words(); // reset words
    fclose(bd_speak);

    FILE *nw_speak = fopen("newspeak.txt", "r");

    // populate bf and ht with newspeak words
    while ((word = next_word(nw_speak, &re)) != NULL) {
        bf_insert(bf, word);
        char *old = strdup(word); // oldspeak
        word = next_word(nw_speak, &re); // next word is newspeak
        ht_insert(ht, old, word);
        free(old);
        old = NULL;
    }
    clear_words(); // reset words
    fclose(nw_speak);

    Node *bad_words = bst_create();
    Node *new_words = bst_create();
    Node *n = NULL; // node to hold looked up word in ht

    while ((word = next_word(stdin, &re)) != NULL) {
        char *lword = stringlower(word); // copy of word in lowercase
        if (bf_probe(bf, lword)) {
            if ((n = ht_lookup(ht, lword)) != NULL) {
                if (n->newspeak) { // there is an oldspeak and newspeak
                    new_words = bst_insert(new_words, n->oldspeak, n->newspeak);
                } else { // there is only oldspeak
                    bad_words = bst_insert(bad_words, n->oldspeak, NULL);
                }
            }
        }
        free(lword); // free the copy (lowercase) word
        lword = NULL;
    }
    clear_words(); // reset words

    bool bad_exist = bst_size(bad_words) > 0; // there exists badspeak words in the BST
    bool new_exist = bst_size(new_words) > 0; // there exists newspeak words in the BST

    // if stats are toggled off, then print reprimanding messages
    if (!stats) {
        if (bad_exist && new_exist) {
            printf("%s", mixspeak_message);
            bst_print(bad_words);
            bst_print(new_words);
        }
        if (bad_exist && !new_exist) {
            printf("%s", badspeak_message);
            bst_print(bad_words);
        }
        if (!bad_exist && new_exist) {
            printf("%s", goodspeak_message);
            bst_print(new_words);
        }
    } else { // otherwise only print the stats
        printf("Average BST size: %.6lf\n", ht_avg_bst_size(ht));
        printf("Average BST height: %.6lf\n", ht_avg_bst_height(ht));

        double avg_traversed = (double) branches / (double) lookups;
        double ht_load = 100.0 * (double) ht_count(ht) / (double) ht_size(ht);
        double bf_load = 100.0 * (double) bf_count(bf) / (double) bf_size(bf);

        printf("Average branches traversed: %.6lf\n", avg_traversed);
        printf("Hash table load: %.6lf%%\n", ht_load);
        printf("Bloom filter load: %.6lf%%\n", bf_load);
    }

    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&bad_words);
    bst_delete(&new_words);
    regfree(&re);
    return 0;
}
