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


void playGame(void);



#endif /* __PAC_MAN_H__ */
