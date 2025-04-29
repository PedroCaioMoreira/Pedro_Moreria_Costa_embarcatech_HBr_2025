#include <stdio.h> 
#include "unity.h"
#include "temperatura_controle.h"    // Protótipo de adc_to_celsius()

void setUp(void)    { /* opcional, executado antes de cada teste */ }
void tearDown(void) { /* opcional, executado depois de cada teste */ }

void test_adc_to_celsius(void) {
    uint16_t adc_simulado    = 875;    // 0.706 V 876
    float    esperado        = 27.0f;
    float    margem_de_erro  = 0.7f;

    float atual = adc_to_celsius(adc_simulado);
    printf("DEBUG: adc=%u -> temperatura=%.5f\n", adc_simulado, atual);
    TEST_ASSERT_FLOAT_WITHIN(margem_de_erro, esperado, atual);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_adc_to_celsius);
    return UNITY_END();
}