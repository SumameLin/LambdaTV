#ifndef __LAMBDATV_H
#define __LAMBDATV_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "Arduino.h"

#define RGB_R_PIN 1 //TX
#define RGB_G_PIN 3
#define RGB_B_PIN 12

/*
  Icon configuration
  Width and height must match the icon font size
  GAP: Space between the icons
  BGAP: Gap between the display border and the cursor.
*/
#define ICON_WIDTH 32
#define ICON_HEIGHT 32
#define ICON_GAP  48   //每个ICON的距离 如果是8 就是显示3个ICO，如果是48就只显示一个ICO
#define ICON_BGAP 48   //ICO的初始距离
#define ICON_Y ICON_HEIGHT+8

#define OLED_WIDTH  128
#define OLED_HEIGHT 64
#define NUM_WIDTH   25
#define NUM_HEIGHT  42
#define MINU_LOW_X      2+NUM_WIDTH*4//xbm 时间的x坐标
#define MINU_HIGH_X     2+NUM_WIDTH*3
#define MAOHAO_X        2+NUM_WIDTH*2
#define HOUR_LOW_X      2+NUM_WIDTH
#define HOUR_HIGH_X     2
#define TIME_XBM_HEIGHT (OLED_HEIGHT-NUM_HEIGHT)/2

#define EEPROM_SIZE 64 //EEPROM 大小
#define CLOCK_MAX_MODE  3
#define LED_MAX_MODE    1
#define USEING_UART     0  //使用串口，串口引脚和RGB_LED共用

typedef enum
{
    KEY_NOEVENT=0,
    KEY_NEXT,
    KEY_PRVE,
    KEY_HIDDEN,
    KEY_CONFIRM,
    KEY_CANCEL
}KEY_EVENT_INF;

typedef enum
{
    SEL_CLOCK=0,
    SEL_BADAPPLE,
    SHOW_BADAPPLE
}SELECT_EVENT_INF;

typedef struct
{
    KEY_EVENT_INF key_event;
    SELECT_EVENT_INF witch_menu;
}LambdaTV_INF;

typedef struct
{
  const uint8_t *font;
  uint16_t icon;
  const char *name;
  void (*current_operation)(void);
}menu_entry_type;

typedef struct
{
  int16_t menu_start;		/* in pixel */
  int16_t frame_position;		/* in pixel */
  uint8_t position;			/* position, array index */
}menu_state;

typedef struct
{
  uint8_t current;
  uint8_t up;//向上翻索引号
  uint8_t down;//向下翻索引号
  uint8_t enter;//确认索引号
  uint8_t exit;//退出索引号
  void (*current_operation)(void);
}config_table;

typedef struct
{
  uint8_t r_val;
  uint8_t g_val;
  uint8_t b_val;
}RGB_INF;

typedef struct
{
  uint8_t clock_mode;
  uint8_t led_on;
}DATA_STRUCT;

typedef union
{
  byte arry[EEPROM_SIZE];
  DATA_STRUCT data;
}EEPROM_INF;

extern LambdaTV_INF lambdaTV;
extern EEPROM_INF eeprom;
extern RGB_INF rgb;
extern RGB_INF rgb_save;
extern uint32_t config_state,config_last_state;
KEY_EVENT_INF get_keymenu_event(void);
void clear_keymenu_event(void);

#ifdef __cplusplus
}
#endif

#endif //__LAMBDATV_H
