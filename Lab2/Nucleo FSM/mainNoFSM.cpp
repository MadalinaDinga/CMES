#include "mbed.h"

#include "DHT11.h" //temperature and humidity

/* we assume plants are all located outside */

const int moistureThreshold = 40;
const float photodiodeThreshold = 2.0;

PwmOut mypwm(PWM_OUT);  //soil
DigitalOut myled(LED1); //soil

PinName myledD15(D15); //temp
DHT11 d(myledD15); //temp

Serial pc(USBTX, USBRX); //photodiode and temp

int main() {
    AnalogIn led(A0); //moisture
    AnalogIn ledLight(A1); //light
    
    while(1){
        pc.printf("______________START_________________\n");
        int moisturePercent = led.read() * 100;
            
        pc.printf("Moisture: %d < 40 ? \n", moisturePercent);
        if(moisturePercent < moistureThreshold){
            pc.printf("YES \n");
            
            float light = ledLight.read();
            
            pc.printf("Light: %f < 2.0 ? \n", light);
            if(light < photodiodeThreshold){
                pc.printf("YES \n");
                    
                float temperature = d.readTemperature();
                pc.printf("Temperature: %f \n", temperature);
                                          
                if(temperature < 10){
                    pc.printf("Water the plants with just a bit of water! \n");    
                }else{
                    pc.printf("Water the plants with normal amount of water! \n");    
                }
                if(temperature > 30){
                    pc.printf("Water the plants with a LOT of water! \n");  
                } 
            }
        }
        
        wait(1800.0); // wait for half an hour
    }
}
