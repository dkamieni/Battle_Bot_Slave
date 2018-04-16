
#include "VescUart.h"
#include "datatypes.h"
#include "local_datatypes.h"
#include "Wire.h"
#define SERIALIO Serial //SerialIO for VESC
#define DEBUGSERIAL Serial //Serial for debugging
#define NUM_STEP 180
#define STEP_PIN 3 //Step pin for DRV8825
#define DIR_PIN 2 //Direction pin for DRV8825
#define LED_PIN 13 //LED_PIN for troubleshooting

byte throttle = 128;
byte ang = 0;
byte angle;
float rpm;
unsigned long step_timer;
unsigned long previous_time = 0;

void setup() {
  Wire.begin(8);            //Initialize I2C as slave with address 8
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  
  Wire.onReceive(data_available); //function to be called when I2C data from Master is available
  previous_time=micros();
}

void loop() {
  
  rpm = map(throttle, 0, 255, -7000, 7000); //map the throttle byte from -7000 to 7000 rpm (motor min and max)
  angle = map(ang, 0, 255, 0, 360);
  
  VescUartSetCurrent(rpm/1000); //NOTE: the packet data type was changed in VescUart.cpp from COMM_SET_CURRENT to COMM_SET_RPM
  
  step_timer=micros()-previous_time; //used for rotating steppers every 5 seconds
  
  if (step_timer > 5000000) //once step timer reaches 5 seconds, step 90deg
  {
    VescUartSetCurrent(0);
    step_90();
    previous_time = micros();
  }
}

void data_available ()
{
  while (Wire.available())
  {
   ang = Wire.read(); //first byte of data received is angle
   throttle = Wire.read(); //second byte of data received is throttle
  }
}

void step_90()
{
  for (int i = 0; i<=NUM_STEP; i++)
  {
    digitalWrite(STEP_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    delayMicroseconds(4000);
    digitalWrite(STEP_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    delayMicroseconds(4000);
  }
}

