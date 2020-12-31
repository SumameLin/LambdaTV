#include <U8g2lib.h>
#include "LambdaTV.h"
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
  uint8_t i,wid_max=3;
  x = state->menu_start;
  i = 0;
  while(menu_entry_list[i].icon > 0 )  
  {
    if ( x >= -ICON_WIDTH && x < u8g2.getDisplayWidth() )
    {
      u8g2.setFont(menu_entry_list[i].font);
      u8g2.drawGlyph(x, ICON_Y, menu_entry_list[i].icon );
    }
    i++;
    x += ICON_WIDTH + ICON_GAP;
  }
  for(uint8_t wid=0;wid<wid_max;wid++)//多条直线重叠，来实现线的宽度
  {
    u8g2.drawHVLine(state->frame_position-1,ICON_GAP-3-wid,ICON_WIDTH+2,0);//往上增加
    u8g2.drawHVLine(state->frame_position-2-wid,ICON_GAP-3+8-wid_max,8,3);//往左增加
    u8g2.drawHVLine(state->frame_position-1+ICON_WIDTH+2+wid,ICON_GAP-3+8-wid_max,8,3);//往右增加

    u8g2.drawHVLine(state->frame_position-1,ICON_Y+3+wid,ICON_WIDTH+2,0);//往下增加
    u8g2.drawHVLine(state->frame_position-2-wid,ICON_Y+3+wid_max-1,8,3);//往左增加
    u8g2.drawHVLine(state->frame_position-1+ICON_WIDTH+2+wid,ICON_Y+3+wid_max-1,8,3);//往右增加
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
  if ( menu_entry_list[state->position+1].font != NULL )
  {
    //小于128
    if ( (int16_t)state->frame_position+ 2*(int16_t)ICON_WIDTH + (int16_t)ICON_BGAP < (int16_t)u8g2.getDisplayWidth() )//获取显示器的宽度
    {
      state->position++;
      state->frame_position += ICON_WIDTH + (int16_t)ICON_GAP;
    }
    else
    {
      state->position++;      
      state->frame_position = (int16_t)u8g2.getDisplayWidth() - (int16_t)ICON_WIDTH - (int16_t)ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
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
  if ( state->position > 0 )
  {
    if ( (int16_t)state->frame_position >= (int16_t)ICON_BGAP+(int16_t)ICON_WIDTH+ (int16_t)ICON_GAP )
    {
      state->position--;
      state->frame_position -= ICON_WIDTH + (int16_t)ICON_GAP;
    }    
    else
    {
      state->position--; 
      state->frame_position = ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
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
/*
函 数 名:uint8_t towards(menu_state *current,menu_state *destination)
功能说明:
形    参:void
返 回 值:void
时    间：2020-12-30
RAiny
*/
uint8_t towards(menu_state *current,menu_state *destination)
{
  uint8_t r = 0;
  r |= towards_int16( &(current->frame_position), destination->frame_position);
  r |= towards_int16( &(current->frame_position), destination->frame_position);
  r |= towards_int16( &(current->menu_start), destination->menu_start);
  r |= towards_int16( &(current->menu_start), destination->menu_start);
  return r;
}
