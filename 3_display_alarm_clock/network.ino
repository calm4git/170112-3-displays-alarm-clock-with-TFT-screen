#include "datastore.h"
#include "webfunctions.h"
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>

String APSSID = "ESP Universal Clock";
String ssid ="";
String pass="";

ESP8266WebServer * server =NULL;

String SSIDList(String separator = ",") {
  Serial.println("Scanning networks");
  String ssidList;
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    String ssid = WiFi.SSID(i);
    Serial.println(String(i) + ": " + ssid);
    if (ssidList.indexOf(ssid) != -1) {
      Serial.println("SSID already in list");
    }
    else {
      if (ssidList != "")
        ssidList += separator;
      ssidList += ssid;
    }
  }
  return ssidList;
}

//send a list of available networks to the client connected to the webserver
void getSSIDList() {
  Serial.println("SSID list requested");
  sendData(SSIDList());
}

//store the wifi settings configured on the webpage and restart the esp to connect to this network
void setWiFiSettings() {
  credentials_t c;
  Serial.println("WiFi settings received");
  ssid = server->arg("ssid");
  pass = server->arg("pass");
  String response = "Attempting to connect to '" + ssid + "'. The WiFi module restarts and tries to connect to the network.";
  sendData(response);
  Serial.println("Saving network credentials and restart.");
  strncpy((char*)(c.ssid),(char*)(ssid.c_str()),128);
  strncpy((char*)(c.pass),(char*)(pass.c_str()),128);
  Serial.printf("write ssid:%s ,pass:%s \n\r",c.ssid,c.pass);
  write_credentials(c);
  
  c = read_credentials();
  Serial.printf("read ssid:%s ,pass:%s \n\r",c.ssid,c.pass);
  /* if we do this we end up in flashloader */
  WiFi.softAPdisconnect(true);
  delay(2000);
  pinMode(D0,INPUT_PULLUP);
  ESP.restart();
}

//send the wifi settings to the connected client of the webserver
void getWiFiSettings() {
  Serial.println("WiFi settings requested");
  String response;
  response += ssid + ",";
  response += SSIDList(";");
  sendData(response);
}





//restart the esp as requested on the webpage
void restart() {
  sendData("The ESP will restart and you will be disconnected from the '" + APSSID + "' network.");
  delay(1000);
  pinMode(D0,INPUT_PULLUP);
  ESP.restart();
}

//get the content type of a filename
String getContentType(String filename) {
  if (server->hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

//send a file from the SPIFFS to the connected client of the webserver
void sendFile() {
  String path = server->uri();
  Serial.println("Got request for: " + path);
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  Serial.println("Content type: " + contentType);
  if (SPIFFS.exists(path)) {
    Serial.println("File " + path + " found");
    File file = SPIFFS.open(path, "r");
    server->streamFile(file, contentType);
    file.close();
  }
  else {
    Serial.println("File '" + path + "' doesn't exist");
    server->send(404, "text/plain", "The requested file doesn't exist");
  }
  
  lastAPConnection = millis();
}

//send data to the connected client of the webserver
void sendData(String data) {
  //Serial.println("Sending: " + data);
  server->send(200, "text/plain", data);
  
  lastAPConnection = millis();
}

//initialize wifi by connecting to a wifi network or creating an accesspoint
void initWiFi() {

credentials_t c =  read_credentials();
Serial.printf("read ssid:%s ,pass:%s \n\r",c.ssid,c.pass);
  Serial.print("WiFi: ");
  if (0==1) {
    Serial.println("AP");
    configureSoftAP();
  }
  else {
    Serial.println("STA");
    ssid=String(c.ssid);
      pass=String(c.pass);
      Serial.printf("in memory ssid:%s ,pass:%s \n\r",ssid.c_str(), pass.c_str());
      if(true==connectWiFi()){
        configureServer();
      } else {
        configureSoftAP();
      }  
  }
  
}

//connect the esp to a wifi network
bool connectWiFi() {
  

  if (ssid == "") {
    Serial.println("SSID unknown");
    return false;
  }
  WiFi.mode(WIFI_STA);
  Serial.println("Attempting to connect to " + ssid + ", pass: " + pass);
  
  WiFi.begin(( char*)ssid.c_str(), ( char*)pass.c_str());
  for (int timeout = 0; timeout < 15; timeout++) { //max 15 seconds
    int status = WiFi.status();
    if ((status == WL_CONNECTED)  || (status == WL_NO_SSID_AVAIL) || (status == WL_CONNECT_FAILED))
      break;
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to " + ssid);
    Serial.println("Second attemp " + ssid);
    WiFi.disconnect(true);
    WiFi.begin(( char*)ssid.c_str(), ( char*)pass.c_str());
    for (int timeout = 0; timeout < 15; timeout++) { //max 15 seconds
      int status = WiFi.status();
      if ((status == WL_CONNECTED)  || (status == WL_NO_SSID_AVAIL) || (status == WL_CONNECT_FAILED))
        break;
      Serial.print(".");
      delay(1000);
    }
  
  }
  Serial.println();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to " + ssid);
    Serial.println("WiFi status: " + WiFiStatusToString());
    WiFi.disconnect();
    
    return false;
  }
  Serial.println("Connected to " + ssid);
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());  
  return true;
}

