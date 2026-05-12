
#ifndef _CONFIG_BOARD_H_
#define _CONFIG_BOARD_H_

//PortA
#define WKUP_PORT           GPIOA
#define WKUP_PIN            GPIO_PIN_0
#define WKUP_CONFIG()       gpio_init(WKUP_PORT, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, WKUP_PIN);
#define WKUP_STATE()        gpio_input_bit_get(WKUP_PORT, WKUP_PIN)

#define GRIND_DET_PORT      GPIOA
#define GRIND_DET_PIN       GPIO_PIN_1 // capure by timer1
#define GRIND_DET_CONFIG()  gpio_init(GRIND_DET_PORT, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, GRIND_DET_PIN);
#define GRIND_DET_STATE()   gpio_input_bit_get(GRIND_DET_PORT, GRIND_DET_PIN)

#define UART1_TX_PORT       GPIOA
#define UART1_TX_PIN        GPIO_PIN_2
#define UART1_TX_CONFIG()   gpio_init(UART1_TX_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, UART1_TX_PIN);
#define SetUart1_tx_High()  gpio_bit_set(UART1_TX_PORT, UART1_TX_PIN)
#define ClrUart1_tx_Low()   gpio_bit_reset(UART1_TX_PORT, UART1_TX_PIN)

#define UART1_RX_PORT       GPIOA
#define UART1_RX_PIN        GPIO_PIN_3
#define UART1_RX_CONFIG()   gpio_init(UART1_RX_PORT, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, UART1_RX_PIN);
#define UART1_RX_STATE()    gpio_input_bit_get(UART1_RX_PORT, UART1_RX_PIN)

#define AC_ADC_PORT         GPIOA
#define AC_ADC_PIN          GPIO_PIN_4
#define AC_ADC_CONFIG()     gpio_init(AC_ADC_PORT, GPIO_MODE_AIN, GPIO_OSPEED_MAX, AC_ADC_PIN);
#define AC_ADC_STATE()      gpio_input_bit_get(AC_ADC_PORT, AC_ADC_PIN)

#define RELAY1_PORT         GPIOA
#define RELAY1_PIN          GPIO_PIN_5
#define RELAY1_CONFIG()     gpio_init(RELAY1_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, RELAY1_PIN);
#define SetRelay1_On()      gpio_bit_set(RELAY1_PORT, RELAY1_PIN)
#define ClrRelay1_Off()     gpio_bit_reset(RELAY1_PORT, RELAY1_PIN)

#define RELAY2_PORT         GPIOA
#define RELAY2_PIN          GPIO_PIN_6
#define RELAY2_CONFIG()     gpio_init(RELAY2_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, RELAY2_PIN);
#define SetRelay2_On()      gpio_bit_set(RELAY2_PORT, RELAY2_PIN)
#define ClrRelay2_Off()     gpio_bit_reset(RELAY2_PORT, RELAY2_PIN)

#define GRIND_PORT          GPIOA
#define GRIND_PIN           GPIO_PIN_7
#define GRIND_CONFIG()      gpio_init(GRIND_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GRIND_PIN);
#define SetGrind_On()       gpio_bit_set(GRIND_PORT, GRIND_PIN)
#define ClrGrind_Off()      gpio_bit_reset(GRIND_PORT, GRIND_PIN)
#define ToggleGrind()       gpio_bit_write(GRIND_PORT, GRIND_PIN,\
                               (bit_status)(SET-gpio_output_bit_get(GRIND_PORT, GRIND_PIN)))

#define MOTOR_DET_PORT      GPIOA
#define MOTOR_DET_PIN       GPIO_PIN_8
#define MOTOR_DET_CONFIG()  gpio_init(MOTOR_DET_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_10MHZ, MOTOR_DET_PIN);
#define MOTOR_DET_STATE()   gpio_input_bit_get(MOTOR_DET_PORT, MOTOR_DET_PIN)

