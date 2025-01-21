
#include "mcp23017.h"
#include <pca9685.h>

const uint8_t pin_dir[4][2] = {{0, 1}, {2, 3}, {4, 5}, {6, 7}}; // pines del mux mcp23017
const uint8_t pin_motor[4] = {0, 1, 2, 3};                      // salidas pwm del mux PCA9685

static Output low = Output::Low;
static Output high = Output::High;

static int rover_speed = 1000;
int rumbo_adelante=0;


void pinModeMux(int pin, int mode)
{
  if (mode == OUTPUT)
    io_mux.pinMode(pin, Mode::Output);
  else
    io_mux.pinMode(pin, Mode::Input);
}

void digitalWriteMux(int pin, int value)
{
  io_mux.pinDigitalWrite(pin, value==0?low:high);
}

void set_dir_motor (int motor, int dir)           // motor: 1-4,  direccion: 0-1
{
  if (dir == 0)
    {
    digitalWriteMux(pin_dir[motor-1][0], LOW);
    digitalWriteMux(pin_dir[motor-1][1], HIGH);
    }
  else
    {
    digitalWriteMux(pin_dir[motor-1][0], HIGH);
    digitalWriteMux(pin_dir[motor-1][1], LOW);
    }
}

void set_motor(int motor, int dir, int speed)  // motor: 1-4, speed: 0-4095
{
  if (speed == 0)
    {
    digitalWriteMux(pin_dir[motor-1][0], HIGH);
    digitalWriteMux(pin_dir[motor-1][1], HIGH);
    speed_motor.setValue(pin_motor[motor-1], 0);
    }
  else
    {
    set_dir_motor(motor,dir);
    speed_motor.setValue(pin_motor[motor-1], speed);
    }
}

void set_speed_rover(int velocidad)
{
  if (velocidad > 4000)
    velocidad = 4000;
  if (velocidad < 600)
    velocidad = 600;
  rover_speed = velocidad;
  for (int i=0; i<4; i++)
    {
    speed_motor.setValue(i,   rover_speed);
    }
}

void init_motores()
{
  // Initialize I²C
  Wire.begin(21,22);
  // Initialize_MCP23017
  init_MCP23017();
  // Initialize_PCA9685
  init_PCA9685(1600);   // 50 = freq
  
  for (int i=0; i<4; i++)
    {
    speed_motor.setValue(i, 1);
    pinModeMux(pin_dir[i][0], OUTPUT);
    pinModeMux(pin_dir[i][1], OUTPUT);
    digitalWriteMux(pin_dir[i][0], LOW);  // disable
    digitalWriteMux(pin_dir[i][1], LOW);  // disable
    }
  rover_speed = 1000;
}

void rover_stop()
{
    set_motor(1, 1, 0);    // motor, sentido giro, velocidad
    set_motor(2, 1, 0);    // motor, sentido giro, velocidad
    set_motor(3, 1, 0);    // motor, sentido giro, velocidad
    set_motor(4, 1, 0);    // motor, sentido giro, velocidad
}

void rover_adelante()
{
    set_motor(1, 1, rover_speed);    // motor, sentido giro, velocidad
    set_motor(2, 1, rover_speed);    // motor, sentido giro, velocidad
    set_motor(3, 1, rover_speed);    // motor, sentido giro, velocidad
    set_motor(4, 1, rover_speed);    // motor, sentido giro, velocidad
}

void rover_atras()
{
    set_motor(1, 0, rover_speed);    // motor, sentido giro, velocidad
    set_motor(2, 0, rover_speed);    // motor, sentido giro, velocidad
    set_motor(3, 0, rover_speed);    // motor, sentido giro, velocidad
    set_motor(4, 0, rover_speed);    // motor, sentido giro, velocidad
}

void rover_giro_dcha()
{
    set_motor(1, 1, rover_speed);    // motor, sentido giro, velocidad
    set_motor(2, 1, rover_speed);    // motor, sentido giro, velocidad
    set_motor(3, 0, 0);    // motor, sentido giro, velocidad
    set_motor(4, 0, 0);    // motor, sentido giro, velocidad
}

void rover_giro_izda()
{
    set_motor(1, 0, 0);    // motor, sentido giro, velocidad
    set_motor(2, 0, 0);    // motor, sentido giro, velocidad
    set_motor(3, 1, rover_speed);    // motor, sentido giro, velocidad
    set_motor(4, 1, rover_speed);    // motor, sentido giro, velocidad
}

void rover_rot_izda()
{
    set_motor(1, 0, rover_speed);    // motor, sentido giro, velocidad
    set_motor(2, 0, rover_speed);    // motor, sentido giro, velocidad
    set_motor(3, 1, rover_speed);    // motor, sentido giro, velocidad
    set_motor(4, 1, rover_speed);    // motor, sentido giro, velocidad
}

void rover_rot_dcha()
{
    set_motor(1, 1, rover_speed);    // motor, sentido giro, velocidad
    set_motor(2, 1, rover_speed);    // motor, sentido giro, velocidad
    set_motor(3, 0, rover_speed);    // motor, sentido giro, velocidad
    set_motor(4, 0, rover_speed);    // motor, sentido giro, velocidad
}


void conservar_rumbo()
{
  getpos_rover();
  if (direccion < angleZ)
    {
      set_motor(1, 1, rover_speed-600);  
      set_motor(2, 1, rover_speed-600);  
      delay(500);
      set_motor(1, 1, rover_speed);  
      set_motor(2, 1, rover_speed);        // girar a la izquierda
    }
  else if (direccion > angleZ)
    {
       // girar a la derecha
      set_motor(3, 1, rover_speed-600);  
      set_motor(4, 1, rover_speed-600);  
      delay(500);
      set_motor(3, 1, rover_speed);  
      set_motor(4, 1, rover_speed);        // girar a la izquierda
    }

}

