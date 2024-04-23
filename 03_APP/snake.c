
#include "../05_Configs/AppCfg.h"

#if CFG_IS_CURRENT_APP(CFG_APP_MAIN)
#include "../00_LIB/Platform_Types.h"
#include "../01_MCAL/02_Interrupt/Interrupt.h"
#include "../02_HAL/01_Switch/SWITCH.h"
#include "../02_HAL/03_LCD/LCD.h"
#define  F_CPU      8000000
#include <util/delay.h>
uint8_t screen_segments[32][8] = { 0 };


uint8_t tail_x = 0;
uint8_t tail_y = 0;
uint8_t head_x = 0;
uint8_t head_y = 9;
uint8_t direction = 0;
boolean eaten = 1;
uint8_t score = 0;

#define CLR_BIT(NUMBER,BIT)     NUMBER &= ~(1<<BIT)
#define SET_BIT(NUMBER,BIT)     NUMBER |= (1<<BIT)   
#define GET_BIT(byte, position) (((byte) >> (position)) & 1)
uint8_t Food_y;
uint8_t Food_x;
boolean game_end = FALSE;
uint8_t Food_segment;
uint8_t pos ;


void left_button(void* y)
{
	direction++;
	direction %=4;
}

void right_button(void * x)
{
	direction--;
	direction %=4;
}

uint8_t food[] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b10000
};



uint8_t compare_2d_arrays(uint8_t screen_segment_1, uint8_t screen_segment_2) {
	for (int j = 0; j < 8; j++) {
		if (screen_segments[screen_segment_1][j] != screen_segments[screen_segment_2][j]) 
		{
				return 0; 
		}
	}
	return 1; 
}



void display()
{
	int map[32];
	memset(map,-1,sizeof(map));
	uint8_t CGRAM = 0;
	if(eaten)
	{
		for(int i = 0 ; i < 8 ; i++)
		{
			screen_segments[pos][i] |= food[i];
		}
		eaten = FALSE;
	}
	for(uint8_t i = 0 ; i < 32 ; i++)
	{
		for(uint8_t j = 0 ; j < 8 ; j++)
		{
			if(screen_segments[i][j] > 0)
			{
				boolean in = TRUE;
				for(int e = 0 ; e < 32 ;e++)
				{
					if(map[e] != -1)
					{
						if(compare_2d_arrays(e , i))
						{
							map[i] = map[e];
							in = FALSE;
							break;
						}
					}
				}
				if(in)
				{
					map[i] = CGRAM;
					CGRAM++;
				}
				break;
			}
		}
	}
	LCD_enuGotoDDRAM_XY(0,0);
	uint8_t visit[9] ;
	memset(visit,0,sizeof(visit));
	for(uint8_t i = 0 ; i < 32 ; i++)
	{
		if(map[i] != -1 && !visit[map[i]])
		{
			LCD_enuDisplaySpecialPattern(&screen_segments[i],map[i],i/16,i%16);
			visit[map[i]] = 1;
		}
		else
		{
			if(visit[map[i]] == 1)
			{
			    LCD_enuWriteData(map[i]);
			}
			else
			{
		   	    LCD_enuWriteData(' ');
			}
		}
	}
}


void move ()
{

  uint8_t tail_segment = ((tail_x / 8) * 16) + (tail_y / 5);
  uint8_t head_segment ;
  CLR_BIT(screen_segments[tail_segment][tail_x % 8], 4 - (tail_y%5) );
  switch(direction)
  {
      case 0:
            if(head_y + 1 >= 80)
            {
                game_end = TRUE;
                break;
            }
            head_segment = ((head_x / 8) * 16) + ((head_y + 1) / 5);
	    if(head_segment == pos &&  (head_x % 8) == 7)
	    {
		     score++;
   	    }
            SET_BIT(screen_segments[head_segment][head_x % 8],4- ((head_y+1)%5));
            head_y++;
            break;
      case 1:
            if(head_x - 1 < 0)
            {
                game_end = TRUE;
                break;
            }
            head_segment = (((head_x - 1) / 8) * 16) + (head_y  / 5);
 
    	    if(head_segment == pos &&  ((head_x-1) % 8) == 7)
    	    {
	    	    score++;
	     }
            SET_BIT(screen_segments[head_segment][(head_x-1) % 8],4- (head_y%5));
            head_x--;
            break;
      case 2 : 
            if(head_y - 1 < 0)
            {
                game_end = TRUE;
                break;
            }
            head_segment = ((head_x / 8) * 16) + ((head_y - 1) / 5);
    	    if(head_segment == pos &&  (head_x % 8) == 7)
	      {
		       	    score++;
	     }

            SET_BIT(screen_segments[head_segment][head_x % 8],4- ((head_y-1)%5) );
            head_y--;
            break;
       case 3 :
            if(head_x + 1 > 15)
            {
                game_end = TRUE;
                break;
            }
            head_segment = (((head_x +1) / 8) * 16) + (head_y  / 5);
       	    if(head_segment == pos &&  (head_x+1) % 8 == 7)
	       {
	          	    score++;
	        }

             SET_BIT(screen_segments[head_segment][(head_x+1) % 8],4- (head_y%5) );
             head_x++;
            break;
  }
  if(!game_end)
  {
      if(GET_BIT(screen_segments[((tail_x / 8) * 16) + ((tail_y+1) / 5)][tail_x%8], 4 - ((tail_y+1)%5))
      )
      {
          tail_y++;
      }
      else if(GET_BIT(screen_segments[((tail_x / 8) * 16) + ((tail_y-1) / 5)][tail_x%8],4-((tail_y-1)%5))
      )
      {
          tail_y--;
      }
      else if(
          
          GET_BIT(screen_segments[(((tail_x + 1) / 8) * 16) + (tail_y / 5)][(tail_x + 1) % 8],4- ((tail_y) % 5))
          
          )
      {
          tail_x++;
      }
      else
      {
          tail_y--;
      }
      
  }
}





int main ()
{

    screen_segments[0][0] = 31; 
    screen_segments[1][0] = 31;
    LCD_init();
    LED_enmInit();
    pos =rand()%32;
    EXINT_init();
    SWITCH_enmInit();
    EXINT_enuSetCallBack(EXINT_enuEXINT0,left_button,NULL);
    EXINT_enuSetCallBack(EXINT_enuEXINT1,right_button,NULL);
    int cnt = 2;
    while (!game_end)
    {
	
	    display(); 
	    move();
	   _delay_ms(300);
    }
    LCD_enuWriteCommand(DISPLY_CLEAR);
    _delay_ms(150);
    char arr[] = "Score: ";
    LCD_enuWriteString(arr,7);
    LCD_enuWriteNumber(score);
  return 0;
}

#endif