//configure the access point of the esp
void configureSoftAP() {
  Serial.println("Configuring AP: " + String(APSSID));
  
  WiFi.softAP(APSSID.c_str(), NULL, 1, 0, 1);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP: ");
  Serial.println(ip);
  lastAPConnection = millis();

  
  
  configureServer();
}

//initialize the webserver on port 80
void configureServer() {
  server = new ESP8266WebServer (80);
  server->on("/setWiFiSettings", HTTP_GET, setWiFiSettings);
  server->on("/getWiFiSettings", HTTP_GET, getWiFiSettings);
  server->on("/getSSIDList", HTTP_GET, getSSIDList);
  server->on("/restart", HTTP_GET, restart);
  server->on("/timesettings", HTTP_GET, response_settings);
  server->on("/settime.dat", HTTP_POST, settime_update); /* needs to process date and time */
  server->on("/ntp.dat",HTTP_POST,ntp_settings_update); /* needs to process NTP_ON, NTPServerName and NTP_UPDTAE_SPAN */
  server->on("/timezone.dat",timezone_update); /*needs to handel timezoneid */
  server->on("/overrides.dat",timezone_overrides_update); /* needs to handle DLSOverrid,  ManualDLS, dls_offset, ZONE_OVERRRIDE and GMT_OFFSET */
  server->on("/pinballscore.dat", update_pinballscore);
  server->onNotFound(sendFile); //handle everything except the above things
  server->begin();
  Serial.println("Webserver started");
}

void update_pinballscore(){
  /* Needs to get data form the webserver here and also respond accordingly */
  if( ! server->hasArg("pinball_score") || server->arg("pinball_score") == NULL ) { // If the POST request doesn't have username and password data
    /* we are missing something here */
  } else {
   
    Serial.printf("New Score: %s\n\r",server->arg("pinball_score").c_str());
    uint32_t score = server->arg("timezoneid").toInt();
    pinballscore( score ); 
  }
  server->send(200);    

 
}

void WiFiResetSettings(){
  
}

String WiFiStatusToString() {
  switch (WiFi.status()) {
    case WL_IDLE_STATUS:     return "IDLE"; break;
    case WL_NO_SSID_AVAIL:   return "NO SSID AVAIL"; break;
    case WL_SCAN_COMPLETED:  return "SCAN COMPLETED"; break;
    case WL_CONNECTED:       return "CONNECTED"; break;
    case WL_CONNECT_FAILED:  return "CONNECT_FAILED"; break;
    case WL_CONNECTION_LOST: return "CONNECTION LOST"; break;
    case WL_DISCONNECTED:    return "DISCONNECTED"; break;
    case WL_NO_SHIELD:       return "NO SHIELD"; break;
    default:                 return "Undefined: " + String(WiFi.status()); break;
  }
}

void NetworkTask(){

  if(server != NULL){
    server->handleClient();
  }
}

/*
   Return the quality (Received Signal Strength Indicator)
   of the WiFi network.
   Returns a number between 0 and 100 if WiFi is connected.
   Returns -1 if WiFi is disconnected.
*/
int getQuality() {
  if (WiFi.status() != WL_CONNECTED)
    return -1;
  int dBm = WiFi.RSSI();
  if (dBm <= -100)
    return 0;
  if (dBm >= -50)
    return 100;
  return 2 * (dBm + 100);
}

