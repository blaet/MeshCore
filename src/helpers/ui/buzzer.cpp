#ifdef PIN_BUZZER
#include "buzzer.h"

#ifdef T1000_E
#include "variant.h"
#endif

void genericBuzzer::begin() {
//    Serial.print("DBG: Setting up buzzer on pin ");
//    Serial.println(PIN_BUZZER);
    #ifdef PIN_BUZZER_EN
      pinMode(PIN_BUZZER_EN, OUTPUT);
      digitalWrite(PIN_BUZZER_EN, HIGH);
    #endif

    quiet(false);
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW); // need to pull low by default to avoid extreme power draw
    startup();
}

void genericBuzzer::play(const char *melody) {
    if (isPlaying())   // interrupt existing
    {
        rtttl::stop();
    }

    if (_is_quiet) return;

    rtttl::begin(PIN_BUZZER,melody);
//    Serial.print("DBG: Playing melody - isQuiet: ");
//    Serial.println(isQuiet());
}

bool genericBuzzer::isPlaying() {
    return rtttl::isPlaying();
}

void genericBuzzer::loop() {
    if (!rtttl::done()) rtttl::play();
}

void genericBuzzer::startup() {
#ifdef T1000_E
    // For T1000-E, light up LED for 2 seconds instead of playing sound
    #ifdef LED_PIN
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    delay(2000);
    digitalWrite(LED_PIN, LOW);
    #endif
#else
    play(startup_song);
#endif
}

void genericBuzzer::shutdown() {
#ifdef T1000_E
    // For T1000-E, blink LED for 2 seconds instead of playing sound
    #ifdef LED_PIN
    pinMode(LED_PIN, OUTPUT);
    unsigned long start_time = millis();
    bool led_state = false;
    while (millis() - start_time < 2000) {
        digitalWrite(LED_PIN, led_state ? HIGH : LOW);
        led_state = !led_state;
        delay(200);  // Blink every 200ms
    }
    digitalWrite(LED_PIN, LOW);  // Ensure LED is off at the end
    #endif
#else
    play(shutdown_song);
#endif
}

void genericBuzzer::quiet(bool buzzer_state) {
    _is_quiet = buzzer_state;
#ifdef PIN_BUZZER_EN
    if (_is_quiet) {
      digitalWrite(PIN_BUZZER_EN, LOW);
    } else {
      digitalWrite(PIN_BUZZER_EN, HIGH);
    }
#endif
}

bool genericBuzzer::isQuiet() {
    return _is_quiet;
}

#endif  // ifdef PIN_BUZZER
