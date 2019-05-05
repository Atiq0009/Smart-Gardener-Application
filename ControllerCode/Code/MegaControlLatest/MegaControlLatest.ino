// Libraries
#include <LiquidCrystal.h>
#include <Servo.h>
#include <DS3231.h>
#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>
#include "Adafruit_Sensor.h"
#include "DHT.h"

//keypad Layout
char* password = "1234";
int positionning = 0;
int counting = 0;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
  };

// Defined all Pins
#define waterPump   2
#define DHTPIN      3
#define gasSense    4
#define RST_PIN     5
#define SensorPin   6 //pH meter Pin
#define phTO        7
#define rotHandLeft  8
#define rotHandRight 9
#define R_PWM       10
#define L_PWM       11
#define trigHand    12
#define echoHand    13
int Pin4 = 22;
int Pin3 = 23;
int Pin2 = 24;
int Pin1 = 25;
byte rowPins[ROWS] = {26, 27, 28, 29};
byte colPins[COLS] = {30, 31, 32, 33};
LiquidCrystal lcd(39,38,37,36,35,34); //Rs, en, d4, d5, d6, d7
#define ServoCamera   40
#define ServoSecurity 41
#define ultraSecEcho2 42
#define ultraSecTrig2 43
#define ultraSecEcho1 44
#define ultraSecTrig1 45
#define buzzerSec1    46
#define buzzerSec2    47
#define Forward       48
#define Backward      49
#define SS_PIN        53
#define LDR           A0
#define keypadSwitch  A1
#define moistureCheck A2
#define PosSwitch     A3

#define DHTTYPE DHT11

//create Class Constructor
MFRC522 mfrc522(SS_PIN, RST_PIN);
DHT dht(DHTPIN, DHTTYPE);
DS3231 rtc(SDA, SCL);
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo servoC;
Servo servoS;


//Other Veriables
//Tag Store veriable
String Tag1, Tag2, Tag3, Tag4;

//Multiple veriables
bool tagRead = true;
String readSt = "";
String readByte = "";
float humidity, temp, fahren;
String gasPrsnt = "0";
String dayLight = "Not Detect";
String pass = "";
bool inputTag= false;
int _step = 0;
boolean dir = false;// false=clockwise, true=counter clockwise
int count=0;
int runtime = 2500;
int camerastop = 0;
int stepsHand = 100;

//PH Variable
#define Offset 0.00            //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex = 0;

//special veriable
int c = 0;
int w = 0;
bool camera = false;
bool security = false;
bool dailyCheck = false;

int cV=0, sV=0, dV=0;
String GasSt, WaterSt;
float humSt,pHSt;

void setup(){

  // Function Begin
  Serial.begin(9600);
  rtc.begin();
  dht.begin();
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.begin(16,2);

  // Pinmode and functionality
  servoC.attach(ServoCamera);
  servoS.attach(ServoSecurity);
  servoS.write(0);
  servoC.write(0);
  
  //Movement
  pinMode(Forward, OUTPUT);
  pinMode(Backward, OUTPUT);
  digitalWrite(Forward, LOW);
  digitalWrite(Backward, LOW);
  
  // Setup Hand 2
  pinMode(rotHandLeft, OUTPUT);
  pinMode(rotHandRight, OUTPUT);
  digitalWrite(rotHandLeft, LOW);
  digitalWrite(rotHandRight, LOW);
  
  // All inputs
  pinMode(LDR, INPUT);
  pinMode(keypadSwitch, INPUT);
  pinMode(moistureCheck, INPUT);
  pinMode(gasSense, INPUT);
  pinMode(PosSwitch, INPUT);
  
  // Stepper
   pinMode(Pin1, OUTPUT);  
   pinMode(Pin2, OUTPUT);  
   pinMode(Pin3, OUTPUT);  
   pinMode(Pin4, OUTPUT);
   //Stepper Output
   digitalWrite(Pin1, LOW);
   digitalWrite(Pin2, LOW);
   digitalWrite(Pin3, LOW);
   digitalWrite(Pin4, LOW);
  
  // Buzzer & security
   pinMode(buzzerSec1, OUTPUT);
   pinMode(buzzerSec2, OUTPUT);
   //Buzzer Output
   digitalWrite(buzzerSec1, LOW);
   digitalWrite(buzzerSec2, LOW);
  
  // Security & HC-SR04
  pinMode(ultraSecEcho1,INPUT);
  pinMode(ultraSecTrig1,OUTPUT);
  pinMode(ultraSecEcho2,INPUT);
  pinMode(ultraSecTrig2,OUTPUT);
  //Output
  digitalWrite(ultraSecTrig1, LOW);
  digitalWrite(ultraSecTrig2, LOW);

  //Motor Hand 
  pinMode(R_PWM, OUTPUT);
  pinMode(L_PWM, OUTPUT);

  //Ultrasonic Hand
  pinMode(trigHand, OUTPUT);
  pinMode(echoHand, INPUT);
  digitalWrite(trigHand, HIGH);

  //Water
  pinMode(waterPump,OUTPUT);
  digitalWrite(waterPump, LOW);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Smart Gardener");
  lcd.setCursor(0,1);
  lcd.print("--Wait for Tag--");
  while(tagRead){
    RfTag();
    delay(10000);
  }
  lcd.setCursor(0,1);
  lcd.print("-*-Tag Stored-*-");
  Serial.print("set");
  Serial.println("/t");
  delay(500);
  int c = 0;
}

