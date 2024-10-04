import cs50


def main():
    # Prompt the use for some text
    text = cs50.get_string("Text: ")

    # Count the number of letters, words, and sentences in the text
    letter = float(count_letter(text))
    word = float(count_word(text) + 1)
    sentence = float(count_sentence(text))

    # Compute the Coleman-Liau index
    l = (letter / word) * 100
    s = (sentence / word) * 100
    grade = 0.0588 * l - 0.296 * s - 15.8

    # Print the grade level
    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {int(round(grade))}")


def count_letter(text):
    letter = 0
    for i in range(len(text)):
        if text[i].isalpha():
            letter += 1
    return letter


def count_word(text):
    word = 0
    for i in range(len(text)):
        if text[i] == ' ':
            word += 1
    return word


def count_sentence(text):
    sentence = 0
    for i in range(len(text)):
        if text[i] == '.' or text[i] == '!' or text[i] == '?':
            sentence += 1
    return sentence


main()
