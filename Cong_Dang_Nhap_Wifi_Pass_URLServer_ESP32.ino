#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <EEPROM.h>

WebServer server(80);


struct settings {
  char ssid[30];
  char password[30];
  char serverUrl[50]; // Thêm trường server URL
} user_wifi = {};

void setup() {
  Serial.begin(115200);  
  EEPROM.begin(sizeof(struct settings));
  EEPROM.get(0, user_wifi);

  // Kết nối WiFi với SSID và password từ EEPROM
  WiFi.mode(WIFI_STA);
  WiFi.begin(user_wifi.ssid, user_wifi.password);
  
  // Kiểm tra kết nối WiFi
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(user_wifi.ssid);     
  Serial.println(user_wifi.password);   
  Serial.println(user_wifi.serverUrl); 

  // AP Mode if can't connect wifi
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Setup Wifi", "12345678");
    Serial.print("Access Point IP address: ");
    Serial.println(WiFi.softAPIP());
  }

  server.on("/", handlePortal);
  server.begin();
}

void loop() {
  //Kết nối server ap mode nếu không thể truy cập vào wifi station
  if (WiFi.status() != WL_CONNECTED) server.handleClient(); 
}

void handlePortal() {
  if (server.method() == HTTP_POST) {
    // Lấy dữ liệu từ form và lưu vào struct
    strncpy(user_wifi.ssid, server.arg("ssid").c_str(), sizeof(user_wifi.ssid));
    strncpy(user_wifi.password, server.arg("password").c_str(), sizeof(user_wifi.password));
    strncpy(user_wifi.serverUrl, server.arg("serverUrl").c_str(), sizeof(user_wifi.serverUrl));

    EEPROM.put(0, user_wifi);
    EEPROM.commit();

    // Save success
    server.send(200, "text/html", "<html><body><h1>Da luu thong tin Wifi & URL Server!</h1><br><p>Vui long khoi dong lai thiet bi.</p></body></html>");
  } else {
    server.send(200, "text/html", 
    "<html><body>"
    "<h1>Thiet lap Wifi va Server URL</h1>"
    "<form action='/' method='post'>"
    "SSID: <input type='text' name='ssid'><br>"
    "Password: <input type='password' name='password'><br>"
    "Server URL: <input type='text' name='serverUrl'><br>" 
    "<button type='submit'>Kết nối</button>"
    "</form>"
    "</body></html>");
  }
}