void loop(){
  Humidity();
  checking();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T=");
  lcd.print(rtc.getTimeStr());
  
  lcd.print(",V:");
  lcd.print(cV);
  lcd.print(sV);
  lcd.print(dV);
  
  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print(" H:");
  lcd.print(humidity);
  
  delay(1000);
  while(digitalRead(keypadSwitch)){
    newIDcard();
  }
  
  while(camera){
    cameraFunction();
  }
  while(security){
    securityFunction();
  }
  
  while(dailyCheck){
    if(w==0){
      pullUpHand();
      while(digitalRead(PosSwitch)==0){}
      pullStopHand();
      w++;
    }
    dailyFunction();
  }
}

void checking(){
  while(Serial.available()){
    delay(10); //Delay added to make thing stable
    char c = Serial.read(); //Conduct a serial read
    readSt += c;
  }
  if(readSt != ""){
    if(readSt == "Wait"){
      Serial.print("set");
      Serial.println("/t");
    }else if((readSt=="000")||(readSt=="100")||(readSt=="010")||(readSt=="001")){
      cV = ((readSt.substring(0,1)).toInt());
      sV = ((readSt.substring(1,2)).toInt());
      dV = ((readSt.substring(2,3)).toInt());
    }
  }
  readSt = "";
  if((humidity<97)&&(temp>23)){
    if(cV){
      camera = true;
      digitalWrite(Forward, HIGH);
      digitalWrite(Backward, LOW);
      delay(500);
    }
    if(sV){
      security = true;
      digitalWrite(Forward, HIGH);
      digitalWrite(Backward, LOW);
      delay(500);
    }
    if(dV){
      dailyCheck = true;
      digitalWrite(Forward, HIGH);
      digitalWrite(Backward, LOW);
      delay(500);
      w=0;
    }
    
  }
  
}

void dailyFunction(){
  rfScanner();
  bool checkDC;
  if(readByte!=""){
    if(Tag1 == readByte){
      digitalWrite(Forward, LOW);
      digitalWrite(Backward, LOW);
      dailyCheck =false;
      dV =0;
      pullDownHand();
      delay(2000);
      pullStopHand();
      readByte = "";
      delay(1);
    }
    if(Tag3 == readByte){
      checkDC = true;
    }
    if(Tag4 == readByte){
      checkDC = true;
    }
    while(checkDC){
      humSt = humidity;
      gasModule();
      delay(100);
      GasSt = gasPrsnt;
      digitalWrite(Forward, LOW);
      digitalWrite(Backward, LOW);
      delay(500);
      pullDownHand();
      digitalWrite(trigHand, HIGH);
      delay(10);
      digitalWrite(trigHand, LOW);
      long duration1 = pulseIn(echoHand, HIGH);
      long dist = (duration1/29.1);
      while(dist>20){
        digitalWrite(trigHand, HIGH);
        delay(10);
        digitalWrite(trigHand, LOW);
        duration1 = pulseIn(echoHand, HIGH);
        dist = (duration1/29.1);
      }
      pullStopHand();
      delay(500);
      moisCheck();
      delay(200);
      pullUpHand();
      while(digitalRead(PosSwitch)==0){}
      pullStopHand();
      rotateSecondHand();
      delay(500);
      pullDownHand();
      digitalWrite(trigHand, HIGH);
      delay(10);
      digitalWrite(trigHand, LOW);
      long dura1 = pulseIn(echoHand, HIGH);
      long disti = (dura1/29.1);
      while(disti>20){
        digitalWrite(trigHand, HIGH);
        delay(10);
        digitalWrite(trigHand, LOW);
        dura1 = pulseIn(echoHand, HIGH);
        disti = (dura1/29.1);
      }
      pullStopHand();
      delay(1000);
      pHCheck();
      delay(2000);
      pullUpHand();
      while(digitalRead(PosSwitch)==0){}
      pullStopHand();
      Serial.print("TreeTag/t");
      Serial.print(readByte);
      Serial.print("/t");
      Serial.print(humSt);
      Serial.print("/t");
      Serial.print(pHSt);
      Serial.print("/t");
      Serial.print(GasSt);
      Serial.print("/t");
      Serial.print(WaterSt);
      Serial.println("/t");
      readByte = "";
      rotateFirstHand();
      checkDC =false;
      digitalWrite(Forward, HIGH);
      digitalWrite(Backward, LOW);
      delay(1000);
    }
  }
}

