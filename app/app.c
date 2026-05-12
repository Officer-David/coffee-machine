/******************** (C) COPYRIGHT 2017 ST *******************************
* COMPANY:	ST
* DATE:		2024/5
* AUTHOR:	
* IC:		STM32G0B1
*____________________________________________________________________________
* REVISION       V0001		
* SDK Version    SDK_Release_V2.03_0428              
* Date					 		
* Description
*____________________________________________________________________________*/

/*_____ I N C L U D E S ____________________________________________________*/
#define _APP_C_
#include <string.h>
#include "gd32f30x.h"
#include "user_apiopt.h"

/*_____ M A C R O S ______________________________________________________*/
#define _ON_		1u
#define _OFF_		0u

#define MOTOR_FIX_TIME		Ms_300	

/*_____ D E F I N I T I O N S ________________________________________________*/
 uint16_t run_max_time;
 uint8_t Buzzer_times;
 uint8_t Leds_loop_run;

/************ 新增 ***********/
 uint8_t cup_loop;			//杯数循环次数	0：x1，1：x2
 uint8_t turn_off_flag;		//关机标志位
 uint8_t flush_flag;		//冲洗标志位
/*_____ F U N C T I O N S __________________________________________________*/
void app_virtual_croszero_cbfunc(void)
{
	ToggleTest4();
	osal_lyh_over_timer(OS_VIRAC_50HZ, Ms_10, app_virtual_croszero_cbfunc);
}

void app_buzon_cbfunc(void)
{
	if (--Buzzer_times) {
		gd_timer_buz(_ON_);
		osal_lyh_over_timer(OS_TONE_TIME, Ms_80, app_buzoff_cbfunc);
	}
}

void app_buzoff_cbfunc(void)
{
	gd_timer_buz(_OFF_);
	osal_lyh_over_timer(OS_TONE_TIME, Ms_80, app_buzon_cbfunc);
}

void app_buzzer_on(uint8_t time)
{
	if (time) {
		Buzzer_times = time;
		gd_timer_buz(_ON_);
		osal_lyh_over_timer(OS_TONE_TIME, Ms_80, app_buzoff_cbfunc);
	}
	else {
		gd_timer_buz(_OFF_);
		osal_lyh_kill_over_timer(OS_TONE_TIME);
	}
}

void app_resource_init(uint8_t Step)
{
	switch(Step){
	case APP_IDLE_STATE:
		break;
	case APP_STARTUP_STATE:
	case APP_NORMAL_STATE:
		break;
	case APP_ESPRESSO_STATE:
		break;
	case APP_AMERICAN_STATE:
		break;
	case APP_HOTWATER_STATE:
		break;
	case APP_STEAM_STATE:
		break;
	case APP_AQUA_STATE:
		break;
	case APP_CALC_STATE:
		break;
	case APP_SETUP_STATE:
		break;
	case APP_RUNNING_ONOFF_STATE:
		break;
	default:
		break;
	}
}

void app_resource_free(uint8_t Step)
{
	switch(Step){
	case APP_IDLE_STATE:
		break;
	case APP_STARTUP_STATE:
	case APP_NORMAL_STATE:
	case APP_ESPRESSO_STATE:
	case APP_AMERICAN_STATE:
	case APP_HOTWATER_STATE:
	case APP_STEAM_STATE:
	case APP_AQUA_STATE:
	case APP_CALC_STATE:
		osal_lyh_kill_over_timer(OS_LED_TIME);
		break;
	case APP_SETUP_STATE:
		break;
	case APP_RUNNING_ONOFF_STATE:
		osal_lyh_kill_over_timer(OS_LED_TIME);
		break;
	default:
		break;
	}
}

void app_task_nextstep(uint8_t Step, uint8_t suStep)
{
	app_resource_free(app_task_step);
	app_resource_init(Step);
	osal_lyh_restore_task(app_id);
	app_task_substep = suStep;
	app_task_step = Step;
}

void app_task_nextsubstep(uint8_t suStep)
{
	osal_lyh_restore_task(app_id);
	app_task_substep = suStep;
}

uint8_t app_water_ready(void)
{
	return (keyinfo.ulUIOnboardSwitches & WATER_DET_MASK)? TRUE:FAIL;
}

uint8_t app_pallet_ready(void)
{
	return (keyinfo.ulUIOnboardSwitches & PALLET_DET_MASK)? TRUE:FAIL;
}

uint8_t app_meter_ready(void)
{
	return (keyinfo.ulUIOnboardSwitches & METER_DET_MASK)? TRUE:FAIL;
}

uint8_t app_motor_ready(void)
{
	return (!(keyinfo.ulUIOnboardSwitches & MOTOR_DET_MASK))? TRUE:FAIL;
}

void app_turn_heat_off(void)
{
	osal_lyh_set_event(heat_id, HEAT_STOP_EVENT);
}

void app_turn_heat_on(uint16_t target)
{
	osal_lyh_set_event(heat_id, HEAT_START_EVENT);
	target_temp = target;
}

uint8_t app_temperature_state(uint16_t target)
{
	return heat_get_temp(target);
}

uint8_t app_step_state(uint8_t step)
{
	return (app_task_step == step)? TRUE:FAIL;
}

void app_led_heat_cbfunc(void)
{
	uint8_t i, led_id[] = {ESPRESSO1_LED, AMERICANO1_LED, STEAM_LED, HOT_WATER};

	for (i = 0; i<sizeof(led_id); i++){
		display_led(led_id[i], (Leds_loop_run & (1<<i))? LED_ON:LED_OFF);
	}
	Leds_loop_run <<= 1;
	if (Leds_loop_run & (1<<i)) {
		Leds_loop_run++;
	}
	osal_lyh_over_timer(OS_LED_TIME, Ms_500, app_led_heat_cbfunc);
}

void app_led_show_init(void)
{
	Leds_loop_run = 0x03;
	app_leds_all_fun(LED_OFF);
	display_led(ESPRESSO1_LED, LED_ON);
	osal_lyh_over_timer(OS_LED_TIME, Ms_500, app_led_heat_cbfunc);
}

void app_led_start_cbfunc(void)
{
	display_led(START_LED, LED_TOGGLE);
	osal_lyh_over_timer(OS_LED_TIME, Ms_500, app_led_start_cbfunc);
}

void app_led_start_init(void)
{
	display_led(START_LED, LED_ON);
	osal_lyh_over_timer(OS_LED_TIME, Ms_500, app_led_start_cbfunc);
}

void app_led_espresso_cbfunc(void)
{
	//根据杯数显示对应的指示灯
	if(0 == manual_setup.cup){
		display_led(ESPRESSO1_LED, LED_TOGGLE);
	}
	else if(1 == manual_setup.cup){
		display_led(ESPRESSO2_LED, LED_TOGGLE);
	}
	osal_lyh_over_timer(OS_LED_TIME, Ms_500, app_led_espresso_cbfunc);
}

void app_led_espresso_init(void)
{
	//根据杯数显示对应的指示灯
	if(0 == manual_setup.cup){
		display_led(ESPRESSO1_LED, LED_ON);
	}
	else if(1 == manual_setup.cup){
		display_led(ESPRESSO2_LED, LED_ON);
	}
	osal_lyh_over_timer(OS_LED_TIME, Ms_500, app_led_espresso_cbfunc);
}

void app_led_americano_cbfunc(void)
{
	display_led(AMERICANO1_LED, LED_TOGGLE);
	osal_lyh_over_timer(OS_LED_TIME, Ms_1000, app_led_americano_cbfunc);
}

void app_led_americano_init(void)
{
	display_led(AMERICANO1_LED, LED_ON);
	osal_lyh_over_timer(OS_LED_TIME, Ms_1000, app_led_americano_cbfunc);
}

void app_led_hotwater_cbfunc(void)
{
	display_led(HOT_WATER, LED_TOGGLE);
	osal_lyh_over_timer(OS_LED_TIME, Ms_1000, app_led_hotwater_cbfunc);
}

void app_led_hotwater_init(void)
{
	display_led(HOT_WATER, LED_ON);
	osal_lyh_over_timer(OS_LED_TIME, Ms_1000, app_led_hotwater_cbfunc);
}

void app_led_steam_cbfunc(void)
{
	display_led(STEAM_LED, LED_TOGGLE);
	osal_lyh_over_timer(OS_LED_TIME, Ms_1000, app_led_steam_cbfunc);
}

