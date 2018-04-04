/******************************************************************************
  Qwiic Relay Firmware 
  Kevin Kuwata @ SparkX
  March 21, 2018
  https://github.com/sparkfunX/Qwiic_Relay
  Arduino 1.8.5

  This is the firmware on the Qwiic Relay. The default I2C Address is
  0x18;

******************************************************************************/
#include <TinyWire.h> //https://github.com/lucullusTheOnly/TinyWire
#include <EEPROM.h>

#define RELAY_PIN   4
#define SETTING_LOCATION_ADDRESS	1

const float  firmwareVersion = 1.1;
 

volatile byte qwiicRelayAddress  =    0x18; //default

<<<<<<< HEAD
#define COMMAND_RELAY_OFF 0x00
#define COMMAND_RELAY_ON 0x01 
#define COMMAND_CHANGE_ADDRESS 0x03
#define COMMAND_FIRMWARE_VERSION 0x04
#define COMMAND_STATUS 0x05
#define COMMAND_HAS_BEEN_CHECKED 0x99

const byte firmwareVersionMajor = 1;
const byte firmwareVersionMinor = 0;
=======
>>>>>>> parent of 34565a5... Nate helped cleaned this up quite a bit, removed all flags so code is deterministic, and fixed the firmware float version issue. I call it the Beau trick. Take your high and low bytes, cast one to a float, add the lower byte divided by your decimal place, ie 10 for tenths, 100 for hundreths place, and then you're good to go

volatile byte command = 0x99;
volatile byte address = 0x99;

<<<<<<< HEAD
volatile byte command = COMMAND_HAS_BEEN_CHECKED;
=======
volatile byte versionFlag = 99; //this is the flag for flow control within the requestFrom ISR 
					//determines if you are going to send status or version, changed in main after 
					//receive 0x03 command. 
>>>>>>> parent of 34565a5... Nate helped cleaned this up quite a bit, removed all flags so code is deterministic, and fixed the firmware float version issue. I call it the Beau trick. Take your high and low bytes, cast one to a float, add the lower byte divided by your decimal place, ie 10 for tenths, 100 for hundreths place, and then you're good to go

void setup() {
	//Read EEPROM, is it empty (0xFF)? or does it have a value?
	 qwiicRelayAddress =  EEPROM.read(SETTING_LOCATION_ADDRESS);
	if(qwiicRelayAddress == 0xFF){
		//EEPROM has never been written before, use the default address 0x18.
		qwiicRelayAddress = 0x18; //default
	}
	
	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, LOW);

	TinyWire.begin(qwiicRelayAddress);	
	TinyWire.onReceive(receiveEvent); // register event
    TinyWire.onRequest(onI2CRequest);
}

void loop() {

	if(command == 0x01){
		digitalWrite(RELAY_PIN, HIGH);
		command = 0x99;

	}
	
	if(command == 0x00){
		digitalWrite(RELAY_PIN, LOW);
		command = 0x99;
	}
	
	if(command == 0x03){
// check if valid address, if not valid , ignore.
	if(address > 0x07 && address < 78){
			//valid address, update and save to EEPROM
						
		qwiicRelayAddress = address;	
		//save to EEPROM
		EEPROM.write(1, qwiicRelayAddress);
		TinyWire.begin(qwiicRelayAddress);		
		}			
		command = 0x99;
		address = 0x99;
	}
	
	if(command == 0x04 ){ //report back with firmware version
		versionFlag = 1;
	/*
		should be similar to the get status but get status was built when we only sent back status
		now we are deciding the status and the firmware version so we need to make a decision
		that leads me to think we need to set a flag of some sort. 
		
		come back to this fix the other examples first. 
		
		lets make sure the changes I did make work. 
	*/
	}
	/*
	//TODO: this might be necessary and make the master look exactly the same
	// structure as the getFrimwareVersion() in master 
	if(command == 0x05){
		versionFlag = 0;
	}
	*/	
}

/*========================================================*/
//        ISR
/*========================================================*/

