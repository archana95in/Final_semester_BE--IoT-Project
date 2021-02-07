
#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

byte mac[] = { 0x00, 0x1A, 0xB6, 0x03, 0x14, 0xED };
char server[] = "shrikant.16mb.com";

EthernetClient client;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
										//  "{\"P\":1,\"H\":120,\"T\":32,\"ECG\":[567,678,678]}";
String data="{\"P\":";
boolean newline=false, response=false;
String ACK;
int HR,patientID;

const unsigned long durationHR=5000;
const int SW1=8;  // Patient 1
const int SW2=9;  // Patient 2
const int SW3=10;  // Patient 3
const int SW4=30;  // OK
const int pinHR=40; // Interrupt Pin for HR

void setup() {
  delay(1000);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("IoT HealthCare");
  delay(2000);
  initIoT();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Select PatientID");
}

void loop() {
  boolean P1,P2,P3;

  P1=digitalRead(SW1);
  P2=digitalRead(SW2);
  P3=digitalRead(SW3);
  if(P1||P2||P3){
    if(P1)  patientID=1;
    if(P2)  patientID=2;
    if(P3)  patientID=3;
    lcd.clear();
    lcd.setCursor(0,0);
	  lcd.print("PatientID");
	  lcd.setCursor(0,1);
	  lcd.print(patientID);
    delay(1000);  // Delete
    process();
  }
}

void initIoT(){
  pinMode(SW1,INPUT);
  pinMode(SW2,INPUT);
  pinMode(SW3,INPUT);
  pinMode(SW4,INPUT);
  pinMode(pinHR,INPUT_PULLDOWN);
  initializeEthernet();
}

void process(){
  data+=String(patientID)+",\"T\":";
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Take Temperature.?");
  while(!(digitalRead(SW4)));
  lcd.setCursor(0,1);
  lcd.print("Wait...");
  delay(1000);
  takeTemperature();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Take Heart Rate.?");
  while(!(digitalRead(SW4)));
  lcd.setCursor(0,1);
  lcd.print("Wait...");
  delay(1000);
  takeHR();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Take ECG.?");
  while(!(digitalRead(SW4)));
  lcd.setCursor(0,1);
  lcd.print("Wait...");
  delay(1000);
  takeECG();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Send to Server.?");
  while(!(digitalRead(SW4)));
  lcd.setCursor(0,1);
  lcd.print("Wait...");
  delay(1000);
  connectServer();
  ackServer();
}

void takeTemperature(){
  int tempC;
  tempC=analogRead(A3);
  tempC/=12.41;
  data+=String(tempC)+",\"H\":";
  lcd.print(tempC);
  delay(2000);
}

void takeHR(){
  unsigned long startTime;
  HR=0;
  startTime=millis();
  attachInterrupt(pinHR,readHR,RISING);
  while((millis()-startTime)<durationHR){
    
  }
  detachInterrupt(pinHR);
  HR*=12;
  data+=String(HR)+",\"ECG\":[0";
  lcd.print(HR);
  delay(2000);
}

void readHR(){
  HR++;
}

void takeECG(){
  int adc;
  for(int i=0;i<100;i++){
    adc=analogRead(A1);
    data+=","+String(adc);
    delay(20);
  }
  data+="]}";
}

void initializeEthernet(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ethernet Init");
  lcd.setCursor(0,1);
  if (Ethernet.begin(mac) == 0){
    lcd.print("DHCP Error");
    while(1);
  }
  lcd.print("DHCP OK");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IP Address");
  lcd.setCursor(0,1);
  lcd.print(Ethernet.localIP());
  delay(2000);
}

void connectServer(){
  lcd.clear();
  lcd.setCursor(0,0);
  if(client.connect(server, 80)){
    lcd.print("Connected");
    delay(2000);
    client.println("POST /iot/uploadData.php HTTP/1.1");
    client.println("Host: shrikant.16mb.com");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println("Connection: close");
    client.println();
    client.println(data);
    Serial.println(data);
  }
  else{
    lcd.print("Disconnected");
    delay(2000);
  }
}

void ackServer(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Server ACK");
  ACK=" ";
  while(client.connected()){
    if(client.available()){
      char c = client.read();
      if(newline && (c=='\r'))  response=true;
      else  newline=false;
      if(c=='\n') newline=true;
      if(response){
        ACK+=c;
      }
    }
  }
  lcd.setCursor(0,1);
  lcd.print(ACK);
  Serial.println(ACK);
  delay(2000);
  if(!client.connected()){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Disconnected");
    lcd.setCursor(0,1);
    lcd.print("Select PatientID");
    response=false;
    client.stop();
  }
}

