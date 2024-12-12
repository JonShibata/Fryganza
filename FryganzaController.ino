
#include "thermostat.c"
#include "Servo.h"
#include "max6675.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "html.h"

// TODO: test timer logic and resets
// TODO: test lag filter measurement

extern "C" {
#include "user_interface.h"
}

#define iPinServo 5  // GPIO5:  D1

String T_Filt_2 = "0.0";
String T_Filt_3 = "0.0";

Servo valve_servo;

bool b_Enable = false;
bool burner_command;


int ktcSO  = 12;
int ktcCS  = 15;
int ktcCLK = 14;

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

thermostat fryer;

int Cnt_FlameOff = 0;
int Cnt_FlameOn  = 180;

const char* ssid = "Fryganza"; 
const char* password = ""; 

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

ESP8266WebServer server(80);

void setup() {
    Serial.begin(115200);
    fryer.T_Target = 80.0;
    fryer.T_Hyst   = 3.0;
    fryer.k_Filt   = 0.5;

    valve_servo.attach(iPinServo);
    valve_servo.write(Cnt_FlameOff);

    if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("STA Failed to configure");
    }

    WiFi.softAP(ssid, password);

    server.on("/", ProcessRoot);
    server.on("/update", ProcessUpdates);

    server.begin();
    Serial.println("Server started");
    Serial.println(WiFi.localIP());
}


void loop() {

    fryer.T_Meas = (float)ktc.readFahrenheit();
    delay(200);

    thermostat_control(&fryer);

    if (b_Enable == false) {
        fryer.Cnt_Output = Cnt_FlameOff;
    }
    valve_servo.write(fryer.Cnt_Output);

    server.handleClient();
}


void ProcessUpdates() {

    bool b_respond = true;

    String str_Enable = server.arg("b_Enable");
    if (str_Enable == "1" || str_Enable == "True" || str_Enable == "true") {
        b_Enable = true;
    } else if (str_Enable == "0" || str_Enable == "False" || str_Enable == "false") {
        b_Enable = false;
    }

    if (server.arg("Cnt_FlameOff") != NULL) {
        Cnt_FlameOff = server.arg("Cnt_FlameOff").toInt();
    }

    if (server.arg("Cnt_FlameOn") != NULL) {
        Cnt_FlameOn = server.arg("Cnt_FlameOn").toInt();
    }

    if (server.arg("T_Hyst") != NULL) {
        fryer.T_Hyst = server.arg("T_Hyst").toInt();
    }

    if (server.arg("T_Target") != NULL) {
        fryer.T_Target = server.arg("T_Target").toInt();
    }

    if (server.arg("T_Filt_2") != NULL) {
        T_Filt_2 = server.arg("T_Filt_2");
        b_respond = false;
    }
 
    if (server.arg("T_Filt_3") != NULL) {
        T_Filt_3 = server.arg("T_Filt_3");
        b_respond = false;
    }
 
    if (isnan(fryer.T_Filt)) {
        fryer.T_Filt = 0.0;
    }


    char T_Filt1_str[9];
    char T_Filt2_str[9];
    char T_Filt3_str[9];

    sprintf(T_Filt1_str, "%4.1f", fryer.T_Filt);
    sprintf(T_Filt2_str, "%4.1f", T_Filt_2.toFloat());
    sprintf(T_Filt3_str, "%4.1f", T_Filt_3.toFloat());

    String s = "{\"T_Meas\":" + String(T_Filt1_str) +
               ", \"T_Filt_2\":" + String(T_Filt2_str) +
               ", \"T_Filt_3\":" + String(T_Filt3_str) +
               ", \"Cnt_Output\":" + String(fryer.Cnt_Output) + "}";

    if (b_respond){
        server.send(200, "application/json", s);
    }

    Serial.println(String(b_Enable));
    Serial.println(String(Cnt_FlameOff));
    Serial.println(String(Cnt_FlameOn));
    Serial.println(String(fryer.T_Hyst));
    Serial.println(String(fryer.T_Target));
}


void ProcessRoot() {

    server.send(200, "text/html", char_html);
    Serial.println("got request from device");
}
