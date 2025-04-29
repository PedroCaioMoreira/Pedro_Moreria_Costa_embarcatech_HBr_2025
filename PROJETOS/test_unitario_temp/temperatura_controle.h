#ifndef TEMPERATURA_CONTROLE_H
#define TEMPERATURA_CONTROLE_H

#include <stdint.h>

/**
 * @brief  Converte leitura de ADC (0–4095) em °C.
 * @param  adc_val  valor bruto lido do ADC (12 bits)
 * @return Temperatura em graus Celsius
 */
float adc_to_celsius(uint16_t adc_val);

#endif // TEMPERATURA_CONTROLE_H