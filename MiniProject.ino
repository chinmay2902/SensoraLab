#include <SoftwareSerial.h>
#include <dht11.h>
#define RX 9
#define TX 8
#define dht_apin 7 // Analog Pin sensor is connected to
dht11 dhtObject;
float humidityData;
float tempData;
String AP = "Chinmay";       // AP NAME
String PASS = "12345678"; // AP PASSWORD
String API = "YJA8XH5I5Z562IP5";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial esp8266(RX,TX); 
void setup() {
  pinMode(7, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(9600);

  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");

}

void loop() {
 String getData = "GET /update?api_key="+ API +"&field1="+String(getTemperatureValue())+"&field2="+String(getHumidityValue());
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
   
  tempData =  getTemperatureValue();
  humidityData = getHumidityValue();

  if (tempData >32){
    Serial.print("FAN ON");
    digitalWrite(2, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, LOW);
  }
  else{
    Serial.print("FAN OFF");
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    digitalWrite(3, HIGH);
  }
  delay(1000);
  

}

float getTemperatureValue(){

   dhtObject.read(dht_apin);
   Serial.print(" Temperature(C)= ");
   int temp = dhtObject.temperature;
   Serial.println(temp); 
   delay(50);
   return temp; 
  
}


float getHumidityValue(){

   dhtObject.read(dht_apin);
   Serial.print(" Humidity in %= ");
   int humidity = dhtObject.humidity;
   Serial.println(humidity);
   delay(50);
   return humidity; 
  
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
