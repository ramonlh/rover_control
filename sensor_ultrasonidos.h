

const int US_EchoPin = pin_US_receptor;   // 26
const int US_TriggerPin = pin_US_emisor;  // 25

void init_ultrasonidos()
{
  pinMode(US_TriggerPin, OUTPUT);
  pinMode(US_EchoPin, INPUT);
  Serial.println("Sensor ultrasonidos OK");
}

int distancia_US() {
  long duration, distanceCm;
  
  digitalWrite(US_TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  digitalWrite(US_TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(US_TriggerPin, LOW);
  
  duration = pulseIn(US_EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
  
  distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
  return distanceCm;
}

void control_obstaculoUS()
  {
  // mirar la distancia del sensor de ultrasonidos
  int distancia = distancia_US();
  delay(5);
  //Serial.println(distancia);
  if (rumbo_adelante == 1)
    {
    if (distancia <= 15)
      {
      Serial.println("distancia < 10");
      rover_stop();
      rumbo_adelante = 0;
      digitalWrite(pin_led_7colores, LOW);
      }
    else if (distancia <= 35)
      {
      Serial.println("distancia  30");
      set_speed_rover(1000);    
      } 
    }
  }

