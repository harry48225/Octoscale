#include "HX711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define BUTTON_A  1
#define BUTTON_B  38
#define BUTTON_C  33

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN 17
#define LOADCELL_SCK_PIN 18

HX711 scale;

void setup() {
  Serial.begin(115200);

  Serial.println("128x64 OLED FeatherWing test");
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setRotation(1);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  scale.set_scale(400.90);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.tare();
}

float average_reading = 0;
float alpha = 1; // 0.2 is good

int interval = 5;
int count = 0;

double smoothingFactor(double t_e, double cutoff) {
  double r = 2 * PI * cutoff * t_e;
  return r / (r+1);
}

double exponential_smoothing(double a, double x, double x_prev) {
  return a * x + (1 - a) * x_prev;
}

double min_cutoff = 0.0001;
double beta = 0.005;
double d_cutoff = 0.5;
double x_prev = 0;
double dx_prev = 0;
long t_prev = 0;

void loop() {
  while (count < interval) {
    long t = micros();
    double x = scale.get_units();
    double t_e = (t - t_prev)/1e6;
    // Filtered derivative
    double a_d = smoothingFactor(t_e, d_cutoff);
    double dx = (x - x_prev) / t_e;
    double dx_hat = exponential_smoothing(a_d, dx, dx_prev);
    Serial.println(dx_hat);

    // Filtered signal
    double cutoff = (min_cutoff) + (beta * abs(dx_hat));
    Serial.println(cutoff);
    double a = smoothingFactor(t_e, cutoff);
    Serial.println(a);
    double x_hat = exponential_smoothing(a, x, x_prev);

    x_prev = x_hat;
    dx_prev = dx_hat;
    t_prev = t;
    average_reading = x_hat;

    count++;
  }
  count=0;
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Reading:");
  display.println(round(average_reading*10.0)/10.0, 1);
  display.println(average_reading);
  display.display();

  if(!digitalRead(BUTTON_B)) scale.tare(40);

  if(!digitalRead(BUTTON_C)) {
    display.clearDisplay();
    display.println("Calibration mode");
    display.println("Place a 100g mass");
    display.println("Then press A");
    display.display();
    scale.set_scale();
    scale.tare();

    while(digitalRead(BUTTON_A)) delay(10);
    
    float factor = scale.get_units(255) / 100.f;

    scale.set_scale(factor);

    display.clearDisplay();
    display.println("Calibrated!");
    display.print("Factor: ");
    display.print(factor);
    display.display();
    delay(2000);
  }
}