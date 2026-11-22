/*
 * button.h
 *
 */

#ifndef SRC_BUTTON_H_
#define SRC_BUTTON_H_

#include<stdint.h>

/*
 * BUTTON FCT
 */

#define PIN_BPU_CARTE_C 13
#define PIN_BPU_SHIELD_B 8

void init_bouton_carte();
void init_bouton_shield();

uint8_t get_bouton_carte_status();
uint8_t get_bouton_shield_status();

void select_PC13_as_EXTI13();
void select_PB8_as_EXTI8();

void setup_IRQ_on_EXTI13_events();
void setup_IRQ_on_EXTI8_events();

void enable_EXTI15_10_IRQ_simple();
void enable_EXTI9_5_IRQ_simple();

void enable_EXTI15_10_IRQ();
void enable_EXTI9_5_IRQ();

void init_IRQ_BP_CARTE();
void init_IRQ_BP_SHIELD();

/*
 * SWITCH FCT
 */

#define PIN_SWITCH_4 3
#define PIN_SWITCH_3 4
#define PIN_SWITCH_2 5
#define PIN_SWITCH_1 6

void init_switch();
uint8_t get_switch_status();

#endif /* SRC_BUTTON_H_ */
