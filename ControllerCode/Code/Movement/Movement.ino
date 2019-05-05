#define signalSecInForward A0
#define signalSecInBackward A1

#define R_PWM_R 10
#define L_PWM_R 11
#define R_PWM_L 5
#define L_PWM_L 6
#define trigR 13
#define echoR 12
#define trigL 4
#define echoL 3

int sigi, sigj;
long distanceR,durationR, distanceL, durationL;

void setup(){
  Serial.begin(9600);

  //Motor Control
  pinMode(R_PWM_R, OUTPUT);
  pinMode(L_PWM_R, OUTPUT);
  pinMode(R_PWM_L, OUTPUT);
  pinMode(L_PWM_L, OUTPUT);

  // Ultrasonic control
  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);

  //Signal from arduino Mega
  pinMode(signalSecInForward, INPUT);
  pinMode(signalSecInBackward, INPUT);

  digitalWrite(trigR, LOW);
  digitalWrite(trigL, LOW);
  Serial.println("Setup Complete");
  delay(500);
}

void loop(){
  sigi = (digitalRead(signalSecInForward));
  sigj = (digitalRead(signalSecInBackward));

  if(sigi){
    forward();
  }
  /*if(sigj){
    backward();
  }*/
  if(sigi==0){
    stopBot();
  }
}

void stopBot(){
  digitalWrite(R_PWM_R,LOW);
  digitalWrite(L_PWM_R,LOW);
  digitalWrite(R_PWM_L,LOW);
  digitalWrite(L_PWM_L,LOW);
}

void forward(){
  digitalWrite(trigR, HIGH);
  delay(10);
  digitalWrite(trigR, LOW);
  durationR = pulseIn(echoR, HIGH);
  distanceR = durationR/29.1;
  digitalWrite(trigL, HIGH);
  delay(10);
  digitalWrite(trigL, LOW);
  durationL = pulseIn(echoL, HIGH);
  distanceL = durationL/29.1;

  Serial.print("Right = ");
  Serial.print(distanceR);
  Serial.print(", Left = ");
  Serial.println(distanceL);

  analogWrite(R_PWM_R, 80);
  digitalWrite(L_PWM_R,LOW);
  analogWrite(R_PWM_L, 128);
  digitalWrite(L_PWM_L,LOW);
  if(distanceR<25){
    analogWrite(R_PWM_R, 0);
    digitalWrite(L_PWM_R, LOW);  
  }
  if(distanceL<25){
    analogWrite(R_PWM_L,0);
    digitalWrite(L_PWM_L,LOW);
  }
}

/*void backward(){
  digitalWrite(trigR, HIGH);
  delay(10);
  digitalWrite(trigR, LOW);
  durationR = pulseIn(echoR, HIGH);
  distanceR = durationR/29.1;
  digitalWrite(trigL, HIGH);
  delay(10);
  digitalWrite(trigL, LOW);
  durationL = pulseIn(echoL, HIGH);
  distanceL = durationL/29.1;

  Serial.print("Right = ");
  Serial.print(distanceR);
  Serial.print(", Left = ");
  Serial.println(distanceL);

  if((distanceR>=25)&&(distanceL>=25)){
    analogWrite(L_PWM_R, 50);
    digitalWrite(R_PWM_R,LOW);
    analogWrite(L_PWM_L, 75);
    digitalWrite(R_PWM_L,LOW);
  }
  if(distanceR<25){
    analogWrite(R_PWM_R, 50);
    digitalWrite(L_PWM_R, LOW);  
  }
  if(distanceL<25){
    analogWrite(R_PWM_L,50);
    digitalWrite(L_PWM_L,LOW);
  }
}
*/
