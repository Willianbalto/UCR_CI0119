#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#include <Wire.h>

//  Pins for each LED and the buzzer
#define PIN_LED_GREEN 6
#define PIN_LED_YELLOW 5
#define PIN_LED_RED 4
#define PIN_BUZZER 3

//  Gyro measures radians, but degrees are more intuitive,
//  so degree-equivalent constants are defined for convenience.
#define DEGREES_45 3.14/4.0  //  Pi fourths radians. Equivalent to 45 degrees.
#define DEGREES_90 3.14/2.0  //  Pi halves radians. Equivalent to 90 degrees.
#define DEGREES_180 3.14  //  Pi radians. Equivalent to 180 degrees.
#define DEGREES_270 3.0 * (3.14/2.0)  //  Three Pi halves radians. Equivalent to 270 degrees.



Adafruit_MPU6050 mpu;
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);



//  Prints readings from sensor (acceleration and rotation) to serial monitor.
void printMPUReadings(sensors_event_t &a, sensors_event_t &g);



void setup(void) {
  Serial.begin(115200);

  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  lcd.begin(16, 2);

  while (!mpu.begin()) {
    if (!mpu.begin()) {
      Serial.println("MPU6050 not connected!");
      delay(1000);
    }
  }

  Serial.println("MPU6050 ready!");
}



sensors_event_t a, g;



void loop() {

  //  Turn off all LED pins.
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_RED, LOW);

  //  Update and print sensor readings
  mpu.getAccelerometerSensor()->getEvent(&a);
  mpu.getGyroSensor()->getEvent(&g);
  printMPUReadings(a, g);

  lcd.setCursor(2, 0);

  /// Verify the angle
  if (abs(g.gyro.x) > DEGREES_45 || abs(g.gyro.y) > DEGREES_45) {

    // if angle > 90 and lesser than 270, the vehicule is lying down
    if (abs(g.gyro.x) > DEGREES_90 || abs(g.gyro.y) > DEGREES_90) {
      lcd.setCursor(3, 0);
      lcd.print("Calling 911");
      digitalWrite(PIN_LED_RED, HIGH);
      tone(PIN_BUZZER, 250, 1000);
      // and call the 911.....

    // if not, it's because the angle is dangerous but not critical
    } else {
      lcd.setCursor(4, 0);
      lcd.print("Dangerous");
      lcd.setCursor(6, 1);
      lcd.print("Angle");
      digitalWrite(PIN_LED_YELLOW, HIGH);
      tone(PIN_BUZZER, 125, 500);

    }
  } else {  // if all verification fails, is because the angle is correct
    lcd.print("All in Order");
    digitalWrite(PIN_LED_GREEN, HIGH);
  }

  Serial.println("");

  delay(100);
}



void printMPUReadings(sensors_event_t &a, sensors_event_t &g)
{
  Serial.print("[");
  Serial.print(millis());
  Serial.print("] X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");


  Serial.print("[");
  Serial.print(millis());
  Serial.print("] X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");
}