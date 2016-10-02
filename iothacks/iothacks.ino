#include <DebouncedButton.h>
/*
 *  Simple HTTP get webclient test
 */

#include <ESP8266WiFi.h>
//#include <Constants.h>
#define BUTTON_TEST
//#define HTTP_TEST

const char* ssid     = "CalVisitor";
const char* password = "yourpassword";
const int buttonPin = 12;

DebouncedButton* button1;
DebouncedButton* button2;
DebouncedButton* enterButton;
// DebouncedButton* button3;
// DebouncedButton* button4;

const char* host = "wifitest.adafruit.com";

void setup() {
  Serial.begin(115200);

  delay(100);

  button1 = new DebouncedButton(12, 50);
  button2 = new DebouncedButton(14, 50);
  enterButton = new DebouncedButton(5, 50);


  // We start by connecting to a WiFi network
  #ifdef HTTP_TEST
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  #endif
}

int value = 0;

void loop() {
  int code = waitForKeyCode();
  if(code != 0){
    String s = String(code);
    Serial.print("This is the code");
    Serial.println(code);
  }
  delay(100);
  // if(button1->isPressed()){
  //   Serial.println("Button 1 pressed");
  // }
  // else if(button2->isPressed()){
  //   Serial.println("Button 2 pressed");
  // }
  // else if(enterButton->isPressed()){
  //   Serial.println("Enter button pressed");
  // }
  // String s = String(code);
  // Serial.print("This is the code:");
  //Serial.println(s);



  // Serial.printf("Value is %i\n", value);

  #ifdef HTTP_TEST
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/testwifi/index.html";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  #endif
}

int waitForKeyCode(){
  long timeout = 3000;
  long initTime = millis();
  Serial.println(initTime);
  int passkey = 0;
  bool enterPressed = 0;
  while(((millis() - initTime) < timeout) || (!enterPressed)){
      if(button1->isPressed()){
        Serial.println("Button 1 pressed.");
        initTime = millis();
        passkey = (passkey*10) + 1;
      }
      else if(button2->isPressed()){
        Serial.println("Button 2 pressed");
        initTime= millis();
        passkey = passkey*10 + 2;
      }
      else if(enterButton->isPressed()){
        Serial.println("Enter button pressed");
        enterPressed = 1;
      }
    }
    return passkey;
}
