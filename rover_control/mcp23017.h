
#include "MCP23017.h"

// Pins definition
const uint8_t BUTTON_PIN = 0;
const uint8_t LED_PIN    = 8;

// Instantiation
using namespace MCP23017;
MCP23017_IO <> io_mux(I2CAddress::A0_0_A1_0_A2_0, Wire);

void init_MCP23017()
{
  // Check if the MCP23017 is connected
  if (!io_mux.isConnected()) {
    Serial.println("Error: MCP23017 no encontrado");
    }
  else
    {
    // Initialize MCP23017
    Status status = io_mux.init();
    if (status != Status::Ok) {
      Serial.print("Error: No se pudo inicializar MCP23017. Código error: ");
      Serial.println(static_cast<uint8_t>(status));
      }
    Serial.println("MCP23017 inicializado");
  
    // Set pin modes
    status = io_mux.pinMode(BUTTON_PIN, Mode::Input, LED_PIN, Mode::Output);
    if (status != Status::Ok) {
      Serial.print("Error: No se pudo configurar MCP23017. Código error: ");
      Serial.println(static_cast<uint8_t>(status));
      }
    Serial.println("Pin modes set");
    status = io_mux.pinPullUp(BUTTON_PIN, PullUp::Enable);
    if (status != Status::Ok) {
      Serial.print("Error: No se pudo activar pull-up. Código error: ");
      Serial.println(static_cast<uint8_t>(status));
      }
    }
}
