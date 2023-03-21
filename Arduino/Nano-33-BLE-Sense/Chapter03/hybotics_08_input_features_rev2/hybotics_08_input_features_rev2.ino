#define NUM_HOURS 3

constexpr int num_reads = 3;
constexpr float t_mean  = 2.05179f;
constexpr float h_mean  = 82.30551f;
constexpr float t_std   = 7.33084f;
constexpr float h_std   = 14.55707f;

// Circular buffers for the input features
int8_t t_vals[NUM_HOURS] = {0};
int8_t h_vals[NUM_HOURS] = {0};
int cur_idx               = 0;
float   tflu_i_scale      = 0.0f;
int32_t tflu_i_zero_point = 0;
uint16_t looper = 0;

inline int8_t quantize(float x, float scale, float zero_point)
{
  return (x / scale) + zero_point;
}

#if defined(ARDUINO_ARDUINO_NANO33BLE)
#include <Arduino_HS300x.h>

#define READ_TEMPERATURE() HS300x.readTemperature()
#define READ_HUMIDITY()    HS300x.readHumidity()

void setup() {
  Serial.begin(9600);

  while (!Serial);

  if (!HS300x.begin()) {

    Serial.println("Failed initialization of HT300x!");
    while (1);
  }
}
#endif // defined(ARDUINO_ARDUINO_NANO33BLE)

#if defined(ARDUINO_RASPBERRY_PI_PICO)
#include <DHT.h>

const int gpio_pin_dht_pin = 10;

DHT dht(gpio_pin_dht_pin, DHT11);

#define READ_TEMPERATURE() dht.readTemperature()
#define READ_HUMIDITY()    dht.readHumidity()

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize the DHT sensor
  dht.begin();

  // Waiting for the peripheral for being ready
  delay(2000);
}
#endif // defined(ARDUINO_RASPBERRY_PI_PICO)

float toFahrenheit(float celcius) {
  return  (celcius * 1.8) + 32.0;
}

float toCelsius(float fahrenheit) {
  return (fahrenheit - 32) / 1.8;
}

void loop() {
  float c = 0.0f;
  float h = 0.0f;
  float f = 0.0f;

  looper += 1;
  
  for(int i = 0; i < num_reads; ++i) {
    c += READ_TEMPERATURE();
    h += READ_HUMIDITY();

    delay(3000);
  }

  // Take the average
  c /= (float)num_reads;
  h /= (float)num_reads;

  f = toFahrenheit(c);

  Serial.print("Loop #");
  Serial.print(looper, 5);
  Serial.print(": Temperature = ");
  Serial.print(f, 2);
  Serial.print(" °F (");
  Serial.print(c);
  Serial.print(" °C), Humidity = ");
  Serial.print(h, 2);
  Serial.println(" %");

  // Z-score scaling
  c = (c - t_mean) / t_std;
  h = (h - h_mean) / h_std;

  // Store the normalized and quantized samples in the circular buffers
  t_vals[cur_idx] = quantize(c, tflu_i_scale, tflu_i_zero_point);
  h_vals[cur_idx] = quantize(h, tflu_i_scale, tflu_i_zero_point);

  // Update the circular buffer index
  cur_idx = (cur_idx + 1) % NUM_HOURS;

  // We should have a delay of 1 hour but, for practical reasons, we have reduced it to 2 seconds
  delay(2000);
}
