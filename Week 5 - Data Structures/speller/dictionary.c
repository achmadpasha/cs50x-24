// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 143091;
unsigned int counter = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Hash word to obtain a hash value
    unsigned int INDEX = hash(word);
    // Access linked list at that index in the has table
    node *CURSOR = table[INDEX];

    // Keep checking each node for the word, untill you get to NULL
    while (CURSOR != NULL)
    {
        // Compare two strings case-insensitively
        if (strcasecmp(CURSOR->word, word) == 0)
        {
            // Return true if the word is in dictionary, false otherwise
            return true;
        }
        CURSOR = CURSOR->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int VALUE = 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        VALUE += tolower(word[i]);
    }

    if (VALUE > N)
    {
        VALUE %= N;
        return VALUE;
    }
    else
    {
        return VALUE;
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open the dictionary file
    FILE *DICTIONARIES = fopen(dictionary, "r");
    // Remember to check if return value is NULL
    if (DICTIONARIES == NULL)
        return 1;

    char BUFFER[LENGTH + 1];

    // Read strings from file one at time
    while (fscanf(DICTIONARIES, "%s", BUFFER) != EOF)
    {
        // Create a new node for each word
        node *NEW_WORD = malloc(sizeof(node));
        // Remember to check if return value is NULL
        if (NEW_WORD == NULL)
            return 1;

        // Copy word into node
        strcpy(NEW_WORD->word, BUFFER);
        // Hash word to obtain a hash value
        int INDEX = hash(NEW_WORD->word);

        // Insert node into hash table at that location
        if (table[INDEX] != NULL)
        {
            NEW_WORD->next = table[INDEX]->next;
            table[INDEX]->next = NEW_WORD;
        }
        else
        {
            NEW_WORD->next = NULL;
            table[INDEX] = NEW_WORD;
        }
        counter++;
    }

    // Close the dictionary file and return true
    fclose(DICTIONARIES);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *CURSOR, *temp;

    for (int i = 0; i < N; i++)
    {
        CURSOR = table[i];
        while (CURSOR != NULL)
        {
            temp = CURSOR;
            CURSOR = CURSOR->next;
            free(temp);
        }
    }
    free(CURSOR);
    return true;
}
