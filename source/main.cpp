#include <iostream>
#include <ratio>
#include <chrono>
#include <thread>
#include <SDL.h>
#include "Chip8.h"
#define WIDTH 800
#define HEIGHT 600
Chip8 Chip8;

int main() {
//    std::cout << "Hello, World!" << std::endl;
    Chip8.initialize();

    // SDL INITIALIZATION
    SDL_Window *window = nullptr;
    if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)) {
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    window = SDL_CreateWindow("Chip-8 Emulator",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WIDTH,
                              HEIGHT,
                              0);
    if (window == nullptr) {
        fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
        return 1;
    }

    Chip8.loadROM("../output.ch8");
    const double target_frequency = 1000.0;
    const std::chrono::duration<double, std::milli> target_delay(1000.0 / target_frequency);
    auto last_cycle_time = std::chrono::high_resolution_clock::now();

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
//            else if (event.type == SDL_KEYDOWN) {
//                Chip8.handleKeyDown(event.key.keysym.scancode);
//            }

            Chip8.emulateCycle();

            auto current_time = std::chrono::high_resolution_clock::now();
            auto elapsed_time = current_time - last_cycle_time;
            if (elapsed_time < target_delay) {
                // sleep for the remaining time if cycle too fast
                std::this_thread::sleep_for(target_delay - elapsed_time);
            }
            last_cycle_time = std::chrono::high_resolution_clock::now();
        }
    }
    return 0;
}
