
// librerías genéricas
#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <HTTPClient.h>
#include "Arduino_JSON.h"
#include <Wire.h>
#include "JY901_Serial.h"
#include "HardwareSerial.h"
#include <UnixTime.h>

// includes del proyecto
#include "defines.h"
#include "conectar_wifi.h"
#include "ota.h"
#include "4motores.h"
#include "giroscopio.h"
#include "fecha_hora.h"
#include "servidor_web.h"
#include "radio_control.h"
#include "sensor_ultrasonidos.h"

#define OFF LOW
#define ON HIGH

long timedelay1 = 1000;
long time1 = 0;

void cosas_cada_segundo()
{
  unixtime++;
  getpos_rover();
  time1 = millis();
}

void setup() {
  Serial.begin(115200);
  init_giroscopio();
  init_WiFi();
  init_ota();
  init_webserver();
  init_websockets();
  init_motores();
  init_radio_control(pin_radio_control);
  init_ultrasonidos();
}

void loop() {
  ArduinoOTA.handle();
  webSocket.loop();
  server.handleClient();
  if ((millis() - time1) > timedelay1)
    {
    cosas_cada_segundo();  
    }
  // revisa si se ha pulsado un botón del mando de radio
    int boton_RC = lee_RC();
    // si se ha pulsado un botón, se mira que botón y se hace algo
    if (boton_RC == 1)
      {
      Serial.println("Adelante");
      rover_adelante();
      }
    if (boton_RC == 2)
      {
      Serial.println("Atrás");
      rover_atras();
      }
    else if (boton_RC == 15)
      {
        Serial.println("STOP");
        rover_stop();
      }
  /*
  // mirar la distancia del sensor de ultrasonidos
  if (distancia_US() < 30)   // si la distancia es menor de 20 cm bajamos la velocidad a 1000
    {
    Serial.println("Velocidad reducida");
    set_speed_rover(1000);
    }
  if (distancia_US() < 10)   // si la distancia es menor de 10 cm paramos el coche
    {
    Serial.println("Parado por obstáculo");
    rover_stop();
    }
  */
}