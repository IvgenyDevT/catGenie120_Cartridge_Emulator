//
// Created by jenya tokarzhevsky on 07/04/2026.
//




#ifndef CONFIG_H
#define CONFIG_H

#include "hardware/sync.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "hardware/regs/intctrl.h"
#include "hardware/regs/i2c.h"
#include "pico/stdlib.h"
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"



#define I2C0_CLK 1
#define I2C0_SDA 0

#define I2C_ADDR 0x50

#define MASK_4_BIT_LSB 0x0f

#define CATGENIE_COMMAND_BYTE 2
#define CATGENIE_BLOCK_NUB_BYTE 3
#define CATGENIE_BLOCK_DATA_BYTE_OFFSET 4

#define RED_PIN   18
#define GREEN_PIN 19
#define BLUE_PIN  20


#endif //CONFIG_H
