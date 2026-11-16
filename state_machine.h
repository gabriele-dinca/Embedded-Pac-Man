#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

    // States Enum
    typedef enum {
        STATE_INIT,
        STATE_PLAY,
        NUM_STATES
    } State_t;

    // State Machine Struct
    typedef struct {
        State_t state;
        void (*state_function)(void);
    } StateMachine_t;

    // State change functions
    void fn_INIT(void);
    void fn_PLAY(void);


    extern State_t current_state;
    extern StateMachine_t fsm[];


    void state_machine(void);

#endif /* __STATE_MACHINE_H__ */