void securityFunction(){
  lcd.setCursor(0,1);
  lcd.print("Security Process");
  digitalWrite(buzzerSec1, HIGH);
  digitalWrite(buzzerSec2, HIGH);
  delay(200);
  rfScanner();
  if(Tag1 == readByte){
    digitalWrite(Forward, LOW);
    digitalWrite(Backward, LOW);
    security = false;
    sV=0;
    delay(1);
  }
  readByte = "";
  if(c<=180){
      c++; 
    }else{
      c=0;
    }
  if(c>=0 && c<90){
    servoS.write(c); 
  }
  if(c>=90 && c<180){
    int ca = 180-c;
    servoS.write(ca);
  }
    
  rfScanner();
  if(Tag1 == readByte){
      digitalWrite(Forward, LOW);
      digitalWrite(Backward, LOW);
      security = false;
      sV=0;
      delay(1);
      
    }
  readByte = "";
  digitalWrite(buzzerSec1, LOW);
  digitalWrite(buzzerSec2, LOW);
  delay(200);
}


void cameraFunction(){
  switch(_step){ 
   case 0:
   if(count<runtime){  
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, HIGH);
     }else{
      stopMotor(); 
     }
   break;  
   case 1:
   if(count<runtime){ 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, HIGH); 
     digitalWrite(Pin4, LOW);
     }else{
      stopMotor(); 
     } 
   break;  
   case 2:
   if(count<runtime){  
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, HIGH); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW);
     }else{
      stopMotor(); 
     } 
   break;  
   case 3:
   if(count<runtime){  
     digitalWrite(Pin1, HIGH);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW);
     }else{
      stopMotor(); 
     } 
   break; 
   default: 
     stopMotor();
     break;  
 } 
 if(dir){ 
   _step++; 
 }else{ 
   _step--; 
 } 
 if(_step>7){ 
   _step=0; 
 } 
 if(_step<0){ 
   _step=7; 
 }
  if(c<=180){
      c++; 
    }else{
      c=0;
    }
      if(c>=0 && c<90){
        servoC.write(c); 
      }
      if(c>=90 && c<180){
          int ca = 180-c;
          servoC.write(ca);
      }
    if(count < runtime){
       count++;
    }else{
       count = 0;// set to zero
       if(dir){
        dir = false;
        if(camerastop){
          camera = false;
          camerastop = 0;
          cV=0;
        }
       }else{
        dir = true;
       }
    }
  
  
  rfScanner();
  if(Tag1 == readByte){
    digitalWrite(Forward, LOW);
    digitalWrite(Backward, LOW);
    camerastop = 1;
    delay(1);
  }
  readByte = "";
  delay(1);
}

void stopMotor(){
  digitalWrite(Pin1, LOW);  
  digitalWrite(Pin2, LOW); 
  digitalWrite(Pin3, LOW); 
  digitalWrite(Pin4, LOW);
}

void newIDcard(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("New Id input:");
    lcd.setCursor(0,1);
    lcd.print("Pass: ");
    lcd.print(pass);
    delay(200);
    keypadConnect();
    while(inputTag){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Tag Insert:");
      rfScanner();
      if(readByte !=""){
        Serial.print("NewTag");
        Serial.print("/t");
        Serial.print(readByte);
        Serial.println("/t");
        lcd.setCursor(0,1);
        lcd.print(readByte);
        digitalWrite(buzzerSec1, HIGH);
        delay(1000);
        digitalWrite(buzzerSec1, LOW);
        delay(10000);
        if(digitalRead(keypadSwitch)==0){
          readByte="";
          inputTag = false;
          Serial.println("");
        }else{
          inputTag = true;
        }
       }
       delay(500);
      
    }
}

void keypadConnect(){
  char whichKey = myKeypad.getKey();
  if(whichKey == '1' || whichKey == '2' || whichKey == '3' ||
      whichKey == '4' || whichKey == '5' || whichKey == '6' ||
      whichKey == '7' || whichKey == '8' || whichKey == '9' ||
      whichKey == '0' || whichKey == 'A' || whichKey == 'B' ||
      whichKey == 'C' || whichKey == 'D' || whichKey == '*' || whichKey == '#'){
    pass += whichKey;
    counting++; 
  }
  if(counting > 6){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Invalid Password");
    delay(5000);
    pass = "";
    counting = 0;
  }
  
  if(whichKey == password[positionning])
   {
      positionning++;
   }
   if(positionning == 4){
      lcd.clear();      
      lcd.setCursor(0,0);
      lcd.print("***Verified***");
      inputTag = true;
      delay(5000);
    }
}

