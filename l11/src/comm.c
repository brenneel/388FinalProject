#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "eecs388_lib.h"


#define SERVO_PULSE_MAX 2400    /* 2400 us */
#define SERVO_PULSE_MIN 544     /* 544 us */
#define SERVO_PERIOD    20000   /* 20000 us (20ms) */
/**
 * Generate a pwm signal
 * 
 * Input:
 *  @gpio   gpio number
 *  @pos    degree [0,180]
 */
void servo(int gpio, int pos)
{
    // YOUR CODE HERE

    //find duty time
    uint64_t duty_time = ((pos+90)*(SERVO_PULSE_MAX-SERVO_PULSE_MIN))/180;
    uint64_t duty_usec = duty_time + SERVO_PULSE_MIN;

    //turn on the servo
    gpio_write(gpio, ON);
    delay_usec(duty_usec);

    //turn off and wait
    gpio_write(gpio, OFF);
    delay_usec((SERVO_PERIOD-duty_usec));
}


int main()
{
    //initialize LEDs
    char c;
    int led_gpio = GREEN_LED; 

    gpio_mode(RED_LED, OUTPUT);
    gpio_mode(GREEN_LED, OUTPUT);
    gpio_mode(BLUE_LED, OUTPUT);

    //setting up variables for lidar
    int dist; // read distance value. 
    int dist_L;
    int dist_H;

    // initialize UART channels
    ser_setup(0); // uart0 (lidar)
    ser_setup(1); // uart1 (raspberry pi)

    printf("Setup completed.\n");
    printf("Begin the main loop.\n");

    int prev_dist = 0;
    int prev_angle = 0; 


    int gpio = PIN_19;
    gpio_mode(gpio, OUTPUT);

    while (1) {
        // YOUR CODE HERE
        if(ser_isready(1)){
            //turn LED off here
            gpio_write(GREEN_LED,OFF);
            gpio_write(BLUE_LED, OFF);
            gpio_write(RED_LED, OFF);
                
            //read in the value from pi 4    
            uint8_t val = ser_read(1);
           
            
            //taking in distance value from lidar
            if ('Y' == ser_read(0) && 'Y' == ser_read(0)) {
            // YOUR CODE HERE
            dist_L = ser_read(0);
            dist_H = ser_read(0);
            dist = dist_H;
            dist << 8;
            dist =dist + dist_L;
            //printf("%d", dist);

            //clears the last 5 bytes from ser_read()
            for(int i = 0; i<5; i++){
               int temp =  ser_read(0);
            }
        }


            //logic for lidar distance 
            if(dist<50){
                //stop the servo
                gpio_write(RED_LED, ON);
                gpio_write(BLUE_LED, OFF);
                gpio_write(GREEN_LED, OFF);

            }
            else{
                /**this is the servo motor control code */

                /* control the servor for 1 sec duration */
                if(val < -30){
                   
                    //turn the blue light on
                    gpio_write(BLUE_LED, ON);
                    servo(gpio, val);
                }
                else if(val > 30){
                    
                    //turn green light on
                    gpio_write(GREEN_LED,ON);
                    servo(gpio, val);
                }
                else{
                    
                    //turn all three lights on
                    gpio_write(GREEN_LED,ON);
                    gpio_write(BLUE_LED, ON);
                    gpio_write(RED_LED, ON);
                    servo(gpio, val);
                }
            /**end servo motor control code */
            } 
            /**end of lidar control logic */ 
        }
        /**end of is_ready loop*/
    }
}
