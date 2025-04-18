#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h" 

#define BUTTON_A 5
#define BUTTON_B 6

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define OLED_ADDR 0x3C

volatile int    cont = 0;
volatile int    click_cont = 0;
volatile bool   contagem = false;
volatile bool   start = false;
volatile bool   atualizacao = false;

// flags de debounce para B
volatile bool   botao_b_pressionado = false;
bool            botao_b_liberado    = true;

// Buffer e área de renderização para o display
uint8_t             ssd[ssd1306_buffer_length];
struct render_area  frame_area;

// Protótipos
void gpio_callback(uint gpio, uint32_t events);
void start_contagem();
void update_display();
void ssd1306_clear();
void ssd1306_show();

int main() {
    stdio_init_all();

    // ─── Configuração dos botões ─────────────────────────────────────────
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // ─── Interrupções para Button A e Button B ───────────────────────────
    // Button A: inicia contagem apenas se não estiver em curso
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    // Button B: marca pressionado; debouce/soltar é tratado no loop
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // ─── Inicialização do barramento I2C1 para o OLED ────────────────────
    i2c_init(I2C_PORT, ssd1306_i2c_clock * 1000);  // 400 kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // ─── Inicialização do display SSD1306 ────────────────────────────────
    ssd1306_init(I2C_PORT, OLED_ADDR, 128, 64);
    frame_area = (struct render_area){
        .start_column = 0,
        .end_column   = ssd1306_width - 1,
        .start_page   = 0,
        .end_page     = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);
    ssd1306_clear();
    ssd1306_show();

    // ─── Loop principal ──────────────────────────────────────────────────
    while (1) {
        // Se Button A disparou, reseta variáveis e entra em start_contagem()
        if (start) {
            start = false;
            contagem = true;
            cont = 9;             // contador reinicia em 9
            click_cont = 0;       // zera contador de cliques
            botao_b_liberado = true;  
            start_contagem();     // bloqueante, mas com loop interno para debounces
        }
        // Atualiza display se algum valor mudou
        if (atualizacao) {
            atualizacao = false;
            update_display();
        }
        tight_loop_contents();  // garante tratamento de interrupções
    }

    return 0;
}

// ─── Handler de interrupções para A e B ────────────────────────────────
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_A && !contagem) {
        // Ao pressionar A fora de uma contagem ativa, aciona start
        start = true;
    }
    if (gpio == BUTTON_B && contagem) {
        // Ao detectar o edge FALL em B, marca que foi pressionado
        // mas só se já estiver liberado (evita bouncing imediato)
        if (botao_b_liberado) {
            botao_b_pressionado = true;
            botao_b_liberado    = false;
        }
    }
}

// ─── Função de contagem regressiva com tratamento de debounce ──────────
void start_contagem() {
    while (contagem && cont > 0) {
        // Desenha o valor atual da contagem
        update_display();

        // Marca o instante de início do segundo (em microssegundos desde boot)
        uint64_t t0 = time_us_64();

        // Enquanto não tiver passado 1 000 000 µs (ou seja, 1 s)...
        while (time_us_64() - t0 < 1000000) {
            // …verifica se a interrupção de B já setou a flag (botao_b_pressionado == true)
            // e se o pino já voltou a HIGH (gpio_get == 1), indicando que o botão foi solto.
            if (botao_b_pressionado && gpio_get(BUTTON_B)) {
                click_cont++;           // contabiliza o clique
                atualizacao = true;     // sinaliza para redesenhar imediatamente na tela
                // reseta as flags para permitir novo ciclo de pressão+soltar
                botao_b_pressionado = false;
                botao_b_liberado    = true;
            }
            // permite que o RP2040 trate interrupções pendentes e outras tarefas internas
            tight_loop_contents();
        }
        // após 1 s, decrementa o contador
        cont--;
    }

    // ao sair, cont pode ser zero — sinaliza fim de contagem
    contagem = false;
    update_display();
}

// ─── Desenha no buffer e envia ao display ──────────────────────────────
void update_display() {
    char buffer[32];
    ssd1306_clear();

    sprintf(buffer, "Contagem: %d", cont);
    ssd1306_draw_string(ssd, 0, 0, buffer);

    sprintf(buffer, "Cliques B: %d", click_cont);
    ssd1306_draw_string(ssd, 0, 16, buffer);

    ssd1306_show();
}

// ─── Zera o buffer de vídeo e prepara o frame_area ────────────────────
void ssd1306_clear() {
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
}

// ─── Envia buffer de vídeo ao display ─────────────────────────────────
void ssd1306_show() {
    render_on_display(ssd, &frame_area);
}
