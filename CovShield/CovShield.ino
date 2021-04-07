/*
* CovShield.ino
*
* Created on: 30.03.2021
*
* Author: CovShield
*/

#include <sh1106.h> // OLED screen library
#include <Wire.h> // I2C library
#include <SparkFunMLX90614.h> // Temperature sensor library
#include <stdlib.h> // Standard C library
#include <ArduinoHttpClient.h> // Http library
#include <WiFi101.h> // Arduino  MKR Wifi library
#include <ArduinoJson.h> //Arduino Json library
uint8_t oled_buf[WIDTH * HEIGHT / 8];

// variables set
int OUT = 3;
int OUT1 = 2;
int i = 0;
bool czujnik1  = false;
bool czujnik2 = false;
IRTherm therm;
bool block = false;
float temperature;
bool stop1 = false;
bool clean = false;
char ssid[] = " wifi name ";
char pass[] = " wifi password ";
int deviceId = 0;
int maxPeople = 0;
float maxTemperature = 0;
int status = WL_IDLE_STATUS;
char serverAddress[] = "cov-shield.herokuapp.com";
WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, 443);

void setup()

// Output, input set
{
  Serial.begin(9600);
  Wire.begin();
  pinMode(OUT, INPUT);
  pinMode(OUT1, INPUT);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(4, OUTPUT);

  // Main interface
  SH1106_begin();
  SH1106_string(0, 0, "Liczba os:", 16, 1, oled_buf);
  SH1106_display(oled_buf);
  therm.begin();
  therm.setUnit(TEMP_C);
  ConnectToWiFi();
  GetConfigurationData();
  Serial.println("Setup finished.");
}

void displayEntries()

//adding and subtracting people
{
  SH1106_char(86, 0, 48 + i / 10, 16, 1 , oled_buf);
  SH1106_char(97, 0, 48 + i % 10, 16, 1 , oled_buf);
  SH1106_display(oled_buf);
}

// process of measuring temperature
void checkTemperature()
{
  SH1106_clear(oled_buf);

  //detection of a person at the device
  do
  {
    delay(10);
    therm.read();
    Serial.println("Sensor: " + String(therm.object(), 2));
    SH1106_string(0, 0, "Zbliz czolo do", 16, 1, oled_buf);
    SH1106_string(0, 16, "miernika na odl", 16, 1, oled_buf);
    SH1106_string(0, 32, "10cm przez 5s", 16, 1, oled_buf);
    SH1106_display(oled_buf);
    if (digitalRead(OUT) == 1 && digitalRead(OUT1) == 1)
    {
      stop1 = true;
    }
    if (digitalRead(OUT) == 1 && digitalRead(OUT1) == 0)
    {
      stop1 = true;
    }
  } while (therm.object() <= 33 && stop1 == false);
  stop1 = false;

  // Start measuring process
  if (therm.object() >= 33)
  {
    SH1106_string(0, 48, "Prosze czekac...", 16, 1, oled_buf);
    SH1106_display(oled_buf);
    delay(3000);
    SH1106_clear(oled_buf);
    therm.read();
    Serial.print("Object: " + String(therm.object(), 2));
    Serial.println("C");
    char result [4];
    temperature = therm.object();
    sprintf(result, "%.2f", temperature);
    SH1106_string(30, 15, result, 16, 1, oled_buf);
    SH1106_string(75, 13, "o", 12, 1, oled_buf);
    SH1106_string(85, 15, "C", 16, 1, oled_buf);
    SH1106_display(oled_buf);
    do

    // body temperature too high
    {
      if (therm.object() >= 37.5)
      {
        SH1106_string(0, 46, "Nie wchodzic !!!", 16, 1, oled_buf);
        SH1106_display(oled_buf);
        digitalWrite(1, HIGH);      // Red Led + buzzer
        digitalWrite(4, HIGH);
      }

      //  correct body temperature
      else if (therm.object() < 37.5)
      {
        SH1106_string(0, 46, " Mozna wejsc :)", 16, 1, oled_buf);
        SH1106_display(oled_buf);
        digitalWrite(0, HIGH);     // Green Led
      }
    } while (digitalRead(OUT) == 0 && digitalRead(OUT1) == 0);
  }
  digitalWrite(0, LOW);
  digitalWrite(1, LOW);
  digitalWrite(4, LOW);
  SH1106_clear(oled_buf);
  SH1106_string(0, 0, "Liczba os:", 16, 1, oled_buf);
  SH1106_display(oled_buf);
}

