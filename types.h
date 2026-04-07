//
// Created by jenya tokarzhevsky on 07/04/2026.
//

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef struct {
    uint8_t readBuffer[16];
    int readIndex;

    uint8_t *response;
    int respone_size;
    int responseIndex;
} i2c_state_t;

#endif //TYPES_H
