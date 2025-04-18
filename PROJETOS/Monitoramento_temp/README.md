# 🧊 Leitura de Temperatura com ADC + DMA no Raspberry Pi Pico

Este projeto utiliza o sensor de temperatura interno do Raspberry Pi Pico para realizar medições usando ADC com coleta via DMA, e exibe a média das leituras em um display OLED I2C (SSD1306). O sistema realiza uma média de 1000 amostras para melhorar a precisão da medição e atualiza os dados no display a cada segundo.

---

## 🧠 Visão Geral

O objetivo do projeto é demonstrar o uso combinado de:

- **Sensor de Temperatura Interno** da RP2040
- **Conversão Analógica-Digital (ADC)**
- **Transferência Direta de Memória (DMA)** para eficiência
- **Comunicação I2C** com display OLED SSD1306
- **Processamento de dados em tempo real**

O display mostra a temperatura média em graus Celsius (°C), atualizada a cada segundo com base em 1000 amostras.

---

## ⚙️ Estrutura de Funcionamento

### 🧩 Fluxo de Operação

1. Inicializa o ADC no canal 4 (sensor interno).
2. Configura o DMA para coletar 1000 amostras diretamente do FIFO do ADC.
3. Converte os valores para temperatura em °C usando a fórmula oficial da documentação da RP2040.
4. Calcula a média das temperaturas lidas.
5. Atualiza o display OLED com a temperatura média.
6. Repete o ciclo a cada segundo.

---

## 💻 Código Modular

| Função | Descrição |
|--------|-----------|
| `main()` | Inicializa periféricos, configura ADC, DMA e I2C, e executa o loop principal. |
| `adc_to_temperature()` | Converte um valor ADC (uint16_t) em graus Celsius, utilizando a fórmula recomendada. |

---

## 🔌 Pinos Utilizados

| Função | GPIO | Descrição |
|--------|------|-----------|
| I2C SDA | 14 | Comunicação com o display SSD1306 |
| I2C SCL | 15 | Comunicação com o display SSD1306 |
| ADC | Interno (canal 4) | Sensor de temperatura integrado no chip |

---

## 📐 Fórmula de Conversão

A conversão de tensão para temperatura segue:
Onde:
- `V_ADC = (adc_value * 3.3) / 4096`  
- Baseado na documentação da Raspberry Pi para o sensor de temperatura interno do RP2040.

---

## 🧪 Resultado Esperado

Durante a execução do programa:

- O terminal exibirá algo como:
- - O display mostrará:
 
      TEMPERATURA:
        29.54 C

     leitura será atualizada automaticamente a cada segundo.

---

## 🛠️ Dependências

- Raspberry Pi Pico SDK
- Biblioteca SSD1306 (inclusa como `inc/ssd1306.h`)
- Ambiente de desenvolvimento como CMake + VSCode ou Raspberry Pi Pico SDK Toolchain

---

