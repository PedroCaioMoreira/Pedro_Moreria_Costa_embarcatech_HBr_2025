# 🎮 Leitura de Joystick Analógico - Raspberry Pi Pico (ADC)

Este projeto realiza a leitura de um **joystick analógico** com dois eixos (X e Y), utilizando os conversores analógico-digitais (ADC) do microcontrolador **Raspberry Pi Pico**. Os valores são exibidos diretamente no terminal serial via `printf()`.

Desenvolvido em C com o SDK oficial do Raspberry Pi Pico.

---

## 🧠 Visão Geral

Este projeto tem como foco a utilização de **ADCs** do RP2040 para capturar valores analógicos de dois eixos de um joystick:

- **ADC0 (GPIO 26)** → Eixo **Y**
- **ADC1 (GPIO 27)** → Eixo **X**

A leitura é feita a cada 100ms e exibida no terminal serial.

---

## ⚙️ Estrutura de Funcionamento

| Componente        | GPIO | Função               |
|-------------------|------|----------------------|
| Joystick Eixo Y   | 26   | ADC0 (entrada analógica) |
| Joystick Eixo X   | 27   | ADC1 (entrada analógica) |

- A leitura é feita com `adc_select_input()` e `adc_read()`.
- Os valores são de 0 a 4095 (12 bits de resolução padrão).
- A saída é exibida como:

## 🔧 Desenvolvimento

### 📌 Lógica de Funcionamento

1. Inicializa o sistema padrão e o módulo ADC do Pico.
2. Habilita os pinos 26 e 27 como entradas analógicas.
3. Em loop infinito:
   - Alterna a leitura entre os canais 0 e 1.
   - Armazena os valores crus (sem conversão para tensão).
   - Imprime os valores no terminal com `printf()`.

