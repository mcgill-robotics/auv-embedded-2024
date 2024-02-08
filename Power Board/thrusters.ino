/*

DO NOT FLASH TO TEENSY OR TO ARDUINO

*/

// test MCU kill switch
// test water interrupt
// test delay needed for thruster initialization
// test if ESCs arm after kill switches
// arm all thrusters

#include <Servo.h>

// boolean for interrupts
volatile bool interruptFlag = false;

// defines all MCU pins
#define SYSTEM_KILLED 0
#define THURSTERS_KILLED 1

#define SRG_P_PIN 	2
#define SRG_S_PIN	3
#define SWY_BW_PIN 	4
#define SWY_ST_PIN 	5
#define HVE_BW_P_PIN 	6
#define HVE_BW_S_PIN 	7
#define HVE_ST_S_PIN 	8
#define HVE_ST_P_PIN 	9

#define MCU_KS 10
#define WATER_DETECTED 12

#define TEENSY_LED 13

#define TC_1 14
#define TC_2 15
#define TC_3 16
#define TC_4 17
#define TC_5 18
#define TC_6 19
#define TC_7 20
#define TC_8 21

#define VBAT1_SENSE 22
#define VBAT2_SENSE 23

// defines 8 thursters for initialization in an array
#define SRG_P 	0
#define SRG_S	1
#define SWY_BW 	2
#define SWY_ST 	3
#define HVE_BW_P 	4
#define HVE_BW_S 	5
#define HVE_ST_S 	6
#define HVE_ST_P 	7

// creates array of 8 thrusters
Servo thrusters[8];

// signals to push to thrusters
uint16_t microseconds[] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500}; 
const uint16_t offCommand[] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500}; 

// updates thrusters' pwm signals from array
void updateThrusters(const uint16_t microseconds[8]) {
	thrusters[SRG_P].writeMicroseconds(microseconds[SRG_P]);
	thrusters[SRG_S].writeMicroseconds(microseconds[SRG_S]);
	thrusters[SWY_BW].writeMicroseconds(microseconds[SWY_BW]);
	thrusters[SWY_ST].writeMicroseconds(microseconds[SWY_ST]);
	thrusters[HVE_BW_P].writeMicroseconds(microseconds[HVE_BW_P]);
	thrusters[HVE_BW_S].writeMicroseconds(microseconds[HVE_BW_S]);
	thrusters[HVE_ST_P].writeMicroseconds(microseconds[HVE_ST_P]);
	thrusters[HVE_ST_S].writeMicroseconds(microseconds[HVE_ST_S]);
}

// attaches and arms thrusters
void initThrusters() {
	thrusters[SRG_P].attach(SRG_P_PIN);
	thrusters[SRG_S].attach(SRG_S_PIN);
	thrusters[SWY_BW].attach(SWY_BW_PIN);
	thrusters[SWY_ST].attach(SWY_ST_PIN);
	thrusters[HVE_BW_P].attach(HVE_BW_P_PIN);
	thrusters[HVE_BW_S].attach(HVE_BW_S_PIN);
	thrusters[HVE_ST_S].attach(HVE_ST_S_PIN);
	thrusters[HVE_ST_P].attach(HVE_ST_P_PIN);

	updateThrusters(offCommand);
	delay(7000);

	interruptFlag = false;
}

void killSystem() {
	digitalWrite(MCU_KS, HIGH);
	delay(100);
}

void powerSystem() {
	digitalWrite(MCU_KS, LOW);
	delay(100);
}

void interruptRaised() {
	interruptFlag = true;
}

void waterInterrupt() {
	if (digitalRead(WATER_DETECTED) == HIGH) {
        killSystem();
    } else {
        powerSystem();
    }
}

void setup() {
	pinMode(SYSTEM_KILLED, INPUT_PULLUP);
	pinMode(THURSTERS_KILLED, INPUT_PULLUP);
	pinMode(WATER_DETECTED, INPUT_PULLUP);

	pinMode(MCU_KS, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(SYSTEM_KILLED), interruptRaised, RISING);
	attachInterrupt(digitalPinToInterrupt(THURSTERS_KILLED), interruptRaised, RISING);
	attachInterrupt(digitalPinToInterrupt(WATER_DETECTED), waterInterrupt, CHANGE);

	initThrusters();
}

void loop() {
	if (interruptFlag) {
		initThrusters();
	}

	if (digitalRead(WATER_DETECTED) == LOW) {
		updateThrusters(microseconds);
	}
}
