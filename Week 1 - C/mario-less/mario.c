#include <cs50.h>
#include <stdio.h>

void print_row(int spaces, int bricks);

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
        print_row(height - x - 1, x + 1);
    }
}

void print_row(int spaces, int bricks)
{
    for (int y = 0; y < spaces; y++)
    {
        printf(" ");
    }

    for (int z = 0; z < bricks; z++)
    {
        printf("#");
    }
    printf("\n");
}
