#include <cs50.h>
#include <stdio.h>

void print_row(int spaces1, int bricksLeft, int spaces2, int bricksRight);

int main(void)
{
    int height;

    do
    {
        height = get_int("Height: ");
    }
    while (height < 1);

    for (int x = 0; x < height; x++)
    {
        print_row(height - x - 1, x + 1, 2, x + 1);
    }
}

void print_row(int spaces1, int bricksLeft, int spaces2, int bricksRight)
{
    for (int w = 0; w < spaces1; w++)
    {
        printf(" ");
    }

    for (int x = 0; x < bricksLeft; x++)
    {
        printf("#");
    }

    for (int y = 0; y < spaces2; y++)
    {
        printf(" ");
    }

    for (int z = 0; z < bricksRight; z++)
    {
        printf("#");
    }
    printf("\n");
}
