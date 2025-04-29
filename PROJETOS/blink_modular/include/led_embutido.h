#ifndef LED_EMBUTIDO_H
#define LED_EMBUTIDO_H

// Inicializa o driver CYW43 (Wi-Fi) para controlar o LED interno
void led_embutido_init(void);

// Define o estado do LED interno: state = 1 (liga), state = 0 (desliga)
void led_embutido_set(int state);

#endif // LED_EMBUTIDO_H