#include "irq.h"
#include "sys/devices.h"

void enable_IRQ_by_number(uint32_t num) {
  if (num > 96) return; /* check if number is valid */
  uint32_t bit  = num % 32;
  uint32_t index = num / 32;
  NVIC.ISER[index]=(1<<bit);
}

void disable_IRQ_by_number(uint32_t num) {
  if (num > 96) return; /* check if number is valid */
  uint32_t bit  = num % 32;
  uint32_t index = num / 32;
  NVIC.ICER[index]=(1<<bit);
}

