# STM32 Music Box

Bare-metal music box on an **STM32 Nucleo-F446RE** (ARM Cortex-M4), written in C
with no HAL: all peripherals are configured directly through their registers.
University project, IESE 3, Polytech Grenoble, March 2026.

## What it does

- Plays melodies (e.g. Brahms' Lullaby) on a piezo buzzer.
- **TIM2 in PWM mode**: pitch from the auto-reload value, volume from the duty cycle
  (logarithmic volume scale). Timer preload (ARPE / OC2PE) removes audible clicks
  when a note changes.
- **Frequency calibration**: a correction coefficient measured on an oscilloscope
  compensates the drift seen in the higher octaves.
- **ADC** potentiometer input to control volume, tempo (BPM), pitch or the played note.
- **External interrupts** for the buttons (next song, play/pause) and a **SysTick**
  1 ms time base.
- Melody sequencer, instrument timbres built with ADSR envelopes (buzzer, piano,
  trumpet, electric guitar) and switch-selected modes, including a free-play
  "musician" mode.
- **UART dashboard** (115,200 baud): a live terminal display using ANSI escape codes.

## Code layout

| Path | Content |
| --- | --- |
| `src/main.c` | Main loop, mode state machine, serial dashboard, interrupt handlers |
| `src/pwm.c` | TIM2 PWM driver, buzzer pin setup, potentiometer ADC setup |
| `src/music.c`, `src/notes.c` | Melody data and sequencer, note tables |
| `src/instrument.c` | ADSR envelope and timbre synthesis |
| `src/delays.c` | Timing helpers |
| `src/sys/`, `src/boot/`, `stm32-rules.make`, `devtools/` | Low-level support code and build framework used in the course |

## Build

Requires an `arm-none-eabi` GCC toolchain and the ST-Link tools used by the course
framework.

```
make        # build
make doc    # generate Doxygen documentation
```

## Author

Mohamed Hashem. `src/sys/cmsis_gcc.h` is ARM CMSIS code under its own licence and is unchanged.
