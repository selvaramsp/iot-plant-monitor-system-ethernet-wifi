#include <UIPEthernet.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define LED 5
#define BUZZ 6
#define MOISTURE A2

byte mac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };
IPAddress ip(192,168,1,177);

EthernetServer server(80);

DHT dht(DHTPIN, DHTTYPE);
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(BUZZ, OUTPUT);

  dht.begin();

  Wire.begin();

  accel.begin();

  Ethernet.begin(mac, ip);
  server.begin();

  delay(2000);
}

void loop() {

  // SENSOR READINGS
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int moisture = analogRead(MOISTURE);

  sensors_event_t event;
  accel.getEvent(&event);

  float ax = event.acceleration.x;
  float ay = event.acceleration.y;

  // ALERT LOGIC
  bool waterAlert = (moisture > 700);

  // YOUR CALIBRATION
  bool windAlert = (ax > -0.75);

  // OUTPUT CONTROL
  digitalWrite(LED, waterAlert);

  if (windAlert)
    digitalWrite(BUZZ, HIGH);
  else
    digitalWrite(BUZZ, LOW);

  EthernetClient client = server.available();

  if (client) {

    while (client.available()) client.read();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    client.println("<html><body>");
    client.println("<h2>Smart Agriculture Monitor</h2>");

    // TEMP
    client.print("Temperature: ");
    client.print(temp);
    client.println(" C<br>");

    // HUMIDITY
    client.print("Humidity: ");
    client.print(hum);
    client.println(" %<br>");

    // MOISTURE
    client.print("Moisture: ");
    client.print(moisture);

    if (waterAlert)
      client.println(" - WATER PLANT<br>");
    else
      client.println(" - OK<br>");

    // ADXL
    client.print("Tilt X: ");
    client.print(ax);
    client.println("<br>");

    client.print("Tilt Y: ");
    client.print(ay);
    client.println("<br>");

    // STATUS
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