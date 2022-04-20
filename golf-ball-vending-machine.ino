
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define COIN_ONE_PIN 2
#define COIN_TWO_PIN 3
#define COIN_THREE_PIN 4

#define MOTOR_ONE 5
#define MOTOR_TWO 6

#define VENDING_LED 8

const int vendingDurationUp = 5000;
const int vendingDurationDown = 5000;

enum statesVending
{
  NOT_RUNNING = 0,
  DOWN,
  UP
};

bool ledState;
int vendingState;
int vendingCount = 0 ;

unsigned long timeNow;
unsigned long timeVendingStart;


typedef struct {
  byte uid[4];
  byte credits;
} creditTag;

struct uid {
  byte uidByte[4];
};

creditTag tags[2];

void setup() {

  tags[0].uid[0] = 228; // E4
  tags[0].uid[1] = 47;  // 2F
  tags[0].uid[2] = 51;  // 33
  tags[0].uid[3] = 42;  // 2A
  tags[0].credits = 2;

  Serial.begin(9600);
  Serial.print("Power on\n");

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(COIN_ONE_PIN, INPUT);
  pinMode(COIN_TWO_PIN, INPUT);
  pinMode(COIN_THREE_PIN, INPUT);

  pinMode(MOTOR_ONE, OUTPUT);
  pinMode(MOTOR_TWO, OUTPUT);

  pinMode(8, OUTPUT);

  vendingState = statesVending::UP;
}

void loop() {
  timeNow = millis();
  updateStatusLED();

  switch ( vendingState )
  {
    case statesVending::NOT_RUNNING:

      // Turn off motor
      actuatorStop();
      digitalWrite(VENDING_LED, LOW);

      readInputs();

      if (vendingCount > 0)
      {
        Serial.print("Start vending down\n");
        vendingCount = vendingCount - 1;

        vendingState = statesVending::DOWN;
        timeVendingStart = millis();

      }
      break;

    case statesVending::DOWN:
      flashStatusLED();
      actuatorExtend();

      if (timeNow - timeVendingStart >= vendingDurationDown)
      {
        Serial.print("Start vending up\n");
        vendingState = statesVending::UP;
        timeVendingStart = millis();
      }

      break;

    case statesVending::UP:
      flashStatusLED();
      actuatorRetract();

      if (timeNow - timeVendingStart >= vendingDurationUp)
      {
        Serial.print("Stop vending\n");
        vendingState = statesVending::NOT_RUNNING;
      }

      break;
  }

}

void readInputs(void)
{

  if (digitalRead(COIN_ONE_PIN) == HIGH)
  {
    Serial.print("Coin 1 inserted\n");
    vendingCount = 1;
  }
  if (digitalRead(COIN_TWO_PIN) == HIGH)
  {
    Serial.print("Coin 2 inserted\n");
    vendingCount = 2;
  }

  checkNFC();

}

void actuatorExtend(void) {
  digitalWrite(MOTOR_ONE, HIGH);
  digitalWrite(MOTOR_TWO, LOW);
}

void actuatorRetract(void) {
  digitalWrite(MOTOR_ONE, LOW);
  digitalWrite(MOTOR_TWO, HIGH);
}

void actuatorStop(void) {
  digitalWrite(MOTOR_ONE, HIGH);
  digitalWrite(MOTOR_TWO, HIGH);
}

void updateStatusLED(void)
{
  static unsigned long timeBlink;

  if ((timeNow - timeBlink) >= 250ul)
  {
    timeBlink = timeNow;
    ledState ^= true;
  }
}

void flashStatusLED(void)
{
  if (ledState == false)
  {
    digitalWrite(VENDING_LED, HIGH);
  }
  else
  {
    digitalWrite(VENDING_LED, LOW);
  }
}
