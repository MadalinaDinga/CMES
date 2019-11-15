#include "mbed.h"
#include "DHT11.h" //temperature and humidity
#define DEBUG 1
#define STATE_IDLE 0
#define STATE_CHECK_MOISTURE 1
#define STATE_CHECK_LIGHT 2
#define STATE_CHECK_TEMP 3

// set thresholds
const int MOISTURE_THRESHOLD = 40;
const float PHOTODIODE_THRESHOLD = 2.0;
const int TEMP_UPPER_THRESHOLD = 30;
const int TEMP_LOWER_THRESHOLD = 10;

/* we assume plants are all located outside */

PwmOut mypwm(PWM_OUT);  //soil
DigitalOut myled(LED1); //soil

PinName myledD15(D15); //temp
DHT11 d(myledD15); //temp


int fsm_state; // state variable
Serial pc(USBTX, USBRX); //photodiode and temp

void setup() {
    if (DEBUG) {
        pc.printf(" Serial communication started:\n");  
    }
    fsm_state = STATE_IDLE; // state variable initialization    
}    

void loop(AnalogIn led, AnalogIn ledLight) {
    
    switch(fsm_state) {
        case STATE_IDLE:
            wait(1800.0); // wait for 30 minutes
            
            if(DEBUG){           
                pc.printf(" I am IDLE \n");  
            }
            fsm_state = STATE_CHECK_MOISTURE;
            break;
        
        case STATE_CHECK_MOISTURE:
            int moisturePercent = led.read() * 100; // read moisture
            pc.printf("Moisture: %d < 40 ? \n", moisturePercent);
            
            if(moisturePercent < MOISTURE_THRESHOLD){
                pc.printf("Go to STATE_CHECK_LIGHT \n");
                fsm_state = STATE_CHECK_LIGHT;
            } else {
                pc.printf("Go to STATE_IDLE \n");
                fsm_state = STATE_IDLE; 
            }
            break;
            
        case STATE_CHECK_LIGHT:
            float light = ledLight.read();
            pc.printf("Light: %f < 2.0 ? \n", light);
            
            if(light < PHOTODIODE_THRESHOLD){
                pc.printf("Go to STATE_CHECK_TEMP \n");
                fsm_state = STATE_CHECK_TEMP;
            } else {
                pc.printf("Go to STATE_IDLE \n");
                fsm_state = STATE_IDLE; 
            }
            break;
            
        case STATE_CHECK_TEMP:
            float temperature = d.readTemperature();
            pc.printf("Temperature: %f \n", temperature);
            
            if(temperature < TEMP_LOWER_THRESHOLD){
                pc.printf("Water the plants with just a BIT of water! \n");    
            }else{
                pc.printf("Water the plants with NORMAL amount of water! \n");    
            }
            if(temperature > TEMP_UPPER_THRESHOLD){
                pc.printf("Water the plants with a LOT of water! \n");  
            }
            fsm_state = STATE_IDLE; 
            break; 
    }
}

int main() {
    AnalogIn led(A0); //moisture
    AnalogIn ledLight(A1); //light
    
    while(1){
        loop(led, ledLight);
    }
}
