#ifndef HAL_LED_H
#define HAL_LED_H

// Inicializa a camada HAL do LED (internamente chama led_embutido_init)
void hal_led_init(void);

// Alterna o estado do LED interno (liga/desliga)
void hal_led_toggle(void);

#endif // HAL_LED_H