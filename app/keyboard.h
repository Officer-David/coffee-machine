
#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

#ifdef _KEYBOARD_C_
#define PUBLIC 
#else
#define PUBLIC  extern
#endif

#define KEY_INIT_STATE		0
#define KEY_IDLE_STATE		1
#define KEY_RUN_STATE		2

#define KEY_HOLD_FUNCTION_ENABLE	1 
/****************************************************************************************************
 * key message define 
****************************************************************************************************/
#define  KEY_POS_1			0u
#define  KEY_POS_2			1u
#define  KEY_POS_3			2u
#define  KEY_POS_4			3u
#define  KEY_POS_5			4u
#define  KEY_POS_6			5u
#define  KEY_POS_7			6u
#define  KEY_POS_8			7u
#define  KEY_POS_9			8u
#define  KEY_POS_10			9u 
#define  KEY_POS_11			10u 
#define  KEY_POS_12			11u
#define  KEY_POS_13			12u 
#define  KEY_POS_14			13u 
#define  KEY_POS_15			14u 
#define  KEY_POS_16			15u 
#define  KEY_POS_17			16u
#define  KEY_POS_18			17u
#define  KEY_POS_19			18u
#define  KEY_POS_20			19u
#define  KEY_POS_21			20u
#define  KEY_POS_22			21u
#define  KEY_POS_23			22u
#define  KEY_POS_24			23u
#define  KEY_POS_25			24u
#define  KEY_POS_26			25u 
#define  KEY_POS_27			26u 
#define  KEY_POS_28			27u
#define  KEY_POS_29			28u 
#define  KEY_POS_30			29u 
#define  KEY_POS_31			30u 
#define  KEY_POS_32			31u 
/****************************************************************************************************
 * total key number
****************************************************************************************************/
#define  MAX_KEY_NUMBER				KEY_POS_17 + 1u 

/****************************************************************************************************
 * key message mask
****************************************************************************************************/
#define  KEY_MASK1		((uint32_t)1u << KEY_POS_1)
#define  KEY_MASK2		((uint32_t)1u << KEY_POS_2)
#define  KEY_MASK3		((uint32_t)1u << KEY_POS_3)
#define  KEY_MASK4		((uint32_t)1u << KEY_POS_4)
#define  KEY_MASK5		((uint32_t)1u << KEY_POS_5)
#define  KEY_MASK6		((uint32_t)1u << KEY_POS_6)
#define  KEY_MASK7		((uint32_t)1u << KEY_POS_7)
#define  KEY_MASK8		((uint32_t)1u << KEY_POS_8)
#define  KEY_MASK9		((uint32_t)1u << KEY_POS_9)
#define  KEY_MASK10		((uint32_t)1u << KEY_POS_10)
#define  KEY_MASK11		((uint32_t)1u << KEY_POS_11)
#define  KEY_MASK12		((uint32_t)1u << KEY_POS_12)
#define  KEY_MASK13		((uint32_t)1u << KEY_POS_13)
#define  KEY_MASK14		((uint32_t)1u << KEY_POS_14)
#define  KEY_MASK15		((uint32_t)1u << KEY_POS_15)
#define  KEY_MASK16		((uint32_t)1u << KEY_POS_16)
#define  KEY_MASK17		((uint32_t)1u << KEY_POS_17)
#define  KEY_MASK18		((uint32_t)1u << KEY_POS_18)
#define  KEY_MASK19		((uint32_t)1u << KEY_POS_19)
#define  KEY_MASK20		((uint32_t)1u << KEY_POS_20)
#define  KEY_MASK21		((uint32_t)1u << KEY_POS_21)
#define  KEY_MASK22		((uint32_t)1u << KEY_POS_22)
#define  KEY_MASK23		((uint32_t)1u << KEY_POS_23)
#define  KEY_MASK24		((uint32_t)1u << KEY_POS_24)
#define  KEY_MASK25		((uint32_t)1u << KEY_POS_25)
#define  KEY_MASK26		((uint32_t)1u << KEY_POS_26)
#define  KEY_MASK27		((uint32_t)1u << KEY_POS_27)
#define  KEY_MASK28		((uint32_t)1u << KEY_POS_28)
#define  KEY_MASK29		((uint32_t)1u << KEY_POS_29)
#define  KEY_MASK30		((uint32_t)1u << KEY_POS_30)
#define  KEY_MASK31		((uint32_t)1u << KEY_POS_31)
#define  KEY_MASK32		((uint32_t)1u << KEY_POS_32)

