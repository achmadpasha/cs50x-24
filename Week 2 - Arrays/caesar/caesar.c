#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool only_digits(string s);
char rotate(char c, int key);

int main(int argc, string argv[])
{
    // Make sure program was run with just one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Make sure every character in argv[1] is a digit
    if (!only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Convert argv[1] from a `string` to an `int`
    int key = atoi(argv[1]);

    // Prompt user for plaintext
    string plaintext = get_string("plaintext: ");

    // For each character in the plaintext:
    printf("ciphertext: ");

    int i = 0;
    while (plaintext[i] != '\0')
    {
        // Rotate the character if it's a letter
        char c = rotate(plaintext[i], key);
        printf("%c", c);
        i++;
    }
    printf("\n");
}

bool only_digits(string s)
{
    int i = 0;

    while (s[i] != '\0')
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
        i++;
    }
    return true;
}

char rotate(char c, int key)
{
    if (isalpha(c))
    {
        if (isupper(c))
        {
            char ciphertext = ((c - 'A' + key) % 26) + 'A';
            return ciphertext;
        }
        else
        {
            char ciphertext = ((c - 'a' + key) % 26) + 'a';
            return ciphertext;
        }
    }
    else
    {
        return c;
    }
}
