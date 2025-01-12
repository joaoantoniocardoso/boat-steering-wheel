#include "machine.h"

// machine variables
volatile state_machine_t state_machine;
volatile system_flags_t system_flags;
volatile error_flags_t error_flags;

volatile uint8_t machine_clk;
volatile uint8_t machine_clk_divider;
volatile uint8_t total_errors;           // Contagem de ERROS

// other variables
volatile uint8_t led_clk_div;
volatile uint8_t ui_clk_div;
volatile uint8_t ui_clk_div_2;
volatile uint16_t ui_timeout_clk_div;


/**
 * @brief
 */
void machine_init(void)
{
	clr_bit(PRR0, PRTIM2);           // Activates clock

    // MODE 2 -> CTC with TOP on OCR1
    TCCR2A  =    (1 << WGM21) | (0 << WGM20)        // mode 2
              | (0 << COM2B1) | (0 << COM2B0)       // do nothing
              | (0 << COM2A1) | (0 << COM2A0);      // do nothing

    TCCR2B =
#if MACHINE_TIMER_PRESCALER ==     1
                (0 << CS22) | (0 << CS21) | (1 << CS20) // Prescaler N=1
#elif MACHINE_TIMER_PRESCALER ==   8
                (0 << CS22) | (1 << CS21) | (0 << CS20) // Prescaler N=8
#elif MACHINE_TIMER_PRESCALER ==   32
                (0 << CS22) | (1 << CS21) | (1 << CS20) // Prescaler N=32
#elif MACHINE_TIMER_PRESCALER ==   64
                (1 << CS22) | (0 << CS21) | (0 << CS20) // Prescaler N=64
#elif MACHINE_TIMER_PRESCALER ==   128
                (1 << CS22) | (0 << CS21) | (1 << CS20) // Prescaler N=128
#elif MACHINE_TIMER_PRESCALER ==   256
                (1 << CS22) | (1 << CS21) | (0 << CS20) // Prescaler N=256
#elif MACHINE_TIMER_PRESCALER ==   1024
                (1 << CS22) | (1 << CS21) | (1 << CS20) // Prescaler N=1024
#else
                0
#endif
                | (0 << WGM22);      // mode 2

    OCR2A = MACHINE_TIMER_TOP;       // OCR2A = TOP = fcpu/(N*2*f) -1

    TIMSK2 |=   (1 << OCIE2A);       // Activates interruption

    VERBOSE_MSG_INIT(usart_send_string("\n\rMACHINE_TIMER_TOP: "));
    VERBOSE_MSG_INIT(usart_send_uint16(MACHINE_TIMER_TOP));

    set_machine_initial_state();
    set_state_initializing();
}

/**
 * @brief set machine initial state
 */
inline void set_machine_initial_state(void)
{
    error_flags.all = 0;
    machine_clk = machine_clk_divider = led_clk_div = 0;
}

/**
 * @brief set error state
 */
inline void set_state_error(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string("\n\r>>>STATE ERROR\n\r"));
    state_machine = STATE_ERROR;
}

/**
* @brief set initializing state
*/
inline void set_state_initializing(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string("\n\r>>>INITIALIZING STATE\n\r"));
    state_machine = STATE_INITIALIZING;
}

/**
* @brief set idle state
*/
inline void set_state_idle(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string("\n\r>>>IDLE STATE\n\r"));
    state_machine = STATE_IDLE;
}

/**
* @brief set running state
*/
inline void set_state_running(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string("\n\r>>>RUNNING STATE\n\r"));
    state_machine = STATE_RUNNING;
}

/**
 * @brief set reset state
 */
inline void set_state_reset(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string("\n\r>>>RESET STATE\n\r\r"));
    state_machine = STATE_RESET;
}

/**
 * @breif prints the configurations and definitions
 */
inline void print_configurations(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string("CONFIGURATIONS:\n\r"));

