#include "main.h"
#include "binning.h"
#include "dds.h"

void setup() {
  rgb.begin();
  rgb.on(100, 0, 0);

  Serial.begin(2000000);

  while (!Serial) {
    delay(100);
    rgb.on(0, 255, 0);
    delay(100);
    rgb.off();

    unsigned long now = millis();
    if (now > serial_wait_time && now - serial_wait_time > 0) {
      goto AFTER_SER_WAIT;
    }
  }
AFTER_SER_WAIT:

  rgb.on(100, 50, 0);

  Serial.print("MBED_VERSION=");
  Serial.println(MBED_VERSION);

  display.begin();
  display.setRotation(1);

  if (touch.begin()) {
    Serial.print("Touch controller init - OK");
  } else {
    Serial.print("Touch controller init - FAILED");
    while (true);
  }

  set_screen_off();

  // Resolution, sample rate, number of samples per channel, queue depth.
  if (!adc.begin(AN_RESOLUTION_16, 320000, 32, 128)) {
    Serial.println("Failed to start analog acquisition!");
    while (true) {
      delay(1000);
      rgb.on(255, 0, 0);
      delay(250);
      rgb.off();
      delay(250);
      rgb.on(255, 0, 0);
      delay(250);
      rgb.off();
    }
  }

  pinMode(CS_AD9833, OUTPUT);
  SPI.begin();
  digitalWrite(CS_AD9833, LOW);
  SPI.transfer(0x00);
  digitalWrite(CS_AD9833, HIGH);

  AD.begin();
  dds_set_frequency(dds_freq);
  dds_off();

  touch.onDetect(touch_toggle);
  rgb.on(0, 25, 0);
}

void loop() {
  iter++;

  if (millis() - dds_last_toggle > 5000) {
    dds_last_toggle = millis();
    switch (dds_state) {
      case OFF:
        dds_on();
        dds_state = ON;
        break;
      case ON:
        dds_off();
        dds_state = OFF;
        break;
      default:
        Serial.println("Invalid DDS state!");
        while (true) {
          delay(500);
          rgb.on(255, 255, 0);
          delay(500);
          rgb.off();
        }
    }
  }

  if (adc.available()) {
    SampleBuffer buf = adc.read();
    sprintf(s, "%.8f", to_precision(double(buf[0]) * bit_resolution, 8));
    Serial.println(s);
    buf.release();
  }

  start = micros();
  fourier_bin_frequencies = generate_bins(&b, 1, num_bins);
  end = micros();

//  for (int i = 0; i < int(num_bins); i++) {
//    sprintf(s, "[%6d] %.1f", i+1, fourier_bin_frequencies[i]);
//    Serial.println(s);
//  }

//  sprintf(s, "[%6lu] It took %lu micros to calculate logarithmic Fourier frequencies for %d bins.", iter, end - start, int(num_bins));
//  Serial.println(s);

  free(fourier_bin_frequencies);
//  rgb.off();
//  delay(5);
}

void touch_toggle(uint8_t contacts, GDTpoint_t* points) {
  if (contacts > 0 && (millis() - touch_last > touch_threshold)) {
    Serial.print("Contacts: ");
    Serial.println(contacts);

    /* First touch point */
    Serial.print(points[0].x);
    Serial.print(" ");
    Serial.println(points[0].y);

    if (touch_x < screen_size_x && touch_y < screen_size_y) {
      switch_1 = !switch_1;
      Serial.println("switched");

      if (switch_1) {
        set_screen_on();
        dds_on();
      } else {
        set_screen_off();
        dds_off();
      }
    }
    touch_last = millis();
  }
}

void set_screen_on() {
  display.fillScreen(WHITE);
  display.setTextColor(BLACK);
  display.setCursor(screen_size_x/2, screen_size_y/2);
  display.setTextSize(5);
  display.print("ON");
}

void set_screen_off() {
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setCursor(screen_size_x/2, screen_size_y/2);
  display.setTextSize(5);
  display.print("OFF");
}