#define UART0_TX_PORT       GPIOA
#define UART0_TX_PIN        GPIO_PIN_9
#define UART0_TX_CONFIG()   gpio_init(UART0_TX_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, UART0_TX_PIN);
#define SetUart0_tx_High()  gpio_bit_set(UART0_TX_PORT, UART0_TX_PIN)
#define ClrUart0_tx_Low()   gpio_bit_reset(UART0_TX_PORT, UART0_TX_PIN)

#define UART0_RX_PORT       GPIOA
#define UART0_RX_PIN        GPIO_PIN_10
#define UART0_RX_CONFIG()   gpio_init(UART0_RX_PORT, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, UART0_RX_PIN);
#define UART0_RX_STATE()    gpio_input_bit_get(UART0_RX_PORT, UART0_RX_PIN)

#define BUZZER_PORT         GPIOA
#define BUZZER_PIN          GPIO_PIN_11
#define BUZZER_CONFIG()     gpio_init(BUZZER_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_10MHZ, BUZZER_PIN);
#define SetBUZZER_High()    gpio_bit_set(BUZZER_PORT, BUZZER_PIN)
#define ClrBUZZER_Low()     gpio_bit_reset(BUZZER_PORT, BUZZER_PIN)

#define WATER_DET_PORT      GPIOA
#define WATER_DET_PIN       GPIO_PIN_12
#define WATER_DET_CONFIG()  gpio_init(WATER_DET_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_10MHZ, WATER_DET_PIN);
#define WATER_DET_STATE()   gpio_input_bit_get(WATER_DET_PORT, WATER_DET_PIN)

#define SWDIO_PORT          GPIOA
#define SWDIO_PIN           GPIO_PIN_13
#define SWDIO_CONFIG()      gpio_init(SWDIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_10MHZ, SWDIO_PIN);
#define SWDIO_STATE()       gpio_input_bit_get(SWDIO_PORT, SWDIO_PIN)

#define SWCLK_PORT          GPIOA
#define SWCLK_PIN           GPIO_PIN_14
#define SWCLK_CONFIG()      gpio_init(SWCLK_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_10MHZ, SWCLK_PIN);
#define SWCLK_STATE()       gpio_input_bit_get(SWCLK_PORT, SWCLK_PIN)

#define DBG_TEST4_PORT      GPIOA
#define DBG_TEST4_PIN       GPIO_PIN_15
#define DBG_TEST4_CONFIG()  gpio_init(DBG_TEST4_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, DBG_TEST4_PIN);
#define SetTest4_High()     gpio_bit_set(DBG_TEST4_PORT, DBG_TEST4_PIN)
#define ClrTest4_Low()      gpio_bit_reset(DBG_TEST4_PORT, DBG_TEST4_PIN)
#define ToggleTest4()	    gpio_bit_write(DBG_TEST4_PORT, DBG_TEST4_PIN,\
                               (bit_status)(SET-gpio_output_bit_get(DBG_TEST4_PORT, DBG_TEST4_PIN)))

//PortB
#define ISEN_ADC_PORT       GPIOB
#define ISEN_ADC_PIN        GPIO_PIN_0
#define ISEN_ADC_CONFIG()   gpio_init(ISEN_ADC_PORT, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ISEN_ADC_PIN);
#define ISEN_ADC_STATE()    gpio_input_bit_get(ISEN_ADC_PORT, ISEN_ADC_PIN)

#define NTC_ADC_PORT        GPIOB
#define NTC_ADC_PIN         GPIO_PIN_1
#define NTC_ADC_CONFIG()    gpio_init(NTC_ADC_PORT, GPIO_MODE_AIN, GPIO_OSPEED_MAX, NTC_ADC_PIN);
#define NTC_ADC_STATE()     gpio_input_bit_get(NTC_ADC_PORT, NTC_ADC_PIN)

