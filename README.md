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

## Toolchain

Beside this directory, I have somewhere a directory containing the toolchain.

```bash
.
└── stm32f401_toolchain
    ├── arm-gnu-toolchain-14.3.rel1-x86_64-arm-none-eabi
    └── environtment-setup-cortexm4-none-eabi
```

Content of the environment file
```bash
ENV_DIR=$(cd $(dirname $(readlink -f "${BASH_SOURCE[0]}")) && pwd)
ARM_NONE_EABI_PATH=${ENV_DIR}/arm-gnu-toolchain-14.3.rel1-x86_64-arm-none-eabi/bin/
CROSS_COMPILE=arm-none-eabi-

# Path
export PATH="$ARM_NONE_EABI_PATH:$PATH"
# Compilation tools
export AR=${CROSS_COMPILE}ar
export CC=${CROSS_COMPILE}gcc
export LD=${CROSS_COMPILE}ld
export GDB=${CROSS_COMPILE}gdb
export SIZE=${CROSS_COMPILE}size
export OBJCOPY=${CROSS_COMPILE}objcopy
export OBJDUMP=${CROSS_COMPILE}objdump


stm32_flash()
{
    interface="interface/stlink.cfg"
    target="target/stm32f4x.cfg"
    echo "Use OpenOCD $interface $target bin: '$1'"
    openocd -f $interface -f $target -c "program $1 verify reset exit"
}
```

```bash
source environtment-setup-cortexm4-none-eabi
```

Now you can use ```cmake``` (using the cross compiler and linker) and the ```stm32_flash``` method.