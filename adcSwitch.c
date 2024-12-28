#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t overflow_count = 0;

ISR(TIM0_OVF_vect) {
    overflow_count++; // Increment overflow count
}

void timer_init() {
    TCCR0A = 0; // normal mode
    TCCR0B |= (1<<CS02); // prescaler 256
    TCCR0B &= ~((1<<CS01) | (1<<CS00)); // prescaler 256
    TIMSK0 |= (1<<TOIE0); // interrupt when timer overflows
    TCNT0 = 0; // clear timer register
}

void timer_stop() {
    TCNT0 = 0; // clear timer register
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // Prescaler bits 000 that means timer stop
}

uint16_t adc_init() {
    ADCSRA |= (1<<ADEN); // Enabe ADC 
    ADMUX |= (1<<MUX0); // choose ADC1
    ADCSRA |= (1<<ADPS0) | (1<<ADPS1); // prescaler 8
    ADCSRA |= (1<<ADSC); // Start convertion
    while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
    return ADC; // Read ADC value
}

int main() {
    DDRB |= (1<<PB4); // pin3 to output
    DDRB &= ~(1<<PB2); // PB2 to input
    PORTB &= ~(1<<PB4); // pull-down
    sei(); // enable global interupts

    uint16_t result = 0;
    uint8_t timer_flag = 0;
    uint8_t toggled = 0;
    

    while (1) {
        result = adc_init();
        if (result > 700 && !timer_flag){
            timer_flag = 1;
            timer_init();
        }
        if (overflow_count >= 10 && !toggled){
            if (result > 700){
                PORTB ^= (1<<PB4); // toggle output
                toggled = 1;
            }
        }
        if (overflow_count >= 30) {
            timer_flag = 0;
            timer_stop();
            overflow_count = 0;
            toggled = 0;
        }
    }
}
