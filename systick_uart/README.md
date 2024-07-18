# UART and Systick exercise

Display via UART a timer counting up with 10ms accuracy up till minutes. Make sure to see the ms increase without a new line

Format mm::ss::SS where SS is s/100.


*Use CMSIS(Common Microcontroller Software Interface Standard) way

## UART specs

- Baudrate 115200
- 8 bit
- no parity
- 1 stop bit

## Read UART

On linux via screen

```
screen /dev/ttyACM0 115200
```

