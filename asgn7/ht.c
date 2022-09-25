#include "ht.h"
#include "bst.h"
#include "salts.h"
#include "speck.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint64_t lookups; // extern variable

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

// Usage: Constructor for a hash table. This will set the salt with the specified
//        low and high 64-bit macros defined in salts.h. And then create an array
//        of node pointers, each of which should be the root of a BST.
//
// Parameters: size of hash table.
// Return value: hash table.
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->size = size;
        ht->trees = (Node **) calloc(size, sizeof(Node *));
        if (!ht->trees) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

// Usage: Destructor for a hash table.
//
// Parameters: address of a hash table.
// Return value: void.
void ht_delete(HashTable **ht) {
    if (*ht && (*ht)->trees) {
        for (uint32_t i = 0; i < (*ht)->size; i += 1) {
            if ((*ht)->trees[i]) {
                bst_delete(&(*ht)->trees[i]);
            }
        }
        free((*ht)->trees);
        free(*ht);
        *ht = NULL;
    }
}

// Usage: This function will return the size of a hash table. Where
//        the size means the capacity of BST's of the hash table.
//
// Parameters: hash table.
// Return value: size of hash table.
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// Usage: This function will lookup a specific BST using an index calculated by
//        hashing the salt and oldspeak. Then a call to bst_find() will check
//        whether oldspeak exists in the BST specified by the index. Otherwise,
//        a NULL pointer is returned meaning there is no match of oldspeak.
//
// Parameters: hash table and oldspeak.
// Return value: node containing oldspeak or NULL pointer.
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups += 1;
    uint32_t i = hash(ht->salt, oldspeak) % ht->size;
    if (ht->trees[i]) { // tree exists and oldspeak matches
        return bst_find(ht->trees[i], oldspeak);
    }
    return NULL;
}

// Usage: This function will insert a node containing oldspeak and newspeak
//        into the hash table. The index of the BST to insert that node into
//        is once again calculated by hashing the salt and oldspeak. If the
//        BST being inserted to has not been created yet, then it will be created
//        and then be inserted to.
//
// Parameters: hash table, oldspeak, and newspeak.
// Return value: void.
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups += 1;
    uint32_t i = hash(ht->salt, oldspeak) % ht->size;
    if (ht->trees[i]) {
        ht->trees[i] = bst_insert(ht->trees[i], oldspeak, newspeak);
        return;
    }
    // if there is no BST (root is NULL) create one and insert oldspeak, newspeak
    ht->trees[i] = bst_create();
    ht->trees[i] = bst_insert(ht->trees[i], oldspeak, newspeak);
    return;
}

// Usage: This function will count the number of non-NULL BST's in the hash table.
//
// Parameters: hash table.
// Return value: number of non-NULL BST's.
uint32_t ht_count(HashTable *ht) {
    uint32_t num_bst = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if (ht->trees[i]) {
            num_bst += 1;
        }
    }
    return num_bst;
}

// Usage: This function will return the average BST size of all trees in the hash
//        table. This is calculated as the sum of non-NULL BST sizes divided by
//        the number of non-NULL BST's.
//
// Parameters: hash table.
// Return value: average BST size of hash table.
double ht_avg_bst_size(HashTable *ht) {
    uint32_t size_sum = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        size_sum += bst_size(ht->trees[i]);
    }
    double avg_size = (double) size_sum / (double) ht_count(ht);
    return avg_size;
}

// Usage: This function will return the average BST height of all trees in the
//        hash table. This is calculated as the sum of non-NULL BST heights
//        divided by the number of non-NULL BST's.
//
// Parameters: hash table.
// Return value: average BST height of hash table.
double ht_avg_bst_height(HashTable *ht) {
    uint32_t height_sum = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        height_sum += bst_height(ht->trees[i]);
    }
    double avg_height = (double) height_sum / (double) ht_count(ht);
    return avg_height;
}

// Usage: Debugger function to print the non-NULL trees of the hash table.
//
// Parameters: hash table.
// Return value: void.
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if (ht->trees[i]) {
            bst_print(ht->trees[i]);
            printf("-----------------------\n");
        }
    }
}
