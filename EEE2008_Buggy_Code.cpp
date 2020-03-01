// MBed code for Buggy Project - Group 5. Written by Alex Tuddenham

//Libraries/headers used. 
#include "mbed.h"
#include "math.h"
#include "C12832.h"

+
//Setting outputs and inputs
C12832 lcd(p5, p7, p6, p8, p11);
AnalogIn SensorLeft(p15);
AnalogIn SensorRight(p16);
AnalogIn VRef(p17);
PwmOut SteerMotor(p21);
PwmOut DriveMotor(p22);
BusOut ledr (p23);
BusOut ledg (p24);
BusOut ledb (p25);
DigitalOut ReadySteer (p28);
DigitalOut ReadyDrive (p27);
Serial Serial(USBTX, USBRX);
//Value Declarations
double LeftSignal,LeftSignalA;
double RightSignal,RightSignalA;
float P,I,D,Kp,Ki,Kd,x,IntegratedError,DifferentiatedError;
float Error,Target_Voltage,ScaledError;
float Reference,FinalValue,SensDiff;
float Lim1,Lim2,Lim3,Lim4,Bias,EndPWM;

//Start of Code Body//

int main(){
    lcd.cls();
    
    //Boot Code - Setting Initial values.
    SteerMotor.period_us(50);
    DriveMotor.period_us(50);
    Kp=20; Ki=1;Kd=0;
    ledb=0;ledr=1;ledg=1;
    ReadyDrive=(0); ReadySteer=(0);
    DriveMotor.write(0.8f);
    Lim1=0.2;  Lim2=-0.2; Bias=0.5;    
   
    while (1){
            
            //Reading Sensor Inputs & Motor Inputs
            LeftSignal   =  SensorLeft.read();
            RightSignal  =  SensorRight.read();
            Reference    =  VRef.read();
            
            //scaling sensor values and setting linear range.
            LeftSignalA  = LeftSignal   * 1.0;
            RightSignalA = RightSignal * 1.0;
            SensDiff     = (LeftSignalA - RightSignalA);
     
            //scaling the sensor range
            x=Kp*SensDiff;
            
            //Max steering location limits
            if (x>Lim1)
                {x=Lim1;}
            if (x<Lim2)
                {x=Lim2;}
            
            //Finding the location to steer to and error.    
            Target_Voltage= 0.5 + x;
            Error = Target_Voltage - Reference;
            ScaledError=Ki*Error;
            EndPWM=0.5+ScaledError;
            
            //Outputs various values to screen.
            lcd.locate(3,5);
            lcd.printf("Sensor Diff:%0.5f",SensDiff);
            lcd.locate(3,15);
            lcd.printf("E:%0.2f",ScaledError);
            lcd.locate(30,15);
            lcd.printf("TV:%0.2f",Target_Voltage);
            lcd.locate(65,15);
            lcd.printf("F:%0.2f",Reference);
            lcd.locate(90,15);
            lcd.printf("PWM:%0.2f",EndPWM);
            serial.printf("");
            
            wait(0.1);
            lcd.cls();
           //This sends a write signal to the Steering Motor PWM Pin
            SteerMotor =(0.5 + ScaledError);
            
           
 
          
    }}
    