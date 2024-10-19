#ifndef INC_TIMER_PERIOD_MANAGER_H_
#define INC_TIMER_PERIOD_MANAGER_H_

#include "edge_detector.h"
#include "fsm.h"
#include "timer.h"

// Enum for the FSM states corresponding to the periods
typedef enum {
    PERIOD_1_STATE,  // First period
    PERIOD_2_STATE,  // Second period
    PERIOD_3_STATE   // Third period
} TimerPeriodFSMState;

// Define the period durations (in milliseconds)
#define PERIOD_1 100   // First period duration
#define PERIOD_2 500   // Second period duration
#define PERIOD_3 1000  // Third period duration

// Structure to manage the timer period using FSM
typedef struct {
    FSM fsm;                    // FSM instance for period management
    Timer *timer;               // Pointer to the timer instance
    EdgeDetector *edge_detector;  // Pointer to the edge detector instance
} TimerPeriodManager;

// PUBLIC FUNCTIONS

/**
 * @brief Initializes the TimerPeriodManager module with predefined periods.
 *
 * @param period_manager Pointer to the TimerPeriodManagerFSM structure.
 * @param timer Pointer to the timer instance.
 * @param edge_detector Pointer to the edge detector instance.
 */
void timer_period_manager_init(TimerPeriodManager *period_manager, Timer *timer, EdgeDetector *edge_detector);

/**
 * @brief Updates the TimerPeriodManager module.
 *
 * @param period_manager Pointer to the TimerPeriodManagerFSM structure.
 */
void timer_period_manager_update(TimerPeriodManager *period_manager);

#endif /* INC_TIMER_PERIOD_MANAGER_H_ */
