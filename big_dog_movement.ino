// Include the Servo library 
#include <Servo.h> 
#include "IRremote.h"
#include <LiquidCrystal.h>

const int rs = 2, 
          en = 3, 
          d4 = 6, 
          d5 = 7, 
          d6 = 8, 
          d7 = 9;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/*-----( Global Constants )-----*/
const int receiver = 11;      // Signal Pin of IR receiver to Arduino Digital Pin 11
const int analogInPin = A0;  // Analog input pin for voltage measurement
float voltage[150];        // Variable to store the voltage value
float maxVoltage = 0.0;
int index = 0;
float percent = 0;
String state = "FUNC/STOP";
int intState = 0;
char buffer[5];
bool check = false;
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

String signal = "";

/*-----( Function )-----*/
void translateIR() {          // takes action based on IR code received
// describing Remote IR codes 
  //Serial.println(irrecv.decodedIRData.decodedRawData, HEX);  // Add this line
  switch(irrecv.decodedIRData.decodedRawData){
    case 0xBA45FF00: signal = "POWER"; break;
    case 0xB847FF00: signal = "FUNC/STOP"; break;
    case 0xB946FF00: signal = "VOL+"; break;
    case 0xBB44FF00: signal ="FAST BACK";    break;
    case 0xBF40FF00: signal ="PAUSE";    break;
    case 0xBC43FF00: signal ="FAST FORWARD";   break;
    case 0xF807FF00: signal ="DOWN";    break;
    case 0xEA15FF00: signal ="VOL-";    break;
    case 0xF609FF00: signal ="UP";    break;
    case 0xE619FF00: signal ="EQ";    break;
    case 0xF20DFF00: signal ="ST/REPT";    break;
    case 0xE916FF00: signal ="0";    break;
    case 0xF30CFF00: signal ="1";    break;
    case 0xE718FF00: signal ="2";    break;
    case 0xA15EFF00: signal ="3";    break;
    case 0xF708FF00: signal ="4";    break;
    case 0xE31CFF00: signal ="5";    break;
    case 0xA55AFF00: signal ="6";    break;
    case 0xBD42FF00: signal ="7";    break;
    case 0xAD52FF00: signal ="8";    break;
    case 0xB54AFF00: signal ="9";    break;
    case 0xFFFFFFFF: signal =" REPEAT";break;  

  }// End Case

} //END translateIR

// Create a servo objects

Servo Servo11; 
Servo Servo12; 
Servo Servo13; 
Servo Servo21; 
Servo Servo22; 
Servo Servo23; 
Servo Servo31; 
Servo Servo32; 
Servo Servo33; 
Servo Servo41; 
Servo Servo42; 
Servo Servo43; 
Servo Servo51; 
Servo Servo52; 
Servo Servo53; 
Servo Servo61; 
Servo Servo62; 
Servo Servo63; 

const int trigPin = 13;
const int echoPin = 12;

