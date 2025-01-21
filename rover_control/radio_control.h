

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

// códigos mando blanco RF433
const int num_buttonsRC = 16;
const long codesRC[num_buttonsRC] = {6047491, 6047500, 6047536, 6047692, 6047695, 6047548, 6047551, 6047728,
                          6047683, 6047503, 6047740, 6047731, 6047743, 6047680, 6047539, 6047488};

int decode_RC(long codeRC)
{
  bool encontrado = false;
  int i = 0;
  while ((i<num_buttonsRC) && (!encontrado))
    {
    encontrado = (codesRC[i] == codeRC);
    i++;
    }
  if (encontrado)
    return i;
  else
    return 0;
}

void init_radio_control(int pin)
{
  //pinMode(pin, OUTPUT);
  mySwitch.enableReceive(pin);  // Receiver on interrupt 0 => that is pin #2
}

int lee_RC()
{
  int boton_RC = 0;
  if (mySwitch.available()) {
    long code= mySwitch.getReceivedValue();
    mySwitch.resetAvailable();
    boton_RC = decode_RC(code);
    if (boton_RC != 0)
      {
      return boton_RC;
      }
    
  }
  return boton_RC;
}
