Projeto Blink Modular em C

Este projeto demonstra uma aplicação simples de blink (piscar LED) muito comum em sistemas embarcados, mas organizada de forma modular, dividindo o código em camadas de responsabilidade:

meu_blink_modular/

├── app/           # Lógica de aplicação e sequências de piscar

├── hal/           # Hardware Abstraction Layer (abstrações de hardware)

├── driver/        # Drivers de baixo nível (GPIO, timers, etc.)

└── include/       # Arquivos de cabeçalho (.h) compartilhados

Estrutura de Pastas

app/
Contém o ponto de entrada main.c e funções de alto nível que definem o comportamento do blink (tempo de ON/OFF, modos de piscar, chamadas de inicialização).

hal/
Camada de abstração de hardware. Aqui ficam funções genéricas, independentes do microcontrolador específico, como hal_led_on(), hal_led_off(), hal_delay_ms(). Internamente, a HAL utiliza os drivers para acessar o hardware.

driver/
Drivers específicos do microcontrolador. Por exemplo, driver_gpio.c configura e controla os pinos GPIO para o LED, e driver_timer.c inicializa e usa temporizadores para gerar delays.

include/
Todos os arquivos de cabeçalho para compartilhar APIs entre as camadas. Por exemplo:

app/blink.h

hal/hal_led.h

driver/gpio.h

Benefícios da Modularização em C

Separação de ResponsabilidadesCada camada faz somente o que lhe cabe: aplicação cuida da lógica, HAL abstrai hardware, driver implementa acesso real ao periférico.

Manutenção FacilitadaMudanças em um driver (por exemplo, trocar GPIO por PWM) não afetam diretamente o código de alto nível.

Reutilização e PortabilidadeAo trocar de microcontrolador, basta reimplementar a camada de driver/HAL, mantendo a app/ inalterada.

TestabilidadeMódulos independentes podem ser testados em unidades. A camada de aplicação pode ser simulada sem hardware real, usando mocks dos drivers.

EscalabilidadeProjetos crescem naturalmente: outras funcionalidades (sensores, comunicação) podem ser adicionadas em app/ e seus respectivos drivers e HAL.
