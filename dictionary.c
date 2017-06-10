/**
 * Implements a dictionary's functionality.
 * Used http://www.geeksforgeeks.org/trie-insert-and-search/ as a source for base trie sturcture
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// max word length LENGTH is defined at 45

#define ARR_SIZE 27

// Creating trie node, alphabet size + apostrophe
typedef struct node
{
    bool isWord;
    struct node *children[ARR_SIZE];
}node;

// Creating a trie node
node *createNode(void)
{
    node *newNode = NULL;
    // Mallocing memory
    newNode = (node*)malloc(sizeof(node));
    // If malloc is successful
    if(newNode != NULL)
    {
        // Initialize isWord to zero and elements to NULL
        newNode->isWord = 0;
        for(int i = 0; i < ARR_SIZE; i++)
        {
            newNode->children[i] = NULL;
        }
    }
    else
    {
        printf("Failed to add node.\n");
    }
    return newNode;
}

// Initializing root and dictionary word count
node *root = NULL;
unsigned int *wordCount;

// Adding a dictionary word to trie
void addWord(node *root, const char *word)
{
    int level;
    int length = strlen(word);
    int i;
    
    // Starting at root
    node *travNode = root;
    
    // For all letters in the word
    for(level = 0; level < length; level++)
    {
        // Assigning index value 0-25, 26 for apostrophe
        if(word[level] == '\'')
        {
            i = 26;
        }
        // Case insensitive
        else
        {
            i = toupper(word[level])-65;
        }
        // If a children node is not yet created
        if(!travNode->children[i])
        {
            // Create a node
            travNode->children[i] = createNode();
        }
        // Traverse to next node
        travNode = travNode->children[i];
    }
    // At last letter, declare end of valid word variable
    travNode->isWord = 1;
}

// Searching for a valid text word within the dictionary trie
bool search(node *root, const char *textWord)
{
    int level;
    int length = strlen(textWord);
    int i;
    
    node *searchNode = root;
    
    // Iterating through the textWord letters
    for(level = 0; level < length; level++)
    {
        if(textWord[level] == '\'')
        {
            i = 26;
        }
        // Case insensitive
        else
        {
            i = toupper(textWord[level])-65;
        }
        // If a letter does not match the dicitionary, return false
        if(!searchNode->children[i])
        {
            return 0;
        }
        // Else continue traversing deeper into the trie
        searchNode = searchNode->children[i];
    }
    // If at end of textWord, if isWord is true and node is not NULL
    // Return true as textWord is found in dictionary
    return(searchNode != NULL && searchNode->isWord);
}

// Input of a root node, function traverses the children nodes
// and recursively frees the malloc
bool freeNodes(node *root)
{
    int i;
    // Base case, input node is NULL, return 0
    if(!root)
    {
        return 0;
    }
    for(i = 0; i < 27; i++)
    {
        if(root->children[i] != NULL)
        {
            freeNodes(root->children[i]);
        }
    }
    // Main root freed, return 1
    free(root);
    return 1;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    bool returnVal = search(root, word);
    return returnVal;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // Initialize root
    root = createNode();
    
    // Open dictionary file
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return 0;
    }
    
    // Initialize and malloc space for dicitionary word count
    wordCount = (unsigned int*)malloc(sizeof(unsigned int));
    *wordCount = 0;
    
    char ch;
    
    // Using fgetc to read until end of file
    while((ch = fgetc(inptr)) != EOF)
    {
        // Rewind the fgetc
        fseek(inptr, -sizeof(char), SEEK_CUR);
        
        char wordArray[LENGTH];
        // Scanning one word at the time
        while (fscanf(inptr, " %45s", wordArray) == 1) 
        {
            // Adding the word to the trie
            addWord(root,wordArray);
            // Incrementing dictionary word count
            ++(*wordCount);
        }
    }
    fclose(inptr);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 * Stores the word count from load, frees the malloc
 */
unsigned int size(void)
{
    unsigned int numberOfWords = *wordCount;
    free(wordCount);
    return numberOfWords;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    bool returnVal = freeNodes(root);
    
    return returnVal;
}
