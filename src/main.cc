#include <stdlib.h>
#include <string.h>

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <usb_serial.h>

#define PULSE_PIN 20  // 100µs high pulse when there is a particle. not used yet.

#define SCREEN_WIDTH 128      // OLED display width, in pixels
#define SCREEN_HEIGHT 64      // OLED display height, in pixels
#define SCREEN_ORIENTATION 0  // OLED orientation ([0; 3], 0 is the standard orientation).

#define OLED_DC     8
#define OLED_CS     10
#define OLED_RESET  9
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RESET, OLED_CS);

// A buffer for the data read from the geiger counter.
char buffer[60];
int buffer_index = 0;

// Recent CPS values to graph.
char counts[SCREEN_WIDTH];
char counts_pos = 0;

void setup() {
  Serial.begin(38400);  // USB Serial
  Serial1.begin(9600);  // Hardware Serial on pin 1 and 2 (only RX on pin 1 is used).

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  memset(counts, 0, SCREEN_WIDTH * sizeof(char));

  display.setRotation(SCREEN_ORIENTATION);
  display.clearDisplay();  // The display initially contains an Adafruit image.

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  Serial.println("Setup done");
}

void error(int err) {
  Serial.print("Error: ");
  Serial.println(err);
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Error: ");
  display.println(err);
  display.display();
  delay(5000);
}

void parse_and_print() {
  char* str = buffer;
  if (strncmp(str, "CPS, ", 5) != 0) {
    error(1);
    return;
  }
  str += 5;
  
  char* new_str;
  int cps = strtol(str, &new_str, 10);
  if (str == new_str) {
    error(2);
    return;
  }
  str = new_str;
  
  if (strncmp(str, ", CPM, ", 7) != 0) {
    error(3);
    return;
  }
  str += 7;
  
  int cpm = strtol(str, &new_str, 10);
  if (str == new_str) {
    error(4);
    return;
  }
  str = new_str;

  if (strncmp(str, ", uSv/hr, ", 10) != 0) {
    error(5);
    return;
  }
  str += 10;
  
  float dose = strtof(str, &new_str);
  if (str == new_str) {
    error(6);
    return;
  }
  str = new_str;
  
  if (strncmp(str, ", ", 2) != 0) {
    error(7);
    return;
  }
  str += 2;
  
  counts[counts_pos] = cps;
  counts_pos = (counts_pos + 1) % SCREEN_WIDTH;
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("CPS: ");
  display.println(cps);
  display.print("CPM: ");
  display.println(cpm);
  display.print("dose: ");
  display.print(dose);
  display.println(" \346Sv/h");  // 0346 == 230 == 'µ' en ASCII
  display.print("mode: ");
  display.println(str);
  
  for (int i = 0; i < SCREEN_WIDTH; i++) {
    int val = counts[(counts_pos + i) % SCREEN_WIDTH];
    if (val != 0) {
      display.drawLine(i, SCREEN_HEIGHT - 1, i, SCREEN_HEIGHT - val, WHITE);
    }
  }
  display.display();
}

void loop() {
  while (Serial1.available() > 0) {
    int c = Serial1.read();
    Serial.write(c);
    if (c != '\n') {
      buffer[buffer_index++] = c;
    } else {
      buffer[buffer_index] = 0;
      parse_and_print();
      buffer_index = 0;
    } 
  }
}

int main() {
  setup();
  while(1) {
    loop();
    yield();
  }
}