#include <POP32.h>
void setup() {
  // put your setup code here, to run once
}
void wheel(float spd1,float spd2,float spd3){
  motor(1,spd1);
  motor(2,spd2);
  motor(3,spd3);
}

#define degToRad 0.0174f
#define sin30 sin(30.f * degToRad)
#define cos30 cos(30.f * degToRad)

float thetaRad, vx, vy, spd1, spd2, spd3;
void holonomic(float spd, float theta, float omega){
thetaRad = theta * degToRad;
vx = spd * cos(thetaRad);
vy = spd * sin(thetaRad);
spd1 = vy * cos30 - vx * sin30 + omega;
spd2 = - vy * cos30 - vx * sin30 + omega;
spd3 = vx + omega;
wheel(spd1, spd2, spd3);
}
void zeroYaw() {
  Serial1.begin(115200);delay(100);
  // Sets data rate to 115200 bps
  Serial1.write(0XA5);Serial1.write(0X54);delay(100);
  // pitch correction roll angle
  Serial1.write(0XA5);Serial1.write(0X55);delay(100);
  // zero degree heading
  Serial1.write(0XA5);Serial1.write(0X52);delay(100);
  // automatic mode
}

float pvYaw;
uint8_t rxCnt = 0, rxBuf[8];
bool getIMU() {
  while (Serial1.available()) {
    rxBuf[rxCnt] = Serial1.read();
    if (rxCnt == 0 && rxBuf[0] != 0xAA) return;
    rxCnt++;
    if (rxCnt == 8) { // package is complete
      rxCnt = 0;
      if (rxBuf[0] == 0xAA && rxBuf[7] == 0x55) { // data package is correct
        pvYaw = (int16_t)(rxBuf[1] << 8 | rxBuf[2]) / 100.f;
        return true;
      }}}
  return false;
}

void loop() {
  // put your main code here, to run repeatedly:
  holonomic(50,36,0);
  delay(500);

  holonomic(50,72,0);
  delay(500);

  a0();
}
