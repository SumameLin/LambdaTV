#include "Arduino.h"
#include "LambdaTV.h"
#include <U8g2lib.h>
/*
U8g2的IconMenu.ino example
*/
/*
函 数 名:void draw(menu_state *state)
功能说明:绘制字体字集的符号和选中框
形    参:menu_state *state
返 回 值:void
时    间：2020-12-30
RAiny
*/
void draw(menu_state *state)
{
    int16_t x;
    uint8_t i;
    const uint8_t wid_max = 3, hv_len = 8;
    x = state->menu_start;
    i = 0;
    while (menu_entry_list[i].icon > 0)
    {
        if (x >= -ICON_WIDTH && x < u8g2.getDisplayWidth())
        {
            u8g2.setFont(menu_entry_list[i].font);
            u8g2.drawGlyph(x, ICON_Y, menu_entry_list[i].icon);
        }
        i++;
        x += ICON_WIDTH + ICON_GAP;
    }
    for (uint8_t wid = 0; wid < wid_max; wid++) //多条直线重叠，来实现线的宽度
    {
        u8g2.drawHVLine(state->frame_position - 1, ICON_Y - ICON_HEIGHT - 3 - wid, ICON_WIDTH + 2, 0);                             //往上增加
        u8g2.drawHVLine(state->frame_position - 2 - wid, ICON_Y - ICON_HEIGHT - 3 + hv_len - wid_max, hv_len, 3);                  //往左增加 模式3是从下往上画
        u8g2.drawHVLine(state->frame_position - 1 + ICON_WIDTH + 2 + wid, ICON_Y - ICON_HEIGHT - 3 + hv_len - wid_max, hv_len, 3); //往右增加

        u8g2.drawHVLine(state->frame_position - 1, ICON_Y + 3 + wid, ICON_WIDTH + 2, 0);                        //往下增加
        u8g2.drawHVLine(state->frame_position - 2 - wid, ICON_Y + 3 + wid_max - 1, hv_len, 3);                  //往左增加
        u8g2.drawHVLine(state->frame_position - 1 + ICON_WIDTH + 2 + wid, ICON_Y + 3 + wid_max - 1, hv_len, 3); //往右增加
    }
}
/*
函 数 名:void to_right(menu_state *state)
功能说明:向右移动
形    参:menu_state *state
返 回 值:void
时    间：2020-12-30
RAiny
*/
void to_right(menu_state *state)
{
    if (menu_entry_list[state->position + 1].font != NULL)
    {
        //小于128
        if ((int16_t)state->frame_position + 2 * (int16_t)ICON_WIDTH + (int16_t)ICON_BGAP < (int16_t)u8g2.getDisplayWidth()) //获取显示器的宽度
        {
            state->position++;
            state->frame_position += ICON_WIDTH + (int16_t)ICON_GAP;
        }
        else
        {
            state->position++;
            state->frame_position = (int16_t)u8g2.getDisplayWidth() - (int16_t)ICON_WIDTH - (int16_t)ICON_BGAP;
            state->menu_start = state->frame_position - state->position * ((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
        }
    }
}
/*
函 数 名:void to_left(menu_state *state)
功能说明:向左移动
形    参:menu_state *state
返 回 值:void
时    间：2020-12-30
RAiny
*/
void to_left(menu_state *state)
{
    if (state->position > 0)
    {
        if ((int16_t)state->frame_position >= (int16_t)ICON_BGAP + (int16_t)ICON_WIDTH + (int16_t)ICON_GAP)
        {
            state->position--;
            state->frame_position -= ICON_WIDTH + (int16_t)ICON_GAP;
        }
        else
        {
            state->position--;
            state->frame_position = ICON_BGAP;
            state->menu_start = state->frame_position - state->position * ((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
        }
    }
}
/*
函 数 名:uint8_t towards_int16(int16_t *current, int16_t dest)
功能说明:
形    参:void
返 回 值:void
时    间：2020-12-30
RAiny
*/
uint8_t towards_int16(int16_t *current, int16_t dest)
{
    if (*current < dest)
    {
        (*current)++;
        return 1;
    }
    else if (*current > dest)
    {
        (*current)--;
        return 1;
    }
    return 0;
}
/*
函 数 名:uint8_t towards(menu_state *current,menu_state *destination)
功能说明:
形    参:void
返 回 值:void
时    间：2020-12-30
RAiny
*/
uint8_t towards(menu_state *current, menu_state *destination)
{
    uint8_t r = 0;
    r |= towards_int16(&(current->frame_position), destination->frame_position);
    r |= towards_int16(&(current->frame_position), destination->frame_position);
    r |= towards_int16(&(current->menu_start), destination->menu_start);
    r |= towards_int16(&(current->menu_start), destination->menu_start);
    return r;
}
/*
函 数 名:void config_fun(void)
功能说明:主显示菜单
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void config_fun(int8_t state)
{
    u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
    u8g2.setCursor(5, 16 * state - 2);
    u8g2.print("时钟模式");
    u8g2.setCursor(5, 16 * (state + 1) - 2);
    u8g2.print("设置RGB_LED");
    u8g2.setCursor(5, 16 * (state + 2) - 2);
    u8g2.print("清除WiFi");
    u8g2.setCursor(5, 16 * (state + 3) - 2);
    u8g2.print("操作说明");
    u8g2.setCursor(5, 16 * (state + 4) - 2);
    u8g2.print("关于");
}
/*
函 数 名:void clock_mode(void)
功能说明:时钟模式
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void clock_mode()
{
    config_state = 0;
    if (config_state < config_last_state)
    {
        do
        {
            u8g2.clearBuffer();
            u8g2.setFontMode(1);
            u8g2.setDrawColor(1);
            u8g2.drawBox(0, (config_last_state)--, 128, 16);
            u8g2.setDrawColor(2);
            config_fun(1);
            u8g2.setCursor(110, 16 * 1 - 2);
            if (eeprom.data.clock_mode > CLOCK_MAX_MODE)
                u8g2.print("0");
            else
                u8g2.print(eeprom.data.clock_mode);
            u8g2.sendBuffer();
        } while (config_last_state > config_state);
    }
    else
    {
        do
        {
            u8g2.clearBuffer();
            u8g2.setFontMode(1);
            u8g2.setDrawColor(1);
            u8g2.drawBox(0, (config_last_state)++, 128, 16);
            u8g2.setDrawColor(2);
            config_fun(1);
            u8g2.setCursor(110, 16 * 1 - 2);
            if (eeprom.data.clock_mode > CLOCK_MAX_MODE)
                u8g2.print("0");
            else
                u8g2.print(eeprom.data.clock_mode);
            u8g2.sendBuffer();
        } while (config_last_state <= config_state);
    }
    config_last_state = config_state;
}
/*
函 数 名:void instrustions(void)
功能说明:LambdaTV介绍
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void instrustions(void)
{
    config_state = 16 * 3;
    if (config_state < config_last_state)
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)--, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state > config_state);
    }
    else
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)++, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state <= config_state);
    }
    config_last_state = config_state;
}
/*
函 数 名:void clear_wifi(void)
功能说明:清除WIFI信息
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void clear_wifi(void)
{
    config_state = 16 * 2;
    if (config_state < config_last_state)
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)--, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state > config_state);
    }
    else
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)++, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state <= config_state);
    }
    config_last_state = config_state;
}
/*
函 数 名:void config_about(void)
功能说明:关于
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void config_about(void)
{
    config_state = 16 * 3;
    if (config_state < config_last_state)
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(0);
            u8g2.drawBox(0, (config_last_state)--, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state > config_state);
    }
    else
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(0);
            u8g2.drawBox(0, (config_last_state)++, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state <= config_state);
    }
    config_last_state = config_state;
}
/*
函 数 名:void instrustions_enter(void)
功能说明:进入介绍页面
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void instrustions_enter(void)
{
    const char *instru_title = "操作说明";
    const char *instru_1 = "单击C 下一步";
    const char *instru_2 = "双击C 上一步";
    const char *instru_3 = "单击S确认 长按S退出";
    uint8_t str_len = 0;
    str_len = u8g2.getUTF8Width(instru_title);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 1 - 2);
    u8g2.print(instru_title);
    u8g2.drawBox(((OLED_WIDTH - str_len) / 2) - 2, 0, str_len + 4, 16);
    // u8g2.drawHLine(((OLED_WIDTH-str_len)/2-2),16,str_len+4);
    u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
    str_len = u8g2.getUTF8Width(instru_1);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 2 - 2);
    u8g2.print(instru_1);
    str_len = u8g2.getUTF8Width(instru_2);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 3 - 2);
    u8g2.print(instru_2);
    str_len = u8g2.getUTF8Width(instru_3);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 4 - 2);
    u8g2.print(instru_3);
    u8g2.sendBuffer();
}
/*
函 数 名:void about_enter(void)
功能说明:进入关于界面
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void about_enter(void)
{
    const char *about_title = "关于";
    const char *about_1 = "LambdaTV";
    uint8_t str_len = 0;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
    str_len = u8g2.getUTF8Width(about_title);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 1 - 2);
    u8g2.print(about_title);
    u8g2.drawBox(((OLED_WIDTH - str_len) / 2) - 2, 0, str_len + 4, 16);
    // u8g2.drawHLine(((OLED_WIDTH-str_len)/2)-2,16,14*2+4);
    u8g2.setFont(u8g2_font_ncenB14_tr);
    str_len = u8g2.getUTF8Width(about_1);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 2);
    u8g2.print(about_1);
    u8g2.setFont(u8g2_font_unifont_t_shopl16);
    u8g2.setCursor(0, 16 * 3);
    u8g2.print("by RAiny");
    u8g2.sendBuffer();
}
/*
函 数 名:void clock_mode_enter(void)
功能说明:进入时钟模式界面
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void clock_mode_enter(void)
{
    uint8_t mode = 0;
    uint8_t box_y = 0;
    const char *mode_title = "时钟模式";
    uint8_t str_len = 0;
    mode = eeprom.data.clock_mode;
    while (1)
    {
        while (box_y < 27)
        {
            u8g2.clearBuffer();
            u8g2.setFontMode(1); /* activate transparent font mode */
            u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
            str_len = u8g2.getUTF8Width(mode_title);
            u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 1 - 2);
            u8g2.setDrawColor(2);
            u8g2.print(mode_title);
            u8g2.drawBox(((OLED_WIDTH - str_len) / 2) - 2, 0, str_len + 4, 16);
            // u8g2.drawHLine(((OLED_WIDTH-str_len)/2)-2,16,14*4+4);
            u8g2.setFont(u8g2_font_ncenB24_tr);
            str_len = u8g2.getUTF8Width("1");
            u8g2.setDrawColor(1); //白 /* color 1 for the box */
            u8g2.drawBox(((OLED_WIDTH - str_len) / 2) - 4, 16 + 12 - 1, box_y++, box_y++);
            u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 + 12 + 24);
            if (eeprom.data.clock_mode > CLOCK_MAX_MODE) //一开始EPPROM不为零
            {
                mode = eeprom.data.clock_mode = 0;
            }
            u8g2.setDrawColor(0); //黑
            u8g2.print(mode);
            u8g2.sendBuffer();
            delay(10);
        }
        if (get_keymenu_event() == KEY_NEXT)
        {
            clear_keymenu_event();
            box_y = 0;
            mode += 1;
            if (mode >= CLOCK_MAX_MODE)
                mode = 0;
        }
        else if (get_keymenu_event() == KEY_PRVE)
        {
            clear_keymenu_event();
            box_y = 0;
            mode -= 1;
            if (mode <= 0)
                mode = CLOCK_MAX_MODE;
        }
        else if (get_keymenu_event() == KEY_CONFIRM)
        {
            clear_keymenu_event();
            eeprom.data.clock_mode = mode;
            eeprom_write();
        }
        else if (get_keymenu_event() == KEY_CANCEL)
        {
            //这里不要清除，外面会进行清除
            // clear_keymenu_event();
            break;
        }
        delay(50);
    }
}
/*
函 数 名:void select_cancel_anima(uint8_t x1)
功能说明:清除WiFi 选择框移动动画
形    参:void
返 回 值:void
时    间：2020-1-5
RAiny
*/
void select_cancel_anima(const char *title, const char *content, uint8_t x1)
{
    uint8_t str_len = 0;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy14_t_gb2312a);
    str_len = u8g2.getUTF8Width(title);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 1 - 2);
    u8g2.print(title);
    u8g2.drawBox(((OLED_WIDTH - str_len) / 2) - 2, 0, str_len + 4, 16);
    // u8g2.drawHLine(((OLED_WIDTH-str_len)/2)-2,16,14*4+4);
    str_len = u8g2.getUTF8Width(content);
    u8g2.setCursor(((OLED_WIDTH - str_len) / 2), 16 * 2 - 2 + 5);
    u8g2.print(content);
    u8g2.setCursor(24, 16 * 4 - 7);
    u8g2.print("取消");
    u8g2.setCursor(24 + 28 + 28, 16 * 4 - 7);
    u8g2.print("确认");
    u8g2.drawFrame(0, 16 + 2, 128, 64 - 18);   //大框框
    u8g2.drawBox(x1, 16 * 4 - 7 - 16, 34, 20); //确认框
    // u8g2.drawFrame(x2,16*4-7-16,34,20);
    u8g2.sendBuffer();
}
/*
函 数 名:void clear_wifi_enter(void)
功能说明:清除WiFi
形    参:void
返 回 值:void
时    间：2020-1-1
RAiny
*/
void clear_wifi_enter(void)
{
    int8_t select = 0; //0 是取消，1是确认
    uint8_t show_flag = 0, frame_x = 0;
    const uint8_t x1 = 24 - 4, x2 = 24 + 28 + 28 - 4;
    const char *wifi_title = "清除WiFi";
    const char *wifi_1 = "清除所有WiFi信息";
    while (1)
    {
        if (show_flag == 0) //初次进入
        {
            select_cancel_anima(wifi_title, wifi_1, x1);
        }
        else if (show_flag == 1)
        {
            if (select == 1)
            {
                while (frame_x < x2)
                {
                    select_cancel_anima(wifi_title, wifi_1, frame_x++);
                }
            }
            else if (select == 0)
            {
                while (frame_x > x1)
                {
                    select_cancel_anima(wifi_title, wifi_1, frame_x--);
                }
            }
        }
        if (get_keymenu_event() == KEY_NEXT)
        {
            clear_keymenu_event();
            select += 1;
            show_flag = 1; //按下按键
            if (select > 1)
            {
                select = 0;
            }
            if (select == 1)
            {
                frame_x = x1;
            }
            if (select == 0)
            {
                frame_x = x2;
            }
        }
        else if (get_keymenu_event() == KEY_PRVE)
        {
            clear_keymenu_event();
            select -= 1;
            show_flag = 1;
            if (select < 0)
            {
                select = 1;
            }
            if (select == 1)
            {
                frame_x = x1;
            }
            if (select == 0)
            {
                frame_x = x2;
            }
        }
        else if (get_keymenu_event() == KEY_CONFIRM)
        {
            clear_keymenu_event();
            if (select == 1)
            {
                //清除ESP8266所存储的WiFi连接信息
                wifiManager.resetSettings();
                ESP.restart();
            }
            else if (select == 0)
            {
                set_keymenu_event(KEY_CANCEL);
                break;
            }
        }
        else if (get_keymenu_event() == KEY_CANCEL)
        {
            //这里不要清除，外面会进行清除
            // clear_keymenu_event();
            break;
        }
        delay(50);
    }
}
/*
函 数 名:void close_open_rgb(void)
功能说明:设置RGB_LED
形    参:void
返 回 值:void
时    间：2020-1-8
RAiny
*/
void close_open_rgb(void)
{
    config_state = 16;
    if (config_state < config_last_state)
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)--, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state > config_state);
    }
    else
    {
        do
        {
            u8g2.clearBuffer();
            config_fun(1);
            u8g2.drawBox(0, (config_last_state)++, 128, 16);
            u8g2.sendBuffer();
        } while (config_last_state <= config_state);
    }
    config_last_state = config_state;
}
/*
函 数 名:void close_open_rgb_enter(void)
功能说明:进入打开关闭RGB设置页面
形    参:void
返 回 值:void
时    间：2020-1-8
RAiny
*/
void close_open_rgb_enter(void)
{
    int8_t select = 0; //0 是取消，1是确认
    uint8_t show_flag = 0, frame_x = 0;
    const uint8_t x1 = 24 - 4, x2 = 24 + 28 + 28 - 4;
    const char *rgb_title = "设置RGB_LED";
    const char *rgb_content = "开启RGB_LED";
    while (1)
    {
        if (show_flag == 0) //初次进入
        {
            select_cancel_anima(rgb_title, rgb_content, x1);
        }
        else if (show_flag == 1)
        {
            if (select == 1)
            {
                while (frame_x < x2)
                {
                    select_cancel_anima(rgb_title, rgb_content, frame_x++);
                }
            }
            else if (select == 0)
            {
                while (frame_x > x1)
                {
                    select_cancel_anima(rgb_title, rgb_content, frame_x--);
                }
            }
        }
        if (get_keymenu_event() == KEY_NEXT)
        {
            clear_keymenu_event();
            select += 1;
            show_flag = 1; //按下按键
            if (select > 1)
            {
                select = 0;
            }
            if (select == 1)
            {
                frame_x = x1;
            }
            if (select == 0)
            {
                frame_x = x2;
            }
        }
        else if (get_keymenu_event() == KEY_PRVE)
        {
            clear_keymenu_event();
            select -= 1;
            show_flag = 1;
            if (select < 0)
            {
                select = 1;
            }
            if (select == 1)
            {
                frame_x = x1;
            }
            if (select == 0)
            {
                frame_x = x2;
            }
        }
        else if (get_keymenu_event() == KEY_CONFIRM)
        {
            clear_keymenu_event();
            if (select == 1)
            {
                eeprom.data.led_on = 1;
                eeprom_write();
                set_keymenu_event(KEY_CANCEL);
                break;
            }
            else if (select == 0)
            {
                eeprom.data.led_on = 0;
                eeprom_write();
                set_keymenu_event(KEY_CANCEL);
                break;
            }
        }
        else if (get_keymenu_event() == KEY_CANCEL)
        {
            //这里不要清除，外面会进行清除
            // clear_keymenu_event();
            break;
        }
        delay(50);
    }
}