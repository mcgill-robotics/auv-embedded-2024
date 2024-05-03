#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Wire.h>
#include "MS5837.h"

#define TFT_DC 9
#define TFT_CS 10

#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 240
#define RECT_WIDTH 105
#define RECT_HEIGHT 46

MS5837 sensor;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

float newValues[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float oldValues[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

String selections[10] = {"B1", "B2", "T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8"};
String types[10] = {"battery", "battery", "thruster", "thruster", "thruster", "thruster", "thruster", "thruster", "thruster", "thruster"};

int myBox[2];
int myCursor[2];

ros::NodeHandle nh;

void batt1VoltageCallback(const std_msgs::Float32& msg) {
    newValues[0] = msg.data;
}

void batt2VoltageCallback(const std_msgs::Float32& msg) {
    newValues[1] = msg.data;
}

void thrust1CurrentCallback(const std_msgs::Float32& msg) {
    newValues[2] = msg.data;
}

void thrust2CurrentCallback(const std_msgs::Float32& msg) {
    newValues[3] = msg.data;
}

void thrust3CurrentCallback(const std_msgs::Float32& msg) {
    newValues[4] = msg.data;
}

void thrust4CurrentCallback(const std_msgs::Float32& msg) {
    newValues[5] = msg.data;
}

void thrust5CurrentCallback(const std_msgs::Float32& msg) {
    newValues[6] = msg.data;
}

void thrust6CurrentCallback(const std_msgs::Float32& msg) {
    newValues[7] = msg.data;
}

void thrust7CurrentCallback(const std_msgs::Float32& msg) {
    newValues[8] = msg.data;
}

void thrust8CurrentCallback(const std_msgs::Float32& msg) {
    newValues[9] = msg.data;
}

std_msgs::Float64 depthmsg;

ros::Publisher depth("sensors/depth/z", &depthmsg);
ros::Subscriber<std_msgs::Float32> batt1VoltageSub("batt1_voltage", &batt1VoltageCallback);
ros::Subscriber<std_msgs::Float32> batt2VoltageSub("batt2_voltage", &batt2VoltageCallback);
ros::Subscriber<std_msgs::Float32> thrust1CurrentSub("thrust1_current", &thrust1CurrentCallback);
ros::Subscriber<std_msgs::Float32> thrust2CurrentSub("thrust2_current", &thrust2CurrentCallback);
ros::Subscriber<std_msgs::Float32> thrust3CurrentSub("thrust3_current", &thrust3CurrentCallback);
ros::Subscriber<std_msgs::Float32> thrust4CurrentSub("thrust4_current", &thrust4CurrentCallback);
ros::Subscriber<std_msgs::Float32> thrust5CurrentSub("thrust5_current", &thrust5CurrentCallback);
ros::Subscriber<std_msgs::Float32> thrust6CurrentSub("thrust6_current", &thrust6CurrentCallback);
ros::Subscriber<std_msgs::Float32> thrust7CurrentSub("thrust7_current", &thrust7CurrentCallback);
ros::Subscriber<std_msgs::Float32> thrust8CurrentSub("thrust8_current", &thrust8CurrentCallback);

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

void setup() {
  Wire.begin();

  while (!sensor.init()) {
    delay(5000);
  }

  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997);
  
  tft.begin();
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);

  tft.setCursor(10, 10);
  tft.println("Dougie Status");

  nh.initNode();
  nh.advertise(depth);
  nh.subscribe(batt1VoltageSub);
  nh.subscribe(batt2VoltageSub);
  nh.subscribe(thrust1CurrentSub);
  nh.subscribe(thrust2CurrentSub);
  nh.subscribe(thrust3CurrentSub);
  nh.subscribe(thrust4CurrentSub);
  nh.subscribe(thrust5CurrentSub);
  nh.subscribe(thrust6CurrentSub);
  nh.subscribe(thrust7CurrentSub);
  nh.subscribe(thrust8CurrentSub);
}

void loop() {
  sensor.read();
  depthmsg.data = sensor.depth();
  depth.publish(&depthmsg);

  displayValues();

  nh.spinOnce();

  delay(1000);
}
