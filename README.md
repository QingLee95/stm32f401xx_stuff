# Bare metal programming
Playing around with the STM32F401xx board without the STM32Cube IDE.
Own linker script. Own startup code. No libc.

Credits to [Klein Embedded](https://kleinembedded.com/stm32-without-cubeide-part-1-the-bare-necessities/) for getting me started.

# Depencencies

* [arm-none-eabi toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) 
* [OpenOCD](https://github.com/openocd-org/openocd)

# Compilation



# OpenOCD - Flash binary on target
Installation on Debian

```bash
sudo apt install openocd
```
Or compile from source.

## ELF file

Connect the MCU with a USB cable and replace {BINARY.elf}.

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program {BINARY.elf} verify reset exit"
```

# Binary

Connect the MCU with a USB cable and replace {BINARY.bin}.

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg  -c "init; reset halt; flash write_image erase {BINARY.bin} 0x08000000 bin; reset run; exit"
```
