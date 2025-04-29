Teste Unitário de Conversão de Temperatura para Raspberry Pi Pico W

Descrição

Este repositório contém a implementação e os testes unitários para a função adc_to_celsius(), que converte leituras do ADC interno de 12 bits do RP2040 em graus Celsius, usando o sensor de temperatura embutido da Raspberry Pi Pico W.

Estrutura do Projeto

meu-projeto-pico/
├── temperatura_controle.h       # Protótipo de adc_to_celsius()
├── temperatura_controle.c       # Implementação da função e main() do firmware
├── test_temp.c                  # Teste unitário usando Unity
├── unity.c                      # Implementação do framework Unity
├── unity.h                      # Header do Unity
├── unity_internals.h            # Arquivo interno do Unity


temperatura_controle.h: Declara a função adc_to_celsius(uint16_t).

temperatura_controle.c: Contém a implementação de adc_to_celsius() sempre visível, e o bloco #ifndef UNITY_TEST para isolar o código de firmware (dependente do Pico SDK) do código de teste.

test_temp.c: Arquivo de teste que utiliza o Unity para validar adc_to_celsius() com um valor simulado de ADC.

Arquivos do Unity (unity.c, unity.h, unity_internals.h): Framework de teste unitário em C.
