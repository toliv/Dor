#include <DebouncedButton.h>
#include <ESP8266WiFi.h>
#define BUTTON_TEST
#include <SPI.h>

const char* ssid     = "CalVisitor";
const char* password = "yourpassword";
const int buttonPin = 12;

DebouncedButton* button1;
DebouncedButton* button2;
DebouncedButton* button3;
DebouncedButton* enterButton;
int redPin = 0;
int greenPin = 16;
int bluePin = 2;


bool enterButtonPressed = 0;
int sequence = 0;
int timeout = 5000;
int lastKeyPress = 0;


const char* host = "https://glacial-basin-21655.herokuapp.com";

void setup() {
  Serial.begin(115200);

  delay(100);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
 

  button1 = new DebouncedButton(14, 50);
  button2 = new DebouncedButton(12, 50);
  button3 = new DebouncedButton(5, 50);
  enterButton = new DebouncedButton(4, 50);


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
  setColor(255, 0, 255);  // purple
  if(millis() - lastKeyPress > timeout){
    lastKeyPress = millis();
    sequence = 0;
  }
  if(enterButtonPressed){
    if(sequence != 0){
      String s = String(sequence);
      Serial.println(s);
      bool res = isValidKeyCode(s);
      sequence = 0;
      enterButtonPressed = 0;
    }
  }
  if(button1->isPressed()){
    Serial.println("Button 1 pressed");
    sequence = sequence*10 + 1;
    lastKeyPress = millis();
  }
  else if(button2->isPressed()){
    Serial.println("Button 2 pressed");
    sequence = sequence *10 + 2;
    lastKeyPress = millis();
  }
  else if(button3->isPressed()){
    Serial.println("Button 3 pressed");
    sequence = sequence * 10 + 3;
  }
  else if(enterButton->isPressed()){
    Serial.println("Enter button pressed");
    enterButtonPressed = 1;
  }
  delay(30);
}

bool isValidKeyCode(String code){
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return 0;
  }
  
  // We now create a URI for the request
  String url = "/api/v1/verify";
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.println("POST /api/v1/verify HTTP/1.1");
  client.println("Host: glacial-basin-21655.herokuapp.com");
  client.println("Cache-Control: no-cache");
  client.println("Connection: close");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  String json = String("{\"pass\":\""+code+ "\"}");
  client.println(json.length());
  client.println();
  Serial.println(json);
  client.print(json);

  delay(500);
  
  Serial.println("Sent POST request");

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    if (line.startsWith("200 OK", 9)) {
      Serial.println("Got an OK from the server");
      setColor(0, 255, 0);  // green
      delay(5000);
      break;
    }
    else {
      //bad response
      setColor(255, 0, 0);  // red
      delay(5000);
      break;
    }
  }
  
  Serial.println();
  Serial.println("closing connection");
  return 1;
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

