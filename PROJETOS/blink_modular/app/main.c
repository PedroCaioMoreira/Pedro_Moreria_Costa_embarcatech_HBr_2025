#include <stdio.h>
#include "pico/stdlib.h"
#include "hal_led.h"

int main() {
    stdio_init_all();    // Inicializa USB/serial (opcional para debug)
    hal_led_init();      // Inicializa o LED via HAL

    while (true) {
        hal_led_toggle();      // Alterna o LED
        sleep_ms(300);         // Aguarda 500 milissegundos
    }
    return 0;
}