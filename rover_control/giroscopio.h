

float angleX = 0.0;
float angleY = 0.0;
float angleZ = 0.0;
float direccion;

void init_giroscopio()
{
  JY901.startIIC(0x50);
  Serial.println("Giroscopio OK");
}

void getpos_rover()
{
  angleX = JY901.getRoll();
  angleY = JY901.getPitch();
  angleZ = JY901.getYaw();
  if (angleZ <= 0)
    angleZ = -angleZ;
  else
    angleZ = 360 - angleZ;
}

