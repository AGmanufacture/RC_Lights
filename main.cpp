/* do zrobienia:
- aktywacja swiateł
- włączanie tylko pozycyjnych i mijania
- włączenie świateł drogowych
- włączenie kierunkowskazów
- włączenie świateł awaryjnych
- włączenie światła cofania
- włączenie swiateł policyjnych
- włączenie świateł hamowania
- syrena policyjna z buzzera

*/



#include <Arduino.h>

#define DriveLight 11
#define BlinkerLeft 10
#define BlinkerRight 9
#define StopLight 5
#define PoliceRed 8
#define PoliceBlue 7
#define ReverseLight 6
#define ButtonLeft 2
#define ButtonRight 3
#define JoystickButton 4 
#define AxisX A5
#define AxisY A4

#define ON true
#define OFF false

bool Lights_ONOFF = OFF;
bool Drive_Lights, Long_Lights, Left_Turn, Right_Turn, Emergency_Lights, Stop,  Police;
bool  Break, Reverse, Neutral;
int odczytX;
int odczytY;
int LED_STATE = LOW;
int DriveBrightness = 20;
int DriveBrightnessMax = 255;
byte StopBrightness = 20;
byte StopBrightnessMax = 255;
byte LightMode, ModeCounter = 0;
unsigned long ButtonLeftMicros;
//kierunkowskazy
int wypelnienie, wypelnienieLeft, wypelnienieRight = 0;
int iloscWypelnienia = 5;
unsigned long czasPoprzedniegoWypelnienia = 0;
unsigned long czasDoWypelnienia = 10;
//kierunkowskazy
int ButtonLeftPress, ButtonRightPress, ButtonJoystickPress;
bool ButtonLeftStatus, ButtonRightStatus, ButtonJoystickStatus = false;


void setup() {

for (int i = 5; i <= 11; i++)
pinMode(i,OUTPUT);
pinMode(ButtonLeft, INPUT_PULLUP);
pinMode(ButtonRight, INPUT_PULLUP);
pinMode(JoystickButton, INPUT_PULLUP);
Serial.begin(9600); 
/* Odczyt osi
neutral X - 527
neutral Y > 490
przód X - 1023 a y 940
tył X - 0 a y 127
Lewo Y - < 440
prawo Y - > 525
*/

}

