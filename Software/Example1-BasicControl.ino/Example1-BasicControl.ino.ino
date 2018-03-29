/******************************************************************************
  Qwiic Relay Example 1 - Basic Control
  Kevin Kuwata @ SparkX
  March 21, 2018
  https://github.com/sparkfunX/Qwiic_Relay
  Arduino 1.8.5

  This sketch controls the Qwiic Relay breakout over I2C and demonstrates
  the basic functionality.


******************************************************************************/

#include <Wire.h>

const byte qwiicRelayAddress = 0x18;     //Default Address

void setup() {
  Serial.begin(9600);
  Serial.println("Qwiic Relay Example 1 Basic Control");
  Wire.begin(); // join the I2C Bus
  
  testForConnectivity();
  
}

void loop() {
  relayOn();    // Turn the Relay On
  Serial.println("Relay is On.");
  delay(2000);  // Wait 2 Seconds
  relayOff();   // Turn the Relay Off
  Serial.println("Relay is Off.");
  delay(2000);  // Wait 2 Seconds
}

// RelayOn() turns on the relay at the qwiicRelayAddress
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOn() {
  Wire.beginTransmission(qwiicRelayAddress);
  Wire.write(0x01);          
  Wire.endTransmission(); 
}


// RelayOff() turns off the relay at the qwiicRelayAddress
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOff() {
  Wire.beginTransmission(qwiicRelayAddress); 
  Wire.write(0x00);              
  Wire.endTransmission();       
}

void testForConnectivity(){
	Wire.beginTransmission(qwiicRelayAddress);
	//check here for an ACK from the slave, if no ack don't allow change?
	if(Wire.endTransmission() != 0){
		Serial.println("Check Connections. No slave attached.");
		while(1);
	}
}