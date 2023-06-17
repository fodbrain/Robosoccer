#include <POP32.h>
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
#define head_Kp 1.0f
#define head_Ki 0.0f
#define head_Kd 0.0f
float head_error, head_pError,head_w ,head_d,head_i;
uint8_t rxCnt = 0, rxBuf[8];

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
void heading(float spd, float theta, float spYaw){
head_error = spYaw - pvYaw;
head_i = head_i + head_error;
head_i = constrain(head_i ,-180,180);
head_d = head_error - head_pError;
head_w = (head_error * head_Kp) + (head_i * head_Ki) + (head_d * head_Kd);
head_w = constrain(head_w ,-100,100);
holonomic(spd, theta, head_w);
head_pError = head_error;
}
void setup() {
 zeroYaw();
}
void loop() {
  if (SW_A()) {
    zeroYaw();
  }
  getIMU();
  heading(50,0,0);
  delay(500);

  getIMU();
  heading(50,90,0);
  delay(500);

  getIMU();
  heading(50,180,0);
  delay(500);

  getIMU();
  heading(50,270,0);
  delay(500);
}
