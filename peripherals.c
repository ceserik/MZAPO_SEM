#include "peripherals.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include <stdlib.h>

uint8_t get_blue_val(unsigned char *mem_base){
    return *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
}
uint8_t get_green_val(unsigned char *mem_base){
    return (*(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o)) >>8;
}
uint8_t get_red_val(unsigned char *mem_base){
    return (*(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o)) >>16;
}
void write_led_val(unsigned char *mem_base, uint32_t val_line){
    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
}