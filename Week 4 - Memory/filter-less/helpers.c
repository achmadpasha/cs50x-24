#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, and blue
            int avg =
                round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // Update pixel values
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Compute sepia values
            int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen +
                                 .189 * image[i][j].rgbtBlue);
            if (sepiaRed > 255)
                sepiaRed = 255;

            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen +
                                   .168 * image[i][j].rgbtBlue);
            if (sepiaGreen > 255)
                sepiaGreen = 255;

            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen +
                                  .131 * image[i][j].rgbtBlue);
            if (sepiaBlue > 255)
                sepiaBlue = 255;

            // If the result is a number greater than 255
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp == NULL)
    {
        return;
    }

    int i;
    int j;

    // Loop over all pixels
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
        // Swap pixels
        for (j = 0; j < width; j++)
        {
            image[i][j] = temp[i][width - j - 1];
        }
    }
    free(temp);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary image to store the blurred values
    RGBTRIPLE(*copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (copy == NULL)
    {
        return;
    }

    // Iterate over each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize sums for each color & counter
            float totalBlue = 0;
            float totalGreen = 0;
            float totalRed = 0;
            float counter = 0;

            // Iterate over the neighboring pixels
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int h = i + x;
                    int w = j + y;

                    // Check if neighbor is within image bounds
                    if (h >= 0 && h < height && w >= 0 && w < width)
                    {
                        totalBlue += image[h][w].rgbtBlue;
                        totalGreen += image[h][w].rgbtGreen;
                        totalRed += image[h][w].rgbtRed;
                        counter++;
                    }
                }
            }
            // Calculate the average color and assign it to `copy`
            copy[i][j].rgbtBlue = round(totalBlue / counter);
            copy[i][j].rgbtGreen = round(totalGreen / counter);
            copy[i][j].rgbtRed = round(totalRed / counter);
        }
    }

    // Copy the `copy` array back to `image` array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    free(copy);
    return;
}
