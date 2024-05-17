#include <iostream>
#include <ratio>
#include <chrono>
#include <thread>
#include "Chip8.h"

Chip8 Chip8;

int main()
{
//    std::cout << "Hello, World!" << std::endl;
    Chip8.initialize();
    Chip8.loadROM("../output.ch8");

    const double target_frequency = 60.0;
    const std::chrono::duration<double, std::milli> target_delay(1000.0 / target_frequency);

    auto last_cycle_time = std::chrono::high_resolution_clock::now();

    while (true) {
        Chip8.emulateCycle();

        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = current_time - last_cycle_time;

        if (elapsed_time < target_delay) {
            // sleep for the remaining time if cycle too fast
            std::this_thread::sleep_for(target_delay - elapsed_time);
        }

        last_cycle_time = std::chrono::high_resolution_clock::now();
    }
    return 0;
}
