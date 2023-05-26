#ifndef _INIT_H
#define _INIT_H

/**
 * @brief Initializes the Arduino board and sets up the necessary environment.
 *
 * This function is an abstraction from the core Arduino SDK's main() initialization function.
 * It is responsible for initializing the Arduino board and setting up the necessary environment
 * for the sketch to run. It performs board-specific initialization, calls the Arduino core
 * library's initialization function, and allows for additional setup tasks specific to the project.
 *
 * @note This function should be called before entering the main program loop.
 */
void arduino_init(void);

void circuit_init(void);

void run_diagnostics(void);

#endif