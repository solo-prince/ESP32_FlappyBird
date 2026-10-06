// Flappy Bird - ESP32 DevKit + HC-SR04 + SSD1306 128x64 OLED (I2C)
// Libraries: Adafruit SSD1306, Adafruit GFX (Library Manager)
//
// WIRING
// OLED   : VCC->3V3, GND->GND, SDA->GPIO21, SCL->GPIO22
// HC-SR04: VCC->5V(VIN), GND->GND, TRIG->GPIO5,
//          ECHO->GPIO18 THROUGH divider: ECHO--1k--GPIO18--2k--GND  (5V echo -> 3.3V safe)
//
// Sensor faces UP. Hand above it: far = bird up, near = bird down.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>

#define TRIG 5
#define ECHO 18
#define W 128
#define H 64

Adafruit_SSD1306 oled(W, H, &Wire, -1);
Preferences pref;

const int BX = 20;        // bird x
const int BS = 8;         // bird size
const int PW = 12;        // pipe width
const int GAP = 26;       // gap height
const int NP = 2;         // number of pipes
const int SPACING = 64;   // distance between pipes
const float D_MIN = 5.0;  // cm (bird bottom)
const float D_MAX = 30.0; // cm (bird top)

float birdY = 28, dist = 15;
int px[NP], gy[NP];
bool passed[NP];
int score = 0, best = 0;
int speedPx = 2;

float readCm() {
  digitalWrite(TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long t = pulseIn(ECHO, HIGH, 6000);  // ~100 cm timeout
  return t ? t * 0.0343f / 2.0f : -1;
}

void resetGame() {
  score = 0; speedPx = 2; birdY = 28;
  for (int i = 0; i < NP; i++) {
    px[i] = W + i * SPACING;
    gy[i] = random(6, H - GAP - 6);
    passed[i] = false;
  }
}

void gameOver() {
  if (score > best) { best = score; pref.putInt("best", best); }
  oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setTextSize(2); oled.setCursor(10, 8);  oled.print("GAME OVER");
  oled.setTextSize(1);
  oled.setCursor(34, 36); oled.print("Score: "); oled.print(score);
  oled.setCursor(34, 48); oled.print("Best : "); oled.print(best);
  oled.display();
  delay(2000);
  resetGame();
}

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Wire.begin(21, 22);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pref.begin("flappy", false);
  best = pref.getInt("best", 0);
  randomSeed(analogRead(34));
  resetGame();
}

void loop() {
  // --- input ---
  float c = readCm();
  if (c > 0) {
    c = constrain(c, D_MIN, D_MAX);
    dist = 0.6f * dist + 0.4f * c;  // smoothing
  }
  birdY = (H - BS) * (1.0f - (dist - D_MIN) / (D_MAX - D_MIN));

  // --- update pipes ---
  bool dead = false;
  for (int i = 0; i < NP; i++) {
    px[i] -= speedPx;
    if (px[i] < -PW) {
      int far = 0;
      for (int j = 0; j < NP; j++) far = max(far, px[j]);
      px[i] = far + SPACING;
      gy[i] = random(6, H - GAP - 6);
      passed[i] = false;
    }
    if (!passed[i] && px[i] + PW < BX) {
      passed[i] = true; score++;
      if (score % 5 == 0 && speedPx < 4) speedPx++;
    }
    if (BX + BS > px[i] && BX < px[i] + PW &&
        (birdY < gy[i] || birdY + BS > gy[i] + GAP)) dead = true;
  }
  if (dead) { gameOver(); return; }

  // --- draw ---
  oled.clearDisplay();
  for (int i = 0; i < NP; i++) {
    oled.fillRect(px[i], 0, PW, gy[i], WHITE);
    oled.fillRect(px[i], gy[i] + GAP, PW, H - gy[i] - GAP, WHITE);
  }
  oled.fillCircle(BX + 4, (int)birdY + 4, 4, WHITE);
  oled.fillCircle(BX + 6, (int)birdY + 3, 1, BLACK);  // eye
  oled.drawLine(BX, (int)birdY + 4, BX - 3, (int)birdY + 2, WHITE); // tail
  oled.setTextSize(1); oled.setTextColor(WHITE, BLACK);
  oled.setCursor(0, 0);  oled.print(score);
  oled.setCursor(80, 0); oled.print("BEST "); oled.print(best);
  oled.display();
  delay(20);
}