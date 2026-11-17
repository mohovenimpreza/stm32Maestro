#include <stdint.h>
#include "sys/devices.h"
#include "sys/clock.h"
#include "systick.h"

void SysTick_init(uint32_t ticks_per_second)
{
	uint32_t cycles_per_second = get_SYSCLK();
	uint32_t cycles_per_tick = cycles_per_second / ticks_per_second;
	SysTick.LOAD = ((cycles_per_tick-1)&0x00ffffff);
	SysTick.VAL=0;
	SysTick.CTRL|=0x7;
}
