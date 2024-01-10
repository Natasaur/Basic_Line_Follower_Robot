#include <QTRSensors.h>
#include <OrangutanMotors.h>
#include <OrangutanLEDs.h>

QTRSensors qtr;         //Create an instance for the QTR Sensors
OrangutanMotors motors; //Create an instance for the Motors
OrangutanLEDs led;      //Create an instance for the build-in Led
int buttonPin = A5;
int button;
int v = 235; //Set Motors' speed | You can adjust it just in this line
boolean start;

const uint8_t SensorCount = 8; //Number of sensors you're using
uint16_t sensorValues[SensorCount];


void setup() {
  // put your setup code here, to run once:
    
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){8, 9, 10, 12, 13, 2, 4, 7}, SensorCount);

  delay(500);
  led.red(HIGH); // turn on the LED to indicate we are in calibration mode

  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  led.red(LOW); // turn off the LED to indicate we are through with calibration

  //Waiting for button to be pressed to start
  button = digitalRead(buttonPin);
  while(button == 1){ //It exits the while when the button is pressed to continue
    button = digitalRead(buttonPin);
    //do nothing
  }
  delay(5000); //Waits 5 seconds after the button is pressed
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t position = qtr.readLineBlack(sensorValues); //This line returns the value of the sensor over the line
  // Sensor 1 |  Sensor 2 |  Sensor 3 |  Sensor 4 |  Sensor 5 |  Sensor 6 |  Sensor 7 |  Sensor 8
  //  0000    |    1000   |    2000   |    3000   |    4000   |    5000   |    6000   |    7000
  //We want to keep the line in the middle, so we divide 7000/2=3500
  //Change this value to the number of sensors you're using
  
  if (position == 3500) { //go straight forward
    forward(v);
  } else if (position > 3500) {//turn left
    left_(v);
  } else if (position < 3500) {//turn right
    right_(v);
  }
}

void forward(int v) {
  motors.setSpeeds(v,v);
}

void right_(int v) {
  motors.setSpeeds(v,0); //You can change the order regardless the polaritie of the motors, you want both to go forward
}

void left_(int v) {
  motors.setSpeeds(0,v);
}