// counter operation
void loop()
{
  if (digitalRead(OUT) == 0 && digitalRead(OUT1) == 1)
  {
    if (block == false)
    {
      checkTemperature();
    }
    Serial.println("Wykryto czujnik1");
    if (czujnik2 == true)
    {
      if ( i > 0)
      {
        --i;
        SendMeasurement(i);
      }
      czujnik1 = false;
      czujnik2 = false;
      Serial.print("Wczesniej wykryty byl czujnik2, inkrementacja i: ");
      Serial.println(i);
    }
    else
    {
      czujnik1 = true;
      Serial.println("Ustawienie flagi czujnika 1");
    }
  }
  else if (digitalRead(OUT) == 1 && digitalRead(OUT1) == 0)
  {
    Serial.println("Wykryto czujnik2");
    if (czujnik1 == true)
    {
      if (i < 100)
      {
        ++i;
        SendMeasurement(i);
      }
      czujnik1 = false;
      czujnik2 = false;
      Serial.print("Wczesniej wykryty byl czujnik1, dekrementacja i: ");
      Serial.println(i);
    }
    else
    {
      block = true;
      czujnik2 = true;
      Serial.println("Ustawienie flagi czujnika 2");
    }
  }
  else if (digitalRead(OUT) == 0 && digitalRead(OUT1) == 0)
  {
    // Do nothing
  }
  else if (digitalRead(OUT) == 1 && digitalRead(OUT1) == 1)
  {
    block = false;
    czujnik1 = false;
    czujnik2 = false;
  }
  displayEntries();
  // Logs
  Serial.print("czujnik1: ");
  Serial.print(czujnik1);
  Serial.print("\tczujnik2: ");
  Serial.println(czujnik2);
  delay(5);
}

// Downloading configuration
void GetConfigurationData() {
  SimpleConnectToWiFi();
  Serial.println("Downloading configuration... please wait...");
  client.get("/api/admin/configuration/123");
  int statusCode = client.responseStatusCode();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  if (statusCode == 200) {
    String response = client.responseBody();
    ParseConfigurationResponse(response);
  }
  client.stop();
  WiFi.end();
}

// Conecting to Wifi
void ConnectToWiFi() {
  while ( status != WL_CONNECTED) {
    Serial.println("Attempting to connect to Network named: ");
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.print("Connected to wifi: ");
  Serial.println(WiFi.SSID());
}

void SimpleConnectToWiFi() {
  status = WiFi.begin(ssid, pass);
  while ( status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }
}

// Configuration response
void ParseConfigurationResponse(String response) {
    StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(response);
  deviceId = root["id"];
  maxPeople = root["maxPeople"];
  maxTemperature = root["maxTemperature"];
  Serial.println("Configuration downloaded: ");
  Serial.print("Device id: ");
  Serial.println(deviceId);
  Serial.print("Max people: ");
  Serial.println(maxPeople);
  Serial.print("Max temperature: ");
  Serial.println(maxTemperature);
}

// Measurment sent
void SendMeasurement(int peopleCount) {
  Serial.println("Sending measurement to server... ");
  SimpleConnectToWiFi();
  String object = "{\"deviceId\":\"";
  object += deviceId;
  object += "\",\"peopleCount\":\"";
  object += peopleCount;
  object += "\"}";
  Serial.println("Doing post request ");
  String path = "/api/measurement";
  String contentType = "application/json";
  client.post(path, contentType, object);
  int statusCode = client.responseStatusCode();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  client.stop();
  WiFi.end();
}
