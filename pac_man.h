#ifndef __PAC_MAN_H__
#define __PAC_MAN_H__

#include <stdint.h>

// Pac-Man starting position
#define START_X 13
#define START_Y 8

// Current position of Pac-Man on the maze
typedef struct {
    uint8_t x;
    uint8_t y;
} Pac_Man_t;

typedef enum {
    PAC_UP,
    PAC_DOWN,
    PAC_RIGHT,
    PAC_LEFT,
    PAC_NONE,
    NUM_PAC_DIRECTIONS
} PacDirection_t;


void playGame(void);



#endif /* __PAC_MAN_H__ */