// When the master initiates a command and data to slave
//		ie) the master says 0x01, then sends a 1, means command: 0x01 then the slave listens for the next thing, which is the relay state 1
//This is modifying ReceivedData array, which stores all the bytes that a master sent to the slave.
void receiveEvent(int bytesReceived) {
	byte count = 0; //count of all the data received from master.
<<<<<<< HEAD

	while(TinyWire.available() > 0){ 
		if(count == 0){
			command = TinyWire.read();
			
			if(command == COMMAND_RELAY_ON) {
				digitalWrite(RELAY_PIN, HIGH);
				command = COMMAND_HAS_BEEN_CHECKED;
			}
			else if(command == COMMAND_RELAY_OFF) {
				digitalWrite(RELAY_PIN, LOW);
				command = COMMAND_HAS_BEEN_CHECKED;
			}
		}
		else if(count == 1){
			if(command == COMMAND_CHANGE_ADDRESS) {
				byte address = TinyWire.read();

				if(address > 0x07 && address < 0x78){
					//valid address, update and save to EEPROM
					qwiicRelayAddress = address;	

					EEPROM.write(SETTING_LOCATION_ADDRESS, qwiicRelayAddress);
					TinyWire.begin(qwiicRelayAddress);		
				}			
			}
		}
		else{
			TinyWire.read(); //read the data coming in but ignore it.
		}
		count++;
	}
=======
  while(TinyWire.available() > 0){ 
	if(count == 0){
	command = TinyWire.read();
	}
	
	else if(count == 1){
	address = TinyWire.read();
	}
	else{
		TinyWire.read(); //read the data coming in but ignore it.
  }
  	count++;
  }
>>>>>>> parent of 34565a5... Nate helped cleaned this up quite a bit, removed all flags so code is deterministic, and fixed the firmware float version issue. I call it the Beau trick. Take your high and low bytes, cast one to a float, add the lower byte divided by your decimal place, ie 10 for tenths, 100 for hundreths place, and then you're good to go
}// end of receive ISR



//When the master requests data from the slave, this
  //    ISR is triggered. 

void onI2CRequest() {
	//not sure if this will work because the master sends 0x04, then the salve enters the RX event isr
	// then goes to main, sets the flag. then if that happens slower than versionFlag we have a race condition. 
<<<<<<< HEAD
	if(command == COMMAND_FIRMWARE_VERSION){
		//TinyWire.send(firmwareVersion);// tiny wire can't send multiple bytes.
		TinyWire.send(firmwareVersionMajor);
		TinyWire.send(firmwareVersionMinor);
		
		command = COMMAND_HAS_BEEN_CHECKED;
	}
	else if(command == COMMAND_STATUS){
		if(digitalRead(RELAY_PIN) == HIGH) 
			TinyWire.send(COMMAND_RELAY_ON);
		else
			TinyWire.send(COMMAND_RELAY_OFF); 
		
		command = COMMAND_HAS_BEEN_CHECKED;
=======

	if(versionFlag == 1){
	//TinyWire.send(firmwareVersion);// tiny wire can't send multiple bytes.
	TinyWire.send(0); //hard coded but we need to go to tiny core now. 
	TinyWire.send(1);
	TinyWire.send(1);
	TinyWire.send(0);
	versionFlag = 0; // reset the flag
	}
	else{
		TinyWire.send(0x11); //tiny.wire doesn't like to send multiple bytes so lets see how this goes. we can always_noconv
	//make the version a number just 1 byte then convert over to TinyCore.
	TinyWire.send(0x22); //tiny.wire doesn't like to send multiple bytes so lets see how this goes. we can always_noconv
	TinyWire.send(0x33); //tiny.wire doesn't like to send multiple bytes so lets see how this goes. we can always_noconv
	TinyWire.send(0x44); //tiny.wire doesn't like to send multiple bytes so lets see how this goes. we can always_noconv
	}
/*
 else if(versionFlag == 0 ){ 
	if(digitalRead(RELAY_PIN) == HIGH) TinyWire.send(0x01); 
	else{
		TinyWire.send(0x00); 
>>>>>>> parent of 34565a5... Nate helped cleaned this up quite a bit, removed all flags so code is deterministic, and fixed the firmware float version issue. I call it the Beau trick. Take your high and low bytes, cast one to a float, add the lower byte divided by your decimal place, ie 10 for tenths, 100 for hundreths place, and then you're good to go
	}
	versionFlag = 99;
}

*/
//put the else if inside so only 1 thing happens per ISR request. 

	
	/*
		so now that we have the version number [float] we want to send we need to have some flow control.
		
	*/
}// end of request ISR
