#include <WiFi.h>
#include <Wire.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

#define LED 5
#define BUZZ 18

const char* ssid = "Ram Mobile";
const char* password = "e4my86xi";

WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE);

float ax = 0, ay = 0;

void setup() {
  delay(2000);
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);

  Wire.begin(21, 22);
  dht.begin();

  // ADXL345 init
  Wire.beginTransmission(0x53);
  Wire.write(0x2D);
  Wire.write(0x08);
  Wire.endTransmission();

  // WiFi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  // SENSOR READINGS
  int moisture = analogRead(34);
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  readADXL();

  // ALERT LOGIC
  bool waterAlert = (moisture > 3000);

  // YOUR CALIBRATED STORM LOGIC
  bool windAlert = (ax > -0.75);

  // LED for dry soil
  digitalWrite(LED, waterAlert);

  // buzzer for storm
  digitalWrite(BUZZ, windAlert);

  WiFiClient client = server.available();

  if (client) {

    while (client.available()) client.read();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    client.println("<html><body>");
    client.println("<h2>Smart Agriculture Monitor</h2>");

    client.print("Temperature: ");
    client.print(temp);
    client.println(" C<br>");

    client.print("Humidity: ");
    client.print(hum);
    client.println(" %<br>");

    client.print("Moisture: ");
    client.print(moisture);

    if (waterAlert)
      client.println(" - WATER PLANT<br>");
    else
      client.println(" - OK<br>");

    client.print("Tilt X: ");
    client.print(ax);
    client.println("<br>");

    client.print("Tilt Y: ");
    client.print(ay);
    client.println("<br>");

    client.print("Status: ");

    if (windAlert)
      client.println("STORM ALERT!");
    else if (waterAlert)
      client.println("DRY SOIL!");
    else
      client.println("NORMAL");

    client.println("</body></html>");

    delay(10);
    client.stop();
  }

  delay(1000);
}

void readADXL() {
  Wire.beginTransmission(0x53);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(0x53, 4);

  if (Wire.available() >= 4) {
    int16_t x = Wire.read() | (Wire.read() << 8);
    int16_t y = Wire.read() | (Wire.read() << 8);

    ax = x / 256.0;
    ay = y / 256.0;
  }
}