void loop() {
// odczytX = analogRead(AxisX);
// odczytY = analogRead(AxisY);
           

/* Serial.print("odczyt X: \t");
Serial.print(odczytX);
Serial.print("\t");
Serial.print("odczyt Y: \t");
Serial.println(odczytY);
delay(500); */

    // włączanie lewego przycisku - Światła ON/OFF
    ButtonLeftPress = digitalRead(ButtonLeft);
    if (ButtonLeftPress == LOW) {
        if (ButtonLeftStatus == false) {
            Lights_ONOFF = !Lights_ONOFF;
            ButtonLeftStatus = true;
        }
    }
    else { ButtonLeftStatus = false;}
     // Serial.println(Lights_ONOFF);
     // Serial.println(LightMode);
    // włączanie lewego przycisku - Światła ON/OFF

    // włączanie świateł awaryjnych
    ButtonJoystickPress = digitalRead(JoystickButton);
    if (ButtonJoystickPress == LOW) {
        if (ButtonJoystickPress == false) {
            Emergency_Lights = !Emergency_Lights;
            ButtonJoystickStatus = true;
        }
    }
    else { ButtonJoystickStatus = false;}
    // włączanie świateł awaryjnych

    // tryb świateł
    ButtonRightPress = digitalRead(ButtonRight);
    if (ButtonRightPress == LOW) {
        if (ButtonRightStatus == false) {
        ModeCounter += 1;
        LightMode = ModeCounter % 3;
        ButtonRightStatus = true;
        }
    }
    else { ButtonRightStatus = false;}

    if (Lights_ONOFF == ON) {
        if (LightMode == 0) {
           Drive_Lights = false;
           Long_Lights = false;
           // Emergency_Lights = false;
           Stop = true;
           Left_Turn = true;
           Right_Turn = false;
           Police = false;
           digitalWrite(PoliceRed, HIGH);
           digitalWrite(PoliceBlue, LOW);
           digitalWrite(ReverseLight, LOW);
        }
        if (LightMode == 1) {
            Drive_Lights = false;
            Long_Lights = false;
            // Emergency_Lights = false;
            Stop = true; 
            Right_Turn = true;
            Left_Turn = false;
            Police = false;
            digitalWrite(PoliceBlue, HIGH);
            digitalWrite(PoliceRed, LOW);
            digitalWrite(ReverseLight, LOW);
         }
        if (LightMode == 2) {
            Drive_Lights = false;
            Long_Lights = false;
            // Emergency_Lights = true;
            Stop = true;
            Right_Turn = false;
            Left_Turn = false;
            Police = true;
            digitalWrite(ReverseLight,HIGH);
            digitalWrite(PoliceBlue, LOW);
            digitalWrite(PoliceRed, LOW);
        }
    }
    else if (Lights_ONOFF == OFF) {
        Drive_Lights = false;
        Long_Lights = false;
        // Emergency_Lights = false;
        Stop = false; 
        Left_Turn = false;
        Right_Turn = false;
        Police = false;
        analogWrite(BlinkerLeft, LOW);
        analogWrite(BlinkerRight, LOW);
        analogWrite(DriveLight, LOW);
        analogWrite(StopLight, LOW);
        digitalWrite(PoliceRed, LOW);
        digitalWrite(PoliceBlue, LOW);
        digitalWrite(ReverseLight, LOW);
        LightMode = 0;
    }
    // tryb świateł
    
    // drive lights
    if (Drive_Lights == true && Long_Lights == false) {
        analogWrite(DriveLight, DriveBrightness);
    } 
    // drive lights

    //  long lights
    else if (Drive_Lights == false && Long_Lights == true) {
        analogWrite(DriveLight, DriveBrightnessMax);
    }
    // long lights
   
    // blinker left
    if (Left_Turn == true && Emergency_Lights == false) {
        if (millis() - czasPoprzedniegoWypelnienia >= czasDoWypelnienia) {
            wypelnienieLeft = wypelnienieLeft + iloscWypelnienia;
            czasPoprzedniegoWypelnienia = millis();
        }  
        analogWrite(BlinkerLeft, wypelnienieLeft);
        digitalWrite(BlinkerRight, LOW);

            if ( wypelnienieLeft >= 255) {
             wypelnienieLeft = 255;
            iloscWypelnienia = -iloscWypelnienia;
            }
            if ( wypelnienieLeft <= 0) {
             wypelnienieLeft = 0;
            iloscWypelnienia = -iloscWypelnienia;
             }  
    }
    // blinker left 

    // blinker right
    if (Right_Turn == true && Emergency_Lights == false) {
        if (millis() - czasPoprzedniegoWypelnienia >= czasDoWypelnienia) {
            wypelnienieRight = wypelnienieRight + iloscWypelnienia;
            czasPoprzedniegoWypelnienia = millis();
        }  
        analogWrite(BlinkerRight, wypelnienieRight);
        digitalWrite(BlinkerLeft, LOW);

            if ( wypelnienieRight >= 255) {
             wypelnienieRight = 255;
            iloscWypelnienia = -iloscWypelnienia;
            }
            if ( wypelnienieRight <= 0) {
             wypelnienieRight = 0;
            iloscWypelnienia = -iloscWypelnienia;
             }  
    }
    // blinker right

    if (Left_Turn == false && Right_Turn == false && Emergency_Lights == false ) {
        digitalWrite(BlinkerLeft, LOW);
        digitalWrite(BlinkerRight, LOW);
    }   
    
    

   // if (Left_Turn == false && Right_Turn == false & Emergency_Lights == false) {
    //    analogWrite(BlinkerRight, LOW);
    //    analogWrite(BlinkerLeft, LOW);
  //  }

    //emergency lights
    if (Emergency_Lights == true) {
        if (millis() - czasPoprzedniegoWypelnienia >= czasDoWypelnienia) {
            wypelnienie = wypelnienie + iloscWypelnienia;
            czasPoprzedniegoWypelnienia = millis();
        }  
        analogWrite(BlinkerLeft, wypelnienie);
        analogWrite(BlinkerRight, wypelnienie);
        Left_Turn = false;
        Right_Turn = false;

            if ( wypelnienie >= 255) {
             wypelnienie = 255;
            iloscWypelnienia = -iloscWypelnienia;
            }
            if ( wypelnienie <= 0) {
             wypelnienie = 0;
            iloscWypelnienia = -iloscWypelnienia;
             }          
    }
    else if (Emergency_Lights == false) {
        wypelnienie = 0;    
    } 
 //emergency lights 

  //  if (Left_Turn == false && Right_turn == false && Emergency_Lights == false) {

 //   }



    
  
 
}