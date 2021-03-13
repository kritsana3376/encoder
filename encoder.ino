// PID control REV8.8 **TEST can Reviece RN from webserver and Encoder 
/*
     ---- PIN FOR CONTROL Timer ---
    TIMER1_OVF_vect   Pin XX
    TIMER3_OVF_vect   Pin XX

    ---- PIN FOR CONTROL MOTOR ---
    PWM               PWMPin(D10)
    IN1               D5
    IN2               D6

   ---- PIN FOR CONTROL ENCODER (SPI INTERFACE) ---
     MOSI (7)  -------------------   SDO (D51)//51 or ICSP-4
     MISO (6)  -------------------   SDI (D50)//50 or ICSP-1
     SCK  (5)  -------------------   SCK (D52)//52 or ICSP-3
     SS1    -------------------      SS1 (D53)
     GND    -------------------      GND
     VDD    -------------------   VCC (5.0V)
    ---- PIN FOR ETHERNET (SPI INTERFACE) ---
    
            Enc28j60          Arduino Mega 2560
              
              GND               GND
              5.0               5V
              SO                Pin (D50)
              SI                Pin (D51)
              SCK               Pin (D52)
              CS                Pin (D48)

*/

// PID control REV4.0
/*
  En  = Eror
  Cn  = Collect_INPUT
  Rn  = SETPOINT
  Mn  = PID_Calculate
  Mn1 = PID_Calculate_Prev
  K0  = coefficient
  K1  = coefficient
  K2  = coefficient
  T   = Sample Period
*/
#include <SPI.h>

double   T, Cn, En, Mn, Mn1, En1, En2;
double   K0 = 0;
double   K1 = 0;
double   K2 = 0;
double timer2_counter , timer1_counter;
double   Rn = 0;


//---**********------------------------------------------------------------------------------

//--- Encoder FUNCTION ----------------------------------------------------------------------
const int slaveSelectEnc1 = 15; //// Change pin encoder for 32 //from 49
//GPIO15 for SS2-ESP32
//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

const int slaveSelectEnthernet1 = 10; //10
signed long encoder1count = 0;
//---******************************************************************--------------------------

//---Test------------------------------------------------------------------------------------
#define ledPin1   12
#define ledPin2   13
//boolean Count = LOW;
double  Text, Test_DIR , PWM_1;
char c[10] = " " ;
int i;

//---******************************************************************----------------------

//--Watch DOG------------------------------------------------------------------------------------
#include <Arduino.h>
//---******************************************************************----------------------

//timer init esp 32 ***************************************************----------------------
hw_timer_t *timer1 = NULL;
hw_timer_t *timer2 = NULL;

//---******************************************************************----------------------

long readEncoder() {

  // Initialize temporary variables for SPI read
  unsigned int count_1, count_2, count_3, count_4;
  long count_value;

  digitalWrite(slaveSelectEnc1, LOW);     // Begin SPI conversation
  SPI.transfer(0x60);                     // Request count , Read to CNTR
  count_1 = SPI.transfer(0x00);           // Read highest order byte
  count_2 = SPI.transfer(0x00);
  count_3 = SPI.transfer(0x00);
  count_4 = SPI.transfer(0x00);           // Read lowest order byte
  digitalWrite(slaveSelectEnc1, HIGH);    // Terminate SPI conversation



  // Calculate encoder count
  count_value = (count_1 << 8) + count_2;
  count_value = (count_value << 8) + count_3;
  count_value = (count_value << 8) + count_4;

  return count_value;  
}

void clearEncoder() {

  // Set encoder1's data register to 0
  digitalWrite(slaveSelectEnc1, LOW);     // Begin SPI conversation
  // Write to DTR
  SPI.transfer(0x98);
  // Load data
  SPI.transfer(0x00);  // Highest order byte
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);  // lowest order byte
  digitalWrite(slaveSelectEnc1, HIGH);    // Terminate SPI conversation

  delayMicroseconds(100);  // provides some breathing room between SPI conversations

  // Set encoder1's current data register to center
  digitalWrite(slaveSelectEnc1, LOW);     // Begin SPI conversation
  SPI.transfer(0xE0);                     //Load value from DTR to CNTR
  digitalWrite(slaveSelectEnc1, HIGH);    // Terminate SPI conversation 

}

void initEncoder() {

  // Set slave selects as outputs
  pinMode(slaveSelectEnc1, OUTPUT);
  // Raise select pins
  // Communication begins when you drop the individual select signsl
  digitalWrite(slaveSelectEnc1, HIGH);
  SPI.begin();
  //Initialize encoder 1
  //Clock division factor:0
  //Negative index input
  //free-running count mode
  //x4 quatrature count mode (four counts per quadrature cycle)
  // NOTE: For more information on commands, see datasheet
  digitalWrite(slaveSelectEnc1, LOW);       // Begin SPI conversation
  SPI.transfer(0x88);                       // Write to MDR0            //0x88 Check IR Register
  SPI.transfer(0x03);                       // Configure to 4 byte mode //0x03
  digitalWrite(slaveSelectEnc1, HIGH);      // Terminate SPI conversation


 
}


//---******************************************************************----------------------

void IRAM_ATTR Timer1interrupt(){
  Cn = readEncoder();
  i++;
}

void IRAM_ATTR Timer2interrupt(){
  digitalWrite(ledPin2,!digitalRead(ledPin2));
  Serial.println("Encoder : " + String(Cn) ); 
}

//---******************************************************************----------------------
void setup() {
 
  Serial.begin(115200);
  pinMode(slaveSelectEnc1,OUTPUT);
  digitalWrite(slaveSelectEnc1,HIGH);

    
  initEncoder();
  clearEncoder();

  //Read value from IC 
  timer1 = timerBegin(1, 80, true);
  timerAttachInterrupt(timer1, &Timer1interrupt, true);
  timerAlarmWrite(timer1, 50000, true); // 30000 microsec or 0.03 sec

  //Show value to monitor 0.5 sec
  timer2 = timerBegin(3, 80, true); // timer 3 for 80 MHz
  timerAttachInterrupt(timer2, &Timer2interrupt, true); // call function Timer2interrupt
  timerAlarmWrite(timer2, 500000, true); // 500000 micro sec or 0.5 sec

  timerAlarmEnable(timer1);
  timerAlarmEnable(timer2);
  

}

void loop() {


}
