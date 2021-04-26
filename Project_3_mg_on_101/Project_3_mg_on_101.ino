#include "CurieIMU.h"

int ledPin = 12;                // LED connected to digital pin 13
float px, py, pz;  // store previous values
int switchFlag; // flag variable to indicate if we should perform a task due to motion
float sensitivity = 0.10; // how much variation resets the timer
int counter = 0; // we count seconds of no movement

void setup()                    // run once, when the sketch starts
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  // code borrowed from https://www.arduino.cc/en/Tutorial/Genuino101CurieIMUAccelerometer
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open
  // initialize device
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();
  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);

  px = 0.0;
  py = 0.0;
  pz = 0.0;
}

void loop()
{
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(500);                  // waits for a second
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(500);                  // waits for a second

  // Intel's accelerometer code...
  float ax, ay, az;   //scaled accelerometer values

  // read accelerometer measurements from device, scaled to the configured range
  CurieIMU.readAccelerometerScaled(ax, ay, az);

  // display tab-separated accelerometer x/y/z values
  Serial.print("a:\t");
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(ay);
  Serial.print("\t");
  Serial.print(az);
  //  Serial.println();

  Serial.print("\tp:\t");
  Serial.print(px);
  Serial.print("\t");
  Serial.print(py);
  Serial.print("\t");
  Serial.print(pz);
  Serial.println();

  if (testMovement(px, ax) or testMovement(py, ay) or testMovement(pz, az)) {
    Serial.println("Movement!!!!!");
    Serial.print("Number of seconds passed: ");
    Serial.println(counter);
    counter = 0; // reset counter
    px = ax;
    py = ay;
    pz = az;
  } else {  
    counter += 1;
  }
}

int testMovement(float previous, float recent)
{
  if (abs(previous - recent) > sensitivity) {
    return 1;
  } else {
    return 0;
  }
}
