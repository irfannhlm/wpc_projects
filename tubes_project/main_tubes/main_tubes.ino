#include "MPU6050.h"
#include "Wire.h"
#include "ESP32Servo.h"

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define LED_PIN 19
#define BUTTON_PIN 15
#define SERVO_PIN 12

Servo servo;

const int len_seq = 4;
const int sign_seq[len_seq] = {0, 1, 0, 1}; // 0 for positive ; 1 for negative
const int axis_seq[len_seq] = {0, 0, 1, 1}; // 0 for x axis; 1 for y axis
int corr_seq[len_seq] = {0, 0, 0, 0};
int buffer = 1;
bool state = false;

void setup() {
  Wire.begin();
  accelgyro.initialize();
//  servo.attach(SERVO_PIN);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(115200);
}

bool checkAngle(int16_t ax, int16_t ay, int axis, int sign) {
  const int16_t min_angle = 7000;
  int16_t u_ax = sign ? -ax : ax;
  int16_t u_ay = sign ? -ay : ay;
  bool state;

  if (axis) {state = u_ay > min_angle;}
  else {state = u_ax > min_angle;};

  return state;
}

bool checkSequence (int corr_seq[], int len_seq) {
  bool state = true;
  for (int i = 0; i < len_seq; i++) {
    if (!corr_seq[i]) {state = false; break;}
  }
  return state;
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  if (digitalRead(BUTTON_PIN)) {
    Serial.print("a/g:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.println();

    // reset sequence if already finish
    if (checkSequence(corr_seq, len_seq)) {
      for (int j = 0; j < len_seq; j++) {corr_seq[j] = 0;}
    }

    // check the sequence if right
    for (int i = 0; i < len_seq; i++) {
      if (corr_seq[i]) {continue;}
      else {
        if (checkAngle(ax, ay, axis_seq[i], sign_seq[i])) {
          // correct for one seq
          corr_seq[i] = 1;
          break;
        }
        else {
          // reset sequence
          for (int j = 0; j < len_seq; j++) {corr_seq[j] = 0;}
          break;
        }
      }
    }

    // printout sequence
    for (int i = 0; i < len_seq; i++) {
      if (i < len_seq-1) {Serial.print(corr_seq[i]);}
      else {Serial.println(corr_seq[i]);}
    }

  }

  // check right sequence signal for rising edge
  if ((buffer != checkSequence(corr_seq, len_seq)) && checkSequence(corr_seq, len_seq)) {
    state = !state;
    Serial.print("Current LED state: "); Serial.println(state);
  }

  buffer = checkSequence(corr_seq, len_seq);

  // if (state) {
  //   servo.write(180);
  // } else {
  //   servo.write(0);
  // }

  digitalWrite(LED_PIN, state);

  while (digitalRead(BUTTON_PIN)) {delay(20);}
  delay(20);
}
