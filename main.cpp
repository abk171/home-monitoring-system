#include "mbed.h"

DigitalIn S(p20); // Switch Input
AnalogIn PM1(p15); // Temp Input
AnalogIn PM2(p16); // Light Input

DigitalOut Buzzer(p21); // Buzzer
DigitalOut L1(p22);
DigitalOut L2(p23);
DigitalOut L3(p24);
PwmOut tempSensor(p25);
Timer t;
void LightFunction();
void DoorSystemFunction();
void TemperatureFunction();


Serial pc(USBTX, USBRX); // To communicate with TeraTerm
float data, temp, waitTime;

int main() {
    S.mode(PullUp);
    waitTime = 0.2;
    while(1) {
        LightFunction();
        
        DoorSystemFunction();
        
        TemperatureFunction();
        pc.printf("\n\n\r");
        wait(waitTime);
    }
}

void LightFunction() {
    data = PM2.read();
    pc.printf("The light intensity = %f \n\r", data);
    if(data > 0.95) {
        L1 = 0;
        L2 = 0;
        L3 = 0;
    }
    else {
        L1 = 1;
        L2 = 1;
        L3 = 1;
    }
}

void DoorSystemFunction() {

    if(S == 0) {
        pc.printf("The door has been open for %f seconds.\n\r", t.read());
        if(t == 0) t.start();
        if(t >= 10) {
            Buzzer = 1;
               pc.printf("WARNING! Please close the door!\n\r");
        }
    }
    else {
        Buzzer = 0;
        t.stop();
        t.reset();
    }
}

void TemperatureFunction() {
    temp = PM1.read();
    pc.printf("The temperature reading = %f \n\r", temp);
    if(temp > 0.25) {
        if(temp < 0.45) tempSensor.write((temp - 0.25)*5);
        else tempSensor.write(1);
    }
    else tempSensor.write(0);
}