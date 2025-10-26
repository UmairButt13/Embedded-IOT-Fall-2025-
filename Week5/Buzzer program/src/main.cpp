#include <Arduino.h>

#define BUZZER_PIN  27     // GPIO connected to buzzer (speaker)
#define LED1_PIN    18     // First LED
#define LED2_PIN    19     // Second LED
#define PWM_CH      0      // PWM channel (0–15)
#define LEDC_FREQ   4000   // Base PWM frequency used for ledcSetup (Hz)
#define RESOLUTION  10     // Bit resolution (0..(2^RESOLUTION - 1))
#define DUTY_LEVEL  512    // Mid duty for audible buzzer (for 10-bit resolution)

void playTone(uint8_t channel, uint32_t frequency, uint16_t duty, uint32_t duration_ms) {
  if (frequency == 0 || duty == 0) return;
  ledcWriteTone(channel, frequency);  // Set frequency
  ledcWrite(channel, duty);           // Set duty (non-zero)
  delay(duration_ms);
  ledcWrite(channel, 0);              // Stop tone
  ledcWriteTone(channel, 0);
}

void setup() {
  // LEDs setup
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // Buzzer setup
  pinMode(BUZZER_PIN, OUTPUT);
  ledcSetup(PWM_CH, LEDC_FREQ, RESOLUTION);
  ledcAttachPin(BUZZER_PIN, PWM_CH);

  // Startup blink sequence
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  delay(300);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  delay(300);
}

void loop() {
  // --- 1. Simple beep pattern ---
  for (int i = 0; i < 3; i++) {
    uint32_t toneHz = 4000 + i * 600; // High-pitched tones
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
    playTone(PWM_CH, toneHz, DUTY_LEVEL, 150);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, HIGH);
    delay(150);
  }

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  delay(200);

  // --- 2. Frequency sweep (1kHz → 6kHz) ---
  for (int f = 1000; f <= 6000; f += 200) {
    // alternate LED states for each step
    bool state = ((f / 200) % 2);
    digitalWrite(LED1_PIN, state);
    digitalWrite(LED2_PIN, !state);
    playTone(PWM_CH, f, DUTY_LEVEL, 20);
    delay(5);
  }

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  delay(300);

  // --- 3. Short melody ---
  const int melody[] = {523, 659, 784, 880, 988, 1047, 1175, 1319};
  const int notes = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < notes; i++) {
    // Alternate LEDs each note
    if (i % 2 == 0) {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);
    } else {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);
    }

    playTone(PWM_CH, melody[i], DUTY_LEVEL, 200);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    delay(80);
  }

  delay(1000); // Pause before repeating
}