#define POWER_5V_PORT       GPIOB
#define POWER_5V_PIN        GPIO_PIN_2
#define POWER_5V_CONFIG()   gpio_init(POWER_5V_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, POWER_5V_PIN);
#define SetPower_5V_On()    gpio_bit_set(POWER_5V_PORT, POWER_5V_PIN)
#define ClrPower_5V_Off()   gpio_bit_reset(POWER_5V_PORT, POWER_5V_PIN)

#define NC2_PORT            GPIOB
#define NC2_PIN             GPIO_PIN_3
#define NC2_CONFIG()        gpio_init(NC2_PORT, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, NC2_PIN);
#define NC2_STATE()         gpio_input_bit_get(NC2_PORT, NC2_PIN)

#define METER_DET_PORT      GPIOB
#define METER_DET_PIN       GPIO_PIN_4
#define METER_DET_CONFIG()  gpio_init(METER_DET_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_10MHZ, METER_DET_PIN);
#define METER_DET_STATE()   gpio_input_bit_get(METER_DET_PORT, METER_DET_PIN)

#define P3V_DET_PORT        GPIOB
#define P3V_DET_PIN         GPIO_PIN_5
#define P3V_DET_CONFIG()    gpio_init(P3V_DET_PORT, GPIO_MODE_IPD, GPIO_OSPEED_10MHZ, P3V_DET_PIN);
#define P3V_DET_STATE()     gpio_input_bit_get(P3V_DET_PORT, P3V_DET_PIN)

#define PUMB_PORT           GPIOB
#define PUMB_PIN            GPIO_PIN_6
#define PUMB_CONFIG()       gpio_init(PUMB_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, PUMB_PIN);
#define SetPumb_On()        gpio_bit_set(PUMB_PORT, PUMB_PIN)
#define ClrPumb_Off()       gpio_bit_reset(PUMB_PORT, PUMB_PIN)

#define HEAT_PORT           GPIOB
#define HEAT_PIN            GPIO_PIN_7
#define HEAT_CONFIG()       gpio_init(HEAT_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, HEAT_PIN);
#define SetHeat_On()        gpio_bit_set(HEAT_PORT, HEAT_PIN)
#define ClrHeat_Off()       gpio_bit_reset(HEAT_PORT, HEAT_PIN)

#define ZERO_PORT           GPIOB
#define ZERO_PIN            GPIO_PIN_8
#define ZERO_CONFIG()       gpio_init(ZERO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, ZERO_PIN);
#define SetZero_On()        gpio_bit_set(ZERO_PORT, ZERO_PIN)
#define ClrZero_Off()       gpio_bit_reset(ZERO_PORT, ZERO_PIN)

#define ZERO_DET_PORT       GPIOB
#define ZERO_DET_PIN        GPIO_PIN_9
#define ZERO_DET_CONFIG()   gpio_init(ZERO_DET_PORT, GPIO_MODE_IPU, GPIO_OSPEED_10MHZ, ZERO_DET_PIN);
#define ZERO_DET_STATE()    gpio_input_bit_get(ZERO_DET_PORT, ZERO_DET_PIN)

#define IIC_SCL_PORT        GPIOB
#define IIC_SCL_PIN         GPIO_PIN_10
#define IIC_SCL_CONFIG()    gpio_init(IIC_SCL_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, IIC_SCL_PIN);
#define Setiic_scl_High()   gpio_bit_set(IIC_SCL_PORT, IIC_SCL_PIN)
#define Clriic_scl_Low()    gpio_bit_reset(IIC_SCL_PORT, IIC_SCL_PIN)

#define IIC_SDA_PORT        GPIOB
#define IIC_SDA_PIN         GPIO_PIN_11
#define IIC_SDA_CONFIG()    gpio_init(IIC_SDA_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, IIC_SDA_PIN);
#define Setiic_sda_High()   gpio_bit_set(IIC_SDA_PORT, IIC_SDA_PIN)
#define Clriic_sda_Low()    gpio_bit_reset(IIC_SDA_PORT, IIC_SDA_PIN)

