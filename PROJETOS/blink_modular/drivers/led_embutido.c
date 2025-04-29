#include "led_embutido.h"
#include <pico/cyw43_arch.h>

// Inicializa o driver CYW43 (chama cyw43_arch_init)
void led_embutido_init(void) {
    cyw43_arch_init();
}

// Define o estado do LED interno usando CYW43_WL_GPIO_LED_PIN
void led_embutido_set(int state) {
    // CYW43_WL_GPIO_LED_PIN é a constante para o pino do LED interno
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
}