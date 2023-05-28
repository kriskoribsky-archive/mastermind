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

/**
 * @brief Initializes the circuit and sets up any required configurations.
 *
 * This function is responsible for initializing the circuit and setting up any required
 * configurations specific to the project. It may include tasks such as configuring pins,
 * initializing communication interfaces, or setting initial states for components.
 *
 * @note This function should be called after arduino_init() and before entering the main program loop.
 */
void circuit_init(void);

/**
 * @brief Runs diagnostics and performs self-tests if necessary.
 *
 * This function is responsible for running diagnostics and performing any necessary self-tests
 * before the main program logic starts executing. It can be used to verify the proper functioning
 * of components, check communication interfaces, or perform any other necessary checks or tests.
 *
 * @note This function should be called after circuit_init() and before entering the main program loop.
 */
void run_diagnostics(void);


#endif