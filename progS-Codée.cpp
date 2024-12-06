#include <Arduino.h>

//#define project1
#define project2
//#define project3
//#define project4
//#define project5
//#define project6

#if defined (project1)
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Wire.h>
#include <Keypad_I2C.h>

//LCD interface
const int pinRS     = D2;
const int pinEnable = D3;
const int pinD4     = D4;
const int pinD5     = D5;
const int pinD6     = D6;
const int pinD7     = D7;

LiquidCrystal lcd(pinRS, pinEnable, pinD4, pinD5, pinD6, pinD7);

#define I2CADDR 0x20
char keys[4][4] = {
  {'1', '2', '3', 'F'},
  {'4', '5', '6', 'E'},
  {'7', '8', '9', 'D'},
  {'A', '0', 'B', 'C'}
};

byte rowPins[4] = {0,01,2,3};      //connect to the row pinouts of the keypad
byte colPins[4] = {7,6,5,4}; //connect to the column pinouts of the keypad
Keypad_I2C keypad (makeKeymap (keys), rowPins, colPins, 4, 4, I2CADDR, PCF8574);
#define relay_pin D13

#elif defined (project2)

HardwareSerial Serial1(USART1);
#define LCD_NHD_Serial Serial1
#define relay_pin D9
char KEYS[] = {
  '1', '2', '3', 'F',
  '4', '5', '6', 'E',
  '7', '8', '9', 'D',
  'A', '0', 'B', 'C'
};

#elif defined (project3)
#include <Keypad.h>
#include <Wire.h>

#define LCD_I2C 0x28 // 0x50 shifted to right

//Clavier 4x4 pins:
const int ROW0 = D8;
const int ROW1 = D9;
const int ROW2 = D10;
const int ROW3 = D11;
const int COL0 = D4;
const int COL1 = D5;
const int COL2 = D6;
const int COL3 = D7;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'F'},
  {'4', '5', '6', 'E'},
  {'7', '8', '9', 'D'},
  {'A', '0', 'B', 'C'}
};
byte rowPins[ROWS] = {ROW0, ROW1, ROW2, ROW3};      //connect to the row pinouts of the keypad
byte colPins[COLS] = {COL0, COL1, COL2, COL3}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define relay_pin D3

#elif defined (project4)
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>

#define chipSelectPin D8
//Clavier 4x4 pins:
const int ROW0 = D6;
const int ROW1 = D7;
const int ROW2 = PA1;
const int ROW3 = PA0;
const int COL0 = D2;
const int COL1 = D3;
const int COL2 = D4;
const int COL3 = D5;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'F'},
  {'4', '5', '6', 'E'},
  {'7', '8', '9', 'D'},
  {'A', '0', 'B', 'C'}
};
byte rowPins[ROWS] = {ROW0, ROW1, ROW2, ROW3};      //connect to the row pinouts of the keypad
byte colPins[COLS] = {COL0, COL1, COL2, COL3}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#elif defined (project5)
#include <LiquidCrystal.h>
#include <OnewireKeypad.h>
#include <Servo.h>

//LCD interface
const int pinRS     = D2;
const int pinEnable = D3;
const int pinD4     = D4;
const int pinD5     = D5;
const int pinD6     = D6;
const int pinD7     = D7;

LiquidCrystal lcd(pinRS, pinEnable, pinD4, pinD5, pinD6, pinD7);

char KEYS[] = {
  '1', '2', '3', 'F',
  '4', '5', '6', 'E',
  '7', '8', '9', 'D',
  'A', '0', 'B', 'C'
};

OnewireKeypad <Print, 16 > KP(Serial, KEYS, 4, 4, A0, 3200,1000, 11358);// 1000, 4700, 12000 ::1200,4700, 10000 ::1200,4700, 15000

#elif defined (project6)
#include <Keypad.h>
#include <Servo.h>

HardwareSerial Serial1(USART1);
#define LCD_NHD_Serial Serial1

//Clavier 4x4 pins:
const int ROW0 = D6;
const int ROW1 = D7;
const int ROW2 = D10;
const int ROW3 = D11;
const int COL0 = D5;
const int COL1 = D4;
const int COL2 = D3;
const int COL3 = D2;

char keys[4][4] = {
  {'1', '2', '3', 'F'},
  {'4', '5', '6', 'E'},
  {'7', '8', '9', 'D'},
  {'A', '0', 'B', 'C'}
};

byte rowPins[4] = {ROW0, ROW1, ROW2, ROW3};      //connect to the row pinouts of the keypad
byte colPins[4] = {COL0, COL1, COL2, COL3}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, 4, 4 );

#endif
#if defined (project2) || defined (project5)

#if defined (project2)
uint16_t comp_value[17]={118,170,220,272,320,370,422,475,526,578,629,686,741,801,862,897,1024};
#elif defined (project5)
uint16_t comp_value[17]={80,130,188,244,285,335,390,439,495,549,605,659,720,780,845,998,1024};
#endif

const char BUTTONS[16] = {'1', '4', '7', 'A', '2', '5', '8', '0', '3', '6', '9', 'B','F','E','D','C'};

#define Keypad_analog_input A1

int Read_search(void)
{
  int adc_val;
  int index=16;
  
  adc_val = analogRead(Keypad_analog_input); // read again the analog input
      
  for (int i =0; i < 16 ; i++)
  {
    if( adc_val > comp_value[i]  && adc_val < comp_value[i+1] )
    { 
      index = i;  
      break;
    }   
  }
  return index;

}

