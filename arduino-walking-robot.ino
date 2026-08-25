#include <Servo.h>
#include <IRremote.hpp>

Servo A;
Servo B;
Servo C;
Servo D;

const int A_PIN = 9;
const int B_PIN = 6;
const int C_PIN = 5;
const int D_PIN = 11;

const int IR_PIN = 2;

const unsigned long VOLUME_UP   = 0x46;
const unsigned long VOLUME_DOWN = 0x15;
const unsigned long MUTE        = 0x47;

int A_pos = 90;
int B_pos = 90;
int C_pos = 90;
int D_pos = 90;

const int STEP = 20;
const int MOVE_DELAY = 600;
const int SETTLE_DELAY = 250;

int direction = 0;

void moveAD(int amount) {
  A_pos -= amount;
  D_pos += amount;

  A_pos = constrain(A_pos, 0, 180);
  D_pos = constrain(D_pos, 0, 180);

  A.write(A_pos);
  D.write(D_pos);

  delay(MOVE_DELAY);
}

void moveBC(int amount) {
  B_pos += amount;
  C_pos -= amount;

  B_pos = constrain(B_pos, 0, 180);
  C_pos = constrain(C_pos, 0, 180);

  B.write(B_pos);
  C.write(C_pos);

  delay(MOVE_DELAY);
}

void walkCycle() {
  if (direction == 0) {
    return;
  }

  moveAD(STEP * direction);
  delay(SETTLE_DELAY);

  moveBC(STEP * direction);
  delay(SETTLE_DELAY);

  moveAD(-STEP * direction);
  delay(SETTLE_DELAY);

  moveBC(-STEP * direction);
  delay(SETTLE_DELAY);
}

void checkIR() {
  if (IrReceiver.decode()) {

    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {

      unsigned long command =
        IrReceiver.decodedIRData.command;

      if (command == VOLUME_UP) {
        direction = 1;
      }

      else if (command == VOLUME_DOWN) {
        direction = -1;
      }

      else if (command == MUTE) {
        direction = 0;
      }
    }

    IrReceiver.resume();
  }
}

void setup() {
  delay(5000);

  A.attach(A_PIN);
  B.attach(B_PIN);
  C.attach(C_PIN);
  D.attach(D_PIN);

  A.write(A_pos);
  B.write(B_pos);
  C.write(C_pos);
  D.write(D_pos);

  delay(2000);

  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (direction == 0) {
    checkIR();
    delay(500);
  }

  else {
    walkCycle();
    checkIR();
    delay(250);
  }
}