void app_led_steam_init(void)
{
	display_led(STEAM_LED, LED_ON);
	osal_lyh_over_timer(OS_LED_TIME, Ms_1000, app_led_steam_cbfunc);
}

void app_led_aqua_cbfunc(void)
{
	display_led(AQUA_CLEAN_LED, LED_TOGGLE);
	osal_lyh_over_timer(OS_LED_TIME, Ms_1000, app_led_aqua_cbfunc);
}

void app_led_aqua_init(void)
{
	display_led(AQUA_CLEAN_LED, LED_ON);
	osal_lyh_over_timer(OS_LED_TIME, Ms_1000, app_led_aqua_cbfunc);
}

void app_led_calc_cbfunc(void)
{
	display_led(CALC_CLEAN_LED, LED_TOGGLE);
	osal_lyh_over_timer(OS_LED_TIME, Ms_1000, app_led_calc_cbfunc);
}

void app_led_calc_init(void)
{
	display_led(CALC_CLEAN_LED, LED_ON);
	osal_lyh_over_timer(OS_LED_TIME, Ms_1000, app_led_calc_cbfunc);
}

void app_leds_all_fun(uint8_t status)
{
	display_led(STEAM_LED, status);
	display_led(HOT_WATER, status);
	display_led(ESPRESSO1_LED, status);
	display_led(AMERICANO1_LED, status);
	display_led(AQUA_GREEN_LED, status);
	if (status == LED_OFF){
		display_led(ESPRESSO2_LED, status);
		display_led(AMERICANO2_LED, status);
		display_led(AQUA_CLEAN_LED, status);
		display_led(CALC_CLEAN_LED, status);
	}
}

void app_leds_all_ind(uint8_t status)
{
	display_led(COFFEE1_LED, status);
	display_led(COFFEE2_LED, status);
	display_led(COFFEE3_LED, status);
	display_led(COFFEE4_LED, status);
	display_led(WATER1_LED, status);
	display_led(WATER2_LED, status);
	display_led(WATER3_LED, status);
}

uint8_t app_led_oneloop(void)
{
	if (display_get_led(ESPRESSO1_LED) && display_get_led(AMERICANO1_LED))
	{
		return TRUE;
	}
	return FAIL;
}

/**************************************************************************
 * @brief 冲煮流程结束后LED显示处理函数
 * @return none
 * @note 流水灯常量，aqua亮蓝灯，start_led熄灭
 *************************************************************************/
void app_led_finish_step(void)
{
	app_leds_all_fun(LED_OFF);
	app_leds_all_ind(LED_OFF);
	display_led(START_LED, LED_OFF);

	app_leds_all_fun(LED_ON);
}

/**************************************************************************
 * @brief on/off键处理函数
 * @return none
 * @note 1.在INIT和IDLE状态下，按下on/off键进入开机流程STARTUP
 * 		 2.在STARTUP、NORMAL、HOTWATER和STEAM状态下，
 * 		   按下on/off键根据冲洗标志位flush_flag判断是否需要进入冲洗流程
 * 		 3.在冲煮流程ESPRESSO和AMERICANO状态下，按下on/off键根据当前子状态判断对应的on/off流程（AMERICANO未添加）
 *************************************************************************/
void app_onoff_key_deal(void)
{
	switch(app_task_step)
	{
	case APP_INIT_STATE:
	case APP_IDLE_STATE:
		app_task_nextstep(APP_STARTUP_STATE, STARTUP_INIT_STATE);
		break;
	case APP_STARTUP_STATE:
	case APP_NORMAL_STATE:
		/* 判断是否需要冲洗 */
		if (1 == flush_flag) {														//需要冲洗	
			app_task_nextstep(APP_RUNNING_ONOFF_STATE, RUNNING_ONOFF_FLUSH);		//切换冲洗状态
			app_leds_all_fun(LED_OFF);			
			app_leds_all_ind(LED_OFF);			
			display_led(START_LED, LED_OFF);
			app_led_show_init();													//只开启流水灯
																					//其他灯熄灭
		}
		else {																		//不需要冲洗
			app_task_nextstep(APP_SETUP_STATE, SETUP_INIT_STATE);					//切换到SETUP状态
		}
		break;
	case APP_ESPRESSO_STATE:
		switch (app_task_substep)
		{
			/* 萃取电机位置检测与研磨过程中触发on/off */
			case ESPRESSO_INIT_STATE:
			case ESPRESSO_MOTO_CHECK_STATE:
			case ESPRESSO_MOTO_CHECK_JUDGE_STATE:
			case ESPRESSO_MOTO_FIRST_RST_STATE:
			case ESPRESSO_MOTO_CHECK_COMPLETE_STATE:
			case ESPRESSO_GRIND_TIME_JUDGE_STATE:
			case ESPRESSO_GRIND_STATE:
				app_task_nextstep(APP_RUNNING_ONOFF_STATE, RUNNING_ONOFF_GRIND_STATE);
				break;
			/* 前往接水过程中触发on/off */
			case ESPRESSO_MOTO_GO_BREW_STATE:
			case ESPRESSO_MOTO_GO_BREW_JUDGE_STATE:	
				app_task_nextstep(APP_RUNNING_ONOFF_STATE, RUNNING_ONOFF_GO_WATER_STATE);
				break;
			/* 预浸泡过程中触发on/off */
			case ESPRESSO_PRE_BREW_STATE:
			case ESPRESSO_WAIT_BREW_STATE:				
				app_task_nextstep(APP_RUNNING_ONOFF_STATE, RUNNING_ONOFF_PRE_BREW_STATE);
				break;
			/* 冲煮过程中触发on/off */
			case ESPRESSO_BREW_STATE:
			case ESPRESSO_BREW_FINISH_STATE:				
				app_task_nextstep(APP_RUNNING_ONOFF_STATE, RUNNING_ONOFF_BREW_STATE);
				break;
			/* 冲煮完毕后触发on/off */
			case ESPRESSO_GO_DROP_COFFEE_GROUNDS_STATE:
			case ESPRESSO_DROP_COFFEE_GROUNDS_STATE:
			case ESPRESSO_BACK_STATE:
			case ESPRESSO_INPLACE_JUDGE_STATE:
			case ESPRESSO_INPLACE_STATE:
			case ESPRESSO_MOTO_SECOND_RST_STATE:
			case ESPRESSO_JUDGE_TWO_CUP_STATE:
			case ESPRESSO_COMPLETE_STATE:				
				cup_loop = 0;			//取消第二杯的循环，不强制停止第一杯的倒粉、萃取电机复位操作
				turn_off_flag = 1;		//设置关机标志位，流程结束后执行关机
				app_led_show_init();	//流水灯初始化
				break;
			default:		
				break;
		}
		break;
	case APP_AMERICAN_STATE:
		/* 萃取电机位置检测与研磨过程中触发on/off */
			//app_task_nextstep(APP_RUNNING_ONOFF_STATE, RUNNING_ONOFF_GRIND_STATE);

		/* 前往接水过程中触发on/off */
			//app_task_nextstep(APP_RUNNING_ONOFF_STATE, RUNNING_ONOFF_GO_WATER_STATE);

		/* 预浸泡过程中触发on/off */
			//app_task_nextstep(APP_RUNNING_ONOFF_STATE, RUNNING_ONOFF_PRE_BREW_STATE);

		/* 冲煮过程中触发on/off */
			//app_task_nextstep(APP_RUNNING_ONOFF_STATE, RUNNING_ONOFF_BREW_STATE);

		/* 第一次萃取完成后触发on/off（倒粉、第二次去接水等剩余状态的处理方式都一致） */
			// cup_loop = 0;			
			// turn_off_flag = 1;		
			// app_led_show_init();	
				
		break;	
	case APP_HOTWATER_STATE:
	case APP_STEAM_STATE:
		/* 判断是否需要冲洗 */
		if (1 == flush_flag) {													//需要冲洗	
			app_task_nextstep(APP_RUNNING_ONOFF_STATE, RUNNING_ONOFF_FLUSH);	//切换冲洗状态
			app_leds_all_fun(LED_OFF);
			app_leds_all_ind(LED_OFF);
			display_led(START_LED, LED_OFF);
			app_led_show_init();												//只开启流水灯
																				//其他灯熄灭
		}
		else {																	//不需要冲洗
			app_task_nextstep(APP_SETUP_STATE, SETUP_INIT_STATE);				//切换到SETUP状态
		}
		break;
	default:
		break;
	}
}

