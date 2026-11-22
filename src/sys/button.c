#include "button.h"
#include "sys/devices.h"
#include "irq.h"

/*
 * BUTTON FUNCTIONS
 */

void init_bouton_carte(){
	GPIOC.MODER &= ~(3 << (PIN_BPU_CARTE_C << 1));	// bit clear (input)
	GPIOC.PUPDR &= ~(3 << (PIN_BPU_CARTE_C << 1)); 	// bit clear (no pull-up or pull-down)
}

void init_bouton_shield(){
	GPIOB.MODER &= ~(3 << (PIN_BPU_SHIELD_B << 1)) ;	// bit clear (input)
	GPIOB.PUPDR &= ~(3 << (PIN_BPU_SHIELD_B << 1)); 	// bit clear (no pull-up or pull-down)
}

uint8_t get_bouton_carte_status(){
	return (GPIOC.IDR & (1 << PIN_BPU_CARTE_C))>>PIN_BPU_CARTE_C;
}

uint8_t get_bouton_shield_status(){
	return (GPIOB.IDR & (1 << PIN_BPU_SHIELD_B))>>PIN_BPU_SHIELD_B;
}

void select_PC13_as_EXTI13() {
  enable_SYSCFG();
  SYSCFG.EXTICR4 = (SYSCFG.EXTICR4 & ~ (0xf << 4) ) | (2 << 4);
}

void select_PB8_as_EXTI8() {
  enable_SYSCFG();
  SYSCFG.EXTICR3 = (SYSCFG.EXTICR3 & ~ (0xf << 0) ) | (1 << 0);
}

void setup_IRQ_on_EXTI13_events() {
  /* EXTI is always enabled (no RCC mask) */
  EXTI.IMR  |= (1<<13); /* IRQ enable */
  EXTI.RTSR |= (1<<13); /* trigger on rising edge */
  EXTI.PR    = (1<<13); /* write 1 to reset pending flag */
}

void setup_IRQ_on_EXTI8_events() {
  /* EXTI is always enabled (no RCC mask) */
  EXTI.IMR  |= (1<<8); /* IRQ enable */
  EXTI.RTSR |= (1<<8); /* trigger on rising edge */
  EXTI.PR    = (1<<8); /* write 1 to reset pending flag */
}

void enable_EXTI15_10_IRQ_simple() {
  NVIC.ISER[1]=(1<<8);
}

void enable_EXTI9_5_IRQ_simple() {
  NVIC.ISER[0]=(1<<23);
}

void enable_EXTI15_10_IRQ() {
    enable_IRQ_by_number(40);
}

void enable_EXTI9_5_IRQ() {
    enable_IRQ_by_number(23);
}

void init_IRQ_BP_CARTE() {
	init_bouton_carte();
	select_PC13_as_EXTI13();
	setup_IRQ_on_EXTI13_events();
	enable_EXTI15_10_IRQ();
}

void init_IRQ_BP_SHIELD() {
	init_bouton_shield();
	select_PB8_as_EXTI8();
	setup_IRQ_on_EXTI8_events();
	enable_EXTI9_5_IRQ();
}

/*
 * SWITCH FUNCTIONS
 */

void init_switch(){
	GPIOB.MODER &= ~( 3 << (PIN_SWITCH_1 << 1) | 3 << (PIN_SWITCH_2 << 1) | 3 << (PIN_SWITCH_3 << 1) | 3 << (PIN_SWITCH_4 << 1) ) ;	// bit clear (input)
	GPIOB.PUPDR &= ~( 3 << (PIN_SWITCH_1 << 1) | 3 << (PIN_SWITCH_2 << 1) | 3 << (PIN_SWITCH_3 << 1) | 3 << (PIN_SWITCH_4 << 1) ); 	// bit clear (no pull-up or pull-down)
}

uint8_t get_switch_status(){
	return (GPIOB.IDR & (0xf << PIN_SWITCH_4)) >> PIN_SWITCH_4;
}