#define KEY_ONOFF_MASK		KEY_MASK1
#define KEY_HOTWATER_MASK	KEY_MASK2
#define KEY_ESPRESSO_MASK	KEY_MASK3
#define KEY_STEAM_MASK		KEY_MASK4
#define KEY_START_MASK		KEY_MASK5
#define KEY_AMERICANO_MASK	KEY_MASK6
#define KEY_COFFE_MASK		KEY_MASK7
#define KEY_PAD_MASK		KEY_MASK8
#define KEY_AQUA_MASK		KEY_MASK9
#define KEY_CALC_MASK		KEY_MASK10
#define KEY_WATER_MASK		KEY_MASK11
//input detecct
#define PALLET_DET_MASK		KEY_MASK12
#define PW3P3V_DET_MASK		KEY_MASK13
#define WKUP_DET_MASK		KEY_MASK14
#define METER_DET_MASK		KEY_MASK15
#define MOTOR_DET_MASK		KEY_MASK16
#define WATER_DET_MASK		KEY_MASK17

#define TOUCH_ONOFF_KEY()		(keyTouch_in & KEY_ONOFF_MASK)
#define TOUCH_HOTWATER_KEY()	(keyTouch_in & KEY_HOTWATER_MASK)
#define TOUCH_ESPRESSO_KEY()	(keyTouch_in & KEY_ESPRESSO_MASK)
#define TOUCH_STEAM_KEY()		(keyTouch_in & KEY_STEAM_MASK)
#define TOUCH_START_KEY()		(keyTouch_in & KEY_START_MASK)		
#define TOUCH_AMERICANO_KEY()	(keyTouch_in & KEY_AMERICANO_MASK)
#define TOUCH_COFFE_KEY()		(keyTouch_in & KEY_COFFE_MASK)
#define TOUCH_PAD_KEY()			(keyTouch_in & KEY_PAD_MASK)
#define TOUCH_AQUA_KEY()		(keyTouch_in & KEY_AQUA_MASK)
#define TOUCH_CALC_KEY()		(keyTouch_in & KEY_CALC_MASK)
#define TOUCH_WATER_KEY()		(keyTouch_in & KEY_WATER_MASK)

/****************************************************************************************************
 * key message data struct
****************************************************************************************************/
typedef struct{
	uint32_t ulPressF;
	uint32_t ulLPressF;
	uint32_t ulReleaseF;	 
	uint32_t ulProhibitkF;
	uint32_t ulLPressMaskF;
	uint32_t ulUIOnboardClockA;
	uint32_t ulUIOnboardClockB;
	uint32_t ulUIOnboardSwitches;
#ifdef KEY_HOLD_FUNCTION_ENABLE
	uint16_t  ulHoldTime[MAX_KEY_NUMBER];
	uint16_t  pulUIHoldCount[MAX_KEY_NUMBER];
#endif
}_keystruct;

PUBLIC _keystruct  keyinfo;
PUBLIC uint16_t keyTouch_in;	

PUBLIC uint8_t keyboard_id;	
PUBLIC uint8_t keyboard_task_step;

void init_key_task(uint8_t id);
void key_task_handler(void);
void key_prohibit_mask(void);
void key_prohibit_setup(uint32_t key, uint8_t status);
uint32_t key_event_handler( uint8_t task_id, uint32_t events);

#undef PUBLIC
#endif




