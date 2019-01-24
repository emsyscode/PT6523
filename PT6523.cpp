/*Begining of Auto generated code by Atmel studio */
/*
This code is not clean and not perfect, this is only 
a reference to extract ideas and adapte to your solution.
*/
void send_char(unsigned char a);
void send_data(unsigned char a);
void send_update_clock(void);
void DigitTo7SegEncoder(unsigned char digit);
void update_clock(void );

#define LCD_in 7  // This is the pin number 7 on Arduino UNO
#define LCD_clk 8 // This is the pin number 8 on Arduino UNO
#define LCD_stb 9 // This is the pin number 9 on Arduino UNO
#define LCD_latch 10 // This is the pin number 10 on Arduino UNO

/*
#define BIN(x) \
( ((0x##x##L & 0x00000001L) ? 0x01 : 0) \
| ((0x##x##L & 0x00000010L) ? 0x02 : 0) \
| ((0x##x##L & 0x00000100L) ? 0x04 : 0) \
| ((0x##x##L & 0x00001000L) ? 0x08 : 0) \
| ((0x##x##L & 0x00010000L) ? 0x10 : 0) \
| ((0x##x##L & 0x00100000L) ? 0x20 : 0) \
| ((0x##x##L & 0x01000000L) ? 0x40 : 0) \
| ((0x##x##L & 0x10000000L) ? 0x80 : 0))
*/

//ATT: On the Uno and other ATMEGA based boards, unsigned ints (unsigned integers) are the same as ints in that they store a 2 byte value.
//Long variables are extended size variables for number storage, and store 32 bits (4 bytes), from -2,147,483,648 to 2,147,483,647.

//*************************************************//
void setup() {
  pinMode(LCD_clk, OUTPUT);
  pinMode(LCD_in, OUTPUT);
  pinMode(LCD_stb, OUTPUT);
  pinMode(LCD_latch, OUTPUT);
  
  pinMode(13, OUTPUT);
  
  Serial.begin(9600);
  /*CS12  CS11 CS10 DESCRIPTION
  0        0     0  Timer/Counter1 Disabled 
  0        0     1  No Prescaling
  0        1     0  Clock / 8
  0        1     1  Clock / 64
  1        0     0  Clock / 256
  1        0     1  Clock / 1024
  1        1     0  External clock source on T1 pin, Clock on Falling edge
  1        1     1  External clock source on T1 pin, Clock on rising edge
 */
  
// Note: this counts is done to a Arduino 1 with Atmega 328... Is possible you need adjust
// a little the value 62499 upper or lower if the clock have a delay or advnce on hours.

  digitalWrite(LCD_stb, LOW);
  delayMicroseconds(5);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
 
}
void send_char(unsigned char a)
{
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  data=a;
  // the validation of data happen when clk go from LOW to HIGH.
  // This lines is because the clk have one advance in data, see datasheet of sn74HC595
  // case don't have this signal instead of "." will se "g"
  digitalWrite(LCD_stb, LOW); // When strobe is low, all output is enable. If high, all output will be set to low.
  delayMicroseconds(5);
  digitalWrite(LCD_clk,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
  digitalWrite(LCD_clk,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(LCD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(LCD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(LCD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
    //
    digitalWrite(LCD_stb, HIGH); // When strobe is low, all output is enable. If high, all output will be set to low.
  delayMicroseconds(5);
  }
}
// I h've created 3 functions to send bit's, one with strobe, other without strobe and one with first byte with strobe followed by remaing bits.
void send_char_without(unsigned char a)
{
 //
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  data=a;
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
  digitalWrite(LCD_clk, LOW);
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(LCD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(LCD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(LCD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
  }
}
void send_char_8bit_stb(unsigned char a)
{
 //
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
 int i = -1;
  data=a;
  // This lines is because the clk have one advance in data, see datasheet of sn74HC595
  // case don't have this signal instead of "." will se "g"
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
   i++;
   digitalWrite(LCD_clk, LOW);
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(LCD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(LCD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(LCD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(1);
    if (i==7){
    Serial.println(i);
    digitalWrite(LCD_stb, HIGH);
    delayMicroseconds(2);
    }
     
  }
}
void patternLCD(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B01010011);  send_char_without(0B10110100); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000111);  send_char_without(0B11111001); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000010);  send_char_without(0B10101110); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00100111);  send_char_without(0B11100110); // 49:56  -57:64  
send_char_without(0B11110001);  send_char_without(0B10011111); // 65:72  -73:80// skip 78    
send_char_without(0B01110011);  send_char_without(0B10111101); // 81:88  -89:96// skip 85
send_char_without(0B11001100);  send_char_without(0B11110110); // 97:104-105:112
send_char_without(0B11101101);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00000000);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void allON(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111); 
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B00001111);
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void allOFF(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000); 
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000); 
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B01100000);
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void loop() {
  for (int i=0; i<1000; i++){
  patternLCD();
  }
  for (int i=0; i<1000; i++){
    allON(); // All on
  }
  //
  for (int i=0; i<1000; i++){
    allOFF(); // All off
  }
  // 
}

