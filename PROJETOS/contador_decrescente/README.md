# Contador Decrescente com Cliques - Raspberry Pi Pico + OLED

Este projeto implementa um contador regressivo de 9 a 0 segundos utilizando dois botões e um display OLED I2C SSD1306, com contagem de cliques no botão B durante a execução. Desenvolvido em C com o SDK oficial do Raspberry Pi Pico.

---

## 🧠 Visão Geral

O objetivo deste projeto é utilizar recursos básicos do Raspberry Pi Pico, como:

- **Interrupções de GPIO**
- **Temporização com `time_us_64()`**
- **I2C com display SSD1306**
- **Leitura de botões com debounce**
- **Renderização de informações em tempo real**

Durante cada segundo da contagem, o usuário pode pressionar o botão **B**. Cada clique completo (pressionar e soltar) é contado e exibido na tela. O botão **A** inicia a contagem.

---

## ⚙️ Estrutura de Funcionamento

- **Botão A (GPIO 5):** inicia a contagem de 9 segundos (apenas se não estiver ativa).
- **Botão B (GPIO 6):** durante a contagem, cada clique é contabilizado e exibido.
- **Display OLED (I2C - GPIO 14 SDA, GPIO 15 SCL):** exibe o valor atual da contagem e o número de cliques.

---

## 🔧 Desenvolvimento

### 📌 Lógica de Funcionamento

- O código inicializa os **pinos dos botões** e o **barramento I2C**.
- O **Botão A (GPIO 5)** dispara a contagem regressiva. Durante esse tempo, o botão A é ignorado.
- O **Botão B (GPIO 6)** é utilizado para registrar cliques. Cada clique é contado apenas se for detectado o ciclo completo de **pressionar + soltar** (debounce manual).
- O display mostra a contagem atual e o número de cliques registrados.
- Toda a lógica de timing é feita com `time_us_64()` para controlar a duração de 1 segundo entre decrementos.

### 💻 Código Modular

O código principal (`main.c`) está dividido em funções organizadas para facilitar a leitura e manutenção:

| Função                  | Responsabilidade                                                                 |
|-------------------------|----------------------------------------------------------------------------------|
| `main()`                | Inicializa periféricos e executa o loop principal                               |
| `gpio_callback()`       | Interrupção para os botões A e B                                                |
| `start_contagem()`      | Executa a contagem de 9 a 0 com lógica de clique durante cada segundo           |
| `update_display()`      | Atualiza o conteúdo visual mostrado no display                                  |
| `ssd1306_clear()`       | Limpa o buffer do display                                                        |
| `ssd1306_show()`        | Envia o buffer renderizado para o display                                       |

---
### 🔌 Pinos Utilizados

| Função       | Pino GPIO |
|--------------|------------|
| Botão A      | 5          |
| Botão B      | 6          |
| I2C SDA      | 14         |
| I2C SCL      | 15         |

---

## 🧪 Resultado Esperado

Durante a execução do código:

1. O display fica inicialmente em branco.
2. Ao apertar **Botão A**, a contagem de 9 a 0 é iniciada.
3. Durante a contagem, cada **clique em Botão B** é registrado e exibido.
4. O display mostrará algo como:

