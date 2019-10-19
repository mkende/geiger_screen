# Geiger Counter Screen

This program reads the serial data written by a
[mightyohm.com Geiger counter](https://mightyohm.com/blog/products/geiger-counter/)
and display the information on an
[SSD1306 OLED screen](https://www.adafruit.com/products/938) using a SPI
interface.

It is made to run on a [Teensy LC](https://www.pjrc.com/teensy/teensyLC.html)
but could probably be adapted for most Arduino compatible boards.

![the screen](screen.jpg?raw=true "The Screen")

## Build

The program is built using
[teensy_builder](https://github.com/mkende/teensy_builder). Just run
`teensy_builder` at the root of the project to build and upload it to a Teensy
LC board.
