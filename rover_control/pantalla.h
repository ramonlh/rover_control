
#include <TFT_eSPI.h> // Hardware-specific library


TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void init_pantalla()
{
  // Pantalla TFT
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_WHITE);

  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.println("Hola");
  tft.setTextSize(2);
  tft.println("Hola");

  Serial.print("TFT_MISO:");   Serial.println(TFT_MISO);  //   19
  Serial.print("TFT_SCLK:");   Serial.println(TFT_SCLK);  //    18
  Serial.print("TFT_CS:");     Serial.println(TFT_CS);  //      15
  Serial.print("TFT_DC:");     Serial.println(TFT_DC);  //       2
  Serial.print("TFT_RST:");    Serial.println(TFT_RST);  //      4    // Set TFT_RST to -1 if the display RESET is connected to RST or 3.3V
  Serial.print("TOUCH_CS:");   Serial.println(TOUCH_CS);  //     5

}
