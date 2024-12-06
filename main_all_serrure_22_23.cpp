#include <Arduino.h>

HardwareSerial Serial1(USART1);
#define LCD_NHD_Serial Serial1
#define relay_pin D9
char KEYS[] = {
  '1', '2', '3', 'F',
  '4', '5', '6', 'E',
  '7', '8', '9', 'D',
  'A', '0', 'B', 'C'
};

uint16_t comp_value[17]={118,170,220,272,320,370,422,475,526,578,629,686,741,801,862,897,1024};
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

void LCD_write_byte(byte x)
{

LCD_NHD_Serial.write(x);
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
void setup() {

// put your setup code here, to run once:
Serial.begin(115200);
pinMode(relay_pin,OUTPUT);
digitalWrite(relay_pin,LOW);
Serial.println("Coded Lock Door");

LCD_NHD_Serial.begin(9600);
delay(100);

}

char password[4]={'1','5','9','7'};
char code_readn[4];

void loop() {
  
  uint8_t counter=0;
  uint8_t j=0;
  char Key;


LCD_NHD_Clear();
LCD_NHD_setCursor(0,0);
String Txt="Enter Code:";
LCD_NHD_write_String(Txt, Txt.length());
LCD_NHD_setCursor(0,1);

while(counter < 4)
{
  
  Key = getKey();
  if ( Key ) {  
            switch (Key)
{
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
   
    code_readn[counter]=Key;
    Serial.println(Key); 
    LCD_write_byte('*');
    ++counter;
    break;
    default:
    break;
}         
             }
}
counter= 0;
// code completed  let's check it
delay(100);// this time is add to be able to see the last key displayed before clearing the LCD
Serial.println("checking code");
for (int i=0; i < 4; i++)
{
    if (code_readn[i] == password[i]) j++;
}

LCD_NHD_Clear();
LCD_NHD_setCursor(0, 0);
if (j == 4) {
    //correct code
    digitalWrite(relay_pin,HIGH);
    Serial.println("Correct Code");
    Txt="Correct Code";
    LCD_NHD_write_String(Txt, Txt.length());
    delay(500);
    digitalWrite(relay_pin,LOW);
    delay(3000);  
}else
{
//false code
Serial.println("Incorrect Code");
Txt="Incorrect Code";
LCD_NHD_write_String(Txt, Txt.length());
delay(3000);
}
j=0;
}
