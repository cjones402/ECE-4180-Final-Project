#include "mbed.h"
#include "Motor.h"
#include "Servo.h"
#include "ultrasonic.h"
#include "rtos.h"

Serial blue(p13,p14);
Serial pc(USBTX, USBRX);
Motor left_motor(p21,p5,p6);
Motor right_motor(p22,p7,p8);

DigitalOut speaker(p10);

Servo blade(p26);

DigitalOut LEDA(p23);
DigitalOut LEDB(p24);

Ticker audio_sampletick;
Ticker lidar_sampletick;

volatile bool sound = false;
volatile bool moveForward = true;
volatile bool doneSettingUp = false;

int status;
uint32_t distanceVal;

void dist(int distance)
{
    //put code here to execute when the distance has changed
    printf("Distance %d mm\r\n", distance);
    if (distance < 200) {
        printf("Distance is less than 100");
        moveForward = false;
        left_motor.speed(0.0);
        right_motor.speed(0.0);
    } else {
        moveForward = true;
    }
}

void thread1(void const *args)
{
    ultrasonic mu(p28, p27, .1, 1, &dist);    //Set the trigger pin to D8 and the echo pin to D9
    //have updates every .1 seconds and a timeout after 1
    //second, and call dist when the distance changes
    mu.startUpdates();//start measuring the distance
    while(true) {
        mu.checkDistance();
    }
}


ultrasonic mu(p28, p27, .1, 1, &dist);    //Set the trigger pin to D8 and the echo pin to D9
//have updates every .1 seconds and a timeout after 1
//second, and call dist when the distance changes

void Alert()
{
    if (!sound) {
        speaker = 0.0;
    } else {
        speaker = 1.0;
        wait(1);
        speaker = 0.0;
        wait(0.5);
        //sound = false;
    }
}

void WallStop()
{
    mu.checkDistance();
}

int main()
{
    Thread t1(thread1); //start thread1
    mu.startUpdates();//start measuring the distance
    float blade_servo = 1.0;
    float forward_value = 0.0;
    audio_sampletick.attach(&Alert,1.6);
    //lidar_sampletick.attach(&WallStop, 0.1);
    char bnum=0;
    blade = 0.5;

    while(1) {
        if (blue.getc()=='!') {
            if (blue.getc()=='B') { //button data packet
                bnum = blue.getc(); //button number
                if (blue.getc()=='1') { //1=hit, 0=release
                    switch (bnum) {
                        case '1': //number button 1
                            blade_servo = blade_servo - 0.125;
                            if (blade_servo <= 0.0)
                                blade_servo = 0.0;
                            break;
                        case '2': //number button 2
                            blade_servo = blade_servo + 0.125;
                            if (blade_servo >= 1.0)
                                blade_servo = 1.0;
                            break;
                        case '3': //number button 3
                            LEDA = !LEDA;
                            LEDB = !LEDB;
                            break;
                        case '4': //number button 4
                            //pc.printf("In the stop motor case");
                            left_motor.speed(0.0);
                            right_motor.speed(0.0);
                            forward_value = 0.0;
                            sound = false;
                            break;
                        case '5': //button 5 up arrow
                            if (moveForward) {
                                if (forward_value==0.0)
                                    forward_value= 1.0;
                                forward_value = 1.0;
                                left_motor.speed(forward_value);
                                right_motor.speed(forward_value);
                            }
                            sound = false;
                            break;
                        case '6': //button 6 down arrow
                            left_motor.speed(-1.0);
                            right_motor.speed(-1.0);
                            forward_value = 0.0;
                            sound = true;
                            break;
                        case '7': //button 7 left arrow
                            if (moveForward) {
                                left_motor.speed(0.4);
                                right_motor.speed(1.0);
                                forward_value = 0.0;
                            }
                            sound = false;
                            break;
                        case '8': //button 8 right arrow
                            if (moveForward) {
                                left_motor.speed(1.0);
                                right_motor.speed(0.4);
                                forward_value = 0.0;
                            }
                            sound = false;
                            break;
                        default:
                            break;
                    }
                    blade = blade_servo;
                    blue.getc();
                }
            }
        }
    }
}