#ifdef ADC_ON
    VERBOSE_MSG_MACHINE(usart_send_string("\n\radc_f: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16( ADC_FREQUENCY ));
    VERBOSE_MSG_MACHINE(usart_send_char(','));
    VERBOSE_MSG_MACHINE(usart_send_uint16( ADC_AVG_SIZE_10 ));
#endif // ADC_ON

    VERBOSE_MSG_MACHINE(usart_send_string("\n\rmachine_f: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16( MACHINE_FREQUENCY ));

    VERBOSE_MSG_MACHINE(usart_send_char('\n'));
    VERBOSE_MSG_MACHINE(usart_send_char('\r'));
}

/**
* @brief prints the system flags
*/
inline void print_system_flags(void)
{
    //VERBOSE_MSG_MACHINE(usart_send_string(" CLOCK \n\r"));
    //VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.enable));
}

/**
* @brief prints the error flags
*/
inline void print_error_flags(void)
{
    //VERBOSE_MSG_MACHINE(usart_send_string(" errFl: "));
    //VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.no_canbus));
}

/**
 * @brief Checks if the system is OK to run
 */
inline void task_initializing(void)
{
#ifdef LED_ON
    set_led(LED2);
#endif

    set_machine_initial_state();
    VERBOSE_MSG_INIT(usart_send_string("System initialized without errors.\n\r"));
    set_state_idle();

    // CONFIGURE BUTTONS 
    input_assign_callback(sw5, chronometer_start, chronometers.uptime);
    //input_assign_callback(sw0, chronometer_reset_delta, chronometers.uptime);

    input_assign_callback(sw9, ui_screen_change, NULL);

    input_assign_callback(sw1, controller_power_channel_increase, NULL);
    input_assign_callback(sw8, controller_power_channel_decrease, NULL);
    input_assign_callback(sw4, controller_power_channel_change, NULL);

}

//#define CHRONOMETER_DEBUG
void task_chronometer(void)
{                              
#ifdef CHRONOMETER_ON

    inputs_update();
    chronometer_update(chronometers.uptime);

#ifdef CHRONOMETER_DEBUG
    chronometer_print(chronometers.uptime);
#endif
#endif
}

/**
 * @brief waits for commands while checking the system
 */
inline void task_idle(void)
{
#ifdef LED_ON
    if(led_clk_div++ >= 100){
        cpl_led(LED1);
        led_clk_div = 0;
    }
#endif

    task_chronometer();
    ui_update();
}


/**
 * @brief running task checks the system and apply the control action to pwm.
 */
inline void task_running(void)
{
#ifdef LED_ON
    if(led_clk_div++ >= 50){
        cpl_led(LED1);
        led_clk_div = 0;
    }
#endif

}

/**
 * @brief error task checks the system and tries to medicine it.
 */
inline void task_error(void)
{
#ifdef LED_ON
    if(led_clk_div++ >= 5){
        cpl_led(LED2);
        cpl_led(LED1);
        led_clk_div = 0;
    }
#endif
/*
    total_errors++;         // incrementa a contagem de erros

    if(error_flags.no_communication_with_mcs)
    {
        VERBOSE_MSG_ERROR(usart_send_string("\t - No canbus communication with MCS!\n\r"));
        ui_no_communication_with_mcs()
    }

   if(error_flags.can_app_task)
        VERBOSE_ON_ERROR(usart_send_string("\t - can_app_task failed"));

    if(total_errors < 2){
        VERBOSE_MSG_ERROR(usart_send_string("I will reset the machine state.\n\r"));
    }
    if(total_errors >= 20){
        VERBOSE_MSG_ERROR(usart_send_string("The watchdog will reset the whole system.\n\r"));
        set_state_reset();
    }

#ifdef LED_ON
    cpl_led(LED1);
#endif
*/
    set_state_initializing();

}

/**
 * @brief reset error task just freezes the processor and waits for watchdog
 */
inline void task_reset(void)
{
#ifndef WATCHDOG_ON
    //wdt_init();
#endif

    cli();  // disable interrupts

    VERBOSE_MSG_ERROR(usart_send_string("WAITING FOR A RESET!\n\r"));
    for(;;)
    {
#ifdef LED_ON
      cpl_led(LED2);
      cpl_led(LED1);
#endif
      _delay_ms(100);
    }
}

void print_infos(void)
{
    static uint8_t i = 0;

    switch(i++){
        case 0:
            //print_system_flags();
            break;
        case 1:
            //print_error_flags();
            break;
        default:
            //VERBOSE_MSG_MACHINE(usart_send_char('\n'));
            i = 0;
            break;
    }
}

/**
 * @brief this is the machine state itself.
 */
inline void machine_run(void)
{
    //print_infos();

    if(machine_clk){
        machine_clk = 0;
        switch(state_machine){
            case STATE_INITIALIZING:
                task_initializing();
                break;

            case STATE_IDLE:
                task_idle();
                #ifdef CAN_ON
                can_app_task();
                #endif /* CAN_ON */
                break;

            case STATE_RUNNING:
                task_running();
                #ifdef CAN_ON
                can_app_task();
                #endif /* CAN_ON */
                break;

            case STATE_ERROR:
                task_error();
                break;

            case STATE_RESET:
            default:
                task_reset();
                break;
        }
    }
}

/**
* @brief ISR para ações de controle
*/
ISR(TIMER2_COMPA_vect)
{
#ifdef CHRONOMETER_ON
    chronometer_counter++;
#endif //CHRONOMETER_ON

    if(++machine_clk_divider == MACHINE_CLK_DIVIDER_VALUE){
        machine_clk_divider = 0;
       /*if(machine_clk){
            for(;;){
        //        pwm_reset();
                VERBOSE_MSG_ERROR(if(machine_clk) usart_send_string("\n\rERROR: CLOCK CONFLICT!!!\n\r"));
            }
        }*/
        machine_clk = 1;
    }
}
