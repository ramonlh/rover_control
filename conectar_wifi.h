

/* Añade tu SSID & Clave para acceder a tu Wifi */
const char* ssid = "MOVISTAR_EDE0";  // Tu SSID
const char* password = "Trujillo31";  //Tu Clave
//const char* ssid = "MOVISTAR_8453";  // Tu SSID
//const char* password = "18921892";  //Tu Clave
IPAddress local_IP(192, 168, 1, 91);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);    //optional
IPAddress secondaryDNS(8, 8, 4, 4);  //optional

const char *ssidAP = "roverdiego";
const char *passwordAP = "12341234";

void  init_WiFi()
{
    WiFi.mode(WIFI_STA);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  Serial.println("");

  int intentos=0;
  Serial.print("Conectando a ");    Serial.print(ssid);
  while ((WiFi.status() != WL_CONNECTED) && (intentos < 30))
    {
    delay(500);
    Serial.print(".");
    intentos++;
    }
  if (WiFi.status() == WL_CONNECTED)
    {
    Serial.println("");
    Serial.print("Conectado a ");    Serial.println(ssid);
    Serial.print("Direccion IP: ");  Serial.println(WiFi.localIP());
    }
  else
    {
    Serial.println();
    if (!WiFi.softAP(ssidAP, passwordAP)) {
      log_e("Error al crear Punto de Acceso");
      while (1);
      }
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    }

}
