#include <SoftwareSerial.h>

/**
 * BT PIN 6666
 * PWD TO UNLOCK 6666
 * 
 * ARDUINO UNO PIN A0 LOCK ANALOG INPUT (RED)
 * ARDUINO UNO PIN 8 - BT TX & PIN 9 - BT RX
 * ARDUINO UNO PIN 7 - RELAY IN2
 */

SoftwareSerial mySerial(8, 9); // PIN 8 - BT TX, 9 - BT RX
float sensorValue = 0;        //variable to store the value coming from the sensor
int SENSOR = A0;              //PIN A0 to RED of lock
int LED = 13;                 // select the pin for the LED
int RESET = 3;                // BT module uses pin 7 for reset
int openCount = 0;
char readChar;
boolean isOpen = false;
boolean isManual = false;
String password;

void setup() {
  pinMode(LED, OUTPUT); // declare the LED and BT RESET pins as output
  pinMode(RESET, OUTPUT);
  pinMode(4, INPUT);
  pinMode(7, OUTPUT); //PIN7 - RELAY IN2
  Serial.begin(9600);
  // Wait for serial port to connect. Needed for Leonardo only
  while (!Serial) {
    ;
  }

  mySerial.begin(9600);
  Serial.println("begin");

  digitalWrite(RESET, HIGH);
  delay(10);
  digitalWrite(RESET, LOW);
  delay(2000);
  mySerial.print("AT\r\n"); //Check Status
  Serial.println("end of AT");
  delay(200);

  mySerial.println("AT+BAUD4\r\n");
  Serial.println("end of AT baud");
  delay(200);

  mySerial.println("AT+NAMEDEVCON\r\n");
  Serial.println("end of AT name");
  delay(200);

  //BT PIN FOR PAIRING
  mySerial.println("AT+PIN6666\r\n");
  delay(200);
  Serial.println("end of AT pin");
  Serial.println("end of setup");

  digitalWrite(7, HIGH);
}

void loop() // Main loop will run unless interrupted
{
  while (mySerial.available()) {
    password = mySerial.readStringUntil('\n');
    Serial.print(password);
    delay(10);
  }
  if (password == "6666") {
    digitalWrite(7, LOW);
    Serial.print("unlock");
    password = "";
    delay(1000);
    digitalWrite(7, HIGH);
    isOpen = true;
    isManual = false;
  }
  while (Serial.available()) {
    readChar = Serial.read();
    Serial.print(readChar);
    password = "";
    mySerial.print(readChar);
    delay(10);
  }
  sensorValue = analogRead(SENSOR);
  if (isOpen) {
    Serial.print(sensorValue);

    Serial.println(" isOpen");
    if (sensorValue > 1000) {
      if (isManual) {
        digitalWrite(LED, LOW);
        delay(100);
      }
      digitalWrite(LED, HIGH);
    } else {
      isOpen = false;
    }
  } else {
    if (sensorValue < 1000) {
      isOpen = false;
      openCount = 0;
    } else {
      openCount++;
      isOpen = openCount >= 20;
      isManual = isOpen;
    }
    Serial.println(sensorValue);
    digitalWrite(LED, LOW);
  }
  delay(100);
}
