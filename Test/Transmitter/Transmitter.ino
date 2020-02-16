//This part of code draws 14.5mA.

// #include <RH_ASK.h>
// #include <LowPower.h>

// RH_ASK driver;
// static constexpr char* motionMessage = "stairs";
// bool sendMotion = false;


// void motionDetected(){
// 	sendMotion = true;
// }

// void setup() {
// 	pinMode(LED_BUILTIN,OUTPUT);
//   	digitalWrite(LED_BUILTIN,LOW);
//   	driver.init();
// }

// void loop() {
//   	attachInterrupt(digitalPinToInterrupt(2), motionDetected, RISING);
//   	LowPower.idle(SLEEP_FOREVER, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
// 	if(sendMotion){
// 		driver.send((uint8_t *)motionMessage, strlen(motionMessage));
// 		driver.waitPacketSent();
// 		sendMotion = false;
// 	}
// 	detachInterrupt(2);
// }




// This sketch draws 5.5mA. This is, with the power led turned on.
// The led has a 1k resistor in series and is supplied with 5v. This means
// the led draws 5mA. Hence, we're left with 0.5mA current draw.
// #include <RH_ASK.h>
// #include <avr/sleep.h>

// RH_ASK driver;
// static constexpr char* motionMessage = "stairs";
// bool sendMotion = false;


// void motionDetected(){
// 	sendMotion = true;
// }

// void setup() {
// 	ADCSRA = 0; 		//Disable ADC

// 	pinMode(LED_BUILTIN,OUTPUT);
// 	pinMode(2, INPUT);

//   	digitalWrite(LED_BUILTIN,LOW);
//   	driver.init();
// }

// void loop() {
//   	sleep_enable();
//   	sleep_bod_disable();
// 	attachInterrupt(digitalPinToInterrupt(2), motionDetected, RISING);
// 	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
// 	sleep_cpu();
// 	if(sendMotion){
// 		driver.send((uint8_t *)motionMessage, strlen(motionMessage));
// 		driver.waitPacketSent();
// 		sendMotion = false;
// 	}
// 	detachInterrupt(2);
// }



// Still the same.
// #include <RH_ASK.h>
// #include <avr/sleep.h>
// #include "climate.hpp"

// RH_ASK transmitter;
// static constexpr char* motionMessage = "stairs";
// bool sendMotion = false;

// tempHumSensor climateSensor(transmitter, 7, 6000);

// void motionDetected(){
// 	sendMotion = true;
// }

// void setup() {
// 	ADCSRA = 0; 		//Disable ADC

// 	pinMode(LED_BUILTIN,OUTPUT);
// 	pinMode(2, INPUT);

//   	digitalWrite(LED_BUILTIN,LOW);
//   	transmitter.init();
// }

// void loop() {
//   	sleep_enable();
//   	sleep_bod_disable();
// 	attachInterrupt(digitalPinToInterrupt(2), motionDetected, RISING);
// 	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
// 	sleep_cpu();
// 	if(sendMotion){
// 		transmitter.send((uint8_t *)motionMessage, strlen(motionMessage));
// 		transmitter.waitPacketSent();
// 		sendMotion = false;
// 	}
// 	detachInterrupt(2);

// 	//if(climateSensor.timeToMeasure()){
//         climateSensor.measureAndPublish();
//     //}
// }


//Lowest thus far. futhermore, sensor readings can be started.
#include <RH_ASK.h>
#include <LowPower.h>
#include "climate.hpp"

RH_ASK transmitter;
static constexpr char* motionMessage = "stairs";
bool sendMotion = false;

tempHumSensor climateSensor(transmitter, 7, 60000);

void motionDetected(){
	sendMotion = true;
}

void setup() {
	pinMode(LED_BUILTIN,OUTPUT);
	pinMode(2, INPUT);

  	digitalWrite(LED_BUILTIN,LOW);
  	transmitter.init();
}

void loop() {
  	attachInterrupt(digitalPinToInterrupt(2), motionDetected, RISING);
  	LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

	if(sendMotion){
		digitalWrite(LED_BUILTIN,HIGH);
		transmitter.send((uint8_t *)motionMessage, strlen(motionMessage));
		transmitter.waitPacketSent();
		sendMotion = false;
	}

	if(climateSensor.timeToMeasure()){
		climateSensor.measureAndPublish();
	}

	digitalWrite(LED_BUILTIN,LOW);
	detachInterrupt(2);
}