uint8_t app_start_key_deal(void)
{
	uint8_t ret = 0;

	switch(app_task_step)
	{
		case APP_NORMAL_STATE:
			switch(app_task_substep){
			case NORMAL_ESPRESSO_RUN_STATE:
				app_task_nextstep(APP_ESPRESSO_STATE, ESPRESSO_INIT_STATE);
				ret = 1;
				break;
			case NORMAL_AMERICANO_RUN_STATE:
				app_task_nextstep(APP_AMERICAN_STATE, AMERICAN_INIT_STATE);
				ret = 1;
				break;
			case NORMAL_HOTWATER_RUN_STATE:
				app_task_nextstep(APP_HOTWATER_STATE, HOTWATER_INIT_STATE);
				ret = 1;
				break;
			case NORMAL_STEAM_RUN_STATE:
				app_task_nextstep(APP_STEAM_STATE, STEAM_INIT_STATE);
				ret = 1;
				break;
			case NORMAL_AQUA_RUN_STATE:
				app_task_nextstep(APP_AQUA_STATE, AQUA_INIT_STATE);
				ret = 1;
				break;
			case NORMAL_CALC_RUN_STATE:
				app_task_nextstep(APP_CALC_STATE, CALC_INIT_STATE);
				ret = 1;
				break;
			default:
				break;
			}
			break;
	
		case APP_ESPRESSO_STATE:
			cup_loop = 0;	//将杯数循环清除，只执行本次冲煮流程
			break;

		default:
			break;
	}
	return ret;
}

uint8_t app_hotwater_key_deal(void)
{
	uint8_t ret = 0;

	switch(app_task_step)
	{
	case APP_NORMAL_STATE:
		switch(app_task_substep){
		case NORMAL_HOTWATER_INIT_STATE:
		case NORMAL_HOTWATER_RUN_STATE:
			break;
		default:
			app_task_nextsubstep(NORMAL_HOTWATER_INIT_STATE);
			break;
		}
		ret = 1;
		break;
	default:
		break;
	}
	return ret;
}

uint8_t app_espresso_key_deal(void)
{
	uint8_t ret = 0;

	switch(app_task_step)
	{
	case APP_NORMAL_STATE:
		switch(app_task_substep){
		case NORMAL_ESPRESSO_INIT_STATE:
		case NORMAL_ESPRESSO_RUN_STATE:
			manual_setup.cup++;
			manual_setup.cup &= 0x01;
			display_led(ESPRESSO1_LED, LED_OFF);
			display_led(ESPRESSO2_LED, LED_OFF);
			if (manual_setup.cup){
				display_led(ESPRESSO2_LED, LED_ON);
			}
			else{
				display_led(ESPRESSO1_LED, LED_ON);
			}
			break;
		default:
			app_task_nextsubstep(NORMAL_ESPRESSO_INIT_STATE);
			break;
		}
		ret = 1;
		break;
	default:
		break;
	}
	return ret;
}

uint8_t app_steam_key_deal(void)
{
	uint8_t ret = 0;

	switch(app_task_step)
	{
	case APP_NORMAL_STATE:
		switch(app_task_substep){
		case NORMAL_STEAM_INIT_STATE:
		case NORMAL_STEAM_RUN_STATE:
			break;
		default:
			app_task_nextsubstep(NORMAL_STEAM_INIT_STATE);
			break;
		}
		ret = 1;
		break;
	default:
		break;
	}
	return ret;
}

uint8_t app_ame_key_deal(void)
{
	uint8_t ret = 0;

	switch(app_task_step)
	{
	case APP_NORMAL_STATE:
		switch(app_task_substep){
		case NORMAL_AMERICANO_INIT_STATE:
		case NORMAL_AMERICANO_RUN_STATE:
			manual_setup.cup++;
			manual_setup.cup &= 0x01;
			display_led(AMERICANO1_LED, LED_OFF);
			display_led(AMERICANO2_LED, LED_OFF);
			if (manual_setup.cup){
				display_led(AMERICANO2_LED, LED_ON);
			}
			else{
				display_led(AMERICANO1_LED, LED_ON);
			}
			break;
		default:
			app_task_nextsubstep(NORMAL_AMERICANO_INIT_STATE);
			break;
		}
		ret = 1;
		break;
	default:
		break;
	}
	return ret;
}

