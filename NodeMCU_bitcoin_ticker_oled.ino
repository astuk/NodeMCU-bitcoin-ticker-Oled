/**
 * Bitcoin ticker Node Mcu Oled 128x64
 *
 *  Created by Davide Caminati 2017
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "OLED.h"
#include <ArduinoJson.h>

OLED display(4, 5);
#define USE_SERIAL Serial
ESP8266WiFiMulti WiFiMulti;

const char str_SSID[] = "---your SSID---";
const char str_password[] = "---your password---!";

void setup() {

    USE_SERIAL.begin(115200);
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
    WiFiMulti.addAP(str_SSID, str_password);
    display.begin();    // initialize display oled
}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
        HTTPClient http;
        http.begin("http://api.coindesk.com/v1/bpi/currentprice.json"); //HTTP
        // Start connection and send HTTP header
        int httpCode = http.GET();
        // httpCode will be negative on error
        if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                String answer = http.getString();
                // Convert response into JSON
                  String jsonAnswer;
                  int jsonIndex;
                  for (int i = 0; i < answer.length(); i++) {
                    if (answer[i] == '{') {
                      jsonIndex = i;
                      break;
                    }
                  }
                  // Parsing JSON
                  jsonAnswer = answer.substring(jsonIndex);
                  jsonAnswer.trim();
                
                  // Get bitcoin Value
                  int rateIndex = jsonAnswer.indexOf("rate_float");
                  String priceString = jsonAnswer.substring(rateIndex + 12, rateIndex + 20);
                  display.clear();
                  char price_arr[10];
                  priceString.toCharArray(price_arr, 10);
                  display.print("BTC : ");
                  display.print(price_arr,0,6);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    delay(30000);
}
