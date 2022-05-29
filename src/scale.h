#include "HX711.h"

// Constants for 1 euro rolling average
#define MIN_CUTOFF 0.0001
#define D_CUTOFF 0.5
#define BETA 0.01
#define PI 3.14159265358979323846

#define SETTLED_TOLERANCE 10
#define UNSETTLED_TOLERANCE 50

class Scale {
  public:
    Scale(int doutPin, int sclPin);
    double getReading();
    double getLastSettledReading();
    float millisBetweenSettledReadings = -1;
    void updateReading();
    void tare();
    void setScale();
    void setScale(float factor);
    float getUnits(int samples);

    bool hasSettled = false;
  private:
    double offset = 0;
    HX711 loadCell;
    unsigned long lastSettledMillis = 0;
    double smoothedReading = 0;
    double lastSettledReading = 0;
    double x_prev = 0;
    double dx_prev = 0;
    long t_prev = 0;

    double smoothingFactor(double t_e, double cutoff);
    double exponentialSmoothing(double a, double x, double x_prev);
};