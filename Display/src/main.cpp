#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 9
#define TFT_CS 10

#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 240
#define RECT_WIDTH 105
#define RECT_HEIGHT 46

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

float newValues[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float oldValues[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

String selections[10] = {"B1", "B2", "T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8"};
String types[10] = {"battery", "battery", "thruster", "thruster", "thruster", "thruster", "thruster", "thruster", "thruster", "thruster"};

int myBox[2];
int myCursor[2];

void setup() {
  tft.begin();
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);

  tft.setCursor(10, 10);
  tft.println("Dougie Status");

  Serial.begin(9600);
}

void loop() {
  getValuesSerial();
  displayValues();
  delay(500);
}

void getValuesSerial() {
  Serial.println("Enter 10 new values separated by commas:");

  while (!Serial.available()) {
    // Wait for input
  }

  // Read input from serial monitor
  String input = Serial.readStringUntil('\n');

  // Split the input string into individual values
  int commaIndex = 0;
  int prevCommaIndex = -1;
  for (int i = 0; i < 10; i++) {
    commaIndex = input.indexOf(',', prevCommaIndex + 1);
    if (commaIndex == -1) {
      commaIndex = input.length();
    }
    String numString = input.substring(prevCommaIndex + 1, commaIndex);
    newValues[i] = numString.toFloat();
    prevCommaIndex = commaIndex;
  }

  // Print the updated array
  Serial.println("Updated values:");
  for (int i = 0; i < 10; i++) {
    Serial.print(newValues[i]);
    if (i < 9) {
      Serial.print(", ");
    }
  }
  Serial.println();
}

void areaCalculator(String selection) {
  if (selection == "B1") {
    myBox[0] = 10;
    myBox[1] = 40;
  } else if (selection == "B2") {
    myBox[0] = 10 + RECT_WIDTH + 10;
    myBox[1] = 40;
  } else if (selection == "T1") {
    myBox[0] = 10;
    myBox[1] = 40 + RECT_HEIGHT + 10;
  } else if (selection == "T2") {
    myBox[0] = 10 + RECT_WIDTH + 10;
    myBox[1] = 40 + RECT_HEIGHT + 10;
  } else if (selection == "T3") {
    myBox[0] = 10;
    myBox[1] = 40 + 2 * (RECT_HEIGHT + 10);
  } else if (selection == "T4") {
    myBox[0] = 10 + RECT_WIDTH + 10;
    myBox[1] = 40 + 2 * (RECT_HEIGHT + 10);
  } else if (selection == "T5") {
    myBox[0] = 10;
    myBox[1] = 40 + 3 * (RECT_HEIGHT + 10);
  } else if (selection == "T6") {
    myBox[0] = 10 + RECT_WIDTH + 10;
    myBox[1] = 40 + 3 * (RECT_HEIGHT + 10);
  } else if (selection == "T7") {
    myBox[0] = 10;
    myBox[1] = 40 + 4 * (RECT_HEIGHT + 10);
  } else if (selection == "T8") {
    myBox[0] = 10 + RECT_WIDTH + 10;
    myBox[1] = 40 + 4 * (RECT_HEIGHT + 10);
  }
}

void updateBackground(String selection, String type, float value) {
  areaCalculator(selection);
  if (type == "battery" && value <= 12) {
    tft.fillRect(myBox[0], myBox[1], RECT_WIDTH, RECT_HEIGHT, ILI9341_WHITE);
  } else if (type == "battery" && value > 12) {
    tft.fillRect(myBox[0], myBox[1], RECT_WIDTH, RECT_HEIGHT, ILI9341_GREEN);
  } else if (type == "thruster" && value <= 1) {
    tft.fillRect(myBox[0], myBox[1], RECT_WIDTH, RECT_HEIGHT, ILI9341_WHITE);
  } else if (type == "thruster" && value <= 8.5) {
    tft.fillRect(myBox[0], myBox[1], RECT_WIDTH, RECT_HEIGHT, ILI9341_YELLOW);
  } else if (type == "thruster") {
    tft.fillRect(myBox[0], myBox[1], RECT_WIDTH, RECT_HEIGHT, ILI9341_RED);
  }
}

void textAreaCalculator(String selection) {
  if (selection == "B1") {
    myCursor[0] = 15;
    myCursor[1] = 55;
  } else if (selection == "B2") {
    myCursor[0] = 15 + RECT_WIDTH + 10;
    myCursor[1] = 55;
  } else if (selection == "T1") {
    myCursor[0] = 15;
    myCursor[1] = 55 + RECT_HEIGHT + 10;
  } else if (selection == "T2") {
    myCursor[0] = 15 + RECT_WIDTH + 10;
    myCursor[1] = 55 + RECT_HEIGHT + 10;
  } else if (selection == "T3") {
    myCursor[0] = 15;
    myCursor[1] = 55 + 2 * (RECT_HEIGHT + 10);
  } else if (selection == "T4") {
    myCursor[0] = 15 + RECT_WIDTH + 10;
    myCursor[1] = 55 + 2 * (RECT_HEIGHT + 10);
  } else if (selection == "T5") {
    myCursor[0] = 15;
    myCursor[1] = 55+ 3 * (RECT_HEIGHT + 10);
  } else if (selection == "T6") {
    myCursor[0] = 15 + RECT_WIDTH + 10;
    myCursor[1] = 55 + 3 * (RECT_HEIGHT + 10);
  } else if (selection == "T7") {
    myCursor[0] = 15;
    myCursor[1] = 55 + 4 * (RECT_HEIGHT + 10);
  } else if (selection == "T8") {
    myCursor[0] = 15 + RECT_WIDTH + 10;
    myCursor[1] = 55 + 4 * (RECT_HEIGHT + 10);
  }
}

void updateValueOnScreen(String selection, String type, float value) {
  textAreaCalculator(selection);
  tft.setCursor(myCursor[0], myCursor[1]);
  tft.print(selection);
  tft.print(" ");
  tft.print(String(value, 1).substring(0, 4));
  if (type == "battery") {
    tft.print("V");
  } else if (type == "thruster") {
    tft.print("A");
  }
}

void displayValues() {
  for (int i = 0; i < 10; i++) {
    if (newValues[i] != oldValues[i]) {
      oldValues[i] = newValues[i];
      updateBackground(selections[i], types[i], newValues[i]);
      updateValueOnScreen(selections[i], types[i], newValues[i]);
    }
  }
}
