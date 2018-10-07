#define PINCLK  2 // YELLOW
#define PINLAT  3 // WHITE
#define PINDAT  4 // RED
#define PINDAT2  5 // BLUE
#define PINVCC  7 // GREEN

#define WRITE_DAT_HIGH PORTD |= (1 << PINDAT)  // Pin 4 (dat) B00010000
#define WRITE_DAT_LOW  PORTD &= ~(1 << PINDAT) // Pin 4 (dat) B11101111

volatile int clockCount = 0;
volatile int ltc = 0;
volatile int currentInput = 0;
volatile int normalInput = (B00000000 << 8) + B00000000;
volatile int inputs[] = {
  normalInput + 1 << 0,  // B
  normalInput + 1 << 1,  // Y
  normalInput + 1 << 2,  // SELECT
  normalInput + 1 << 3,  // START
  normalInput + 1 << 4,  // UP
  normalInput + 1 << 5,  // DOWN
  normalInput + 1 << 6,  // LEFT
  normalInput + 1 << 7,  // RIGHT
  normalInput + 1 << 8,  // A
  normalInput + 1 << 9,  // X
  normalInput + 1 << 10, // L
  normalInput + 1 << 11, // R
 };
 volatile int currentInputNumber = 0;


void setup() {
  attachInterrupt(digitalPinToInterrupt(PINCLK), doClock, RISING);
  attachInterrupt(digitalPinToInterrupt(PINLAT), doLatch, RISING);

  pinMode(PINDAT, OUTPUT);
  pinMode(PINVCC, INPUT);

  while (((PIND >> PINVCC) & 1) == 0);
}

void loop() {
}

void doLatch() {
  currentInput = inputs[getInputNumber()];
  clockCount = 0;
  writeData();
}

void doClock() {
  clockCount++;
  writeData();
}

void writeData() {
  if (((currentInput >> clockCount) & 1) || clockCount >= 16) WRITE_DAT_LOW;
  else WRITE_DAT_HIGH;
}

int getInputNumber() {
  if (currentInputNumber == sizeof(currentInputNumber)) {
    currentInputNumber = 0;
  } else {
    currentInputNumber += 1;
  }
  return currentInputNumber;
}
