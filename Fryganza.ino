#include <Arduino.h>
#include <ESP32Servo.h>
#include "max6675.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "src/Thermostat/src/Thermostat.hpp"

String T_Filt_2 = "0.0";

WebServer server(80);

Thermostat thermostat;

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin(true)) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  IPAddress local_IP(192, 168, 4, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 0, 0);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  const char* ssid = "Fryganza";
  const char* password = "";
  WiFi.softAP(ssid, password);

  server.on("/", ProcessRoot);
  server.on("/update", ProcessUpdates);

  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());


  // Initialize core 0
  xTaskCreatePinnedToCore(
          taskCore0,    // Function to run on core 0
          "taskCore0",  // Name of the task
          10000,        // Stack size (bytes)
          NULL,         // Parameter to pass to the task
          1,            // Priority (0 = lowest, 1 = default, 2 = highest)
          NULL,         // Task handle
          0             // Core to run the task on (0 or 1)
  );

  // Initialize core 1
  xTaskCreatePinnedToCore(
          taskCore1,    // Function to run on core 1
          "taskCore1",  // Name of the task
          10000,        // Stack size (bytes)
          NULL,         // Parameter to pass to the task
          1,            // Priority
          NULL,         // Task handle
          0             // Core to run the task on (0 or 1)
  );
}

void loop() {}

void taskCore0(void* parameter) {

  MAX6675 ktc(SCK, MOSI, MISO);
  Servo valve_servo;

  valve_servo.attach(D0);
  valve_servo.write(thermostat.getValvePositionOff());

  while (true) {
    double T = ktc.readFahrenheit();
    delay(200);
    thermostat.filterMeasuredTemp(T);
    valve_servo.write(thermostat.getControlOutput());
  }
}

void taskCore1(void* parameter) {
  while (true) {
    server.handleClient();
  }
}


void ProcessUpdates() {

  bool b_respond = true;

  String str_Enable = server.arg("b_Enable");
  if (str_Enable == "1" || str_Enable == "True" || str_Enable == "true") {
    thermostat.setSystemEnabled(true);
  } else if (str_Enable == "0" || str_Enable == "False" || str_Enable == "false") {
    thermostat.setSystemEnabled(false);
  }

  if (server.arg("Cnt_FlameOff") != NULL) {
    thermostat.setValvePositionOff(server.arg("Cnt_FlameOff").toInt());
  }

  if (server.arg("Cnt_FlameOn") != NULL) {
    thermostat.setValvePositionOn(server.arg("Cnt_FlameOn").toInt());
  }

  if (server.arg("T_Hyst") != NULL) {
    thermostat.setHyst(server.arg("T_Hyst").toInt());
  }

  if (server.arg("T_Target") != NULL) {
    thermostat.setTarget(server.arg("T_Target").toInt());
  }

  if (server.arg("T_Filt_2") != NULL) {
    T_Filt_2 = server.arg("T_Filt_2");
    b_respond = false;
  }

  char T_Filt1_str[9];
  char T_Filt2_str[9];

  sprintf(T_Filt1_str, "%4.1f", thermostat.getFilteredTemp());
  sprintf(T_Filt2_str, "%4.1f", T_Filt_2.toFloat());

  String s = "{\"T_Meas\":" + String(T_Filt1_str) + ", \"T_Filt_2\":" + String(T_Filt2_str)
          + ", \"Cnt_Output\":" + String(thermostat.getControlOutput()) + "}";

  if (b_respond) {
    server.send(200, "application/json", s);
  }
}

void ProcessRoot() {
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  String fileContent = file.readString();
  file.close();
  server.send(200, "text/html", fileContent);
  Serial.println("got request from device");
}