float duration, distance;
void setup() { 
   // We need to attach the servo to the used pin number  
   Servo11.attach(22);
   Servo12.attach(23);
   Servo13.attach(24);   
   Servo21.attach(25);
   Servo22.attach(26);
   Servo23.attach(27); 
   Servo31.attach(28);
   Servo32.attach(29);
   Servo33.attach(30); 
   Servo41.attach(31);
   Servo42.attach(32);
   Servo43.attach(33); 
   Servo51.attach(34);
   Servo52.attach(35);
   Servo53.attach(36); 
   Servo61.attach(37);
   Servo62.attach(38);
   Servo63.attach(39); 
  irrecv.enableIRIn();  
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop(){
  if (check == false) {
    signal = "";
  }
  decodeReceiver();
  controlsig();
  measureVoltage();
  
  
}

void measureVoltage() {
  //voltage read is implemented with percent so we just need to figure out lcd soldering and coding - probably on youtube
  int sensorValue = analogRead(analogInPin);  // Read the analog input value (0-1023)
  // Convert the analog reading to voltage (0-5V range)
  voltage[index] = sensorValue * (6.0 / 1023.0);
  index = index + 1;
  delay(10);
  if (index == 149) {
    //Serial.print("voltage: ");
    //Serial.println(maxVoltage);
    float sum = 0.0;
    for (int i = 0; i < (sizeof(voltage) / sizeof(voltage[0])); i++) {
      sum = sum + voltage[i];
    }
    maxVoltage = sum/150;
    if (maxVoltage < 5.0) {
      percent = 0.0;
    } else {
      percent = (maxVoltage - 5.0) * 100;
    }
    //Serial.println(percent);
    index = 0;
    dtostrf(percent, 5, 2, buffer);
    lcd.print(buffer);
    lcd.print("%  Albert  ");
    lcd.print(buffer);
    lcd.print("%");
    lcd.print("  Albert  ");
  } 
  
}



void controlsig() {
  switch (intState) {
    case -9:
      //standing up
      stand();
      break;

    case -11:
      //stop so sit down
      sit();
      break;

    case -10:
      forward();
      distanceRead();
      break;

    case -4:
      backward();
      break;

    case -6:
      /*
      Rotate counterclockwise one motion at a time
      */
      break;

    case -8:
      /*
      Rotate clockwise one motion at a time
      */
      break;

    case 1:
      motor1up();
      delay(200);
      motor1down();
      break;

    case 2:
      motor2up();
      delay(200);
      motor2down();
      break;

    case 3:
      motor3up();
      delay(200);
      motor3down();
      break;

    case 4:
      motor4up();
      delay(200);
      motor4down();
      break;

    case 5:
      motor5up();
      delay(200);
      motor5down();
      break;

    case 6:
      motor6up();
      delay(200);
      motor6down();
      break;
    
    case 7:
      /*autonomous drive mode*/
      distanceRead();
      break;
  }
}

void distanceRead() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  if (distance < 50 && intState == -10) {
    signal = "POWER";
    state = signal;
    intState = -9;

  }
  delay(50);
}

void checkdecode() {
  if (irrecv.decode() && check == false) {
    translateIR(); 
    delay(100);
    irrecv.resume();            // receive the next value
    check = true;
  } else {
    delay(100);
  }
}

void backward() {
  Servo12.write(90);
  Servo42.write(90);
  Servo52.write(90);
  checkdecode();
  Servo13.write(90);
  Servo43.write(90);
  Servo53.write(90);
  Servo23.write(110);
  Servo33.write(70);
  Servo63.write(110);
  checkdecode();
  Servo22.write(100);
  Servo32.write(70);
  Servo62.write(110);
  checkdecode();
  Servo11.write(130);
  Servo41.write(100);
  Servo51.write(60);
  Servo21.write(85);
  Servo31.write(65);
  Servo61.write(140);
  checkdecode();
  Servo22.write(90);
  Servo32.write(90);
  Servo62.write(90);
  checkdecode();
  Servo23.write(90);
  Servo33.write(90);
  Servo63.write(90);
  Servo13.write(70);
  Servo43.write(110);
  Servo53.write(70);
  checkdecode();
  Servo12.write(70);
  Servo42.write(110);
  Servo52.write(70);
  checkdecode();
  Servo11.write(105);
  Servo41.write(125);
  Servo51.write(40);
  Servo21.write(60);
  Servo31.write(90);
  Servo61.write(120);
  checkdecode();

}

void forward() {
  Servo12.write(90);
  Servo42.write(90);
  Servo52.write(90);
  checkdecode();
  Servo13.write(90);
  Servo43.write(90);
  Servo53.write(90);
  Servo23.write(110);
  Servo33.write(70);
  Servo63.write(110);
  checkdecode();
  Servo22.write(100);
  Servo32.write(70);
  Servo62.write(110);
  checkdecode();
  Servo11.write(105);
  Servo41.write(125);
  Servo51.write(40);
  Servo21.write(60);
  Servo31.write(90);
  Servo61.write(120);
  checkdecode();
  Servo22.write(90);
  Servo32.write(90);
  Servo62.write(90);
  checkdecode();
  Servo23.write(90);
  Servo33.write(90);
  Servo63.write(90);
  Servo13.write(70);
  Servo43.write(110);
  Servo53.write(70);
  checkdecode();
  Servo12.write(70);
  Servo42.write(110);
  Servo52.write(70);
  checkdecode();
  Servo11.write(130);
  Servo41.write(100);
  Servo51.write(60);
  Servo21.write(85);
  Servo31.write(65);
  Servo61.write(140);
  checkdecode();
  
}
void sit() {
  Servo12.write(90);
  Servo22.write(90);
  Servo32.write(90);
  Servo42.write(90);
  Servo52.write(90);
  Servo62.write(90);
  checkdecode();
  delay(50);
  Servo11.write(130);
  Servo21.write(60);
  Servo31.write(90);
  Servo41.write(100);
  Servo51.write(60);
  Servo61.write(120);
  Servo13.write(90);
  Servo23.write(90);
  Servo33.write(90);
  Servo43.write(90);
  Servo53.write(90);
  Servo63.write(90);
}

