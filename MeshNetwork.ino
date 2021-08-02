#include <WiFi.h>

const String SSIDPrefix = "ESP32-";
const String defaultPassword = "password123";


bool goodConnection;
String currentSSID;
long currentRSSI;

void setup(){
Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  currentSSID = findBestNetworkConnection();
  WiFi.begin(currentSSID.c_str(),defaultPassword.c_str());
  
  String thisNodesNetworkName = generateSSIDName();
  WiFi.softAP(thisNodesNetworkName.c_str(),defaultPassword.c_str());
}

void loop(){
  goodConnection = hasGoodConnection();
Serial.println(currentSSID);
  if(!goodConnection){
      if(findBestNetworkConnection() != currentSSID){
        WiFi.disconnect();
        WiFi.begin(currentSSID.c_str(),defaultPassword.c_str());
      }
    }
}

String findBestNetworkConnection(){
  
  int remoteRSSI;
  int n = WiFi.scanNetworks();
  int i;
  String runningBestRemoteSSID = currentSSID;
  int bestRSSI = -1000;
  String remoteSSID;
  

    for(i=0;i<n;i++){

      remoteSSID = WiFi.SSID(i);
      remoteRSSI = WiFi.RSSI(i);
      if(remoteSSID>SSIDPrefix){
        for(int j = 0; j < SSIDPrefix.length(); j++){
          if(SSIDPrefix[j]!=remoteSSID[j]){
              remoteRSSI = -1000;
              goto leaveLoop;
          }
        }
 
        remoteRSSI = WiFi.RSSI();  //store info about current connection
      }else{
        remoteRSSI = -1000;
      }
      leaveLoop:
      if(remoteRSSI>currentRSSI){
        runningBestRemoteSSID = remoteSSID;
        currentRSSI=remoteRSSI;
      }   
    }
return runningBestRemoteSSID;
}

String generateSSIDName(){

  return SSIDPrefix + "-1";
  }

boolean hasGoodConnection(){
  if(WiFi.RSSI()==0)
    return false;
  else
    return WiFi.RSSI()>-70; 
  }
