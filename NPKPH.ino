#include <SoftwareSerial.h>
#define RE 8
#define DE 7
const byte npkRequest[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x07, 0x04, 0x08};  
const byte phRequest[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x04, 0x44, 0x09};   
SoftwareSerial mod(2, 3);  
void setup() {
  Serial.begin(9600);      
  mod.begin(4800);         
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  delay(500);
}
void loop() {
  handleNPK();
  delay(1000);  
  handlePH();
  delay(1000);  
}
void handleNPK() {
  Serial.println("Handling NPK...");
  mod.flush();
  Serial.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);  
  mod.write(npkRequest, sizeof(npkRequest));
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(100);
  byte response[19];
  for (byte i = 0; i < 19; i++) {
    if (mod.available()) {
      response[i] = mod.read();
    } else {
      response[i] = 0; 
    }
  }
  Serial.print("NPK Response (Hex): ");
  for (byte i = 0; i < 19; i++) {
    Serial.print("0x");
    if (response[i] < 0x10) Serial.print("0");
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  unsigned int N = (response[11] << 8) | response[12];
  unsigned int K = (response[15] << 8) | response[16];
  unsigned int P = (response[13] << 8) | response[14];
  Serial.print("Nitrogen (N): ");
  Serial.println(N);
  Serial.print("Potassium (K): ");
  Serial.println(K);
  Serial.print("Phosphorous (P): ");
  Serial.println(P);
}
void handlePH() {
  Serial.println("Handling PH...");
  mod.flush();
  Serial.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);  
  mod.write(phRequest, sizeof(phRequest));
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(100);
  byte response[13];
  for (byte i = 0; i < 13; i++) {
    if (mod.available()) {
      response[i] = mod.read();
    } else {
      response[i] = 0;
    }
  }
  Serial.print("PH Response (Hex): ");
  for (byte i = 0; i < 13; i++) {
    Serial.print("0x");
    if (response[i] < 0x10) Serial.print("0");
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  unsigned int PH = (response[9] << 8) | response[10]; 
  Serial.print("PH Value before division: ");
  Serial.println(PH);  
  float PHResult = PH / 10.0;
  Serial.print("PH Value after division: ");
  Serial.println(PHResult, 1);  
}