#define POWER_3V_PORT       GPIOB
#define POWER_3V_PIN        GPIO_PIN_12
#define POWER_3V_CONFIG()   gpio_init(POWER_3V_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, POWER_3V_PIN);
#define SetPower_3V_On()    gpio_bit_reset(POWER_3V_PORT, POWER_3V_PIN)
#define ClrPower_3V_Off()   gpio_bit_set(POWER_3V_PORT, POWER_3V_PIN)

#define PALLET_DET_PORT     GPIOB
#define PALLET_DET_PIN      GPIO_PIN_13	// extern 47k pull down
#define PALLET_DET_CONFIG() gpio_init(PALLET_DET_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_10MHZ, PALLET_DET_PIN);
#define PALLET_DET_STATE()  gpio_input_bit_get(PALLET_DET_PORT, PALLET_DET_PIN)

#define MOTOR_IN1_PORT      GPIOB
#define MOTOR_IN1_PIN       GPIO_PIN_14
#define MOTOR_IN1_CONFIG()  gpio_init(MOTOR_IN1_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, MOTOR_IN1_PIN);
#define SetMotor_IN1_High() gpio_bit_set(MOTOR_IN1_PORT, MOTOR_IN1_PIN)
#define ClrMotor_IN1_Low()  gpio_bit_reset(MOTOR_IN1_PORT, MOTOR_IN1_PIN)

#define MOTOR_IN2_PORT      GPIOB
#define MOTOR_IN2_PIN       GPIO_PIN_15
#define MOTOR_IN2_CONFIG()  gpio_init(MOTOR_IN2_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, MOTOR_IN2_PIN);
#define SetMotor_IN2_High() gpio_bit_set(MOTOR_IN2_PORT, MOTOR_IN2_PIN)
#define ClrMotor_IN2_Low()  gpio_bit_reset(MOTOR_IN2_PORT, MOTOR_IN2_PIN)

//PortC
#define DBG_TEST3_PORT      GPIOC
#define DBG_TEST3_PIN       GPIO_PIN_13
#define DBG_TEST3_CONFIG()  gpio_init(DBG_TEST3_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, DBG_TEST3_PIN);
#define SetTest3Pin()       gpio_bit_set(DBG_TEST3_PORT, DBG_TEST3_PIN)
#define ClrTest3Pin()       gpio_bit_reset(DBG_TEST3_PORT, DBG_TEST3_PIN)
#define ToggleTest3Pin()    gpio_bit_write(DBG_TEST3_PORT, DBG_TEST3_PIN,\
                               (bit_status)(SET-gpio_output_bit_get(DBG_TEST3_PORT, DBG_TEST3_PIN)))

#define NC3_PORT            GPIOC
#define NC3_PIN             GPIO_PIN_14
#define NC3_CONFIG()        gpio_init(NC3_PORT, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, NC3_PIN);
#define NC3_STATE()         gpio_input_bit_get(NC3_PORT, NC3_PIN)

#define NC4_PORT            GPIOC
#define NC4_PIN             GPIO_PIN_15
#define NC4_CONFIG()        gpio_init(NC4_PORT, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, NC4_PIN);
#define NC4_STATE()         gpio_input_bit_get(NC4_PORT, NC4_PIN)

//PortD
#define OSCIN_PORT          GPIOD
#define OSCIN_PIN           GPIO_PIN_0
#define OSCIN_CONFIG()      gpio_init(OSCIN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, OSCIN_PIN);
#define SetOscin()          gpio_bit_set(OSCIN_PORT, OSCIN_PIN)
#define ClrOscin()          gpio_bit_reset(OSCIN_PORT, OSCIN_PIN)

#define OSCOUT_PORT         GPIOD
#define OSCOUT_PIN          GPIO_PIN_1
#define OSCOUT_CONFIG()     gpio_init(OSCOUT_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, OSCOUT_PIN);
#define SetOscout()         gpio_bit_set(OSCOUT_PORT, OSCOUT_PIN)
#define ClrOscout()         gpio_bit_reset(OSCOUT_PORT, OSCOUT_PIN)

#endif

