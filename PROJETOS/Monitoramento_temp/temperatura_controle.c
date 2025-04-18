#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/adc.h"      // Controle do ADC.
#include "hardware/i2c.h"
#include "hardware/dma.h"      // Controle do DMA.

// Pinos I2C para o display SSD1306
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// Definição do canal ADC do sensor de temperatura interno (canal 4)
#define ADC_TEMPERATURE_CHANNEL 4

// Número de amostras coletadas via DMA (ajuste conforme necessário)
#define AMOSTRAS 1000

// Buffer global para armazenar as amostras do ADC (cada amostra em 16 bits)
volatile uint16_t adc_dma_buffer[AMOSTRAS];

// Função para converter o valor do ADC em temperatura (°C)
// Fórmula típica para o sensor de temperatura interno da RP2040.
void adc_to_temperature(uint16_t adc_value, float *temperature_celsius, char *temp_display) {
    const float conversion_factor = 3.3f / (1 << 12);  
    float voltage = adc_value * conversion_factor;     
    *temperature_celsius = 27.0f - (voltage - 0.706f) / 0.001721f;  
    sprintf(temp_display, "%.2f", *temperature_celsius);
}

int main() {

   
    
    stdio_init_all();

    // --- Configuração do ADC ---
    adc_init();
    adc_set_temp_sensor_enabled(true);         // Habilita o sensor de temperatura interno
    adc_select_input(ADC_TEMPERATURE_CHANNEL);   // Seleciona o canal 4
    adc_fifo_setup(
        true,   // habilita FIFO
        true,   // habilita a geração de requisição DMA
        1,      // threshold para requisição (1 leitura)
        false,  // não usar erro
        false   // não fazer downscale
    );
    
    // --- Configuração do I²C para o display ---
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    // Inicializa o display SSD1306
    ssd1306_init();
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&frame_area);
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
    
    // Desenha um título no display
    char title[] = "TEMPERATURA:";
    ssd1306_draw_string(ssd, 35, 0, title);
    render_on_display(ssd, &frame_area);

    // --- Configuração do DMA para leitura do ADC ---
    int dma_chan = dma_claim_unused_channel(true);
    dma_channel_config dma_cfg = dma_channel_get_default_config(dma_chan);
    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16); // 16 bits por transferência
    channel_config_set_read_increment(&dma_cfg, false);  // Leitura sempre do mesmo endereço (adc_hw->fifo)
    channel_config_set_write_increment(&dma_cfg, true);  // Incrementa o ponteiro de escrita no buffer
    channel_config_set_dreq(&dma_cfg, DREQ_ADC);         // Aciona o DMA com o DREQ do ADC

    float temperature;
    char temp_str[30];
    
    while (true) {
        float sum = 0.0f;
        
        // Limpa o FIFO do ADC antes de iniciar
        adc_fifo_drain();
        
        // Configura e inicia o DMA para transferir SAMPLES valores do ADC para adc_dma_buffer
        dma_channel_configure(
            dma_chan,
            &dma_cfg,
            adc_dma_buffer,      // Destino (buffer)
            &adc_hw->fifo,       // Origem (registro FIFO do ADC)
            AMOSTRAS,             // Número de transferências
            true                 // Inicia imediatamente
        );
        
        // Liga o ADC e inicie a conversão
        adc_run(true);
        // Aguarda a finalização da transferência DMA
        dma_channel_wait_for_finish_blocking(dma_chan);
        // Desliga o ADC para evitar novas conversões enquanto processa
        adc_run(false);
        
        // Processa o buffer: calcula a média das temperaturas
        for (int i = 0; i < AMOSTRAS; i++) {                         
            float temp;
            adc_to_temperature(adc_dma_buffer[i], &temp, temp_str);
            sum += temp;
        }
        
        float average = sum / AMOSTRAS;
        printf("Temperatura média: %.2f C\n", average);

        // Atualiza o display com o valor médio
        sprintf(temp_str, " %.2f C", average); // Tranforma o valor da temperatura (float) em string
        ssd1306_draw_string(ssd, 40, 12, temp_str);
        render_on_display(ssd, &frame_area);
        
        sleep_ms(1000);
    }
    
    return 0;
}
