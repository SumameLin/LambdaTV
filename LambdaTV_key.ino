#include "Arduino.h"
#include "LambdaTV.h"
/*
函 数 名:void s_click(void)
功能说明:s键按一下
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void s_click(void)
{
    lambdaTV.key_event = KEY_CONFIRM;
    Serial.println("s_click");
}
/*
函 数 名:void s_doubleclick(void)
功能说明:s键按两下
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void s_doubleclick(void)
{
    Serial.println("s_doubleclick");
}
/*
函 数 名:void s_longclick(void)
功能说明:s键长按
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void s_longclick(void)
{
    lambdaTV.key_event = KEY_CANCEL;
    Serial.println("s_longclick");
}
/*
函 数 名:void c_click(void)
功能说明:c键按一下
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void c_click(void)
{
    lambdaTV.key_event = KEY_NEXT;
    Serial.println("c_click");
}
/*
函 数 名:void c_doubleclick(void)
功能说明:c键按两下
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void c_doubleclick(void)
{
    lambdaTV.key_event = KEY_PRVE;
    Serial.println("c_doubleclick");
}
/*
函 数 名:void c_longclick(void)
功能说明:c键长按
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void c_longclick(void)
{
    lambdaTV.key_event = KEY_HIDDEN;
    Serial.println("c_longclick");
}
/*
函 数 名:KEY_EVENT_INF get_keymenu_event(void)
功能说明:获取按键事件
形    参:void
返 回 值:void
时    间：2020-12-25
RAiny
*/
KEY_EVENT_INF get_keymenu_event(void)
{
    KEY_EVENT_INF event;
    event = lambdaTV.key_event;
    return event;
}
/*
函 数 名:void set_keymenu_event(KEY_EVENT_INF KEY)
功能说明:设置按键事件
形    参:void
返 回 值:void
时    间：2020-1-5
RAiny
*/
void set_keymenu_event(KEY_EVENT_INF KEY)
{
    lambdaTV.key_event = KEY;
}
/*
函 数 名:void clear_keymenu_event(void)
功能说明:清除按键事件
形    参:void
返 回 值:void
时    间：2020-12-25
RAiny
*/
void clear_keymenu_event(void)
{
    lambdaTV.key_event = KEY_NOEVENT;
}
/*
函 数 名:void key_init(void)
功能说明:按键初始化，用了OneButton库
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void key_init(void)
{
    s_button.attachClick(s_click);
    s_button.attachDoubleClick(s_doubleclick);
    s_button.attachLongPressStart(s_longclick);
    c_button.attachClick(c_click);
    c_button.attachDoubleClick(c_doubleclick);
    c_button.attachLongPressStart(c_longclick);
}
/*
函 数 名:void rgb_led_init(void)
功能说明:RGB LED初始化 IO占用了串口
形    参:void
返 回 值:void
时    间：2020-12-24
RAiny
*/
void rgb_led_init(void)
{
    #if USEING_UART
    pinMode(RGB_R_PIN, OUTPUT);
    #endif
    pinMode(RGB_G_PIN, OUTPUT);
    pinMode(RGB_B_PIN, OUTPUT);
    set_rgb_val(255, 255, 255);
}
/*
函 数 名:void rgb_led_set(RGB_INF rgb_set)
功能说明:直插三脚RGB共阳极
形    参:void
返 回 值:void
时    间：2020-12-27
RAiny
*/
void rgb_led_set(RGB_INF rgb_set)
{
    if(eeprom.data.led_on)
    {
        #if USEING_UART
        analogWrite(RGB_R_PIN, 1024 - rgb_set.r_val * 4);
        #endif
        analogWrite(RGB_G_PIN, 1024 - rgb_set.g_val * 4);
        analogWrite(RGB_B_PIN, 1024 - rgb_set.b_val * 4);
    }
    else
    {
        analogWrite(RGB_R_PIN, 1024);
        analogWrite(RGB_G_PIN, 1024);
        analogWrite(RGB_B_PIN, 1024);
    }
}
/*
函 数 名:void set_rgb_val(uint8_t r_val,uint8_t g_val,uint8_t b_val)
功能说明:设置RGB的数值，呼吸效果
形    参:void
返 回 值:void
时    间：2020-12-31
RAiny
*/
void set_rgb_val(uint8_t r_val, uint8_t g_val, uint8_t b_val)
{
    rgb_save.r_val = rgb.r_val = r_val;
    rgb_save.g_val = rgb.g_val = g_val;
    rgb_save.b_val = rgb.b_val = b_val;
}