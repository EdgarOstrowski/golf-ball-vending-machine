
const int coinOne = 2;
const int coinTwo = 3;
const int coinThree = 4;

const int motorOne = 8;
const int motorTwo = 9;
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

void setup() {
  Serial.begin(9600);

  pinMode(coinOne, INPUT);
  pinMode(coinTwo, INPUT);
  pinMode(coinThree, INPUT);
  pinMode(motorOne, OUTPUT);
  pinMode(motorTwo, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

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
      digitalWrite(LED_BUILTIN, LOW);

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

  if (digitalRead(coinOne) == HIGH)
  {
    Serial.print("Coin 1 inserted\n");
    vendingCount = 1;
  }
  if (digitalRead(coinTwo) == HIGH)
  {
    Serial.print("Coin 2 inserted\n");
    vendingCount = 2;
  }

}

void actuatorExtend(void) {
  digitalWrite(motorOne, HIGH);
  digitalWrite(motorTwo, LOW);
}

void actuatorRetract(void) {
  digitalWrite(motorOne, LOW);
  digitalWrite(motorTwo, HIGH);
}

void actuatorStop(void) {
  digitalWrite(motorOne, HIGH);
  digitalWrite(motorTwo, HIGH);
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
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