void stand() {
  Servo13.write(70);
  Servo23.write(110);
  Servo33.write(70);
  Servo43.write(110);
  Servo53.write(70);
  Servo63.write(110);
  checkdecode();
  delay(50);
  Servo12.write(70);
  Servo22.write(100);
  Servo32.write(70);
  Servo42.write(110);
  Servo52.write(70);
  Servo62.write(110);

}
void decodeReceiver() {
  if (irrecv.decode() || check == true) {
    if (check != true) {
      translateIR(); 
      delay(300);                 // Do not get immediate repeat
      irrecv.resume();            // receive the next value
    }
    check = false;
    state = signal;
    if (state == "POWER") {
      //standing up
      intState = -9;
    } else if (state == "FUNC/STOP") {
      //stop so sit down
      intState = -11;

    } else if (state == "VOL+") {
      /*
      Forward Directon
      */
      intState = -10; 

    } else if (state == "VOL-") {
      /*
      Backward Direction
      */
      intState = -4; 
    } else if (state == "FAST BACK") {
      /*
      Rotate counterclockwise one motion at a time
      */
      intState = -6; 
    } else if (state == "FAST FORWARD") {
      /*
      Rotate clockwise one motion at a time
      */
      intState = -8; 
    } else if (state == "1") {
      intState = 1; 

    } else if (state == "2") {
      intState = 2;
    } else if (state == "3") {
      intState = 3;
    } else if (state == "4") {
      intState = 4;
    } else if (state == "5") {
      intState = 5;
    } else if (state == "6") {
      intState = 6;
    } else if (state == "7") {
      intState = 7; /*autonomous drive mode*/
    }
    if (state == "FAST FORWARD" || state == "FAST BACK") {
      state = "";
      /*
      state is now a clean slate
      */
      intState = 0;
    }
  
  }
  
}
//motor 1 down up
void motor1down() {
  Servo13.write(70);
  delay(250);
  Servo12.write(70);  
  delay(200);
}
void motor1up() {
  Servo12.write(90);
  delay(200);
  Servo13.write(90);
  delay(200);
}
//motor 2 down up
void motor2down() {
  Servo23.write(110);
  delay(250);
  Servo22.write(100);  
  delay(200);
}
void motor2up() {
  Servo22.write(90);
  delay(200);
  Servo23.write(90);
  delay(200);
}
//motor 3 down up
void motor3down() {
  Servo33.write(70);
  delay(250);
  Servo32.write(70);  
  delay(200);
}
void motor3up() {
  Servo32.write(90);
  delay(200);
  Servo33.write(90);
  delay(200);
}
//motor 4 down up
void motor4down() {
  Servo43.write(110);
  delay(250);
  Servo42.write(110);  
  delay(200);
}
void motor4up() {
  Servo42.write(90);
  delay(200);
  Servo43.write(90);
  delay(200);
}
//motor 5 down up
void motor5down() {
  Servo53.write(70);
  delay(250);
  Servo52.write(70);  
  delay(200);
}
void motor5up() {
  Servo52.write(90);
  delay(200);
  Servo53.write(90);
  delay(200);
}
void motor6down() {
  Servo63.write(110);
  delay(250);
  Servo62.write(110);  
  delay(200);
}
void motor6up() {
  Servo62.write(90);
  delay(200);
  Servo63.write(90);
  delay(200);
}
void motor1forward() {
  motor1up();
  delay(600);
  Servo11.write(70);
  delay(400);
  motor1down();
}
void motor1backward() {
  motor1up();
  delay(600);
  Servo11.write(130);
  delay(400);
  motor1down();
}

