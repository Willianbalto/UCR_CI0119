#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

#define LED_PINa 4
#define LED_PINb 5
#define LED_PINc 6
#define Speaker 3


void setup(void) {
  Serial.begin(115200);
  lcd.begin(16, 2);
  pinMode(LED_PINa, OUTPUT);
  pinMode(LED_PINb, OUTPUT);
  pinMode(LED_PINc, OUTPUT);
  pinMode(Speaker, OUTPUT);

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

  for (int i = 4; i < 7; ++i) {
    digitalWrite(i, LOW);
  }

  mpu.getAccelerometerSensor()->getEvent(&a);
  mpu.getGyroSensor()->getEvent(&g);

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
  lcd.setCursor(2, 0);
  /// Verify the angle
  if ((g.gyro.x > 45.00) || (g.gyro.y > 45.00)
    || (g.gyro.z > 45.00)) {
    // if angle > 90 and lesser than 270, the vehicule is lying down
    if (((g.gyro.x > 90.00) && (g.gyro.x < 270.00) ) ||
      ((g.gyro.y > 90.00) && (g.gyro.y < 270.00)) ||
      ((g.gyro.z > 90.00) && (g.gyro.z < 270.00))) {
      lcd.setCursor(3, 0);
      lcd.print("Calling 911");
      digitalWrite(LED_PINa, HIGH);
      tone(Speaker, 250, 1000);
      // and call the 911.....
    } else if ((g.gyro.x < 90.00) || (g.gyro.y < 90.00) 
    || (g.gyro.z < 90.00)) {
    // if not, is because the angle is dangerous but not critical 
      lcd.setCursor(4, 0);
      lcd.print("Dangerous ");
      lcd.setCursor(6, 1);
      lcd.print("Angle");
      digitalWrite(LED_PINb, HIGH);
      tone(Speaker, 125, 500);
    }
  } else { // if all verification fails, is because the angle is correct
    lcd.print("All in Order");
    digitalWrite(LED_PINc, HIGH);
  }

  Serial.println("");

  delay(2000);
}
