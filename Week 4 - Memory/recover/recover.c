#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 2;
    }

    FILE *img = NULL;

    char filename[8];
    int counter = 0;

    // Create a buffer for a block of data
    typedef uint8_t BYTE;
    BYTE buffer[BLOCK_SIZE];

    // Read 512 bytes into a buffer, repeat untill end of card
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, card) == 512)
    {
        // If start of new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // If first JPEG
            if (counter == 0)
            {
                sprintf(filename, "%03i.jpg", counter);
                img = fopen(filename, "w");
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, img);
                counter++;
            }
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", counter);
                img = fopen(filename, "w");
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, img);
                counter++;
            }
        }
        else
        {
            // If already found JPEG
            if (counter > 0)
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, img);
        }
    }

    // Close any remaining files
    fclose(card);
    fclose(img);
}

// TODO
// Open memory card
// Look for beginning of a JPEG
// Open a new JPEG file
// Write 512 bytes until a new JPEG is found
