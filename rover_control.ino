
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
#include <JY901.h>
#include "HardwareSerial.h"
#include <UnixTime.h>

// includes del proyecto
#include "defines.h"
#include "conectar_wifi.h"
#include "ota.h"

#include "giroscopio.h"
#include "4motores.h"
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
  //getpos_rover();
  //Serial.println(angleX);
  time1 = millis();
}

void setup() {
  Serial.begin(115200);
  init_WiFi();
  init_webserver();
  init_websockets();
  init_ota();
  init_motores();
  init_radio_control(pin_radio_control);
  init_ultrasonidos();
  init_giroscopio();
}

int rumbo_adelante=0;
void loop() {
  if ((rumbo_adelante==1) || (rumbo_adelante==2))
    {
      conservar_rumbo();
    }
  
  webSocket.loop();
  server.handleClient();
  ArduinoOTA.handle();
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
      rumbo_adelante=1;
      getpos_rover();
      direccion=angleZ;
      rover_adelante();
      }
    if (boton_RC == 2)
      {
      //rumbo_adelante=2;
      Serial.println("Atrás");
      rover_atras();
      }
    if (boton_RC == 3)
      {
      rumbo_adelante=0;
      rover_giro_izda();
      }    
    if (boton_RC == 4)
      {
      rumbo_adelante=0;
      rover_giro_dcha();
      }
    if (boton_RC == 5)
      {
      rumbo_adelante=0;
      rover_rot_izda();
      }    
    if (boton_RC == 6)
      {
      rumbo_adelante=0;
      rover_rot_dcha();
      }
    if (boton_RC == 9)
      {
      //rumbo_adelante=0;
      set_speed_rover(rover_speed-500);
      Serial.println(rover_speed);      }    
    if (boton_RC == 10)
      {
      //rumbo_adelante=0;
      set_speed_rover(rover_speed+500);
      }
    else if (boton_RC == 15)
      {
        //Serial.println("STOP");
      rover_stop();
      rumbo_adelante=0;
      }
    else if (boton_RC == 16)
      {
        //Serial.println("STOP");
      rover_stop();
      rumbo_adelante=0;
      }


  // mirar la distancia del sensor de ultrasonidos
  int distancia = distancia_US();
  delay(5);
  if (rumbo_adelante == 1)
    {
    if (distancia <= 15)
      {
      Serial.println("distancia < 10");
      rover_stop();
      rumbo_adelante = 0;
      }
    else if (distancia <= 35)
      {
      Serial.println("distancia  30");
      set_speed_rover(1000);    
      } 
    }
}