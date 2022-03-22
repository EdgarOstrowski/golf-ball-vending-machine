
const int coinOne = 2;
const int coinTwo = 3;
const int coinThree = 4;

const int motorOne = 8;
const int motorTwo = 9;
const int vendingDuration = 5000;

enum statesVending
{
  NOT_RUNNING = 0,
  DOWN,
  UP
};

bool ledState;
int vendingState;

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
  // NOT_RUNNING;
}

void loop() {
  timeNow = millis();
  updateStatusLED();

  int buttonState = digitalRead(coinOne);

  switch ( vendingState )
  {
    case statesVending::NOT_RUNNING:

      // Turn off motor
      digitalWrite(motorOne, HIGH);
      digitalWrite(motorTwo, HIGH);
      digitalWrite(LED_BUILTIN, LOW);

      if (buttonState == HIGH)
      {
        vendingState = statesVending::DOWN;
        timeVendingStart = millis();
      }
      break;

    case statesVending::DOWN:
      flashStatusLED();

      // Turn on motor
      digitalWrite(motorOne, HIGH);
      digitalWrite(motorTwo, LOW);

      if (timeNow - timeVendingStart >= vendingDuration)
      {
        vendingState = statesVending::UP;
        timeVendingStart = millis();
      }

      break;

    case statesVending::UP:
      flashStatusLED();

      // Reverse motor
      digitalWrite(motorOne, LOW);
      digitalWrite(motorTwo, HIGH);

      if (timeNow - timeVendingStart >= vendingDuration)
      {
        vendingState = statesVending::NOT_RUNNING;
      }

      break;
  }

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
