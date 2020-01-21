#include <WiFi.h>
#include <WebServer.h>
#include <M5StickC.h>
#include "DHT12.h"
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "bmm150.h"
#include "bmm150_defs.h"
DHT12 dht12; 
BMM150 bmm = BMM150();
bmm150_mag_data value_offset;
Adafruit_BMP280 bme;

float pressure = 0.0;
float headingDegrees = 0.0;
float tmp = 0.0;
float hum = 0.0;

/* Put your SSID & Password */
const char* ssid = "HackedNetwork";
const char* password =  "12345678";

WebServer server(80);

void calibrate(uint32_t timeout)
{
  int16_t value_x_min = 0;
  int16_t value_x_max = 0;
  int16_t value_y_min = 0;
  int16_t value_y_max = 0;
  int16_t value_z_min = 0;
  int16_t value_z_max = 0;
  uint32_t timeStart = 0;

  bmm.read_mag_data();  
  value_x_min = bmm.raw_mag_data.raw_datax;
  value_x_max = bmm.raw_mag_data.raw_datax;
  value_y_min = bmm.raw_mag_data.raw_datay;
  value_y_max = bmm.raw_mag_data.raw_datay;
  value_z_min = bmm.raw_mag_data.raw_dataz;
  value_z_max = bmm.raw_mag_data.raw_dataz;
  delay(100);

  timeStart = millis();
  
  while((millis() - timeStart) < timeout)
  {
    bmm.read_mag_data();
    
    /* Update x-Axis max/min value */
    if(value_x_min > bmm.raw_mag_data.raw_datax)
    {
      value_x_min = bmm.raw_mag_data.raw_datax;
      // Serial.print("Update value_x_min: ");
      // Serial.println(value_x_min);

    } 
    else if(value_x_max < bmm.raw_mag_data.raw_datax)
    {
      value_x_max = bmm.raw_mag_data.raw_datax;
      // Serial.print("update value_x_max: ");
      // Serial.println(value_x_max);
    }

    /* Update y-Axis max/min value */
    if(value_y_min > bmm.raw_mag_data.raw_datay)
    {
      value_y_min = bmm.raw_mag_data.raw_datay;
      // Serial.print("Update value_y_min: ");
      // Serial.println(value_y_min);

    } 
    else if(value_y_max < bmm.raw_mag_data.raw_datay)
    {
      value_y_max = bmm.raw_mag_data.raw_datay;
      // Serial.print("update value_y_max: ");
      // Serial.println(value_y_max);
    }

    /* Update z-Axis max/min value */
    if(value_z_min > bmm.raw_mag_data.raw_dataz)
    {
      value_z_min = bmm.raw_mag_data.raw_dataz;
      // Serial.print("Update value_z_min: ");
      // Serial.println(value_z_min);

    } 
    else if(value_z_max < bmm.raw_mag_data.raw_dataz)
    {
      value_z_max = bmm.raw_mag_data.raw_dataz;
      // Serial.print("update value_z_max: ");
      // Serial.println(value_z_max);
    }
    
    Serial.print(".");
    delay(1);

  }

  value_offset.x = value_x_min + (value_x_max - value_x_min)/2;
  value_offset.y = value_y_min + (value_y_max - value_y_min)/2;
  value_offset.z = value_z_min + (value_z_max - value_z_min)/2;
}
void setup() {
  Serial.begin(115200);

  // put your setup code here, to run once:
  M5.begin();
  Wire.begin(0,26);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  
  WiFi.begin(ssid, password);
  // Setting the hostname
  WiFi.setHostname("weather-stick");

  Serial.print("Start WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting ..");
  }

  pinMode(M5_BUTTON_HOME, INPUT);

  if(bmm.initialize() == BMM150_E_ID_NOT_CONFORM) {
    Serial.println("Chip ID can not read!");
    while(1);
  } else {
    Serial.println("Initialize done!");
  }
  if (!bme.begin(0x76)){  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
  }
  calibrate(10);
  Serial.print("\n\rCalibrate done..");

    delay(100);
  
  server.on("/", handle_JsonResponse);
  server.onNotFound(handle_NotFound);

    server.begin();
  Serial.println("HTTP server started");

    Serial.print("Connected to the WiFi network. IP: ");
  Serial.println(WiFi.localIP());
}

