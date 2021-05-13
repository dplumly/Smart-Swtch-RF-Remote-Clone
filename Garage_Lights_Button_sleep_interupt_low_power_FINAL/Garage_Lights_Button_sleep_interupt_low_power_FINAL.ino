 //To read the remote codes open the rc-switch “ReceiveDemo_Advanced” example sketch.


#include <avr/sleep.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

const uint8_t buttonPin = 2;
const uint8_t transmitterPin = 4;
int debounceDelay = 20;
int lastDebounceTime = 0;        // the last time the output pin was toggled

volatile byte currentState = LOW;


//boolean currentState = LOW;       //stroage for current button state
boolean lastState = LOW;          //storage for last button state
boolean lightsState = LOW;      //storage for the current state of the light (off/on)

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  mySwitch.enableTransmit(transmitterPin); // Digital pin for transmitter
  mySwitch.setPulseLength(173); // Pulse length from reading remote
  mySwitch.setProtocol(1); // needed to send signal

  setupBlick();
}

void loop()
{
  Serial.println("loop!");
  currentState = digitalRead(buttonPin);
  Serial.println(currentState);

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (currentState != lastState)
    {
      if (lightsState == currentState)
      {
        turnOffLights();
        lightsState = LOW;
        Serial.println("Off");
        lastDebounceTime = millis();
      } else {
        turnOnLights();
        lightsState = HIGH;
        Serial.println("On");
        lastDebounceTime = millis();
      }
    }
    lastState = currentState;
  }
  delay(100);
  Serial.println("Going to sleep");
  sleepFunction();
}

// ====================================================================================
void sleepFunction()
{
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  attachInterrupt(digitalPinToInterrupt(buttonPin), interruptFunction, CHANGE);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  sleep_cpu();
  Serial.println("Woke up!");
}

void interruptFunction()
{
  Serial.println("Interrupt!");
  sleep_disable();
  detachInterrupt(0);
}

// ====================================================================================
void turnOnLights()
{
  Serial.println("Lights on function");
  mySwitch.send("100001110110100000001100"); // On binary from remote
  blinkLed();
}
void turnOffLights()
{
  Serial.println("Lights off function");
  mySwitch.send("100001110110100000000100"); // Off binary from remote
  blinkLed();
}

// ====================================================================================
void blinkLed()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(10);
}

void setupBlick()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  for (int i = 0; i <= 5; i++) 
  {
    digitalWrite( LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite( LED_BUILTIN, LOW);
    delay(100);
  }
}




/*
   void loop()
  {
  Serial.println("loop!");
  currentState = digitalRead(buttonPin);
  Serial.println(currentState);

  if (currentState != lastState)
  {
    if (currentState == LOW) {
      turnOffLights();
      Serial.println("Off");
    } else {
      turnOnLights();
      Serial.println("On");
    }
  }
  lastState = currentState;
  delay(100);
  Serial.println("Going to sleep");
  sleepFunction();
  }
 * */
