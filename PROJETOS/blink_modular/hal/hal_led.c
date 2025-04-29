#include "hal_led.h"
#include "led_embutido.h"

static int led_state = 0; // Estado atual do LED (0 = apagado, 1 = aceso)

// Inicializa a HAL do LED (chama o driver)
void hal_led_init(void) {
    led_embutido_init();
    led_state = 0; // Garante estado inicial (desligado)
    led_embutido_set(led_state);
}

// Alterna o estado do LED interno
void hal_led_toggle(void) {
    led_state = !led_state;        // Inverte estado (0->1, 1->0)
    led_embutido_set(led_state);   // Atualiza o LED via driver
}