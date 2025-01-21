
#include <Bonezegei_PCA9685.h>

// Default Address of PCA9685 = 0x40 
Bonezegei_PCA9685 speed_motor(0x40);

void init_PCA9685(int freq)
{
  if (speed_motor.begin()) {
    Serial.println("PCA9685 Detectado");
    // Frequency in Hertz 
    speed_motor.setFrequency(freq);
    } 
  else {
    Serial.println("Error: PCA9685 no Conectado");
    }
}