char getKey(void)
{
 int index, new_index;
 char key;
 index = Read_search();
  if (index == 16) return 0;// non push button is detected
     else
     {
      delay(50);// wait for end of rebond
      new_index=Read_search();
      if (new_index == 16) return 0;// error rebond!!!
         else
        {
          if (new_index == index)// the first sample is confirmed by the second sample
          {
            while ( analogRead(Keypad_analog_input) > comp_value[0]); // read again the analog input is low that the first threshoold
            return BUTTONS[index];
          }    
          else
          return 0;
        }      
     }
}
#endif

void LCD_write_byte(byte x)
{
#if defined (project3)

Wire.beginTransmission(LCD_I2C);
Wire.write(x);  
Wire.write(0); 
Serial.println(Wire.endTransmission());
#elif defined (project4)
digitalWrite(chipSelectPin, LOW);
SPI.transfer(x); 
// take the chip select high to de-select:
digitalWrite(chipSelectPin, HIGH);
delay(1);
#elif defined (project2) || defined (project6)
LCD_NHD_Serial.write(x);
#endif 
char motdepasse[]="0909";
int i=0,count=0,number=sizeof(motdepasse)-1;
char pass[sizeof(motdepasse)-1];
}

void LCD_NHD_Clear(void)
{
LCD_write_byte(0xFE);
LCD_write_byte(0x51);
delay(3);
}

void LCD_NHD_write_String(String x, uint8_t length)
{
for (int i=0; i < length; i++)
LCD_write_byte(x[i]);
}

void LCD_NHD_setCursor(uint8_t col, uint8_t row)
{
  uint8_t offset = 0;
  if (row == 1 ) offset = 0x40; 
  LCD_write_byte(0xFE);
  LCD_write_byte(0x45);
  LCD_write_byte(col + offset);
}

#if defined (project4)||defined (project5)||defined (project6)
Servo servo_barriere ;
#endif

void setup() {

// put your setup code here, to run once:
Serial.begin(9600);
Serial.println("Test");
#if defined (project1) || defined (project2) || defined (project3)
pinMode(relay_pin,OUTPUT);
digitalWrite(relay_pin,HIGH);
#endif

#if defined (project4)||defined (project5)||defined (project6)
servo_barriere.attach(PC7); 
for(int angle = 0; angle <= 90; angle ++) 	 // command to move from 0 degrees to 180 degrees 
           {                                  
              servo_barriere.write(angle);              	 //command to rotate the servo to the specified angle
               delay(15);   
           }  
            delay(1000);
  
             for(int angle =90; angle>=0; angle--)     // command to move from 180 degrees to 0 degrees 
           {                                
             servo_barriere.write(angle);              //command to rotate the servo to the specified angle
             delay(15);                       
           } 
#endif

#if defined (project1)
lcd.begin(16, 2);
lcd.setCursor(0, 0);
lcd.print("TP SEA/SEE");
lcd.setCursor(0,1);
lcd.print("TIC 3");

Wire.begin (); // Call the connection Wire
keypad.begin (makeKeymap (keys));

#elif defined (project2)

LCD_NHD_Serial.begin(9600);
delay(100);
LCD_NHD_Clear();
String Txt="saisir code";
LCD_NHD_write_String(Txt, Txt.length());
LCD_NHD_setCursor(0,1);
Txt="    :";
LCD_NHD_write_String(Txt, Txt.length());
#elif defined (project3)
Wire.begin();
delay(100);
LCD_NHD_Clear();
String Txt="TP SEA/SEE";
LCD_NHD_write_String(Txt, Txt.length());
LCD_NHD_setCursor(0,1);
Txt="TIC 3";
LCD_NHD_write_String(Txt, Txt.length());

#elif defined (project4)

pinMode(chipSelectPin,OUTPUT);
SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));
LCD_NHD_Clear();
String Txt="TP SEA/SEE";
LCD_NHD_write_String(Txt, Txt.length());
LCD_NHD_setCursor(0,1);
Txt="TIC 3";
LCD_NHD_write_String(Txt, Txt.length());
#elif defined (project5)
lcd.begin(16, 2);
lcd.setCursor(0, 0);
lcd.print("TP SEA/SEE");
lcd.setCursor(0,1);
lcd.print("TIC 3");

#elif defined (project6)
LCD_NHD_Serial.begin(9600);
delay(100);
#endif
}

void loop() {
  char Key;
#if defined (project1)
Key = keypad.getKey();
 if (Key != NO_KEY){
    Serial.println(Key); 
  }
#elif defined (project2)

  Key = getKey();

  if (Key) {
    Serial.println(Key);
   LCD_write_byte(Key);
  }
  
}
#elif defined (project3)
Key = keypad.getKey();
 if (Key != NO_KEY){
    Serial.println(Key);
  }
#elif defined (project4)
Key = keypad.getKey();
 if (Key != NO_KEY){
    Serial.println(Key);
  }
#elif defined (project5)
Key = getKey();
  if (Key) {
    Serial.println(Key);
  }
#elif defined (project6)
Key = keypad.getKey();
 if (Key != NO_KEY){
    Serial.println(Key);
  }
#endif


