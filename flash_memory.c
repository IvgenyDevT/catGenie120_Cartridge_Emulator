//
// Created by jenya tokarzhevsky on 07/04/2026.
//

#include "flash_memory.h"

void flash_write_counter(uint8_t value) {
    uint32_t ints = save_and_disable_interrupts();

    flash_range_erase(FLASH_OFFSET, 4096);

    uint8_t buffer[256] = {0};
    buffer[0] = value;

    flash_range_program(FLASH_OFFSET, buffer, 256);

    restore_interrupts(ints);
}

uint8_t flash_read_counter() {
    const uint8_t *flash_ptr = (const uint8_t *)(XIP_BASE + FLASH_OFFSET);
    return flash_ptr[0];
}