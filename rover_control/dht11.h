
#include "DHTesp.h" 

DHTesp dht;
/** Pin number for DHT11 data pin */
// int dhtPin = 27;

DHTesp dht11;
TempAndHumidity valores_DHT11;

void lee_DHT11()
{
  valores_DHT11 = dht.getTempAndHumidity();
}

void init_DHT11()
{
  // datos por el pin 13
	dht.setup(pin_DHT11, DHTesp::DHT11);
	Serial.println("DHT11 iniciado");

}
