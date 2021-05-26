#ifndef peripherals_H
#define peripherals_H
#include <stdint.h>

uint8_t get_red_val(unsigned char *membase);
uint8_t get_green_val(unsigned char *membase);
uint8_t get_blue_val(unsigned char *mem_base);
void write_led_val(unsigned char *mem_base, uint32_t val_line);

#endif