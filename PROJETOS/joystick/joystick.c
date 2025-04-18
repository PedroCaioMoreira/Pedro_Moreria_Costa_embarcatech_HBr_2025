#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int main() {
    stdio_init_all();
    adc_init();

    // Habilita os pinos GPIO 26 (ADC0) e 27 (ADC1)
    adc_gpio_init(26);  // Eixo Y
    adc_gpio_init(27);  // Eixo X

    while (1) {
        adc_select_input(0);                // Lê o canal 0 (GPIO 26)
        uint16_t adc_y_raw = adc_read();    // Valor do eixo Y

        adc_select_input(1);                // Lê o canal 1 (GPIO 27)
        uint16_t adc_x_raw = adc_read();    // Valor do eixo X

        // Exibe os valores diretamente no terminal
        printf("Joystick - X: %u\tY: %u\n", adc_x_raw, adc_y_raw);

        sleep_ms(100);  // Delay de 100ms
    }
}