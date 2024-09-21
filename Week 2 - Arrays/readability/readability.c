#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = count_letters(text);
    int words = count_words(text) + 1;
    int sentences = count_sentences(text);

    // Compute the Coleman-Liau index
    float letter = (float) letters;
    float word = (float) words;
    float sentence = (float) sentences;

    float L = (letter / word) * 100;
    float S = (sentence / word) * 100;

    float grade = 0.0588 * L - 0.296 * S - 15.8;

    // Print the grade level
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(grade));
    }
}

int count_letters(string text)
{
    // Return the number of letters in text
    int letter = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {   
            letter++;
        }
    }
    return letter;
}

int count_words(string text)
{
    // Return the number of words in text
    int word = 0;
    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (text[i] == ' ')
        {
            word++;
        }
    }
    return word;
}

int count_sentences(string text)
{
    // Return the number of sentences in text
    int sentence = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence++;
        }
    }
    return sentence;
}
