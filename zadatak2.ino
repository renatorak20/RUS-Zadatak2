/**
 * @file sleep_wakeup_example.ino
 *
 * @mainpage Sleep & Wake-Up System with LED and Button
 *
 * @section description Description
 * This Arduino project demonstrates energy-saving techniques using sleep modes.
 * An LED blinks for 5 seconds, and the system then enters a low-power sleep mode.
 * It can be woken up either by a button press (external interrupt) or after a timed interval
 * using the Watchdog Timer interrupt (~4 seconds).
 *
 * @section hardware Hardware Setup
 * - LED connected to pin 13.
 * - Button connected to pin 2 (used with external interrupt INT0).
 *
 * @section libraries Libraries
 * - AVR Sleep Library
 * - AVR Watchdog Timer Library
 * - AVR Interrupt Library
 *
 * @section notes Notes
 * - The button uses the internal pull-up resistor and triggers on the falling edge.
 * - The Watchdog Timer is configured to wake the device approximately every 4 seconds.
 *
 * @section author Author
 * - Created by Renato Rak on 08/04/2025.
 */

// Libraries
#include <avr/sleep.h>     ///< Library for sleep modes.
#include <avr/wdt.h>       ///< Library for Watchdog Timer control.
#include <avr/interrupt.h> ///< Library for interrupt handling.

// Pin definitions
const int ledPin = 13;     ///< Pin connected to the LED.
const int buttonPin = 2;   ///< Pin connected to the button (INT0).

volatile bool isSleeping = true; ///< Flag to determine sleep/wake state.

/**
 * @brief External interrupt service routine for button press.
 * This ISR is triggered when the button is pressed (falling edge),
 * and it sets the flag to wake the device.
 */
void buttonWakeUp() {
    isSleeping = false;
}

/**
 * @brief Watchdog Timer interrupt service routine.
 * This ISR is triggered automatically after ~4 seconds,
 * and it sets the flag to wake the device.
 */
ISR(WDT_vect) {
    isSleeping = false;
}

/**
 * @brief Arduino setup function.
 * Initializes serial communication, sets pin modes, configures external interrupt,
 * and sets the microcontroller to use the power-down sleep mode.
 */
void setup() {
    Serial.begin(9600); ///< Initialize serial communication.

    pinMode(ledPin, OUTPUT);      ///< Set LED pin as output.
    pinMode(buttonPin, INPUT);    ///< Set button pin as input (can be combined with pull-up if needed).

    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonWakeUp, FALLING); ///< Configure external interrupt on falling edge.
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); ///< Set the sleep mode to power-down for minimal power consumption.
}

/**
 * @brief Arduino main loop function.
 * Blinks an LED for 5 seconds, then enters sleep mode.
 * Wakes up on either button press or Watchdog Timer interrupt.
 */
void loop() {
    digitalWrite(ledPin, HIGH); ///< Turn on LED.
    delay(5000);                ///< Wait for 5 seconds.
    digitalWrite(ledPin, LOW);  ///< Turn off LED.

    Serial.println("*Entering sleep mode*");
    configureWatchdog(); ///< Configure Watchdog Timer before sleep.
    enterSleep();        ///< Enter sleep mode and wait for wake-up signal.

    Serial.println("I've woken up!");
}

/**
 * @brief Puts the microcontroller into sleep mode until an interrupt occurs.
 */
void enterSleep() {
    noInterrupts();          ///< Disable interrupts to prepare for sleep.
    isSleeping = true;       ///< Reset the sleeping flag.

    sleep_enable();          ///< Enable sleep mode.
    interrupts();            ///< Re-enable interrupts before sleeping.
    sleep_cpu();             ///< Put the CPU to sleep.

    while (isSleeping) {}    ///< Wait here until an interrupt occurs.

    sleep_disable();         ///< Disable sleep after waking up.
}

/**
 * @brief Configures the Watchdog Timer to trigger an interrupt after ~4 seconds.
 */
void configureWatchdog() {
    cli();           ///< Disable interrupts.
    wdt_reset();     ///< Reset Watchdog Timer.

    // Set WDT for ~4 seconds (WDP3 bit)
    WDTCSR = (1 << WDCE) | (1 << WDE);               ///< Enable configuration changes.
    WDTCSR = (1 << WDIE) | (1 << WDP3);              ///< Enable interrupt mode with 4s timeout.

    sei();           ///< Re-enable interrupts.
}
