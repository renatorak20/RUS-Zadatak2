#include <avr/sleep.h>
#include <avr/interrupt.h>

const int ledPin = 13;
const int buttonPin = 2;
int ledDelay = 5000; // LED on duration
float currentConversion = 5.0 / 1023.0 / 0.1;

volatile bool wakeUpFlag = false;

void wakeUp() {
  Serial.println("Button pressed!");
  wakeUpFromSleep();
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(buttonPin), wakeUp, FALLING);
}

void loop() {
  // LED ON
  digitalWrite(ledPin, HIGH);
  delay(ledDelay);
  digitalWrite(ledPin, LOW);

  // Measure current
  float current = analogRead(A0) * currentConversion;
  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" mA");

  // Go to sleep
  Serial.println("Sleeping... Press button to wake up.");
  delay(100); // Short delay to let Serial print

  goToSleep();

  if (wakeUpFlag) {
    wakeUpFromSleep();
  }
}

void goToSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Use deeper sleep for better savings
  sleep_enable();
  sleep_cpu();  // Sleep here
  wakeUpFlag = true;
}

void wakeUpFromSleep() {
  sleep_disable(); // Disable sleep mode after waking
  Serial.println("Woke up!");
  wakeUpFlag = false;
}