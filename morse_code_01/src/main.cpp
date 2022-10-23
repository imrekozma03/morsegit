#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include<Arduino.h>
#include<string>


//joystick pins
#define vrx 4
#define vry 15
#define buttonpin 23

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char alphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char numberarray[11] = {'-','0','1','2','3','4','5','6','7','8','9'};
char alphabet_capital[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
int row = 1;
int coloumn_letters = 0;
int coloumn_numbers = 0;
String wordstring;

void translate_to_morse(){


}

//function that displays stuff
void draw(int lettercoloumn, int numbercoloumn){

  //create numberstring
  String numberstring;
  int firstdig = numbercoloumn - 1;
  int secdig = numbercoloumn + 1;
  if(firstdig < 0)
    firstdig = 10;
  if(secdig > 10)
    secdig = 0;

  if(row == 2){
    numberstring += numberarray[firstdig];
    numberstring += ' ';
    numberstring += numberarray[numbercoloumn];
    numberstring += ' ';
    numberstring += numberarray[secdig];
  }else{
    numberstring += numberarray[firstdig];
    numberstring += numberarray[numbercoloumn];
    numberstring += numberarray[secdig];
  }

 /* if((numbercoloumn != 0) && (numbercoloumn != 10)){
    numberstring += numberarray[numbercoloumn - 1];
    numberstring += ' ';
    numberstring += numberarray[numbercoloumn];
    numberstring += ' ';
    numberstring += numberarray[numbercoloumn + 1];
    }else if(numbercoloumn == 0){
    numberstring += numberarray[10];
    numberstring += ' ';
    numberstring += numberarray[0];
    numberstring += ' ';
    numberstring += numberarray[1];  
    }else if(numbercoloumn == 10){
    numberstring += numberarray[9];
    numberstring += ' ';
    numberstring += numberarray[10];
    numberstring += ' ';
    numberstring += numberarray[0];   
    }*/


  //create letterstring
  String letterstring;
      //add the first 3
  for(int i = 3; i > 0; i-- ){
    int pos = lettercoloumn - i;
    if(pos < 0){
      pos += 26;
    }
    letterstring += alphabet[pos];
  }

    //add middle CAPITAL
if(row == 1){
letterstring += ' ';
letterstring += alphabet_capital[lettercoloumn];
letterstring += ' ';
}else{
  letterstring += alphabet[lettercoloumn];
}
   //add last 3
  for(int i = 1; i < 4; i++ ){
    int pos = lettercoloumn + i;
    if(pos > 25){
      pos -= 26;
    }
    letterstring += alphabet[pos];
  }

//display the created words

  //display the word
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(8, 8);
  display.println(wordstring);

  //display the alphabet
  
  display.setTextSize(2);
  if(row == 1)
  display.setCursor(12, 30);
  else
  display.setCursor(23, 30);

  display.println(letterstring);

//display numbers
  display.setTextSize(1);
  display.setCursor(50, 53);
  display.println(numberstring);
  display.display();
}

void setup() {
  Serial.begin(115200);

//joystick pinmodes
pinMode(vrx, INPUT);
pinMode(vry, INPUT);
pinMode(buttonpin, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
draw(coloumn_letters, coloumn_numbers);
  
}

void loop() {
  int axis_x = analogRead(vrx);
  int axis_y = analogRead(vry);
  int button_state = digitalRead(buttonpin);
  //selection 
  if(button_state == 0){
    if(row == 0)
    translate_to_morse();
    else if(row == 1)
      wordstring += alphabet[coloumn_letters];
    else if(row == 2)
      wordstring += numberarray[coloumn_numbers];

  draw(coloumn_letters, coloumn_numbers);
  delay(200);
}

  //move up or down
  if((axis_y == 0) && (row != 0)){
    row--;
    draw(coloumn_letters, coloumn_numbers);

    delay(250);
  }
  else if((axis_y == 4095) && (row != 2)){
    row++;
    draw(coloumn_letters, coloumn_numbers);

    delay(250);
  }

//move right or left
if((axis_x == 0) || (axis_x == 4095)){
  if((row == 1) && (axis_x == 0))
    coloumn_letters--;
  else if((row == 1) && (axis_x == 4095))
    coloumn_letters++;
  else if((row == 2) && (axis_x == 4095))
    coloumn_numbers++;
  else if((row == 2) && (axis_x == 0))
    coloumn_numbers--;

//correction
  if(coloumn_letters < 0)
    coloumn_letters = 25;
  if(coloumn_letters > 25)
    coloumn_letters = 0;

  if(coloumn_numbers < 0)
    coloumn_numbers = 10;
  if(coloumn_numbers > 10)
    coloumn_numbers = 0;

draw(coloumn_letters, coloumn_numbers);


delay(200);
}


  
/*Serial.print("letter: ");
Serial.print(coloumn_letters);
Serial.print(" number: ");
Serial.print(coloumn_numbers);
Serial.print(" row: ");
Serial.print(row);
Serial.print(" \n ");*/


  
}