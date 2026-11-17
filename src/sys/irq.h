/*
 * irq.h
 *
 */

#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_

#include<stdint.h>

void enable_IRQ_by_number(uint32_t num);

void disable_IRQ_by_number(uint32_t num);

#endif /* SRC_IRQ_H_ */
