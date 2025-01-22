
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
//#include DHTesp.h" 


// includes del proyecto
#include "defines.h"
#include "conectar_wifi.h"
#include "ota.h"

#include "giroscopio.h"
#include "4motores.h"
#include "fecha_hora.h"
#include "sensor_ultrasonidos.h"
#include "dht11.h"
#include "servidor_web.h"
#include "radio_control.h"
#include "radar.h"
//#include "pantalla.h"

#define OFF LOW
#define ON HIGH

long timedelay1 = 1000;
long timedelay10 = 10000;
long timedelay60 = 60000;
long time1 = 0;
long time10 = 0;
long time60 = 0;

void cosas_cada_segundo()
{
  unixtime++;

  time1 = millis();
}

void cosas_cada_10segundos()
{
  if (tcpclient) 
    {  
     Serial.println("tcpclient OK"); 
     if (tcpclient.connected()) 
       {
        tcpclient.println("hola");
       }
    }
  else
    {  
     Serial.println("tcpclient DESCONECTADO"); 
    }
  time10 = millis();
}

void cosas_cada_60segundos()
{
  lee_DHT11();
  delay(1000);
  Serial.print("Temperatura: " + String(valores_DHT11.temperature) + " C");
  Serial.println("Humedad: " + String(valores_DHT11.humidity) + " %");
  time60 = millis();
}

void setup() {
  Serial.begin(115200);
  init_WiFi();
  init_webserver();
  //init_websockets();
  init_tcpserver();
  init_ota();
  init_motores();
  init_radio_control(pin_radio_control);
  init_ultrasonidos();
  init_giroscopio();
  init_radar();
  init_DHT11();   lee_DHT11();
  Serial.print("Temperatura: " + String(valores_DHT11.temperature) + " C");
  Serial.println("Humedad: " + String(valores_DHT11.humidity) + " %");

  //init_pantalla();

  pinMode(pin_led_7colores, OUTPUT);
  digitalWrite(pin_led_7colores, HIGH);
  delay(2000);
  digitalWrite(pin_led_7colores, LOW);
}

void loop() {
  if ((rumbo_adelante==1) || (rumbo_adelante==2))
    {
    conservar_rumbo();

    }
  handle_tcpserver();
  //webSocket.loop();
  server.handleClient();
  ArduinoOTA.handle();
  if ((millis() - time1) > timedelay1)
    {
    cosas_cada_segundo();  
    }
  if ((millis() - time10) > timedelay10)
    {
    cosas_cada_10segundos();  
    }
  if ((millis() - time60) > timedelay60)
    {
    cosas_cada_60segundos();  
    }
  handle_radar();
  // revisa si se ha pulsado un botón del mando de radio
  int boton_RC = lee_RC();
  //Serial.println("RC leido");
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
  if (boton_RC == 7)
    {
    digitalWrite(pin_led_7colores, HIGH);
    }
  if (boton_RC == 7)
    {
    digitalWrite(pin_led_7colores, HIGH);
    }
  if (boton_RC == 8)
    {
    digitalWrite(pin_led_7colores, LOW);
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
  else if ((boton_RC == 15) || (boton_RC == 16))
    {
    //Serial.println("STOP");
    rover_stop();
    rumbo_adelante=0;
    }
  control_obstaculoUS();
  // sensor DHT11 temperatura y humedad
}