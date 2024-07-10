/*
  Knock Sensor

  This sketch reads a piezo element to detect a knocking sound.
  It reads an analog pin and compares the result to a set threshold.
  If the result is greater than the threshold, it writes "knock" to the serial
  port, and toggles the LED on pin 13.

  The circuit:
	- positive connection of the piezo attached to analog in 0
	- negative connection of the piezo attached to ground
	- 1 megohm resistor attached from analog in 0 to ground

  created 25 Mar 2007
  by David Cuartielles <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Knock
*/


// these constants won't change:
const int ledPin = 13;       // LED connected to digital pin 13
const int knockSensor = A0;  // the piezo is connected to analog pin 0
const int middleSensor = A1;
const int threshold = 20;   // threshold value to decide when the detected sound is a knock or not


// these variables will change:
int sensorReading = 0;  // variable to store the value read from the sensor pin
int middleReading = 0;
int ledState = LOW;  // variable used to store the last LED status, to toggle the light
int count = 0; 

byte noteON = 144;


void setup() {
  pinMode(ledPin, OUTPUT);  // declare the ledPin as as OUTPUT
  Serial.begin(9600);       // use the serial port
}

void loop() {
  // read the sensor and store it in the variable sensorReading:
  sensorReading = analogRead(knockSensor);
  middleReading = analogRead(middleSensor);

  byte velocity = map(sqrt(sensorReading), 0, sqrt(1023), 0, 127);
  byte velocityMiddle = map(sqrt(middleReading), 0, sqrt(1023), 0, 127);
  byte nam = 60;
  byte dhi = 62;

  // if the sensor reading is greater than the threshold:
  if (sensorReading >= threshold) {
    if (middleReading < 20) {
      // toggle the status of the ledPin:
      ledState = !ledState;
      // update the LED pin itself:
      digitalWrite(ledPin, ledState);
      // send the string "Knock!" back to the computer, followed by newline
      // Serial.println("Knock!");
      MIDImessage(noteON, nam, velocity);//turn note on
      delay(40);//hold note for 300ms
      MIDImessage(noteON, nam, 0);//turn note off (note on with velocity 0)
    }
    if (middleReading >= threshold) {
      MIDImessage(noteON, dhi, velocity);//turn note on
      delay(40);//hold note for 300ms
      MIDImessage(noteON, dhi, 0);//turn note off (note on with velocity 0)
    }
  }
  if (middleReading >= threshold) {
    if (sensorReading < 20) {
      MIDImessage(noteON, dhi, velocity);//turn note on
      delay(40);//hold note for 300ms
      MIDImessage(noteON, dhi, 0);//turn note off (note on with velocity 0)
    }
    if (sensorReading >= threshold) {
      MIDImessage(noteON, nam, velocity);//turn note on
      delay(40);//hold note for 300ms
      MIDImessage(noteON, nam, 0);//turn note off (note on with velocity 0)
    }
  }
  delay(100);  // delay to avoid overloading the serial port buffer
}

//send MIDI message
void MIDImessage(byte command, byte data1, byte data2) {
  Serial.write(command);
  Serial.write(data1);
  Serial.write(data2);
}