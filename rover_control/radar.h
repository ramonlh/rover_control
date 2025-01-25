

#include "RCWL0516.h"

RCWL0516 radar(pin_radar);

void init_radar()
{
  // Initialize sensor radar
  radar.activate();
	Serial.println("Radar iniciado");  
}

void handle_radar()
{
  int value = digitalRead(pin_radar);  
  if (value == HIGH)
    {
    //digitalWrite(pin_led_7colores, HIGH);
    }
  else
    {
    //digitalWrite(pin_led_7colores, LOW);
    }  }