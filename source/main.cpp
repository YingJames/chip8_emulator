#include <iostream>
#include <ratio>
#include <chrono>
#include <thread>
#include <SDL.h>
#include "Chip8.h"
#define WIDTH 640
#define HEIGHT 320
#define ROM_PATH "../roms/2-ibm-logo.ch8"
Chip8 Chip8;

int main() {

    Chip8.initialize();

    // SDL INITIALIZATION
    static SDL_Surface *src_buffer = nullptr;
    static SDL_Surface *arg_buffer = nullptr;
    static SDL_Texture *texture = nullptr;
    static SDL_Renderer *renderer = nullptr;
    static SDL_Window *window = nullptr;
    SDL_Rect dstRect = {0,0,640,320};
    uint32_t rgbBuffer[64 * 32] = {0};

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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 64, 32);
//    SDL_SetRenderTarget(renderer, texture);

    Chip8.loadROM(ROM_PATH);
    const std::chrono::duration<double, std::milli> target_delay(1000.0 / Chip8.target_frequency);
    auto last_cycle_time = std::chrono::high_resolution_clock::now();


    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyWindow(window);
                SDL_Quit();
                quit = true;
            }
//            else if (event.type == SDL_KEYDOWN) {
//                Chip8.handleKeyDown(event.key.keysym.scancode);
//            }
        }

            Chip8.emulateCycle();

            // DISPLAY
            for (int y = 0; y < 32; ++y) {
                for (int x = 0; x < 64; ++x) {
                    uint8_t pixel = Chip8.gfx[y][x];
                    uint32_t color = (pixel == 0) ? 0x00000000 : 0xFFFFFFFF; // Black for 0, white for 1
                    rgbBuffer[y * 64 + x] = color;
                }
            }

            SDL_UpdateTexture(texture, nullptr, rgbBuffer, 64 * sizeof(uint32_t));
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
            SDL_RenderPresent(renderer);

            // update timers
            auto current_time = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_cycle_time).count() >= 1000 / 60) {
                if (Chip8.delay_timer > 0) {
                    --Chip8.delay_timer;
                }

                if (Chip8.sound_timer > 0) {
                    --Chip8.sound_timer;
                }

                last_cycle_time = current_time;
            }


            auto elapsed_time = current_time - last_cycle_time;
            if (elapsed_time < target_delay) {
                // sleep for the remaining time if cycle too fast
                std::this_thread::sleep_for(target_delay - elapsed_time);
            }
            last_cycle_time = std::chrono::high_resolution_clock::now();
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
