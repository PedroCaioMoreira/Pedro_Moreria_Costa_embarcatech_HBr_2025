/*
 * File: temperatura_controle.c
 * ----------------------------
 * Implementa a conversão de valor bruto de ADC (12 bits) para temperatura em °C
 * usando a fórmula oficial da Raspberry Pi Pico W.
 *
 * Recapitulando a estrutura do projeto de teste com Unity:
 * 1) temperatura_controle.h : protótipo de adc_to_celsius().
 * 2) temperatura_controle.c :
 *    - Implementação de adc_to_celsius() sempre disponível para compilação e testes.
 *    - Bloco #ifndef UNITY_TEST abriga todo o código dependente do Pico SDK.
 * 3) test_temp.c : inclui unity.h e temperatura_controle.h, define setUp/tearDown,
 *    cria testes unitários com TEST_ASSERT_FLOAT_WITHIN, e seu próprio main() para executar testes.
 * 4) unity.c, unity.h, unity_internals.h : arquivos do framework Unity na mesma pasta.
 * 5) Compilação do teste:
 *      gcc -I. -DUNITY_TEST -Wall -Wextra -std=c99 \
 *          temperatura_controle.c test_temp.c unity.c -o run_tests -lm
 * 6) Execução:
 *      ./run_tests      (ou run_tests.exe no PowerShell)
 *
 * Comentários adicionais:
 * - O bloco UNITY_TEST garante que, ao definir UNITY_TEST na compilação,
 *   todo o código específico de hardware (Pico SDK) seja omitido,
 *   evitando dependências externas para rodar testes unitários.
 * - A função adc_to_celsius() permanece sempre visível para que
 *   tanto o firmware real quanto os testes possam ligá-la corretamente.
 * - Este padrão (protótipo em .h, implementação isolada e guardada)
 *   facilita manutenção, teste e integração contínua.
 */

 #include "temperatura_controle.h"  // Declaração de adc_to_celsius()

 /*
  * Converte valor bruto de ADC (0–4095) para temperatura em °C.
  * Fórmula baseada em Vref = 3.3V, sensor interno da RP2040:
  *   T (°C) = 27 - (Vsensor - 0.706) / 0.001721
  */
 float adc_to_celsius(uint16_t adc_val) {
     const float conversion_factor = 3.3f / (1 << 12);  // 3.3V / 4096
     float voltage = adc_val * conversion_factor;       // Tensão correspondente
     return 27.0f - (voltage - 0.706f) / 0.001721f;     // Cálculo da temperatura
 }
 
 #ifndef UNITY_TEST  /*
                      * Bloco protegido: somente código para firmware real
                      * (não é compilado nos testes unitários com -DUNITY_TEST).
                      */
 
 #include <stdio.h>
 #include "pico/stdlib.h"   // SDK do Pico (inicialização UART, sleep, etc.)
 #include "hardware/adc.h"  // Controla o ADC interno da RP2040
 
 #define ADC_TEMPERATURE_CHANNEL 4  // Canal 4 = sensor interno de temperatura
 
 int main() {
     // Inicializa UART/stdio e o hardware do ADC
     stdio_init_all();
     adc_init();
     adc_set_temp_sensor_enabled(true);     // Liga sensor de temperatura
     adc_select_input(ADC_TEMPERATURE_CHANNEL);  // Seleciona canal 4
 
     while (true) {
         // Leitura direta do ADC (12 bits)
         uint16_t raw = adc_read();
         // Conversão para °C via nossa função testável
         float temperature = adc_to_celsius(raw);
         // Impressão no console (via USB/UART)
         printf("Temperatura: %.2f C\n", temperature);
         sleep_ms(1000);  // aguarda 1 segundo para próxima leitura
     }
     return 0;
 }
 #endif  /* UNITY_TEST */
 