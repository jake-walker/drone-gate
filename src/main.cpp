#include <Arduino.h>
#include <NewPing.h>
#include "U8glib.h"

NewPing sonar(12, 11, 200);
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

int lapStart = -1;
int maxDistance = -1;
int lastLap = -1;
int distanceThreshold = 2;

void setup() {
  Serial.begin(9600);

  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

String timeStr(int ms) {
  int diff = ms / 1000;
  int min = round(diff / 60);
  int sec = round(diff % 60);
  return String(min) + String("m ") + String(sec) + String("s");
}

void draw() {
  String timeString;
  if (lapStart == -1) {
    timeString = "---";
  } else {
    timeString = timeStr(millis() - lapStart);
  }

  String lastString;
  if (lastLap == -1) {
    lastString = "Last: ---";
  } else {
    lastString = String("Last: ") + timeStr(lastLap);
  }

  String statusText;
  if (maxDistance < 0) {
    statusText = "Calibrating...";
  } else {
    statusText = "Ready!";
  }

  u8g.setFont(u8g_font_profont22);
  u8g.drawStr(0, 22, timeString.c_str());

  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, 36, statusText.c_str());
  u8g.drawStr(0, 48, lastString.c_str());
}

void calibrate() {
  Serial.println("Calibrating...");

  int sum = 0;
  for (int i = 0; i < 10; i++) {
    delay(100);

    Serial.print("Distance ");
    Serial.print(i);
    Serial.print("...");
    int dist = sonar.ping_cm();
    Serial.println(dist);

    sum += dist;
  }

  maxDistance = sum / 10;

  Serial.print("AVERAGE DISTANCE: ");
  Serial.println(maxDistance);
}

void doLap() {
  if (lapStart == -1) {
    Serial.println("-> Starting lap...");
    lapStart = millis();
  } else {
    Serial.println("-> Finishing lap...");
    lastLap = millis() - lapStart;
    lapStart = -1;
    Serial.print("LAP TIME: ");
    Serial.println(lastLap);
  }
}

void loop() {
  u8g.firstPage();
  do {
    draw();
  } while( u8g.nextPage() );

  if (maxDistance < 0) {
    calibrate();
  } else {
    int currentDistance = sonar.ping_cm();
    delay(50);

    // Serial.print("MAX: ");
    // Serial.print(maxDistance);
    // Serial.print(", CURRENT: ");
    // Serial.print(currentDistance);
    // Serial.print(", DIFF: ");
    // Serial.println(maxDistance - currentDistance);

    if ((maxDistance - currentDistance) > distanceThreshold) {
      Serial.println("LAP!");
      doLap();
      delay(200);
    }
  }
}
