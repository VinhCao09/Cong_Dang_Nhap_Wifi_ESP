/*  ============================================================================================================

  Code mau tao Webserver voi Access point de tien hanh setup Wifi Mode cho ESP8266.
  Coding by Vinhcao
  Website: www.vinhcaodatabase.com

  ============================================================================================================== */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
ESP8266WebServer    server(80);

struct settings {c
  char ssid[30];
  char password[30];
} user_wifi = {};
void setup() {
  Serial.begin(115200);  
  EEPROM.begin(sizeof(struct settings) );
  EEPROM.get( 0, user_wifi );
//##########CHE DO WIFI MODE#########//  
  WiFi.mode(WIFI_STA);
  WiFi.begin(user_wifi.ssid, user_wifi.password);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());     
  Serial.println(user_wifi.ssid);     
  Serial.println(user_wifi.password);   
//##########CHE DO WIFI MODE#########// 
//##########CHE DO AP MODE#########// 
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Setup Wifi", "12345678");

  IPAddress myIP = WiFi.softAPIP();        //IP Address of our Esp32 accesspoint(where we can host webpages, and see data)
  Serial.print("Access Point IP address: ");
  Serial.println(myIP);
//##########CHE DO AP MODE#########// 
  server.on("/",  handlePortal);
  server.begin();
}


void loop() {
//Neu khong the ket noi Wifi thi moi cho phep vao handleClient de ket noi wifi khac
  if (WiFi.status() != WL_CONNECTED) server.handleClient();
}

void handlePortal() {
  if (server.method() == HTTP_POST) {

    strncpy(user_wifi.ssid,     server.arg("ssid").c_str(),     sizeof(user_wifi.ssid) );
    strncpy(user_wifi.password, server.arg("password").c_str(), sizeof(user_wifi.password) );
    user_wifi.ssid[server.arg("ssid").length()] = user_wifi.password[server.arg("password").length()] = '\0';
    EEPROM.put(0, user_wifi);
    EEPROM.commit();

    server.send(200,   "text/html",  "<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Thiết lập Wifi</title><style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1,p{text-align: center}</style> </head> <body><main class='form-signin'> <h1>Thiết lập Wifi</h1> <br/> <p>Dữ liệu Wifi của bạn đã lưu lại!<br />Vui lòng Reset thiết bị để kết nối với Wifi.</p></main></body></html>" );
  } else {

    server.send(200,   "text/html", "<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Thiết lập Wifi thành công</title> <style>*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;font-weight:400;line-height:1.5;color:#212529;background-color:#f5f5f5;}.form-control{display:block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{cursor: pointer;border:1px solid transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:400px;padding:15px;margin:auto;}h1{text-align: center}</style> </head> <body><main class='form-signin'> <form action='/' method='post'> <h1 class=''>Thiết lập Wifi</h1><br/><div class='form-floating'><label>SSID</label><input type='text' class='form-control' name='ssid'> </div><div class='form-floating'><br/><label>Password</label><input type='password' class='form-control' name='password'></div><br/><br/><button type='submit'>Kết nối</button><p style='text-align: right'><a href='https://www.vinhcaodatabase.com/vi' style='color: #32C5FF'>coding by vinhcao 2024</a></p></form></main> </body></html>" );
  }
}