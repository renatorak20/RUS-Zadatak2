/**
 * @file sleep_wakeup_example.ino
 *
 * @mainpage Sustav za Uspavljivanje i Buđenje s LED diodom i Tipkalom
 *
 * @section description Opis
 * Ovaj Arduino projekt demonstrira tehnike uštede energije korištenjem sleep modova.
 * LED dioda svijetli 5 sekundi, nakon čega sustav prelazi u način niske potrošnje (sleep).
 * Mikrokontroler se može probuditi pritiskom na tipkalo (vanjski prekid) ili nakon određenog vremena
 * pomoću Watchdog Timer prekida (~4 sekunde).
 *
 * @section hardware Hardverska postava
 * - LED dioda spojena na pin 13.
 * - Tipkalo spojeno na pin 2 (koristi vanjski prekid INT0).
 *
 * @section libraries Korištene biblioteke
 * - AVR biblioteka za Sleep modove
 * - AVR biblioteka za Watchdog Timer
 * - AVR biblioteka za prekide
 *
 * @section notes Napomene
 * - Tipkalo koristi internu pull-up otpornicu i aktivira se na silazni brid signala (falling edge).
 * - Watchdog Timer je konfiguriran da probudi uređaj otprilike svakih 4 sekunde.
 *
 * @section author Autor
 * - Autor: Renato Rak, 08/04/2025.
 */

// Biblioteke
#include <avr/sleep.h>     ///< Biblioteka za upravljanje sleep modovima.
#include <avr/wdt.h>       ///< Biblioteka za upravljanje Watchdog Timerom.
#include <avr/interrupt.h> ///< Biblioteka za rukovanje prekidima.

// Definicija pinova
const int ledPin = 13;     ///< Pin na koji je spojena LED dioda.
const int buttonPin = 2;   ///< Pin na koji je spojeno tipkalo (INT0).

volatile bool isSleeping = true; ///< Zastavica koja označava stanje spavanja/buđenja.

/**
 * @brief ISR za vanjski prekid pritiska tipkala.
 * Ova funkcija se aktivira kada korisnik pritisne tipkalo (falling edge),
 * i postavlja zastavicu za buđenje mikrokontrolera.
 */
void buttonWakeUp() {
    isSleeping = false;
}

/**
 * @brief ISR za prekid Watchdog Timera.
 * Ova funkcija se aktivira automatski nakon ~4 sekunde,
 * i postavlja zastavicu za buđenje mikrokontrolera.
 */
ISR(WDT_vect) {
    isSleeping = false;
}

/**
 * @brief Inicijalna funkcija postavljanja.
 * Pokreće serijsku komunikaciju, postavlja ulazno/izlazne pinove,
 * konfigurira vanjski prekid i odabire sleep mod.
 */
void setup() {
    Serial.begin(9600); ///< Pokretanje serijske komunikacije.

    pinMode(ledPin, OUTPUT);      ///< Postavi LED pin kao izlaz.
    pinMode(buttonPin, INPUT);    ///< Postavi pin za tipkalo kao ulaz (može se koristiti s pull-up otpornikom).

    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonWakeUp, FALLING); ///< Konfiguracija vanjskog prekida na silazni brid.
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); ///< Postavi sleep mod na Power-down za minimalnu potrošnju.
}

/**
 * @brief Glavna petlja programa.
 * LED svijetli 5 sekundi, zatim mikrokontroler prelazi u sleep mod.
 * Buđenje se odvija pomoću tipkala ili Watchdog Timera.
 */
void loop() {
    digitalWrite(ledPin, HIGH); ///< Uključi LED.
    delay(5000);                ///< Pričekaj 5 sekundi.
    digitalWrite(ledPin, LOW);  ///< Isključi LED.

    Serial.println("*Ulazim u sleep mod*");
    configureWatchdog(); ///< Konfiguriraj Watchdog Timer prije spavanja.
    enterSleep();        ///< Uđi u sleep mod i čekaj prekid.

    Serial.println("Probudili smo se!");
}

/**
 * @brief Funkcija za ulazak u sleep mod do sljedećeg prekida.
 */
void enterSleep() {
    noInterrupts();          ///< Onemogući prekide prije ulaska u sleep.
    isSleeping = true;       ///< Resetiraj zastavicu spavanja.

    sleep_enable();          ///< Omogući sleep mod.
    interrupts();            ///< Ponovno omogući prekide.
    sleep_cpu();             ///< Uspavaj CPU.

    while (isSleeping) {}    ///< Ostani u petlji dok se ne dogodi prekid.

    sleep_disable();         ///< Onemogući sleep nakon buđenja.
}

/**
 * @brief Konfigurira Watchdog Timer da generira prekid nakon otprilike 4 sekunde.
 */
void configureWatchdog() {
    cli();           ///< Onemogući sve prekide.
    wdt_reset();     ///< Resetiraj Watchdog Timer.

    // Postavi WDT na ~4 sekunde (WDP3 bit)
    WDTCSR = (1 << WDCE) | (1 << WDE);               ///< Omogući promjene postavki.
    WDTCSR = (1 << WDIE) | (1 << WDP3);              ///< Omogući interrupt mod s timeoutom od 4 sekunde.

    sei();           ///< Ponovno omogući prekide.
}
