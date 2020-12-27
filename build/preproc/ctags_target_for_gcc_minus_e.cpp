# 1 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
# 2 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino" 2
# 3 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino" 2
# 4 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino" 2
# 5 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino" 2
# 6 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino" 2
# 7 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino" 2
# 8 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino" 2
# 9 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino" 2
# 10 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino" 2
# 11 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino" 2
# 12 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino" 2

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2((&u8g2_cb_r0), /* cs=*/ 2, /* dc=*/4 );
OneButton s_button(0, true);
OneButton c_button(5, true);
File dataFile;
Ticker key_tick;
WiFiManager wifiManager;// 建立WiFiManager对象
WiFiUDP ntpUDP;
// 'time.nist.gov' is used (default server) with +1 hour offset (3600 seconds) 60 seconds (60000 milliseconds) update interval
// NTPClient timeClient(ntpUDP, "time.nist.gov", 60*60*7, 60000);
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com",60*60*8, 30*60*1000);
LambdaTV_INF lambdaTV;
uint8_t badapple_buf[1024] ={};//更新badapple的数组
uint8_t num_kongbai[168] ={0x00};//空白 闪烁效果
void time_update(void);
void bad_apple(void);
void web_introduce(void);
void config(void);
void (*current_operation_index)();
menu_state current_state = { 8, 8, 0 };
menu_state destination_state = { 8, 8, 0 };
// encoding values, see: https://github.com/olikraus/u8g2/wiki/fntgrpiconic
menu_entry_type menu_entry_list[] =
{
  { u8g2_font_open_iconic_app_4x_t,69,"Clock",(*time_update)},
  { u8g2_font_open_iconic_play_4x_t, 78, "BadApple",(*bad_apple)},
  { u8g2_font_open_iconic_embedded_4x_t,68, "Web",(*web_introduce)},
  { u8g2_font_open_iconic_embedded_4x_t,72, "Config",(*config)},
  { __null, 0, __null,__null}
};
/*

函 数 名:void print_fs_info(void)

功能说明:打印fs信息

形    参:void

返 回 值:void

时    间：2020-12-20

RAiny

*/
# 50 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void print_fs_info(void)
{
  FSInfo fs_info;
  // 闪存文件系统信息
  SPIFFS.info(fs_info);

  // 可用空间总和（单位：字节）
  Serial.print("totalBytes: ");
  Serial.print(fs_info.totalBytes);
  Serial.println(" Bytes");

  // 已用空间（单位：字节）
  Serial.print("usedBytes: ");
  Serial.print(fs_info.usedBytes);
  Serial.println(" Bytes");

  // 最大文件名字符限制（含路径和'\0'）
  Serial.print("maxPathLength: ");
  Serial.println(fs_info.maxPathLength);

  // 最多允许打开文件数量
  Serial.print("maxOpenFiles: ");
  Serial.println(fs_info.maxOpenFiles);
}
/*

函 数 名:void LambdaTV(void)

功能说明:开机LOGO

形    参:void

返 回 值:void

时    间：2020-12-20

RAiny

*/
# 82 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void LambdaTV(void)
{
  uint8_t x=20;
  do
  {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    //u8g2.setFont(u8g2_font_courB24_tn);
    u8g2.clearBuffer();
    /* Solid font mode with XOR drawing: Does not make much sense */
    u8g2.setFontMode(0);
    u8g2.setDrawColor(2);
    u8g2.drawStr(x,26, "LambdaTV");
    u8g2.drawHLine(0, 29, 128);

    /* Transparent font mode with XOR drawing: looks good */
    u8g2.setFontMode(1);
    u8g2.setDrawColor(2);
    u8g2.drawStr(x,26+32, "LambdaTV");
    u8g2.drawHLine(0, 29+32, 128);
    u8g2.sendBuffer();
    delay(200);
    x--;
  } while( x != 0 );
}
/*

函 数 名:void bad_apple(void)

功能说明:BadApple

形    参:void

返 回 值:void

时    间：2020-12-21

RAiny

*/
# 114 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void bad_apple(void)
{
  char data_read;
  static int data_len=0;
  //建立File对象用于从SPIFFS中读取文件
  String file_name="/apple.bin";
  dataFile = SPIFFS.open(file_name, "r");
  //确认闪存中是否有file_name文件
  if (SPIFFS.exists(file_name))
  {
    Serial.println(file_name+" FOUND");
    Serial.println(dataFile.size());
  }
  else
  {
    Serial.print(file_name+" NOT FOUND");
  }
  // dataFile.readBytes
  for(uint64_t xbm_num=0;xbm_num<dataFile.size();xbm_num++)
  {
    data_read=(char)dataFile.read();
    badapple_buf[data_len++]=data_read;
    if(data_len==1024)//分辨率 128*64
    {
      delayMicroseconds(34500);//不延时3029张一共用时107S，差不多FPS=30.7
      u8g2.clearBuffer();
      u8g2.drawXBM(0,0,128,64, badapple_buf);
      u8g2.sendBuffer();
      data_len=0;
    }
    if(get_keymenu_event()==KEY_CANCEL)
    {
      clear_keymenu_event();
      data_len=0;
      break;
    }
  }
  Serial.print("BadApple Play finish");
  //完成文件读取后关闭文件
  dataFile.close();
  delay(10);//可以延时一下
}
/*

函 数 名:void time_ipdate_anima(void)

功能说明:时间数字更新动画

形    参:void

返 回 值:void

时    间：2020-12-26

RAiny

*/
# 164 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void time_ipdate_anima(uint8_t x,uint8_t y,uint8_t bin_num)
{
  char file_name_buff[10];
  char data_read;
  static int data_len=0;
  //建立File对象用于从SPIFFS中读取文件
  sprintf(file_name_buff,"/%d.bin",bin_num);
  String file_name=file_name_buff;
  dataFile = SPIFFS.open(file_name, "r");
  //确认闪存中是否有file_name文件
  if (SPIFFS.exists(file_name))
  {
    Serial.println(file_name+" FOUND");
    Serial.println(dataFile.size());
  }
  else
  {
    Serial.print(file_name+" NOT FOUND");
  }
  // dataFile.readBytes
  for(uint64_t xbm_num=0;xbm_num<dataFile.size();xbm_num++)
  {
    data_read=(char)dataFile.read();
    badapple_buf[data_len++]=data_read;
    if(data_len==168)//分辨率 25*42
    {
      delay(150);
      u8g2.clearBuffer();
      u8g2.drawXBM(x,y,25,42, badapple_buf);
      // u8g2.sendBuffer();  
      //局部刷新有个问题就是不能全部覆盖xbm图片，并不是8的完整倍数
      if(x==2)
      {
        u8g2.updateDisplayArea(0,y/8,25/8,42/8);
      }
      else if(x==(2+25))
      {
        u8g2.updateDisplayArea(3,y/8,25/8,42/8);//局部刷新要/8
      }
      else if(x==(2+25*3))
      {
        u8g2.updateDisplayArea(10,y/8,25/8,42/8);
      }
      else if(x==(2+25*4/*xbm 时间的x坐标*/))
      {
        u8g2.updateDisplayArea(13,y/8,25/8,42/8);
      }
      data_len=0;
    }
  }
  //完成文件读取后关闭文件
  dataFile.close();
  delay(200);
}
/*

函 数 名:void time_select_draw(uint8_t x,uint8_t y,uint8_t num)

功能说明:将时间分为十位和个位显示手写数字xbm图片

形    参:uint8_t x：x坐标

        uint8_t y：y坐标

        uint8_t num：显示数字 大于等于10为空白显示

返 回 值:void

时    间：2020-12-26

RAiny

*/
# 228 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void time_select_draw(uint8_t x,uint8_t y,uint8_t num)
{
  switch (num)
  {
  case 0:
    u8g2.drawXBM(x,y,25,42, write_num0);
    break;
  case 1:
    u8g2.drawXBM(x,y,25,42, write_num1);
    break;
  case 2:
    u8g2.drawXBM(x,y,25,42, write_num2);
    break;
  case 3:
    u8g2.drawXBM(x,y,25,42, write_num3);
    break;
  case 4:
    u8g2.drawXBM(x,y,25,42, write_num4);
    break;
  case 5:
    u8g2.drawXBM(x,y,25,42, write_num5);
    break;
  case 6:
    u8g2.drawXBM(x,y,25,42, write_num6);
    break;
  case 7:
    u8g2.drawXBM(x,y,25,42, write_num7);
    break;
  case 8:
    u8g2.drawXBM(x,y,25,42, write_num8);
    break;
  case 9:
    u8g2.drawXBM(x,y,25,42, write_num9);
    break;
  case 10://显示冒号：
    u8g2.drawXBM(x,y,25,42, write_maohao);
    break;
  default://其它均不显示
    u8g2.drawXBM(x,y,25,42, num_kongbai);
    break;
  }
}
/*

函 数 名:void time_show(uint8_t hour,uint8_t minu)

功能说明:显示手写数字时间，将数字对应的xbm图片显示

形    参:uint8_t hour：时

        uint8_t minu：分

返 回 值:void

时    间：2020-12-26

RAiny

*/
# 279 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void time_show(uint8_t hour,uint8_t minu)
{
  uint8_t hour_high=0,hour_low=0,minu_high=0,minu_low=0;
  static uint8_t maohao_flag=0;
  static uint8_t last_hour_high,last_hour_low,last_minu_high,last_minu_low;
  hour_high=hour/10;
  hour_low=hour%10;
  minu_high=minu/10;
  minu_low=minu%10;
  u8g2.clearBuffer();
  if(last_minu_low==minu_low)//不去更新
  {
    time_select_draw(2+25*4/*xbm 时间的x坐标*/,(64 -42)/2,minu_low);
  }
  if(last_minu_high==minu_high)
  {
    time_select_draw(2+25*3,(64 -42)/2,minu_high);
  }
  if(maohao_flag==0)
  {
    time_select_draw(2+25*2,(64 -42)/2,10);
    maohao_flag=1;
  }
  else if(maohao_flag==1)
  {
    time_select_draw(2+25*2,(64 -42)/2,11);
    maohao_flag=0;
  }
  if(last_hour_low==hour_low)
  {
    time_select_draw(2+25,(64 -42)/2,hour_low);
  }
  if(last_hour_high==hour_high)
  {
    time_select_draw(2,(64 -42)/2,hour_high);
  }
  u8g2.sendBuffer();
  //在这里更新动画
  if(last_minu_low!=minu_low)
  {
    time_ipdate_anima((2+25*4/*xbm 时间的x坐标*/),(64 -42)/2,minu_low);
  }
  if(last_minu_high!=minu_high)
  {
    time_ipdate_anima((2+25*3),(64 -42)/2,minu_high);
  }
  if(last_hour_low!=hour_low)
  {
    time_ipdate_anima((2+25),(64 -42)/2,hour_low);
  }
  if(last_hour_high!=hour_high)
  {
    time_ipdate_anima(2,(64 -42)/2,hour_high);
  }
  last_hour_high=hour_high;
  last_hour_low=hour_low;
  last_minu_high=minu_high;
  last_minu_low=minu_low;
}
/*

函 数 名:void time_update(void)

功能说明:NTP Time 更新 周期100ms

形    参:void

返 回 值:void

时    间：2020-12-25

RAiny

*/
# 346 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void time_update(void)
{
  uint8_t time_minu,time_hour;
  while(1)
  {
    timeClient.update();
    unsigned long unix_epoch = timeClient.getEpochTime();
    time_minu=minute(unix_epoch); // get minutes (0 - 59)
    time_hour=hour(unix_epoch); // get hours   (0 - 23)
    Serial.print("hour is ");
    Serial.print(time_hour);
    Serial.print("minu is ");
    Serial.print(time_minu);
    Serial.print("\r\n");
    time_show(time_hour,time_minu);
    if(get_keymenu_event()==KEY_CANCEL)
    {
      clear_keymenu_event();
      break;
    }
    delay(1000);
  }
}
/*

函 数 名:void web_introduce(void)

功能说明:Web访问

形    参:void

返 回 值:void

时    间：2020-12-24

RAiny

*/
# 377 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void web_introduce(void)
{
  while(1)
  {
    Serial.print("web_introduce");
    if(get_keymenu_event()==KEY_CANCEL)
    {
      clear_keymenu_event();
      break;
    }
    delay(500);
  }
}
/*

函 数 名:void config(void)

功能说明:设置

形    参:void

返 回 值:void

时    间：2020-12-25

RAiny

*/
# 398 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void config(void)
{
  while(1)
  {
    Serial.print("config");
    if(get_keymenu_event()==KEY_CANCEL)
    {
      clear_keymenu_event();
      break;
    }
    delay(500);
  }
}
/*

函 数 名:void key_check(void)

功能说明:按键检测函数 周期10ms

形    参:void

返 回 值:void

时    间：2020-12-24

RAiny

*/
# 419 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void key_check(void)
{
  s_button.tick();
  c_button.tick();
}
/*

函 数 名:void select_menu(void)

功能说明:选择菜单

形    参:void

返 回 值:void

时    间：2020-12-24

RAiny

*/
# 432 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void select_menu(void)
{
  KEY_EVENT_INF menu_event=KEY_NOEVENT;
  do
  {
    u8g2.clearBuffer();
    draw(&current_state);
    u8g2.setFont(u8g2_font_helvB10_tr);
    u8g2.setCursor((u8g2.getDisplayWidth()-u8g2.getStrWidth(menu_entry_list[destination_state.position].name))/2,u8g2.getDisplayHeight()-5);
    u8g2.print(menu_entry_list[destination_state.position].name);
    u8g2.sendBuffer();
    delay(10);
    menu_event=get_keymenu_event();
    if(menu_event==KEY_NEXT)
    {
      to_right(&destination_state);
      clear_keymenu_event();
    }
    else if(menu_event==KEY_PRVE)
    {
      to_left(&destination_state);
      clear_keymenu_event();
    }
    else if(menu_event==KEY_CONFIRM)
    {
      current_operation_index=menu_entry_list[destination_state.position].current_operation;
      (*current_operation_index)();//执行当前操作函数
      clear_keymenu_event();
    }
  } while(towards(&current_state, &destination_state));
}
/*

函 数 名:void setup(void)

功能说明:ESP8266 初始化函数 

形    参:void

返 回 值:void

时    间：2020-12-20

RAiny

*/
# 471 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void setup(void)
{
  Serial.begin(115200);
  u8g2.begin();
  u8g2.enableUTF8Print();
  SPI.setClockDivider(0x00101001 /*8 MHz*/);
  if(SPIFFS.begin())// 启动闪存文件系统
  {
    Serial.println("SPIFFS Started.");
  }
  else
  {
    Serial.println("SPIFFS Failed to Start.");
  }
  //SPIFFS.format();    // 格式化SPIFFS
  rgb_led_init();
  u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
  u8g2.firstPage();
  do
  {
    u8g2.setCursor(10, 14);
    u8g2.print("连接WiFi中...");
    u8g2.setCursor(10, 28);
    u8g2.print("如果长时间没有进入");
    u8g2.setCursor(10, 42);
    u8g2.print("1.连接LambdaTV");
    u8g2.setCursor(10, 56);
    u8g2.print("2.打开网页配置WiFi");
  }while(u8g2.nextPage());
  // 清除ESP8266所存储的WiFi连接信息以便测试WiFiManager工作效果
  // wifiManager.resetSettings();
  wifiManager.setTimeout(60*3);//配置超时时间S 
  // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称
  if(!wifiManager.autoConnect("LambdaTV"))//xzh1978n1120abc
  {
    //没有WiFi

  }
  // 如果您希望该WiFi添加密码，可以使用以下语句：
  // wifiManager.autoConnect("AutoConnectAP", "12345678");
  // 以上语句中的12345678是连接AutoConnectAP的密码
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.print("ESP8266 Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  print_fs_info();
  key_init();
  LambdaTV();
  key_tick.attach_ms(10, key_check);
}
/*

函 数 名:void loop(void) 

功能说明:ESP8266 循环函数

形    参:void

返 回 值:void

时    间：2020-12-20

RAiny

*/
# 531 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV.ino"
void loop(void)
{
  select_menu();
}
# 1 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"

/*

函 数 名:void s_click(void)

功能说明:s键按一下

形    参:void

返 回 值:void

时    间：2020-12-24

RAiny

*/
# 10 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void s_click(void)
{
    lambdaTV.key_event=KEY_CONFIRM;
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
# 23 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
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
# 35 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void s_longclick(void)
{
    lambdaTV.key_event=KEY_CANCEL;
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
# 48 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void c_click(void)
{
    lambdaTV.key_event=KEY_NEXT;
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
# 61 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void c_doubleclick(void)
{
    lambdaTV.key_event=KEY_PRVE;
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
# 74 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void c_longclick(void)
{
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
# 86 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
KEY_EVENT_INF get_keymenu_event(void)
{
    KEY_EVENT_INF event;
    event=lambdaTV.key_event;
    return event;
}
/*

函 数 名:void clear_keymenu_event(void)

功能说明:清除按键事件

形    参:void

返 回 值:void

时    间：2020-12-25

RAiny

*/
# 100 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void clear_keymenu_event(void)
{
    lambdaTV.key_event=KEY_NOEVENT;
}
/*

函 数 名:void key_init(void)

功能说明:按键初始化，用了OneButton库

形    参:void

返 回 值:void

时    间：2020-12-24

RAiny

*/
# 112 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
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
# 129 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_key.ino"
void rgb_led_init(void)
{
    // pinMode(RGB_R_PIN, OUTPUT);
    pinMode(3, 0x01);
    pinMode(12, 0x01);
    // analogWrite(RGB_R_PIN, 0);
    analogWrite(3, 1024);
    analogWrite(12,0);
}
# 1 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino"
# 2 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino" 2

/*

U8g2的IconMenu.ino example

*/
# 7 "c:\\Users\\HUAWEI\\Desktop\\esp8266_oled\\LambdaTV\\LambdaTV_menu.ino"
void draw(menu_state *state)
{
  int16_t x;
  uint8_t i;
  x = state->menu_start;
  i = 0;
  while(menu_entry_list[i].icon > 0 )
  {
    if ( x >= -32 && x < u8g2.getDisplayWidth() )
    {
      u8g2.setFont(menu_entry_list[i].font);
      u8g2.drawGlyph(x, 32+8, menu_entry_list[i].icon );
    }
    i++;
    x += 32 + 8;
  }
  u8g2.drawFrame(state->frame_position-1, 32+8 -32 -1, 32 +2, 32 +2);
  u8g2.drawFrame(state->frame_position-2, 32+8 -32 -2, 32 +4, 32 +4);
  u8g2.drawFrame(state->frame_position-3, 32+8 -32 -3, 32 +6, 32 +6);
}

void to_right(menu_state *state)
{
  if ( menu_entry_list[state->position+1].font != __null )
  {
    if ( (int16_t)state->frame_position+ 2*(int16_t)32 + (int16_t)8 < (int16_t)u8g2.getDisplayWidth() )
    {
      state->position++;
      state->frame_position += 32 + (int16_t)8;
    }
    else
    {
      state->position++;
      state->frame_position = (int16_t)u8g2.getDisplayWidth() - (int16_t)32 - (int16_t)8;
      state->menu_start = state->frame_position - state->position*((int16_t)32 + (int16_t)8);
    }
  }
}

void to_left(menu_state *state)
{
  if ( state->position > 0 )
  {
    if ( (int16_t)state->frame_position >= (int16_t)8 +(int16_t)32 + (int16_t)8 )
    {
      state->position--;
      state->frame_position -= 32 + (int16_t)8;
    }
    else
    {
      state->position--;
      state->frame_position = 8;
      state->menu_start = state->frame_position - state->position*((int16_t)32 + (int16_t)8);

    }
  }
}

uint8_t towards_int16(int16_t *current, int16_t dest)
{
  if ( *current < dest )
  {
    (*current)++;
    return 1;
  }
  else if ( *current > dest )
  {
    (*current)--;
    return 1;
  }
  return 0;
}

uint8_t towards(menu_state *current,menu_state *destination)
{
  uint8_t r = 0;
  r |= towards_int16( &(current->frame_position), destination->frame_position);
  r |= towards_int16( &(current->frame_position), destination->frame_position);
  r |= towards_int16( &(current->menu_start), destination->menu_start);
  r |= towards_int16( &(current->menu_start), destination->menu_start);
  return r;
}