#include "test.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int IMAGE_WIDTH = SCREEN_WIDTH + 300;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Texture* loadTexture(const std::string& path) {
    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool initSDL()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    gWindow = SDL_CreateWindow("2D Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Set renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Allows for (Initializes) Image Loading abilities using SDL_image library
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void closeSDL()
{
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    // Load textures
    std::vector<SDL_Texture*> backgroundTextures;
    backgroundTextures.push_back(loadTexture("./Assets/Pixel_Forest/sky.jpeg"));   // Index 0: sky
    backgroundTextures.push_back(loadTexture("./Assets/Pixel_Forest/moon3.png"));  // Index 1: moon
    backgroundTextures.push_back(loadTexture("./Assets/Pixel_Forest/tnt2.png"));  // Index 2: tent
    backgroundTextures.push_back(loadTexture("./Assets/Pixel_Forest/tree2.png"));  // Index 3: tree

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen
        SDL_RenderClear(gRenderer);

        // Render sky covering the whole screen
        SDL_RenderCopy(gRenderer, backgroundTextures[0], NULL, NULL);

        // Render tree covering the bottom of the window
        SDL_Rect treeDest = { 0, SCREEN_HEIGHT / 3, SCREEN_WIDTH, 2 * (SCREEN_HEIGHT / 3) };
        SDL_RenderCopy(gRenderer, backgroundTextures[3], NULL, &treeDest);

        // Render moon at the top-left corner (smaller and slightly to the right)
        SDL_Rect moonDest = { SCREEN_WIDTH / 10, SCREEN_HEIGHT / 10, SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6 };
        SDL_RenderCopy(gRenderer, backgroundTextures[1], NULL, &moonDest);

        // Render tent touching the bottom of the screen
        // Adjust the width and height of the tent texture to match the moon's size
        int tentWidth = SCREEN_WIDTH / 6;  // Same as moon's width
        int tentHeight = SCREEN_HEIGHT / 6; // Same as moon's height
        SDL_Rect tentDest = { SCREEN_WIDTH / 2 - tentWidth / 2, SCREEN_HEIGHT - tentHeight, tentWidth, tentHeight };
        SDL_RenderCopy(gRenderer, backgroundTextures[2], NULL, &tentDest);

        // Update screen
        SDL_RenderPresent(gRenderer);
    }

    // Destroy textures
    for (SDL_Texture* texture : backgroundTextures) {
        SDL_DestroyTexture(texture);
    }

    // Close SDL
    closeSDL();

    return 0;
}


 