void handle_JsonResponse(){
//  char response[200];
//  snprintf(response, 200, "{ \"weather\": { \"tempreture\": %04X, \"humidity\": %04X, \"air_pressure\": %04X } }", tmp, hum, pressure);
//  server.send(404, "application/json", response);

  Serial.print("readTemperature()");
  Serial.println(dht12.readTemperature());

  Serial.print("readHumidity()");
  Serial.println(dht12.readHumidity());

Serial.print("readPressure()");
  Serial.println(bme.readPressure());

//  //Serial.printf("Temp: %2.1f Humi: %2.0f%%", tmp, hum);
//  // Response parts.
//  float latitude = 33.546600;
//  float longitude = 75.456912;
//  String buf;
//  buf += F("your location is \nlat:");
//  buf += String(pressure, 6);
//  buf += F("\nlong:");
//  buf += String(longitude, 6);
//  Serial.println(buf);

  String response;
  response += "{ \"weather\": { \"tempreture\": ";
  response += String(dht12.readTemperature(), 6);
  response += ", \"humidity\": ";
  response += String(dht12.readHumidity(), 6);
  response += ", \"pressure\": ";
  response += String(bme.readPressure(), 6);
  response += " } }";
  Serial.println(response);

//  char response[200];
//  strcpy(response, "{ \"weather\": { \"tempreture\": 2"); // copy string one into the result.
//  strcat(response, "9"); // append string two to the result.
////  strcat(response, atof(pressure));
//  strcat(response, " } }"); // append string two to the result.
  
//  char *one = "{ \"weather\": { \"tempreture\": ";
//  float *two = 22.3;
//  char *three = ", \"humidity\": ";
//  int *four = 12;
//  char *five = ", \"air_pressure\": ";
//  float *six = 22.2;
//  char *seven = " } }";
//  char result[100];   // array to hold the result.
//  strcpy(result,one); // copy string one into the result.
//  strcat(result,two); // append string two to the result.
//  strcat(result,three);
//  strcat(result,four);
//  strcat(result,five);
//  strcat(result,six);
//  strcat(result,seven);
  server.send(404, "application/json", response);

  //server.send(404, "application/json", "{ \"weather\": { \"tempreture\": 22.3, \"humidity\": 12, \"air_pressure\": 22 } }");
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

uint8_t setup_flag = 1;
void loop() {
  server.handleClient();

  // put your main code here, to run repeatedly:
  float tmp = dht12.readTemperature();
  float hum = dht12.readHumidity();
  M5.Lcd.setCursor(0, 20, 2);
  M5.Lcd.printf("Temp: %2.1f Humi: %2.0f%%", tmp, hum);


  bmm150_mag_data value;
  bmm.read_mag_data();

  value.x = bmm.raw_mag_data.raw_datax - value_offset.x;
  value.y = bmm.raw_mag_data.raw_datay - value_offset.y;
  value.z = bmm.raw_mag_data.raw_dataz - value_offset.z;

  float xyHeading = atan2(value.x, value.y);
  float zxHeading = atan2(value.z, value.x);
  float heading = xyHeading;

  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  float headingDegrees = heading * 180/M_PI; 
  float xyHeadingDegrees = xyHeading * 180 / M_PI;
  float zxHeadingDegrees = zxHeading * 180 / M_PI;

//  Serial.print("Heading: ");
//  Serial.println(headingDegrees);
//  Serial.print("xyHeadingDegrees: ");
//  Serial.println(xyHeadingDegrees);
//  Serial.print("zxHeadingDegrees: ");
//  Serial.println(zxHeadingDegrees);
  M5.Lcd.setCursor(0, 40, 2);
  M5.Lcd.printf("headingDegrees: %2.1f", headingDegrees);
  
  float pressure = bme.readPressure();
  M5.Lcd.setCursor(0, 60, 2);
  M5.Lcd.printf("pressure: %2.1f", pressure);
  Serial.print("pressure: ");
  Serial.println(pressure);
  delay(100);

  if(!setup_flag){
     setup_flag = 1;

     if(bmm.initialize() == BMM150_E_ID_NOT_CONFORM) {
    Serial.println("Chip ID can not read!");
    while(1);
  } else {
    Serial.println("Initialize done!");
  }
  if (!bme.begin(0x76)){  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
  }
  calibrate(10);
  Serial.print("\n\rCalibrate done..");
 }


 if(digitalRead(M5_BUTTON_HOME) == LOW){
  setup_flag = 0;
  while(digitalRead(M5_BUTTON_HOME) == LOW);
 }

  
}
