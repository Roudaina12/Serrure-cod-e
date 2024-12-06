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

int Read_search(uint32_t Keypad_analog_input)
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
char getKey(uint32_t Keypad_analog_input)
{
  static uint8_t index0=16,index1=16;// static to be able non volatile from call to call
  uint8_t index2;
 
  index2 = Read_search(Keypad_analog_input);// in which intervall we are?

 if((index0 == 16) && (index1 < 16 ) && (index2 < 16) && (index1 == index2))// three samples the last two identical <16 and the first one is 16
   {
    index0=index1;//shifting index1 to index0
    index1=index2;//shiting index2 to index1
    return BUTTONS[index2];// button code returned
   }
   else // juste do shifting and return 0
   {
    index0=index1;
    index1=index2;
    return 0;
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

char Key;

void Update_IT_callback(void) // update frequency is 20 Hz
{ 
//HAL_ResumeTick();

Key = getKey(Keypad_analog_input);
  if (Key) {
    Serial.println(Key);
  }
//HAL_SuspendTick();
}

void Init_Start_Timer(TIM_TypeDef *Instance, uint32_t intervall, TimerFormat_t format)
{
  HardwareTimer *MyTim = new HardwareTimer(Instance);// declare object MyTim
 
  MyTim->setOverflow(intervall, format); 
  MyTim->attachInterrupt(Update_IT_callback);//configure the function Update_IT_callback as an interrupt for the Update event
  MyTim->resume();// start Timer
}

void setup() {
// put your setup code here, to run once:
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
  void Update_IT_callback(void) // update frequency is 20 Hz

HAL_ResumeTick();
switch(etat){
case 0 : 
Key = getKey(A1);
  if ( Key ) {  
            switch (Key){
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    code_readn[counter]=Key;
    Serial.println(Key); 
    LCD_write_byte('*');
    if (++counter == 4)
       {
       counter=0; 
        etat=1;
       }

}

break;
case 1 : 

if(++Compteur_Etat == 2){
    Compteur_Etat=0; // reset counter
    // checking code
  for (int i=0; i < 4; i++)
{
    if (code_readn[i]== password[i]) j++;
}
// clear LCD
LCD_NHD_Clear();
LCD_NHD_setCursor(0, 0);
// test for correct code
if (j == 4) {
    //correct code
    digitalWrite(relay_pin,HIGH);
    Serial.println("Correct Code");
    Txt="Correct Code";
    LCD_NHD_write_String(Txt, Txt.length());
   
}else
{
//false code
Serial.println("Incorrect Code");
Txt="Incorrect Code";
LCD_NHD_write_String(Txt, Txt.length());
}
etat=3;
j=0;
}

break;
case 2 : 
 if(++Compteur_Etat == 10 ){
    Compteur_Etat=0;
    digitalWrite(relay_pin,LOW);
etat=3;
}

break;
case 3 : 
if(++Compteur_Etat == 60 ){
    Compteur_Etat=0;
LCD_NHD_Clear();
LCD_NHD_setCursor(0,0);
String Txt="Enter Code:";
LCD_NHD_write_String(Txt, Txt.length());
LCD_NHD_setCursor(0,1);
etat=0;
}
break;
  
}

   
    code_readn[counter]=Key;
    Serial.println(Key); 
    LCD_write_byte('*');
    ++counter;
    break;
    default:
    break;
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

//loop function
void loop() {
//HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

}