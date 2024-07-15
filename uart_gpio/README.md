# UART and GPIO exercise

When the integrated blue button is pushed a predefined letter is being printed on the laptop. Communication is done via UART.

- Configure TX GPIO for UART
- Configure UART
- Configure GPIO of blue button as input
- Send text to the laptop when blue botton is pushed

For example with 3 button presses:
```
Exercise:
	UART
	GPIO: input
```

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

