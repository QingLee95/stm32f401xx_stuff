# Bare metal programming
Playing around with the STM32F401xx board without the STM32Cube IDE.
Own linker script. Own startup code. No libc.

Credits to [Klein Embedded](https://kleinembedded.com/stm32-without-cubeide-part-1-the-bare-necessities/) for getting me started.

# Depencencies

* [arm-none-eabi toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) 
* [OpenOCD](https://github.com/openocd-org/openocd)

## OpenOCD
Installation on Debian

```bash
sudo apt install openocd
```
Or compile from source.

Connect the MCU with a USB cable and replace {BINARY}

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program {BINARY} verify reset exit"
```
