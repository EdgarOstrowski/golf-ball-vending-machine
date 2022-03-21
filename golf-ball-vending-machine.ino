
/*
 * https://forum.arduino.cc/t/dc-motor-with-l298n-and-led-blinking-same-time/681187/3
 * 
 */
const int pushButton = 2;
const int motorOne = 8;
const int motorTwo = 9;
const int vendingDuration = 5000;

enum statesVending
{
  NOT_RUNNING = 0,
  DOWN,
  UP
};

int vendingState;
unsigned long timeNow;
unsigned long timeVendingStart;
unsigned long timeBlink;

bool ledState;


void setup() {
  Serial.begin(9600);

  pinMode(pushButton, INPUT);
  pinMode(motorOne, OUTPUT);  
  pinMode(motorTwo, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  vendingState = statesVending::NOT_RUNNING;

  // ledState = false;
  // digitalWrite(motorOne, HIGH);
  // digitalWrite(motorTwo, HIGH);  
}

void loop() { 
  statusLED();  
  timeNow = millis();
  int buttonState = digitalRead(pushButton);
  
  switch( vendingState )
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
        flashLED();
        
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
        flashLED();
        
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

void statusLED(void)
{
  static unsigned long timeBlink;

  if ((timeNow - timeBlink) >= 250ul)
  {
    timeBlink = timeNow;
    ledState ^= true;
  }
}

void flashLED(void)
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