void rfScanner(){
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      for(byte i=0; i< mfrc522.uid.size; i++){
        //Serial.print(mfrc522.uid.uidByte[i]< 0x10 ? " 0":" ");
        //Serial.print(mfrc522.uid.uidByte[i], HEX);
        readByte += int(mfrc522.uid.uidByte[i]);
      }
      mfrc522.PICC_HaltA();
    }
  }
}

void RfTag(){
  Serial.print("Tag");
  Serial.println("/t");
  while(Serial.available()){
    delay(10); //Delay added to make thing stable
    char c = Serial.read(); //Conduct a serial read
    readSt += c;
  }
  if(readSt != ""){
    Tag1 = ((readSt.substring(0,10)));
    Tag2 = ((readSt.substring(11,21)));
    Tag3 = ((readSt.substring(22,33)));
    Tag4 = ((readSt.substring(34,44)));

    
    //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
    //rtc.setTime(9, 45, 0);     // Set the time to 12:00:00 (24hr format)
    //rtc.setDate(13, 3, 2019);
    tagRead = false;
  }
  readSt = "";
}

//Sensors Function
void Humidity(){
  //delay(2000);
  
  humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temp = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  fahren = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temp) || isnan(fahren)) {
    //Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(fahren, humidity);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(temp, humidity, false);
  /*Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.print(F("°C "));
  Serial.print(fahren);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));*/
}

void gasModule(){
  if(digitalRead(gasSense) == 1)
  {
    gasPrsnt = "Y";
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Gas Detected");
    delay(500);
  }else{
    gasPrsnt = "N";
  }
}

void LDRCheck(){
  long i= analogRead(LDR);
  if(i>30){
     dayLight = "Day Light";
  }else{
    dayLight = "Dark";
  }
}

void moisCheck(){
  delay(500);
  int val = analogRead(moistureCheck);
  if(val<750){
    lcd.setCursor(0,1);
    lcd.print("Water");
    WaterSt = "N";
  }else{
    lcd.setCursor(0,1);
    lcd.print("No Water");
    WaterSt = "Y";
    digitalWrite(waterPump, HIGH);
    delay(10000);
    digitalWrite(waterPump, LOW);
  }
  delay(500);
}
void pHCheck(){
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  if (millis() - samplingTime > samplingInterval)
  {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth)pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024 + 0.05;
    pHValue = 3.5 * voltage + Offset;
    pHSt = pHValue;
    samplingTime = millis();
  }
  /*if (millis() - printTime > printInterval)  //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Voltage:");
    Serial.print(voltage, 2);
    Serial.print("    pH value: ");
    Serial.println(pHValue, 2);
    printTime = millis();
  }*/
}

double avergearray(int* arr, int number)
{
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0)
  {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if (number < 5) //less than 5, calculated directly statistics
  {
    for (i = 0; i < number; i++)
    {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  }
  else
  {
    if (arr[0] < arr[1])
    {
      min = arr[0]; max = arr[1];
    }
    else
    {
      min = arr[1]; max = arr[0];
    }
    for (i = 2; i < number; i++)
    {
      if (arr[i] < min)
      {
        amount += min;      //arr<min
        min = arr[i];
      }
      else
      {
        if (arr[i] > max)
        {
          amount += max;  //arr>max
          max = arr[i];
        }
        else
        {
          amount += arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount / (number - 2);
  }//if
  return avg;
}

void rotateFirstHand(){
  digitalWrite(rotHandLeft, HIGH);
  digitalWrite(rotHandRight, LOW);
  delay(140);
  digitalWrite(rotHandLeft, LOW);
  digitalWrite(rotHandRight, LOW);
  delay(140);
  
}
void rotateSecondHand(){
  digitalWrite(rotHandLeft, LOW);
  digitalWrite(rotHandRight, HIGH);
  delay(140);
  digitalWrite(rotHandLeft, LOW);
  digitalWrite(rotHandRight, LOW);
  delay(140);
  
}

void pullDownHand(){
  analogWrite(R_PWM, 80);
  digitalWrite(L_PWM,LOW);
}

void pullStopHand(){
  digitalWrite(R_PWM, LOW);
  digitalWrite(L_PWM,LOW);
}

void pullUpHand(){
  analogWrite(L_PWM, 160);
  digitalWrite(R_PWM,LOW);
}

