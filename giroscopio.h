
HardwareSerial mySerial(1);
const int myRx = 4;
const int myTx = 26;

float angleX = 0.0;
float angleY = 0.0;
float angleZ = 0.0;

void init_giroscopio()
{
  mySerial.begin(9600, SERIAL_8N1, myRx, myTx);
  JY901.attach(mySerial);
}

void getpos_rover()
{
  JY901.receiveSerialData();
  angleX = JY901.getRoll();
  angleY = JY901.getPitch();
  angleZ = JY901.getYaw();
  if (angleZ <= 0)
    angleZ = -angleZ;
  else
    angleZ = 360 - angleZ;
}

