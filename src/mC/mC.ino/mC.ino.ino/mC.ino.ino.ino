/*
 * Programmer: Francis Mendoza
 * Function: IoT sensor firmware to transmit temp and humidity data 
 *           in concert w tertiary applications 
 * Iteration: 1.3
 * Most Recent Change: 11-17-2018
 *  > Added possible delay for DHT11- 500 ms 
 *  > Boilerplate for Wifi Hotspot for in-class demonstration
 */
//##################################################################
#include <Wire.h> 
  //Preprocessor command that (passes data?)
#include <LiquidCrystal_I2C.h>
  // Set the LCD address to 0x27 for a 16 chars and 2 line display

LiquidCrystal_I2C lcd(0x27, 16, 2);
//Connect to D1 and D2 on nodemcu

#include <SimpleDHT.h>
  //Preprocessor command that accesses DHT11 temp & humidity sensor
int pinDHT11 = D4;
  //Connect DHT11 to D4 pin
SimpleDHT11 dht11(pinDHT11);
  //(???) unknown command 
//-----------------------------------------------------------
#include <ESP8266WiFi.h>
  //Preprocessor command that enables Wifi connection

//CREATE WIFI NETWORK AND PASSWORD THAT LIMITS ACCESS TO WEBPAGE
/*const char* ssid     = "Tejaswi2";
const char* password = "12345678";*/

const char* ssid     = "Vista Del Sol";
const char* password = "HanchiBoy77**";

/*const char* ssid     = "asu";
const char* password = "HanchiBoy77***";*/

  //(!!!)PAY ATTENTION TO DETAIL (SYNTAX)
/*const char* ssid     = "Tejaswi";
const char* password = "12345678";*/

  //REQUIRED: Wifi Hotspot details for in-class demonstration
  /* const char* ssid     = <name>;
     const char* password = <password>; */

//IP ADDRESS FOR AWS LIGHTSAIL INSTANCE OF SERVER
const char* host = "34.222.19.236";

//-----------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  // initialize the LCD
  lcd.begin();
  lcd.backlight();

  print2Screen("LOADING...", "");

  //#####################################
  //Serial.println();
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WIFI-CLIENT, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
     
  //(???)What does this do?
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //IF WIFI ISN'T CONNECTED
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  //(???)What does serial print do
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //#####################################
}
//-----------------------------------------------------------
void print2Screen(String s1, String s2)
{
  lcd.clear();
  lcd.print(s1);
  lcd.setCursor(0,1);
  lcd.print(s2);
}

float t = 0;
float h = 0;
void loop()
{

  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  //--------------------------------------------------------------------------------------
  //IF YOU WEREN'T ABLE TO READ ANYTHING
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) 
  {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  //--------------------------------------------------------------------------------------
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H"); 

  //CASTS AS FLOATS AND PERFORMS NECESSARY CONVERSION 
  t = (float)temperature * 1.8 + 32;
  h = (float)humidity;

  print2Screen("TEMP:" + String(t) + " *F", "HUM: " + String(humidity) + " %");

  // DHT11 sampling rate is 1HZ.
  delay(1500);
 //################################################
 int value = 0;

//void loop() {
  delay(500);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("connection failed");
    return;
  }
  //--------------------------------------------------------------------------------------
  // We now create a URI for the request
  String path = "/setValue?t=" + String(t) + "&h=" + String(h);


  Serial.print("Requesting URL: ");
  Serial.println(path);
  //--------------------------------------------------------------------------------------
  // This will send the request to the server
  client.print(String("GET ") + path + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) 
  {
    if (millis() - timeout > 5000) 
    {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  int lineno = 1;
  String S1 = "";
  String S2 = "";

  //--------------------------------------------------------------------------------------
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
  }

  Serial.println();
  Serial.println("closing connection");
 //##########################################
  print2Screen("TEMP : " + String(t) + " *F", "HUM : " + String(h) + " %");

  //DHT11 sampling rate 1 HZ
  //delay(500);
}