uint8_t app_coffee_key_deal(void)
{
	uint8_t ret = 0;

	switch(app_task_step)
	{
	case APP_NORMAL_STATE:
		switch(app_task_substep){
		case NORMAL_AMERICANO_RUN_STATE:
		case NORMAL_ESPRESSO_RUN_STATE:
			display_led(COFFEE4_LED, LED_OFF);
			display_led(COFFEE3_LED, LED_OFF);
			manual_setup.coffee++;
			switch(manual_setup.coffee){
			case 3:
				manual_setup.coffee = 0;
				break;
			case 2:
				display_led(COFFEE4_LED, LED_ON);
			case 1:
				display_led(COFFEE3_LED, LED_ON);
				break;
			default:
				break;
			}
			ret = 1;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return ret;
}

uint8_t app_water_key_deal(void)
{
	uint8_t ret = 0;

	switch(app_task_step)
	{
	case APP_NORMAL_STATE:
		switch(app_task_substep){
		case NORMAL_AMERICANO_RUN_STATE:
		case NORMAL_ESPRESSO_RUN_STATE:
		case NORMAL_HOTWATER_RUN_STATE:
			display_led(WATER3_LED, LED_OFF);
			display_led(WATER2_LED, LED_OFF);
			manual_setup.water++;
			switch(manual_setup.water){
			case 3:
				manual_setup.water = 0;
				break;
			case 2:
				display_led(WATER3_LED, LED_ON);
			case 1:
				display_led(WATER2_LED, LED_ON);
				break;
			default:
				break;
			}
			ret = 1;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return ret;
}

uint8_t app_calc_key_deal(void)
{
	uint8_t ret = 0;

	switch(app_task_step)
	{
	case APP_NORMAL_STATE:
		switch(app_task_substep){
		case NORMAL_CALC_INIT_STATE:
		case NORMAL_CALC_RUN_STATE:
			break;
		default:
			app_task_nextsubstep(NORMAL_CALC_INIT_STATE);
			break;
		}
		ret = 1;
		break;
	default:
		break;
	}
	return ret;
}

uint8_t app_aqua_key_deal(void)
{
	uint8_t ret = 0;

	switch(app_task_step)
	{
	case APP_NORMAL_STATE:
		switch(app_task_substep){
		case NORMAL_AQUA_INIT_STATE:
		case NORMAL_AQUA_RUN_STATE:
			break;
		default:
			app_task_nextsubstep(NORMAL_AQUA_INIT_STATE);
			break;
		}
		ret = 1;
		break;
	default:
		break;
	}
	return ret;
}


void app_pad_key_deal(void)
{
	switch(app_task_step)
	{
	case APP_NORMAL_STATE:
		break;
	default:
		break;
	}
}

/************************************************************************
* function name: app_key_handler
* input: non
* output: non
* description: this object's message loop
*************************************************************************/
void app_key_handler(_keystruct* pkey)
{
	if (pkey->ulLPressF & KEY_ONOFF_MASK){
		app_buzzer_on(1);
	}
	if (pkey->ulReleaseF & KEY_ONOFF_MASK){
		if (pkey->ulLPressMaskF & KEY_ONOFF_MASK){
			app_onoff_key_deal();
		}
		else if (FAIL == app_step_state(APP_IDLE_STATE)){
			app_buzzer_on(1);
			app_onoff_key_deal();
		}
		pkey->ulLPressMaskF &= ~KEY_ONOFF_MASK;
	}
	
	if (pkey->ulReleaseF & KEY_START_MASK) {
		if (app_start_key_deal()){
			app_buzzer_on(1);
		}
	}

	if (pkey->ulReleaseF & KEY_ESPRESSO_MASK) {
		if (app_espresso_key_deal()){
			app_buzzer_on(1);
		}
	}
	
	if (pkey->ulReleaseF & KEY_AMERICANO_MASK) {
		if (app_ame_key_deal()){
			app_buzzer_on(1);
		}
	}

	if (pkey->ulReleaseF & KEY_HOTWATER_MASK) {
		if (app_hotwater_key_deal()){
			app_buzzer_on(1);
		}
	}

	if (pkey->ulReleaseF & KEY_STEAM_MASK) {
		if (app_steam_key_deal()){
			app_buzzer_on(1);
		}
	}
	
	if (pkey->ulReleaseF & KEY_COFFE_MASK) {
		if (app_coffee_key_deal()){
			app_buzzer_on(1);
		}
	}
	
	if (pkey->ulReleaseF & KEY_WATER_MASK) {
		if (app_water_key_deal()){
			app_buzzer_on(1);
		}
	}

	if (pkey->ulLPressF & KEY_CALC_MASK) {
		if (app_calc_key_deal()){
			app_buzzer_on(1);
		}
	}

	if (pkey->ulLPressF & KEY_AQUA_MASK) {
		if (app_aqua_key_deal()){
			app_buzzer_on(1);
		}
	}
	
	if (pkey->ulReleaseF & KEY_PAD_MASK) {
		app_pad_key_deal();
	}
}

/************************************************************************
* function name: app_init_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_init_handler(void)
{
	switch(app_task_substep)
	{
		case INIT_STATE:
			/* add init code here */
			osal_lyh_over_timer(OS_VIRAC_50HZ, Ms_10, app_virtual_croszero_cbfunc);
			app_task_substep = INIT_READY_STATE;
			break;
		case INIT_READY_STATE:
			osal_lyh_delay_task(Ms_2);
			app_task_substep = INIT_END_STATE;
			break;
		case INIT_END_STATE:
			app_task_nextstep(APP_IDLE_STATE, IDLE_INIT_STATE);
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: app_idle_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_idle_handler(void)
{
	switch(app_task_substep)
	{
		case IDLE_INIT_STATE:
			display_init();
			app_turn_heat_off();
			app_task_substep = IDLE_RUN_STATE;
			break;
		case IDLE_RUN_STATE:
			osal_lyh_delay_task(Ms_500);
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: app_startup_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_startup_handler(void)
{
	switch(app_task_substep)
	{
		case STARTUP_INIT_STATE:
			app_task_substep = STARTUP_ENTRY_STATE;
			if (FAIL == app_pallet_ready()){
				display_led(PALLET_ERR_LED, LED_ON);
				app_task_substep = STARTUP_FAIL_STATE;
			}
			if (FAIL == app_water_ready()){
				display_led(WATER_ERR_LED, LED_ON);
				app_task_substep = STARTUP_FAIL_STATE;
			}
			break;
		case STARTUP_ENTRY_STATE:
			app_led_show_init();
			dc_motor_ClearFault();
			dc_motor_Start(MOTOR_RIGHT);
			key_prohibit_setup(KEY_ONOFF_MASK, TRUE);
			app_task_substep = STARTUP_RIGHT_RST_STATE;
			run_max_time = RIGHT_TIMER(Ms_2000, Ms_100);
			osal_lyh_delay_task(Ms_100);
			break;
		case STARTUP_RIGHT_RST_STATE:
			if ((MOTOR_STATE_STOPED == dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				osal_lyh_delay_task(Ms_200);
				app_task_substep = STARTUP_LEFT_STATE;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		case STARTUP_LEFT_STATE:
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			app_task_substep = STARTUP_LEFT_RST_STATE;
			break;
		case STARTUP_LEFT_RST_STATE:
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel) 
				&& ((MOTOR_STATE_STOPED == dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				key_prohibit_setup(KEY_ONOFF_MASK, FAIL);
				app_task_substep = STARTUP_HEAT_RDY_STATE;
			}
			osal_lyh_delay_task(Ms_100);
			break;
		case STARTUP_HEAT_RDY_STATE:
			if (TRUE == app_temperature_state(TARGET_TEMP_90D)){
				app_task_substep = STARTUP_COMPLETE_STATE;
			}
			else {
				app_turn_heat_on(TARGET_TEMP_90D);
				run_max_time = HEAT_MAX_TIMER(Ms_100);
				app_task_substep = STARTUP_HEAT_ING_STATE;
			}
			osal_lyh_delay_task(Ms_100);
			break;
		case STARTUP_HEAT_ING_STATE:
			if ((TRUE == app_temperature_state(TARGET_TEMP_90D))
				|| (--run_max_time == 0)){
				display_led(START_LED, LED_ON);
				app_task_substep = STARTUP_HEAT_END_STATE;
			}
			osal_lyh_delay_task(Ms_100);
			break;
		case STARTUP_HEAT_END_STATE:
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(Ms_6000, Ms_100);
			app_task_substep = STARTUP_LEFT_MOVE_STATE;
			break;
		case STARTUP_LEFT_MOVE_STATE:
			if ((MOTOR_STATE_STOPED == dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				app_task_substep = STARTUP_WATER_RDY_STATE;
			}
			osal_lyh_delay_task(Ms_100);
			break;
		case STARTUP_WATER_RDY_STATE:
			ac_pumb_start(CONTINUE_MODE);
			run_max_time = WATER_MAX_TIMER(Ms_100);
			app_task_substep = STARTUP_WATER_ING_STATE;
			break;
		case STARTUP_WATER_ING_STATE:
			if (--run_max_time == 0){
				ac_pumb_stop();
				osal_lyh_delay_task(Ms_6000);
				app_task_substep = STARTUP_WATER_END_STATE;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		case STARTUP_WATER_END_STATE:
			key_prohibit_setup(KEY_ONOFF_MASK, TRUE);
			osal_lyh_delay_task(Ms_500);	// wait motor to move the detecting point
			dc_motor_Start(MOTOR_RIGHT);
			run_max_time = RIGHT_TIMER(Ms_6000, Ms_100);
			app_task_substep = STARTUP_RIGHT_MOVE_STATE;
			break;
		case STARTUP_RIGHT_MOVE_STATE:
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				osal_lyh_delay_task(Ms_200);
				app_task_substep = STARTUP_LEFT_START_STATE;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		case STARTUP_LEFT_START_STATE:
			display_led(START_LED, LED_OFF);
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			app_task_substep = STARTUP_LEFT_END_STATE;
			break;
		case STARTUP_LEFT_END_STATE:
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel)
				&& ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				app_task_substep = STARTUP_COMPLETE_STATE;
			}
			osal_lyh_delay_task(Ms_100);
			break;
		case STARTUP_COMPLETE_STATE:
			key_prohibit_setup(KEY_ONOFF_MASK, FAIL);
			if (TRUE == app_led_oneloop()){
				app_leds_all_fun(LED_ON);
				osal_lyh_kill_over_timer(OS_LED_TIME);
				app_task_nextstep(APP_NORMAL_STATE, NORMAL_INIT_STATE);
			}
			break;
		case STARTUP_FAIL_STATE:
			if (TRUE == app_pallet_ready()){
				display_led(PALLET_ERR_LED, LED_OFF);
				app_task_substep = STARTUP_INIT_STATE;
			}
			if (TRUE == app_water_ready()){
				display_led(WATER_ERR_LED, LED_OFF);
				app_task_substep = STARTUP_INIT_STATE;
			}
			osal_lyh_delay_task(Ms_50);
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: app_normal_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_normal_handler(void)
{
	switch(app_task_substep)
	{
		case NORMAL_INIT_STATE:
			osal_lyh_delay_task(Ms_10);
			break;
		case NORMAL_ESPRESSO_INIT_STATE:
			memset((uint8_t *)&manual_setup, 0, sizeof(st_opDef));
			app_led_start_init();
			app_leds_all_fun(LED_OFF);
			app_leds_all_ind(LED_OFF);
			display_led(ESPRESSO1_LED, LED_ON);
			display_led(COFFEE2_LED, LED_ON);
			display_led(WATER1_LED, LED_ON);
			app_task_substep = NORMAL_ESPRESSO_RUN_STATE;
			break;
		case NORMAL_ESPRESSO_RUN_STATE:
			osal_lyh_delay_task(Ms_10);
			break;
		case NORMAL_AMERICANO_INIT_STATE:
			memset((uint8_t *)&manual_setup, 0, sizeof(st_opDef));
			app_led_start_init();
			app_leds_all_fun(LED_OFF);
			app_leds_all_ind(LED_OFF);
			display_led(AMERICANO1_LED, LED_ON);
			display_led(COFFEE2_LED, LED_ON);
			display_led(WATER1_LED, LED_ON);
			app_task_substep = NORMAL_AMERICANO_RUN_STATE;
			break;
		case NORMAL_AMERICANO_RUN_STATE:
			osal_lyh_delay_task(Ms_10);
			break;
		case NORMAL_HOTWATER_INIT_STATE:
			memset((uint8_t *)&manual_setup, 0, sizeof(st_opDef));
			manual_setup.water = 2;
			app_led_start_init();
			app_leds_all_fun(LED_OFF);
			app_leds_all_ind(LED_OFF);
			display_led(HOT_WATER, LED_ON);
			display_led(WATER1_LED, LED_ON);
			display_led(WATER2_LED, LED_ON);
			display_led(WATER3_LED, LED_ON);
			app_task_substep = NORMAL_HOTWATER_RUN_STATE;
			break;
		case NORMAL_HOTWATER_RUN_STATE:
			osal_lyh_delay_task(Ms_10);
			break;
		case NORMAL_STEAM_INIT_STATE:
			memset((uint8_t *)&manual_setup, 0, sizeof(st_opDef));
			app_led_start_init();
			app_leds_all_fun(LED_OFF);
			app_leds_all_ind(LED_OFF);
			display_led(STEAM_LED, LED_ON);
			app_task_substep = NORMAL_STEAM_RUN_STATE;
			break;
		case NORMAL_STEAM_RUN_STATE:
			osal_lyh_delay_task(Ms_10);
			break;
		case NORMAL_AQUA_INIT_STATE:
			app_led_start_init();
			app_leds_all_fun(LED_OFF);
			app_leds_all_ind(LED_OFF);
			display_led(AQUA_CLEAN_LED, LED_ON);
			app_task_substep = NORMAL_AQUA_RUN_STATE;
			break;
		case NORMAL_AQUA_RUN_STATE:
			osal_lyh_delay_task(Ms_10);
			break;
		case NORMAL_CALC_INIT_STATE:
			app_led_start_init();
			app_leds_all_fun(LED_OFF);
			app_leds_all_ind(LED_OFF);
			display_led(CALC_CLEAN_LED, LED_ON);
			app_task_substep = NORMAL_CALC_RUN_STATE;
			break;
		case NORMAL_CALC_RUN_STATE:
			osal_lyh_delay_task(Ms_10);
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: app_espresso_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_espresso_handler(void)
{
	switch(app_task_substep)
	{
		/* 初始化状态 */
		case ESPRESSO_INIT_STATE:
			app_leds_all_fun(LED_OFF);
			app_leds_all_ind(LED_OFF);
			display_led(START_LED, LED_ON);
			app_led_espresso_init();		//初始化espresso指示灯闪烁
			if(1 == manual_setup.cup){		//判断本次制作杯数
				cup_loop = 1;
			}
			else{
				cup_loop = 0;
			}
			app_task_substep = ESPRESSO_MOTO_CHECK_STATE;
			break;
		/* 萃取电机位置检测 */
		case ESPRESSO_MOTO_CHECK_STATE:
			dc_motor_ClearFault();		
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = RIGHT_TIMER(Ms_3000, Ms_100);
			app_task_substep = ESPRESSO_MOTO_CHECK_JUDGE_STATE;
			break;
		/* 判断萃取电机是否触碰原点触点 */
		case ESPRESSO_MOTO_CHECK_JUDGE_STATE:
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				app_task_substep = ESPRESSO_MOTO_FIRST_RST_STATE;					
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		/* 萃取电机向回转到原点 */
		case ESPRESSO_MOTO_FIRST_RST_STATE:
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			app_task_substep = ESPRESSO_MOTO_CHECK_COMPLETE_STATE;
			break;
		/* 判断萃取电机是否完成回转 */
		case ESPRESSO_MOTO_CHECK_COMPLETE_STATE:
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel)
				&& ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				app_task_substep = ESPRESSO_GRIND_TIME_JUDGE_STATE;					
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		/* 研磨咖啡豆 */
		case ESPRESSO_GRIND_TIME_JUDGE_STATE:
			ac_grind_start(1000);		//启动研磨电机
			if(0 == manual_setup.coffee)	//研磨时间根据coffee程度调整：x1=8s,x2=9s,x3=10s
			{
				run_max_time = GRIND_TIMER(Ms_8000, Ms_100);
				app_task_substep = ESPRESSO_GRIND_STATE;
			}
			else if (1 == manual_setup.coffee)
			{
				run_max_time = GRIND_TIMER(Ms_9000, Ms_100);
				app_task_substep = ESPRESSO_GRIND_STATE;	
			}			
			else if(2 == manual_setup.coffee)
			{
				run_max_time = GRIND_TIMER(Ms_10000, Ms_100);
				app_task_substep = ESPRESSO_GRIND_STATE;	
			}
			break;
		case ESPRESSO_GRIND_STATE:
			if (--run_max_time == 0){	//判断研磨时间是否到达
				ac_grind_stop();
				osal_lyh_delay_task(Ms_1000);
				app_task_substep = ESPRESSO_MOTO_GO_BREW_STATE;	
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		/* 萃取电机转到萃取位置 */
		case ESPRESSO_MOTO_GO_BREW_STATE:
			dc_motor_Start(MOTOR_LEFT);		
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(Ms_7000, Ms_100);
			app_task_substep = ESPRESSO_MOTO_GO_BREW_JUDGE_STATE;
			break;
		/* 判断萃取电机是否到达萃取位置 */
		case ESPRESSO_MOTO_GO_BREW_JUDGE_STATE:
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				app_task_substep = ESPRESSO_PRE_BREW_STATE;	
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}			
			break;
		/* 预冲煮状态 */
		case ESPRESSO_PRE_BREW_STATE:
			ac_pumb_start(CONTINUE_MODE);		//启动水泵
			osal_lyh_delay_task(Ms_2000 - Ms_100);
			app_task_substep = ESPRESSO_WAIT_BREW_STATE;	
			break;
		case ESPRESSO_WAIT_BREW_STATE:
			ac_pumb_stop();						//停止水泵
			osal_lyh_delay_task(Ms_4000);
			app_task_substep = ESPRESSO_BREW_STATE;	
			break;
		/* 冲煮状态 */
		case ESPRESSO_BREW_STATE:			
			if(0 == manual_setup.water)		//根据杯量决定抽水时间：x1=5s,x2=10s,x3=20s
			{
				ac_pumb_start(CONTINUE_MODE);
				osal_lyh_delay_task(Ms_5000);
				app_task_substep = ESPRESSO_BREW_FINISH_STATE;	
			}
			else if(1 == manual_setup.water)
			{
				ac_pumb_start(CONTINUE_MODE);
				osal_lyh_delay_task(Ms_10000);
				app_task_substep = ESPRESSO_BREW_FINISH_STATE;	

			}
			else if(2 == manual_setup.water)
			{
				ac_pumb_start(CONTINUE_MODE);
				osal_lyh_delay_task(Ms_10000 + Ms_10000);
				app_task_substep = ESPRESSO_BREW_FINISH_STATE;	
			}
			break;
		case ESPRESSO_BREW_FINISH_STATE:	//冲煮完成
			ac_pumb_stop();		//停止水泵	
			osal_lyh_delay_task(Ms_6000);
			app_task_substep = ESPRESSO_GO_DROP_COFFEE_GROUNDS_STATE;	
			break;
		/* 丢咖啡渣状态 */
		case ESPRESSO_GO_DROP_COFFEE_GROUNDS_STATE:
			dc_motor_Start(MOTOR_RIGHT);	//电机反转2.9s， 到丢咖啡渣位置
			osal_lyh_delay_task(Ms_3000 - Ms_100);
			app_task_substep = ESPRESSO_DROP_COFFEE_GROUNDS_STATE;	
			break;
		case ESPRESSO_DROP_COFFEE_GROUNDS_STATE:
			dc_motor_Stop();
			osal_lyh_delay_task(Ms_3000);
			app_task_substep = ESPRESSO_BACK_STATE;
			break;
		/* 萃取电机返回原位 */
		case ESPRESSO_BACK_STATE:		//电机向原位转动
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = RIGHT_TIMER(Ms_5000, Ms_100);
			app_task_substep = ESPRESSO_INPLACE_JUDGE_STATE;
			break;	
		case ESPRESSO_INPLACE_JUDGE_STATE:			//判断电机是否到原点触点
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				app_task_substep = ESPRESSO_INPLACE_STATE;					
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		case ESPRESSO_INPLACE_STATE:			//电机回转到原位
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			app_task_substep = ESPRESSO_MOTO_SECOND_RST_STATE;
			break;	
		case ESPRESSO_MOTO_SECOND_RST_STATE:	//判断电机是否回转到原位
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel)
				&& ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				app_task_substep = ESPRESSO_JUDGE_TWO_CUP_STATE;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		/* 判断是否继续制作第二杯 */
		case ESPRESSO_JUDGE_TWO_CUP_STATE:
			if (1 == cup_loop){
				app_task_substep = ESPRESSO_MOTO_CHECK_STATE;	//继续制作第二杯
				cup_loop = 0;
				osal_lyh_delay_task(Ms_500);
			}
			else {
				app_task_substep = ESPRESSO_COMPLETE_STATE;		//制作完成
			}
			break;
		/* 制作完成状态 */
		case ESPRESSO_COMPLETE_STATE:
			if(1 == turn_off_flag){		//判断有无关机指令
				turn_off_flag = 0;		//消耗掉关机指令
				app_task_nextstep(APP_RUNNING_ONOFF_STATE, RUNNING_ONOFF_FLUSH);		//进入清洗状态
			}
			else {
				osal_lyh_kill_over_timer(OS_LED_TIME);		//杀死espresso指示灯闪烁定时器
				app_led_finish_step();						//执行完成后的指示灯显示
				flush_flag = 1;								//设置冲洗标志	
				app_task_nextstep(APP_NORMAL_STATE, NORMAL_INIT_STATE);		//回到normal状态，等待下一次操作
			}
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: app_american_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_american_handler(void)
{
	switch(app_task_substep)
	{
		case AMERICAN_INIT_STATE:
			display_led(START_LED, LED_ON);
			app_led_americano_init();
			osal_lyh_delay_task(Ms_10);
			app_task_substep = AMERICAN_RIGHT_STATE;
			break;
		case AMERICAN_RIGHT_STATE:
			dc_motor_Start(MOTOR_RIGHT);
			key_prohibit_setup(KEY_ONOFF_MASK, TRUE);
			app_task_substep = AMERICAN_RIGHT_RST_STATE;
			run_max_time = RIGHT_TIMER(Ms_2000, Ms_100);
			osal_lyh_delay_task(Ms_100);
			break;
		case AMERICAN_RIGHT_RST_STATE:
			if ((MOTOR_STATE_STOPED == dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				osal_lyh_delay_task(Ms_200);
				app_task_substep = AMERICAN_LEFT_STATE;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		case AMERICAN_LEFT_STATE:
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			app_task_substep = AMERICAN_LEFT_RST_STATE;
			break;
		case AMERICAN_LEFT_RST_STATE:
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel) 
				&& ((MOTOR_STATE_STOPED == dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				key_prohibit_setup(KEY_ONOFF_MASK, FAIL);
				app_task_substep = AMERICAN_GRIND_INIT_STATE;
			}
			osal_lyh_delay_task(Ms_100);
			break;
		case AMERICAN_GRIND_INIT_STATE:
			app_turn_heat_off();
			ac_grind_start(1000);
			osal_lyh_delay_task(Ms_8000);
			if (manual_setup.coffee == 1){
				osal_lyh_delay_task(Ms_9000);
			}
			else if (manual_setup.coffee == 2){
				osal_lyh_delay_task(Ms_10000);
			}
			app_task_substep = AMERICAN_GRIND_RUN_STATE;
			break;
		case AMERICAN_GRIND_RUN_STATE:
			ac_grind_stop();
			app_turn_heat_on(TARGET_TEMP_100D);
			osal_lyh_delay_task(Ms_100);
			app_task_substep = AMERICAN_GRIND_END_STATE;
			break;
		case AMERICAN_GRIND_END_STATE:
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(Ms_6000, Ms_100);
			app_task_substep = AMERICAN_LEFT_MOVE_STATE;
			break;
		case AMERICAN_LEFT_MOVE_STATE:
			if ((MOTOR_STATE_STOPED == dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				app_task_substep = AMERICAN_PRECOFFEE_RDY_STATE;
			}
			osal_lyh_delay_task(Ms_100);
			break;
		case AMERICAN_PRECOFFEE_RDY_STATE:
			ac_pumb_start(CONTINUE_MODE);
			run_max_time = PRECOFFEE_MAX_TIMER(Ms_100);
			app_task_substep = AMERICAN_PRECOFFEE_ING_STATE;
			break;
		case AMERICAN_PRECOFFEE_ING_STATE:
			if (--run_max_time == 0){
				ac_pumb_stop();
				app_task_substep = AMERICAN_PRECOFFEE_END_STATE;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		case AMERICAN_PRECOFFEE_END_STATE:
			osal_lyh_delay_task(Ms_2000 - Ms_200);		// stop 1800ms
			app_task_substep = AMERICAN_COFFEE_RDY_STATE;
			break;
		case AMERICAN_COFFEE_RDY_STATE:
			ac_pumb_start(CONTINUE_MODE);
			run_max_time = COFFEE_MAX_TIMER(Ms_100);
			app_task_substep = AMERICAN_COFFEE_ING_STATE;
			break;
		case AMERICAN_COFFEE_ING_STATE:
			if (--run_max_time == 0){
				ac_pumb_stop();
				osal_lyh_delay_task(Ms_6000);
				app_task_substep = AMERICAN_COFFEE_END_STATE;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		case AMERICAN_COFFEE_END_STATE:
			key_prohibit_setup(KEY_ONOFF_MASK, TRUE);
			osal_lyh_delay_task(Ms_500);	// wait motor to move the detecting point
			dc_motor_Start(MOTOR_RIGHT);
			run_max_time = RIGHT_TIMER((Ms_3000 - Ms_100), Ms_100);
			app_task_substep = AMERICAN_RIGHT_SEG1_STATE;
			break;
		case AMERICAN_RIGHT_SEG1_STATE:
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				osal_lyh_delay_task(Ms_3000);
				app_task_substep = AMERICAN_RIGHT_PAUSE_STATE;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		case AMERICAN_RIGHT_PAUSE_STATE:
			dc_motor_Start(MOTOR_RIGHT);
			run_max_time = RIGHT_TIMER(Ms_3000, Ms_100);
			app_task_substep = AMERICAN_RIGHT_SEG2_STATE;
			osal_lyh_delay_task(Ms_100);
			break;
		case AMERICAN_RIGHT_SEG2_STATE:
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				osal_lyh_delay_task(Ms_200);
				app_task_substep = AMERICAN_LEFT_START_STATE;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		case AMERICAN_LEFT_START_STATE:
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			app_task_substep = AMERICAN_LEFT_END_STATE;
			break;
		case AMERICAN_LEFT_END_STATE:
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel)
				&& ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				osal_lyh_delay_task(Ms_360);
				key_prohibit_setup(KEY_ONOFF_MASK, FAIL);
				app_task_substep = AMERICAN_EXTRACT_RST_STATE;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		case AMERICAN_EXTRACT_RST_STATE:
			osal_lyh_delay_task(Ms_100);
			break;
		case AMERICAN_EXTRACT_CHECK_STATE:
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: app_hotwater_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_hotwater_handler(void)
{
	switch(app_task_substep)
	{
		case HOTWATER_INIT_STATE:
			display_led(START_LED, LED_ON);
			app_led_hotwater_init();
			app_task_substep = HOTWATER_RUN_STATE;
			break;
		case HOTWATER_RUN_STATE:
			osal_lyh_delay_task(Ms_500);
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: app_steam_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_steam_handler(void)
{
	switch(app_task_substep)
	{
		case STEAM_INIT_STATE:
			display_led(START_LED, LED_ON);
			app_led_steam_init();
			app_task_substep = STEAM_RUN_STATE;
			break;
		case STEAM_RUN_STATE:
			osal_lyh_delay_task(Ms_500);
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: app_aqua_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_aqua_handler(void)
{
	switch(app_task_substep)
	{
		case AQUA_INIT_STATE:
			display_led(START_LED, LED_ON);
			app_led_aqua_init();
			app_task_substep = AQUA_RUN_STATE;
			break;
		case AQUA_RUN_STATE:
			osal_lyh_delay_task(Ms_500);
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: app_calc_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_calc_handler(void)
{
	switch(app_task_substep)
	{
		case CALC_INIT_STATE:
			display_led(START_LED, LED_ON);
			app_led_calc_init();
			app_task_substep = CALC_RUN_STATE;
			break;
		case CALC_RUN_STATE:
			osal_lyh_delay_task(Ms_500);
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: app_setup_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_setup_handler(void)
{
	switch(app_task_substep)
	{
		case SETUP_INIT_STATE:
			if (MOTOR_STATE_STOPED == dc_motor_GetState()){
				key_prohibit_setup(KEY_ONOFF_MASK, TRUE);
				app_task_substep = SETUP_ENTER_STATE;
				if (FAIL == app_motor_ready()){
					app_task_substep = SETUP_RIGHT_READY_STATE;
				}
			}
			osal_lyh_delay_task(Ms_100);
			break;
		case SETUP_ENTER_STATE:
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = RIGHT_TIMER(Ms_1000, Ms_100);
			app_task_substep = SETUP_ENTER_ING_STATE;
			break;
		case SETUP_ENTER_ING_STATE:
			if ((FAIL == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				app_task_substep = run_max_time? SETUP_RIGHT_READY_STATE:SETUP_LEFT_READY_STATE;
			}
			osal_lyh_delay_task(Ms_100);
			break;
		case SETUP_RIGHT_READY_STATE:
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_100);
			app_task_substep = SETUP_RIGHT_ING_STATE;
			run_max_time = RIGHT_TIMER(Ms_6000, Ms_100);
			break;
		case SETUP_RIGHT_ING_STATE:
			if ((MOTOR_STATE_STOPED == dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				osal_lyh_delay_task(Ms_200);
				app_task_substep = SETUP_LEFT_READY_STATE;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		case SETUP_LEFT_READY_STATE:
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			app_task_substep = SETUP_LEFT_ING_STATE;
			break;
		case SETUP_LEFT_ING_STATE:
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel) 
				&& ((MOTOR_STATE_STOPED == dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				app_task_substep = SETUP_COMPLETE_STATE;
			}
			osal_lyh_delay_task(Ms_100);
			break;
		case SETUP_COMPLETE_STATE:
			key_prohibit_setup(KEY_ONOFF_MASK, FAIL);
			app_task_nextstep(APP_IDLE_STATE, IDLE_INIT_STATE);
			break;
		default:
			break;
	}
}


/**************************************************************************
 * @brief 在研磨状态下按下ON/OFF键的处理函数
 * @return none
 * @note 该函数在espresso, americano的研磨状态下按下ON/OFF键时被调用
 *************************************************************************/
void app_running_onoff_grind_handler(void)
{
	static uint8_t sta = 0;
	switch (sta)
	{
		/* 停止研磨电机 */
		case 0:		
			key_prohibit_setup(KEY_ONOFF_MASK, TRUE);								
			app_led_show_init();							
			display_led(START_LED, LED_OFF);
			ac_grind_stop();
			osal_lyh_delay_task(Ms_100);
			sta = 1;
			break;
		/* 萃取电机转到接水位置 */
		case 1:										
			dc_motor_Start(MOTOR_LEFT);		
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(Ms_7000, Ms_100);
			sta = 2;
			break;
		/* 判断萃取电机是否到接水位置 */
		case 2:										
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				sta = 3;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}			
			break;
		/* 开始抽水（短时间） */
		case 3:										
			ac_pumb_start(CONTINUE_MODE);
			osal_lyh_delay_task(Ms_2000 - Ms_100);	//此处时间需要根据实际情况整改
			sta = 4;
			break;
		/* 停止抽水 */
		case 4:										
			ac_pumb_stop();
			osal_lyh_delay_task(Ms_4000);	//此处时间需要根据实际情况整改
			sta = 5;
			break;
		/* 萃取电机转到倒粉位置 */
		case 5:										
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_3000 - Ms_100);
			sta = 6;
			break;
		/* 萃取电机停止，倒出咖啡粉 */
		case 6:									
			dc_motor_Stop();
			osal_lyh_delay_task(Ms_3000);
			sta = 7;
			break;
		/* 萃取电机回到原位 */
		case 7:										
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = RIGHT_TIMER(Ms_5000, Ms_100);
			sta = 8;
			break;
		/* 判断萃取电机是否到原位开关 */
		case 8:									
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())	
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				sta = 9;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		/* 萃取电机回转到原位 */
		case 9:									
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			sta = 10;
			break;
		/* 判断萃取电机回转是否完成 */
		case 10:									
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel) 
				&& ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				key_prohibit_setup(KEY_ONOFF_MASK, TRUE);								
				sta = 0;
				app_task_nextsubstep(RUNNING_ONOFF_FLUSH);	//进入冲煮状态下按下ON/OFF键的处理流程的最后一个子状态：flush状态
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		default:
			break;
	}
}

/**************************************************************************
 * @brief 在萃取电机去接水状态下按下ON/OFF键的处理函数
 * @return none
 * @note 该函数在espresso, americano的去接水状态下按下ON/OFF键时被调用
 *************************************************************************/
void app_running_onoff_go_water_handler(void)
{
	static uint8_t sta = 0;
	switch (sta)
	{
		/* 停止萃取电机 */
		case 0:					
			key_prohibit_setup(KEY_ONOFF_MASK, TRUE);						
			app_led_show_init();
			display_led(START_LED, LED_OFF);
			dc_motor_Stop();
			osal_lyh_delay_task(Ms_1000);
			sta = 1;
			break;
		/* 萃取电机转到接水位置 */
		case 1:										
			dc_motor_Start(MOTOR_LEFT);		
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(Ms_7000, Ms_100);
			sta = 2;
			break;
		/* 判断萃取电机是否到接水位置 */
		case 2:										
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				sta = 3;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}			
			break;
		/* 开启水泵（短时间） */
		case 3:										
			ac_pumb_start(CONTINUE_MODE);
			osal_lyh_delay_task(Ms_2000 - Ms_100);		//此处时间需要根据实际情况整改
			sta = 4;
			break;
		 /* 停止水泵 */
		case 4:									
			ac_pumb_stop();
			osal_lyh_delay_task(Ms_4000);	//此处时间需要根据实际情况整改
			sta = 5;
			break;
		/* 萃取电机转到倒粉位置 */
		case 5:										
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_3000 - Ms_100);
			sta = 6;
			break;
		 /* 萃取电机停止，倒出咖啡粉 */
		case 6:									
			dc_motor_Stop();
			osal_lyh_delay_task(Ms_3000);
			sta = 7;
			break;
		 /* 萃取电机回转到原位 */
		case 7:										
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = RIGHT_TIMER(Ms_5000, Ms_100);
			sta = 8;
			break;
		 /* 判断萃取电机回转是否触碰原位开关 */
		case 8:										
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				sta = 9;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		/* 萃取电机回转到原位 */	
		case 9:										
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			sta = 10;
			break;
		 /* 判断萃取电机回转是否完成 */
		case 10:									
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel) 
				&& ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				key_prohibit_setup(KEY_ONOFF_MASK, FAIL);
				sta = 0;
				app_task_nextsubstep(RUNNING_ONOFF_FLUSH);	//进入冲煮状态下按下ON/OFF键的处理流程的最后一个子状态：flush状态
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		default:
			break;
	}
}

/**************************************************************************
 * @brief 在萃取电机预冲煮状态下按下ON/OFF键的处理函数
 * @return none
 * @note 该函数在espresso, americano的预冲煮状态下按下ON/OFF键时被调用
 *************************************************************************/
void app_running_onoff_pre_brew_handler(void)
{
	static uint8_t sta = 0;
	switch (sta)
	{
		/* 停止水泵 */
		case 0:		
			key_prohibit_setup(KEY_ONOFF_MASK, TRUE);											
			app_led_show_init();							
			display_led(START_LED, LED_OFF);
			ac_pumb_stop();
			osal_lyh_delay_task(Ms_1000);
			sta = 1;
			break;
		/* 启动水泵（短时间） */			
		case 1:										
			ac_pumb_start(CONTINUE_MODE);
			osal_lyh_delay_task(Ms_2000 - Ms_100);		//此处时间需要根据实际情况整改
			sta = 2;
			break;
		/* 停止水泵 */
		case 2:										
			ac_pumb_stop();
			osal_lyh_delay_task(Ms_4000);	//此处时间需要根据实际情况整改
			sta = 3;
			break;
		 /* 萃取电机转到倒粉位置 */
		case 3:										
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_3000 - Ms_100);
			sta = 4;
			break;
		 /* 萃取电机停止，倒出咖啡粉 */
		case 4:										
			dc_motor_Stop();
			osal_lyh_delay_task(Ms_3000);
			sta = 5;
			break;
		 /* 萃取电机向原位转动 */
		case 5:										
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = RIGHT_TIMER(Ms_5000, Ms_100);
			sta = 6;
			break;
		 /* 判断萃取电机是否触碰原位触点开关 */
		case 6:										
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				sta = 7;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		 /* 萃取电机回转到原位 */
		case 7:										
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			sta = 8;
			break;
		 /* 判断萃取电机回转是否完成 */
		case 8:									
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel) 
				&& ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				key_prohibit_setup(KEY_ONOFF_MASK, FAIL);
				sta = 0;
				app_task_nextsubstep(RUNNING_ONOFF_FLUSH);	//进入冲煮状态下按下ON/OFF键的处理流程的最后一个子状态：冲洗管路
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		default:
			break;
	}
}

/**************************************************************************
 * @brief 在冲煮状态下按下ON/OFF键的处理函数
 * @return none
 * @note 该函数在espresso, americano的冲煮状态下按下ON/OFF键时被调用
 *************************************************************************/
void app_running_onoff_brew_handler(void)
{
	static uint8_t sta = 0;
	switch (sta)
	{
		 /* 停止水泵 */
		case 0:			
			key_prohibit_setup(KEY_ONOFF_MASK, TRUE);
			app_led_show_init();							
			display_led(START_LED, LED_OFF);
			ac_pumb_stop();
			osal_lyh_delay_task(Ms_4000);
			sta = 1;
			break;
		 /* 萃取电机转到倒粉位置 */
		case 1:										
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_3000 - Ms_100);
			sta = 2;
			break;
		 /* 萃取电机停止，倒出咖啡粉 */
		case 2:										
			dc_motor_Stop();
			osal_lyh_delay_task(Ms_3000);
			sta = 3;
			break;
		/* 萃取电机向原点转动 */
		case 3:										
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = RIGHT_TIMER(Ms_5000, Ms_100);
			sta = 4;
			break;
		 /* 判断萃取电机是否到原点触点开关 */
		case 4:										
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				sta = 5;
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		 /* 萃取电机回转到原位 */
		case 5:										
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			sta = 6;
			break;
		 /* 判断萃取电机是否回转到原位 */
		case 6:									
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel) 
				&& ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				key_prohibit_setup(KEY_ONOFF_MASK, FAIL);
				sta = 0;
				app_task_nextsubstep(RUNNING_ONOFF_FLUSH);	//进入冲煮状态下按下ON/OFF键的处理流程的最后一个子状态：冲洗管路
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		default:
			break;
	}
}

/**************************************************************************
 * @brief 冲洗管路函数处理流程
 * @return none
 * @note 此函数中没有检测萃取电机位置的步骤，使用时的前置条件是：萃取电机已经回转到原位
 *************************************************************************/
void app_running_onoff_flush_handler(void)
{
	static uint8_t sta = 0;
	switch (sta)
	{
		/* 清洗管路初始阶段：屏蔽ON/OFF，萃取电机前往萃取位置 */
		case 0:
			key_prohibit_setup(KEY_ONOFF_MASK, TRUE);					
			dc_motor_Start(MOTOR_LEFT);		
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(Ms_7000, Ms_100);		//此处时间需要根据实际情况整改
			sta = 1;
			break;
		/* 判断萃取电机是否到达萃取位置 */
		case 1:
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				sta = 2;	
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}			
			break;
		 /* 启动水泵冲洗管路 */
		case 2:
			ac_pumb_start(CONTINUE_MODE);		//启动水泵
			osal_lyh_delay_task(Ms_10000);		
			sta = 3;
			break;
		case 3:
			ac_pumb_stop();						//关闭水泵
			osal_lyh_delay_task(Ms_6000);		
			sta = 4;
			break;
		 /* 萃取电机向原点转动 */
		case 4:
			dc_motor_Start(MOTOR_RIGHT);
			osal_lyh_delay_task(Ms_1000);			//此处delay应保持较大的数（>500ms），否则电机还未转出触点就开始检测，
													//容易误判为已经触碰到原点开关，导致电机无法回转到原位
			run_max_time = RIGHT_TIMER(Ms_6000, Ms_100);		
			sta = 5;
			break;	
		 /* 判断电机是否到原点触点 */	
		case 5:													
			if ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (TRUE == app_motor_ready())
				|| (--run_max_time == 0)){
				dc_motor_Stop();
				sta = 6;				
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		/* 判断萃取电机是否回转到原位 */
		case 6:													//电机回转到原位
			dc_motor_Start(MOTOR_LEFT);
			osal_lyh_delay_task(Ms_100);
			run_max_time = LEFT_TIMER(MOTOR_FIX_TIME, Ms_100);
			sta = 7;
			break;	
		case 7:													//判断电机是否回转到原位
			if (1/*(FAIL == app_motor_ready())*/	// deleting point,(temp to cancel)
				&& ((MOTOR_STATE_STOPED ==dc_motor_GetState())
				|| (--run_max_time == 0))){
				dc_motor_Stop();
				key_prohibit_setup(KEY_ONOFF_MASK, FAIL);
				sta = 0;
				app_task_nextstep(APP_IDLE_STATE, IDLE_INIT_STATE);	
			}
			else {
				osal_lyh_delay_task(Ms_100);
			}
			break;
		default:
			break;
	}
}

/**************************************************************************
 * @brief 在espresso, americano的运行状态下，按下ON/OFF键的处理函数
 * @return none
 * @note 1.根据app_onoff_key_dealy函数中的状态，决定进入此函数的app_task_substep（子状态）是研磨状态、去接水状态、预冲煮状态还是冲煮状态
 * 		 2.americano的研磨状态和espresso的研磨状态调用同一个函数处理，其他状态同理
 * 		  在Americano第一次萃取后，剩余的状态与espresso的处理方式一致，都是取消第二杯制作、设置关机标志位和开启流水灯	   
 *************************************************************************/
void app_running_onoff_handler(void)
{

	switch(app_task_substep)
	{
		case RUNNING_ONOFF_GRIND_STATE:		//研磨状态
			app_running_onoff_grind_handler();
			break;
		case RUNNING_ONOFF_GO_WATER_STATE:		//去接水状态
			app_running_onoff_go_water_handler();
			break;
		case RUNNING_ONOFF_PRE_BREW_STATE:		//预冲煮状态
			app_running_onoff_pre_brew_handler();
			break;
		case RUNNING_ONOFF_BREW_STATE:		//冲煮状态
			app_running_onoff_brew_handler();
			break;
		case RUNNING_ONOFF_FLUSH:					//清洗管路状态
			app_running_onoff_flush_handler();		//此处是以上4个状态下按下ON/OFF键的处理流程的最后一个子状态，
													//在以上4个流程的最后进入此清洗函数并复位关机进入IDLE状态
			break;
		default:
			break;
	}
}

/************************************************************************
* function name: init_app
* input: non
* output: non
* description: object thread
*************************************************************************/
void init_app_task(uint8_t id)
{
	app_id = id;
	app_task_step = APP_INIT_STATE;
	app_task_substep = INIT_STATE;
}

/************************************************************************
* function name: app_task_handler
* input: non
* output: non
* description: object thread
*************************************************************************/
void app_task_handler(void)
{
	switch(app_task_step){
	case APP_INIT_STATE:
		app_init_handler();
		break;
	case APP_IDLE_STATE:
		app_idle_handler();
			break;
	case APP_STARTUP_STATE:
		app_startup_handler();
			break;
	case APP_NORMAL_STATE:
		app_normal_handler();
			break;
	case APP_ESPRESSO_STATE:
		app_espresso_handler();
			break;
	case APP_AMERICAN_STATE:
		app_american_handler();
			break;
	case APP_HOTWATER_STATE:
		app_hotwater_handler();
			break;
	case APP_STEAM_STATE:
		app_steam_handler();
			break;
	case APP_AQUA_STATE:
		app_aqua_handler();
			break;
	case APP_CALC_STATE:
		app_calc_handler();
			break;
	case APP_SETUP_STATE:
		app_setup_handler();
			break;
	case APP_RUNNING_ONOFF_STATE:
		app_running_onoff_handler();
			break;
	default:
		break;
	}
}

/************************************************************************
* function name: app_processEvent
* input: non
* output: non
* description: this object's message loop
*************************************************************************/
uint32_t app_event_handler( uint8_t task_id, uint32_t events)
{
	if (app_id != task_id){
		return events;
	}
	if (APP_KEY_CHANGE_EVENT & events)
	{
		app_key_handler(&keyinfo);
	}

	return events;
}

