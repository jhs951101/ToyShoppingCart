/* LCD, LED, Sound, Data */

#include <LiquidCrystal.h>
#include <Wire.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int Red = 8;
int Blue = 10;
int Green = 9;

long int money = 0;

void setup() {
  lcd.begin(16, 2);
  
  lcd.setCursor(3, 0);
  lcd.print("Shopping!");
  lcd.setCursor(5, 1);
  lcd.print("Shopping!");
  
  Wire.begin(4);
  Wire.onReceive(receiveEvent); // register event
  
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);

  digitalWrite(Red, 100);
  digitalWrite(Green, 100);
  digitalWrite(Blue, 100);
}

int isSound = 0;

void loop() {
  
  if(isSound == 1){  // general sound sign
    tone(12, 900, 100);
    delay(150);
    tone(12, 700, 250);
    delay(300);
    noTone(12);
    
    isSound = 0;
  }
  else if(isSound == 2){  // special sound sign
    tone(12, 900, 100);
    delay(200);
    tone(12, 700, 100);
    delay(200);
    tone(12, 400, 100);
    delay(200);
    tone(12, 100, 100);
    delay(200);
    tone(12, 300, 100);
    delay(200);
    tone(12, 600, 100);
    delay(200);
    tone(12, 1000, 100);
    delay(200);
    noTone(12);

    isSound = 0;
  }

}

void receiveEvent(int howMany)
{  
  while( Wire.available() )
  {
    byte sign = Wire.read(); // receive sign
    
    if(sign == 0){  // 초기화
      lcd.clear();
      
      lcd.write("Total: $ ");
      lcd.print(money);
      
      lcd.setCursor(8, 1);
      lcd.write("Thanks!!");
      
      money = 0;
      
      SpecialSound();
      // play special sound
      
      digitalWrite(Red, 0);
      digitalWrite(Green, 0);
      digitalWrite(Blue, 10);
    }
    else if(sign == 1){  // red 상품 get
      lcd.clear();
      
      lcd.write("RED : $ 50,000");
      lcd.setCursor(0, 1);
      
      money = money + 50000;
      
      lcd.write("-> $ ");
      lcd.print(money);
      
      Sound();
      
      digitalWrite(Red, 0);
      digitalWrite(Green, 100);
      digitalWrite(Blue, 100);
    }
    else if(sign == 2){  // green 상품 get
      lcd.clear();
      
      lcd.write("GREEN : $ 30,000");
      lcd.setCursor(0, 1);
      
      money = money + 30000;

      lcd.write("-> $ ");
      lcd.print(money);

      // play sound
      Sound();

      digitalWrite(Red, 100);
      digitalWrite(Green, 0);
      digitalWrite(Blue, 100);
    }
    else if(sign == 3){  // blue 상품 get
      lcd.clear();

      lcd.write("BLUE : $ 25,000");
      lcd.setCursor(0, 1);

      money = money + 25000;

      lcd.write("-> $ ");
      lcd.print(money);

      // play sound
      Sound();

      digitalWrite(Red, 100);
      digitalWrite(Green, 100);
      digitalWrite(Blue, 0);   
    }
  }
}

void Sound(){
  isSound = 1;
}

void SpecialSound(){
  isSound = 2;
}