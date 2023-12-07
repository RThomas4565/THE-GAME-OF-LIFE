#include <iostream>
#include <SDL.h>
#include <cstdlib> // For random number generation
#include <ctime>   // For seeding the random number generator

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int CELL_SIZE = 10;
const int GRID_WIDTH = SCREEN_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = SCREEN_HEIGHT / CELL_SIZE;

bool grid[GRID_WIDTH][GRID_HEIGHT];
bool nextGrid[GRID_WIDTH][GRID_HEIGHT];

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void initializeSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawGrid() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for cells

    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            if (grid[x][y]) {
                SDL_Rect cellRect = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                SDL_RenderFillRect(renderer, &cellRect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void initializeGrid() {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            // Randomly initialize the grid with some cells alive and some dead
            grid[x][y] = std::rand() % 2 == 0;
        }
    }
}

int countLiveNeighbors(int x, int y) {
    int liveNeighbors = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (x + i >= 0 && x + i < GRID_WIDTH && y + j >= 0 && y + j < GRID_HEIGHT && !(i == 0 && j == 0)) {
                if (grid[x + i][y + j]) {
                    liveNeighbors++;
                }
            }
        }
    }

    return liveNeighbors;
}


void updateGrid() {
    memcpy(nextGrid, grid, sizeof(grid));

    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            int liveNeighbors = countLiveNeighbors(x, y);

            if (grid[x][y]) {
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    nextGrid[x][y] = false;
                }
            }
            else {
                if (liveNeighbors == 3) {
                    nextGrid[x][y] = true;
                }
            }
        }
    }

    memcpy(grid, nextGrid, sizeof(grid));
}

int main(int argc, char* argv[]) {
    initializeSDL();
    initializeGrid(); // Initialize the grid with random cells

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update the grid for the next generation
        updateGrid();

        // Draw the grid
        drawGrid();

        SDL_Delay(100); // Adjust the delay to control the speed of simulation
    }

    closeSDL();
    return 0;
}
