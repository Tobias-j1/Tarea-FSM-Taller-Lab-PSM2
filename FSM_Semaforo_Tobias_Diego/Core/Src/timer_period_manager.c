#include "timer_period_manager.h"

// Condition functions
static int rising_edge_detected(void *context) {
    TimerPeriodManager *period_manager = (TimerPeriodManager *)context;
    return get_edge_detector_state(period_manager->edge_detector) == RISING_EDGE;
}

// on_state functions for each state
void on_state_period_1(void *context) {
    TimerPeriodManager *period_manager = (TimerPeriodManager *)context;
    timer_update_duration(period_manager->timer, PERIOD_1);  // Set the timer to PERIOD_1
}

void on_state_period_2(void *context) {
    TimerPeriodManager *period_manager = (TimerPeriodManager *)context;
    timer_update_duration(period_manager->timer, PERIOD_2);  // Set the timer to PERIOD_2
}

void on_state_period_3(void *context) {
    TimerPeriodManager *period_manager = (TimerPeriodManager *)context;
    timer_update_duration(period_manager->timer, PERIOD_3);  // Set the timer to PERIOD_3
}

// Transition arrays for each state
static Transition Period1Transitions[] = {
    {rising_edge_detected, PERIOD_2_STATE}  // Transition to PERIOD_2_STATE
};

static Transition Period2Transitions[] = {
    {rising_edge_detected, PERIOD_3_STATE}  // Transition to PERIOD_3_STATE
};

static Transition Period3Transitions[] = {
    {rising_edge_detected, PERIOD_1_STATE}  // Transition to PERIOD_1_STATE
};

// FSM states with on_state actions
static FSMState PeriodManagerStates[] = {
    {Period1Transitions, 1, on_state_period_1},  // PERIOD_1_STATE with on_state_period_1 action
    {Period2Transitions, 1, on_state_period_2},  // PERIOD_2_STATE with on_state_period_2 action
    {Period3Transitions, 1, on_state_period_3}   // PERIOD_3_STATE with on_state_period_3 action
};

// Initialize the TimerPeriodManager FSM with predefined periods
void timer_period_manager_init(TimerPeriodManager *period_manager, Timer *timer, EdgeDetector *edge_detector) {
    period_manager->timer = timer;
    period_manager->edge_detector = edge_detector;

    // Initialize FSM starting with PERIOD_1_STATE
    fsm_init(&period_manager->fsm, PeriodManagerStates, PERIOD_1_STATE, period_manager);

    // Set the initial period (PERIOD_1)
    timer_update_duration(period_manager->timer, PERIOD_1);
}

// Update the FSM
void timer_period_manager_update(TimerPeriodManager *period_manager) {
    fsm_update(&period_manager->fsm);
}
