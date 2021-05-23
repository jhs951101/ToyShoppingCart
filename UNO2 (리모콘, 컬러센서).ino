/* Remote, Color_sensor */

#include "AFMotor.h" //import your motor shield library
#include "IRremote.h"
#include "Wire.h"

int receiver = 11;

AF_DCMotor motor1(1, MOTOR12_8KHZ); // set up motors.
AF_DCMotor motor2(2, MOTOR12_8KHZ);
AF_DCMotor motor3(3, MOTOR12_8KHZ);
AF_DCMotor motor4(4, MOTOR12_8KHZ);

IRrecv irrecv(receiver);  // create instance of 'irrecv'
decode_results results;   // create instance of 'decode_results'
 
const int s0 = 2;  
const int s1 = 3;  
const int s2 = 6;  
const int s3 = 5;  
const int out = 4;   
// LED pins connected to Arduino
// Variables  
int red = 0;  
int green = 0;  
int blue = 0;  
 
void setup() {
  Serial.begin(9600);  // 시리얼 모니터: 컬러 센서 테스트용
    
  pinMode(s0, OUTPUT);  
  pinMode(s1, OUTPUT);  
  pinMode(s2, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out, INPUT); 
  digitalWrite(s0, HIGH);  
  digitalWrite(s1, HIGH); 
  
  motor1.setSpeed(200);  //set the speed of the motors, between 0-255
  motor2.setSpeed(200);  
  motor3.setSpeed(200);  
  motor4.setSpeed(200);
  irrecv.enableIRIn();  // Start the receiver
  Wire.begin();
}
 
int sendCode;  // 0: 초기화, 1: 빨간색 상품, 2: 초록색 상품, 3: 파란색 상품

void loop()
{
  color();
  Serial.print("R Intensity:");  
  Serial.print(red, DEC);  
  Serial.print(" G Intensity: ");  
  Serial.print(green, DEC);  
  Serial.print(" B Intensity : ");  
  Serial.print(blue, DEC);  
  
  if (red < blue && red < green && red < 20)
  {  
   Serial.println(" - (Red Color)");
   
   Wire.beginTransmission(4);
   sendCode = 1;
   Wire.write(sendCode);  // sends 3 bytes
   Wire.endTransmission(); // stop transmitting  // slave한테 신호를 보냄(사운드, 화면, LED)
  }  
  else if (blue < red && blue < green)   
  {  
   Serial.println(" - (Blue Color)"); 
  
   Wire.beginTransmission(4);
   sendCode = 3;
   Wire.write(sendCode);  // sends 3 bytes
   Wire.endTransmission(); // stop transmitting  // slave한테 신호를 보냄(사운드, 화면, LED) 
  }  

  else if (green < red && green < blue)  
  {  
   Serial.println(" - (Green Color)");
  
   Wire.beginTransmission(4);
   sendCode = 2;
   Wire.write(sendCode);  // sends 3 bytes
   Wire.endTransmission(); // stop transmitting  // slave한테 신호를 보냄(사운드, 화면, LED) 
  } 
  else{
   Serial.println();  
  }
  
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  

  delay(500);   
}

void translateIR() // takes action based on IR code received
		   // describing Remote IR codes 
{
  if(results.value == 0xFF629D){  // forward
     motor1.run(FORWARD);
     motor2.run(FORWARD);
     motor3.run(FORWARD);
     motor4.run(FORWARD);
  }
  else if(results.value == 0xFFA857){  // reverse
     motor1.run(BACKWARD);
     motor2.run(BACKWARD);
     motor3.run(BACKWARD);
     motor4.run(BACKWARD);
  }
  else if(results.value == 0xFF22DD){  // left
     motor1.run(BACKWARD);
     motor2.run(BACKWARD);
     motor3.run(FORWARD);
     motor4.run(FORWARD);
  }
  else if(results.value == 0xFFC23D){  // right
     motor1.run(FORWARD);
     motor2.run(FORWARD);
     motor3.run(BACKWARD);
     motor4.run(BACKWARD);
  }
  else if(results.value == 0xFF02FD){  // ok
    Wire.beginTransmission(4);
    sendCode = 0;
    Wire.write(sendCode);  // sends 3 bytes
    Wire.endTransmission(); // stop transmitting  // slave한테 신호를 보냄(사운드, 화면, LED)
  }
  else if(results.value == 0xFF4AB5){  // stop
     motor1.run(RELEASE);
     motor2.run(RELEASE);
     motor3.run(RELEASE);
     motor4.run(RELEASE);
  }

  delay(500); // Do not get immediate repeat
}

void color()  
{    
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
  //count OUT, pRed, RED  
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);  
  //count OUT, pBLUE, BLUE  
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);  
  //count OUT, pGreen, GREEN  
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
}