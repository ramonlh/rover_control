

const int US_EchoPin = pin_US_receptor;
const int US_TriggerPin = pin_US_emisor;

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

