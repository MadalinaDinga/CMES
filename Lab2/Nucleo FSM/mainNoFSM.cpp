#include "mbed.h"

#include "DHT11.h" //temperature and humidity

/* we assume plants are all located outside */
const int MOISTURE_THRESHOLD = 40;
const float PHOTODIODE_THRESHOLD = 2.0;
const int TEMP_UPPER_THRESHOLD = 30;
const int TEMP_LOWER_THRESHOLD = 10;

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
        if(moisturePercent < MOISTURE_THRESHOLD){
            pc.printf("YES \n");
            
            float light = ledLight.read();
            
            pc.printf("Light: %f < 2.0 ? \n", light);
            if(light < PHOTODIODE_THRESHOLD){
                pc.printf("YES \n");
                    
                float temperature = d.readTemperature();
                pc.printf("Temperature: %f \n", temperature);
                
                if (temperature > TEMP_UPPER_THRESHOLD){ 
                    pc.printf("Water the plants with a LOT of water! \n");  
                }
                if(temperature < TEMP_LOWER_THRESHOLD){
                    pc.printf("Water the plants with just a BIT of water! \n");    
                }else{
                    pc.printf("Water the plants with NORMAL amount of water! \n");    
                }                       
            }
        }
        
        wait(1800.0); // wait for half an hour
    }
}
