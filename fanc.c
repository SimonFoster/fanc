#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

ISR( ADC_vect)
{
    OCR0A = ADCH;                   // Copy ADC result to Output Compare A
    OCR0B = ADCH;                   // Copy ADC result to Output Compare B
}

int main( void )
{

    DDRB    |= _BV(PORTB0) +
               _BV(PORTB1);         // Set PB.0 and PB.1 as output

    CLKPR   = _BV(CLKPCE);          // Set Clock Prescalar Change Enable flag
    CLKPR   = 0;                    // ... and set the Clock Prescalar to zero
                                    // ... for 9.6MHz system clock frequency

    TCCR0A  = 0xE3;                 // COM0A1:0 = 3     Set OC0A on compare match, clear OC0A at TOP
                                    // COM0B1:0 = 2     Clear OC0B on compare match, set OC0B at TOP
                                    // WGM01:00 = 3     Mode 3, Fast PWM

    TCCR0B  = 0x01;                 // WGM02    = 0
                                    // CS02:00  = 1     Clock running, No prescalar
                                    //                  => PWM frequency 37.5kHz (9.6Mhz/256)

    ADMUX   = _BV(ADLAR) + 1;       // REFS0    = 0     Vcc used as reference
                                    // ADLAR    = 1     Left adjust ADC result
                                    // MUX1:0   = 1     Use input ADC1 (PB2)

    ADCSRA  = _BV(ADEN)     +       // ADEN     = 1     ADC Enable
              _BV(ADSC)     +       // ADSC     = 1     ADC Start Conversion
              _BV(ADATE)    +       // ADATE    = 1     ADC Auto Trigger Enable
              _BV(ADIE)     +       // ADIE     = 1     ADC Interrupt Enable
              6;                    // ADPS2:0  = 6     Divide by 64 prescalar
                                    //                  => ADC clock = 9.6Mhz/64 = 150 kHz

    sei();                          // Enable Interrupts

    while(1)
    {
        // All processing is handle by the ADC interrupt
        // Nothing to do here so just sleep

        set_sleep_mode( SLEEP_MODE_IDLE );
        sleep_mode();
    }
}

