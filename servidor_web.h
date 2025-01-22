
#include <UnixTime.h>

WebServer server(port_servidor_web);
WebSocketsServer webSocket = WebSocketsServer(port_websockets);
WiFiServer tcpserver(8080);

String SendHTML(bool refrescar) {
  // Cabecera de todas las paginas WEB
  String ptr = "<!DOCTYPE html>\n<html>\n";
  if (refrescar) ptr += "<META http-equiv=refresh content=10>\n";

  ptr += "<head>\n";
  ptr += "    <title>Control GPIO ESP32</title>\n";
  ptr += "    <style>\n";
  ptr += "    body { font-family: Arial, sans-serif; text-align: center; }\n";
  ptr += "    button { padding: 10px 20px; font-size: 16px; margin: 10px; }\n";
  ptr += "    </style>\n";

  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: inline-block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";

  ptr += "</head>\n";
  
  ptr += "<body>\n";
  ptr += "<h1>Control Rover</h1>\n";

 // stamp.getDateTime(unixtime);
  ptr += "V 1.0 UnixTime - Date - Time: " + String(unixtime)+ " - "  + "\n";
  ptr += String(stamp.year) + "/" + String(stamp.month) + "/" + String(stamp.day) + " - " ;
  ptr += "  " + String(stamp.hour) + ":" + String(stamp.minute) + ":" + String(stamp.second);
  ptr += "<br><br>";

  ptr += "<p>Velocidad: <span id=\"rover_speed\">" + String(rover_speed) + "</span></p>";
  ptr += "<input type=\"range\" id=\"slider\" min=\"0\" max=\"4095\" value=\")rawliteral\" + String(rover_speed) + R\"rawliteral(\" oninput=\"updateSlider(this.value)\"><br>";
  ptr += "<button id=\"controlButton1\" onmousedown=\"sendCommand('adelante')\" onmouseup=\"sendCommand('stop')\">Adelante</button>\n";
  ptr += "<button id=\"controlButton2\" onmousedown=\"sendCommand('atras')\" onmouseup=\"sendCommand('stop')\">Atras</button><br>\n";
  ptr += "<button id=\"controlButton3\" onmousedown=\"sendCommand('giroizda')\" onmouseup=\"sendCommand('stop')\">Giro Izda</button>\n";
  ptr += "<button id=\"controlButton4\" onmousedown=\"sendCommand('girodcha')\" onmouseup=\"sendCommand('stop')\">Giro Dcha</button><br>\n";
  ptr += "<button id=\"controlButton5\" onmousedown=\"sendCommand('rotizda')\" onmouseup=\"sendCommand('stop')\">Rot Izda</button>\n";
  ptr += "<button id=\"controlButton6\" onmousedown=\"sendCommand('rotdcha')\" onmouseup=\"sendCommand('stop')\">Rot Dcha</button><br>\n";

  ptr += "<script>\n";
  ptr += "  let ws = new WebSocket(`ws://${location.hostname}:92/`)\n\n";
  ptr += "  function sendCommand(command) {if (ws.readyState === WebSocket.OPEN) {ws.send(command); } }\n";
  ptr += "  ws.onopen = () => console.log(\"WebSocket conectado\")\n";
  ptr += "  ws.onclose = () => console.log(\"WebSocket desconectado\")\n";
  ptr += "  function updateSlider(value) { document.getElementById('rover_speed').innerText = value; fetch(`/set?value=${value}`);}";
  ptr += "</script>\n";

  ptr += "</body>\n</html>\n";
  return ptr;
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(true)); 
}

void handle_index() {
  getpos_rover();
  server.send(200, "text/html", SendHTML(true));
}

void handle_NotFound() {
  server.send(404, "text/plain", "La pagina no existe");
}

void handleSetValue() {
  if (server.hasArg("value")) {
    rover_speed = server.arg("value").toInt(); // Convertir el valor recibido a entero
    Serial.println(rover_speed);
    }
  server.send(204, "text/plain", ""); // Respuesta sin contenido
}

void handle_adelante() {
  Serial.println("adelante html");
  rover_adelante();
  server.send(204, "text/plain", ""); // Respuesta sin contenido
}

void handle_atras() {
  rover_atras();
  server.send(204, "text/plain", ""); // Respuesta sin contenido
}

void handle_giroizquierda() {
  rover_giro_izda();
  server.send(204, "text/plain", ""); // Respuesta sin contenido
}

void handle_giroderecha() {
  rover_giro_dcha();
  server.send(204, "text/plain", ""); // Respuesta sin contenido
}

void init_webserver()
{
  server.on("/", handle_index); 
  server.on("/index.html", handle_index); 
  server.on("/set", handleSetValue);
  server.on("/adelante", handle_adelante);
  server.on("/atras", handle_atras);
  server.on("/giroizquierda", handle_giroizquierda);
  server.on("/giroderecha", handle_giroderecha);
  server.onNotFound(handle_NotFound); 
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void init_tcpserver()
{
  tcpserver.begin();
  Serial.println("Servidor TCP iniciado.");  

}
// Manejar mensajes de WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    if (type == WStype_TEXT) {
        String message = (char*)payload;
        if (message == "adelante") {
          Serial.println("adelante");
          rover_adelante();
          }
        if (message == "atras") {
          rover_atras();
          }
        if (message == "giroizda") {
          rover_giro_izda();
          }
        if (message == "girodcha") {
          rover_giro_dcha();
          }
        if (message == "rotizda") {
          rover_rot_izda();
          }
        if (message == "rotdcha") {
          rover_rot_dcha();
          }
        else if (message == "stop") {
          Serial.println("STOP");
          rover_stop();
        }
    }
}

WiFiClient tcpclient;

long timedelay10tcp = 10000;
long time10tcp = 0;

void cosas_cada_10segundostcp()
{
  if (tcpclient) 
    {  
     Serial.println("tcpclient OK"); 
     if (tcpclient.connected()) 
       {
        tcpclient.println(String("t") + String(valores_DHT11.temperature));
        tcpclient.println(String("h") + String(valores_DHT11.humidity));
       }
    }
  else
    {  
     Serial.println("tcpclient DESCONECTADO"); 
    }
  time10tcp = millis();
}

void handle_tcpserver()
{
  tcpclient = tcpserver.available();
  if (tcpclient.connected()) {
    Serial.println("Cliente conectado.");
    while (tcpclient.connected()) 
      {
      if ((millis() - time10tcp) > timedelay10tcp)
        {
        cosas_cada_10segundostcp();  
        }
      control_obstaculoUS();
      if (tcpclient.available()) {
        String data = tcpclient.readStringUntil('\n');
        Serial.println("Recibido tcp: " + data);
        data.remove(data.length() - 1);
        // Confirmar al cliente
        //tcpclient.println("ACK: " + data);
        if (data == "adelante")
          {
          rumbo_adelante=1;
          rover_adelante();
          }
        else if (data == "atras")
          {
          //rumbo_adelante=2;
          rover_atras();
          }
        else if (data == "giroizda")
          {
          rumbo_adelante=0;
          rover_giro_izda();
          }        
        else if (data == "girodcha")
          {
          rumbo_adelante=0;
          rover_giro_dcha();
          }
        else if (data == "stop")
          {
          rumbo_adelante=0;
          rover_stop();
          }
        else if (data == "bajavelocidad")
          {
          set_speed_rover(rover_speed-500);
          }
        else if (data == "subevelocidad")
          {
          set_speed_rover(rover_speed+500);
          }
        }
      }
    tcpclient.stop();
    Serial.println("Cliente desconectado.");
   }
}

void init_websockets()
{
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Servidor WebSocket iniciado");
}

