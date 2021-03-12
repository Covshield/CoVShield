#include <ArduinoHttpClient.h>
#include <WiFi101.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <u8g2.h>
#include <u8x8.h>
#include <SparkFunMLX90614.h>
U8G2_SH1106_128X64_VCOMH0_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 4, /* dc=*/ 7, /* reset=*/ 6);
U8X8_SH1106_128X64_VCOMH0_4W_HW_SPI u8x8(/*cs*/ 4, /*dc*/ 7, /*reset*/ 6);
int OUT = 3;
int OUT1 = 2;
int i = 0;
bool czujnik1  = false;
bool czujnik2 = false;
IRTherm therm;
bool block = false;
float temperature;
bool stop1 = false;
bool stop2 = false;
bool clean = false;
float voltage;
int perc;
int later = 1;
char ssid[] = "AndroidAP5808"; 
char pass[] = "12345678"; 
int deviceId = 0;
int maxPeople = 0;
float maxTemperature = 0;
int status = WL_IDLE_STATUS;
char serverAddress[] = "cov-shield.herokuapp.com";
WiFiSSLClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, 443);

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  pinMode(OUT, INPUT);
  pinMode(OUT1, INPUT);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  u8g2.begin();
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFlipMode(1);
  u8g2.drawFrame( 86, 44, 36, 16);
  u8g2.drawFrame( 122, 48, 2, 8);
  u8g2.sendBuffer();
  u8x8.setFont(u8x8_font_8x13B_1x2_f);
  u8x8.drawString( 0, 1, "LICZBA OSOB:");
  therm.begin();
  therm.setUnit(TEMP_C);
  
  ConnectToWiFi();
  GetConfigurationData();
  Serial.println("Setup finished.");
}

void displayEntries()
{
    if (i == 9 && clean == false)
  {
    u8x8.clear();
    u8g2.drawFrame( 86, 44, 36, 16);
    u8g2.drawFrame( 122, 48, 2, 8);
    u8g2.sendBuffer();
    u8x8.setFont(u8x8_font_8x13B_1x2_f);
    u8x8.drawString( 0, 1, "LICZBA OSOB:");
    clean = true;
    later = 1;
  }
  if (i != 9)
  {
    clean = false;
  }
  u8x8.setCursor(12,0);
  u8x8.setFont(u8x8_font_courB18_2x3_n);
  u8x8.print(i);
}

void checkTemperature()
{
  u8x8.setFont(u8x8_font_pxplustandynewtv_u);
  u8x8.clear();
  do
  {
    delay(10);
    therm.read();
    Serial.println("Sensor: " + String(therm.object(), 2));
    u8x8.drawString( 0, 0, "ZBLIZ CZOLO DO");
    u8x8.drawString( 0, 2, "MIERNIKA NA ODL");
    u8x8.drawString( 0, 4, "10CM PRZEZ 5S");
    if (digitalRead(OUT) == 1 && digitalRead(OUT1) == 1)
    {
      stop1 = true;
    }
    if (digitalRead(OUT) == 1 && digitalRead(OUT1) == 0)
    {
      stop2 = true;
    }
  } while (therm.object() <= 30 && stop1 == false && stop2 == false);
  stop1 = false;
  stop2 = false;
  
  if (therm.object() >= 30)
  {
    u8x8.drawString( 0, 7, "PROSZE CZEKAC...");
    delay(3000);
    u8x8.clear();
    therm.read();
    Serial.print("Object: " + String(therm.object(), 2));
    Serial.println("C");
    u8x8.setFont(u8x8_font_courB18_2x3_f);
    temperature = therm.object();
    u8x8.setCursor(1, 4);
    u8x8.print(temperature);
    u8x8.drawString( 14, 4, "C");
    u8x8.setFont(u8x8_font_lucasarts_scumm_subtitle_r_2x2_f);
    u8x8.drawString( 12, 3, "o");
    u8x8.setFont(u8x8_font_8x13B_1x2_f);
    if (therm.object() >= 35)
    {
      u8x8.drawString( 0, 0, "NIE WCHODZIC !!!");
      digitalWrite(0, HIGH);
      delay(3000);          //Dioda LED czerwona + buzzer
      digitalWrite(0, LOW);
    }
    else if (therm.object() < 35)
    {
      u8x8.drawString( 1, 0, "MOZNA WEJSC :)");
      digitalWrite(1, HIGH);
      delay(3000);          //Dioda LED zielona
      digitalWrite(1, LOW);
    }
  }
  u8x8.clear();
  u8g2.drawFrame( 86, 44, 36, 16);
  u8g2.drawFrame( 122, 48, 2, 8);
  u8g2.sendBuffer();
  u8x8.setFont(u8x8_font_8x13B_1x2_f);
  u8x8.drawString( 0, 1, "LICZBA OSOB:");
  later = 1;
}

void batterylevel()
{
  --later;
  if (later == 0)
  {
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    voltage = analogRead(A0) * 5.0 / 1023;
    if (voltage > 2)
    {
      voltage = 2;
    }
    perc = map(voltage * 200, 300, 400, 0, 100);
    u8x8.setCursor(11, 6);
    u8x8.print(perc);
    if (perc != 100)
    {
      u8x8.drawString( 13, 6, "% ");
    }
    if (perc == 100)
    {
      u8x8.drawString( 14, 6, "%");
    }
    later = 100;
  }
}

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

  batterylevel();
  displayEntries();
  // Logs
  Serial.print("czujnik1: ");
  Serial.print(czujnik1);
  Serial.print("\tczujnik2: ");
  Serial.println(czujnik2);
  delay(10);
}


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
