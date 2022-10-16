#include "lcd.h"
#include "rttlogo.h"
#include "font.h"

// #define DRV_DEBUG
#define LOG_TAG             "spi.lcd"
#include <drv_log.h>


#define LCD_SPI_DEVICE_NAME     "spi40"
#define LCD_PWM_DEV_NAME        "pwm1"
#define LCD_PWM_DEV_CHANNEL     (2)
#define LCD_PWM_DEV_TIME        (10000)//pwm frequency:100K = 10000ns

#define WR_RS_PIN      GET_PIN(E, 13)
#define CS_PIN         GET_PIN(E, 11)
//SPI��ʾ���ӿ�
//LCD_RS
#define LCD_RS_SET      rt_pin_write(WR_RS_PIN, PIN_HIGH)
#define LCD_RS_RESET    rt_pin_write(WR_RS_PIN, PIN_LOW)
//LCD_CS
#define LCD_CS_SET      rt_pin_write(CS_PIN, PIN_HIGH)
#define LCD_CS_RESET    rt_pin_write(CS_PIN, PIN_LOW)


static int32_t lcd_init(void);
static int32_t lcd_writereg(uint8_t reg,uint8_t* pdata,uint32_t length);
static int32_t lcd_readreg(uint8_t reg,uint8_t* pdata);
static int32_t lcd_senddata(uint8_t* pdata,uint32_t length);
static int32_t lcd_recvdata(uint8_t* pdata,uint32_t length);

ST7735_IO_t st7735_pIO = {
	lcd_init,
	RT_NULL,
	RT_NULL,
	lcd_writereg,
	lcd_readreg,
	lcd_senddata,
	lcd_recvdata,
	RT_NULL
};
ST7735_Object_t st7735_pObj;
uint32_t st7735_id;
static struct rt_spi_device *spi_dev_lcd;
static struct rt_device_pwm *lcd_pwm_dev;
static uint32_t NowBrightness;
extern unsigned char WeActStudiologo[];

#ifdef DRV_DEBUG
#ifdef FINSH_USING_MSH
static int show_logo(int argc, char **argv)
{
	uint8_t text[20];
	// ST7735_LCD_Driver.ReadID(&st7735_pObj,&st7735_id);

	// LCD_Light(LCD_PWM_DEV_TIME, 300);

	ST7735_LCD_Driver.DrawBitmap(&st7735_pObj,0,0,WeActStudiologo);
    LCD_SetBrightness(LCD_PWM_DEV_TIME-1);
    rt_thread_mdelay(1000);
    // LCD_SetBrightness(0);

	// ST7735_LCD_Driver.FillRect(&st7735_pObj,0,0,160,80,BLACK);

	// sprintf((char *)&text,"WeAct Studio");
	// LCD_ShowString(4,4,160,16,16,text);
	// sprintf((char *)&text,"STM32H7xx 0x%X",HAL_GetDEVID());
	// LCD_ShowString(4,22,160,16,16,text);
	// sprintf((char *)&text,"LCD ID: 0x%X",st7735_id);
	// LCD_ShowString(4,40,160,16,16,text);
    return 0;
}
MSH_CMD_EXPORT(show_logo, show logo);
#endif /* FINSH_USING_MSH */
#endif /* DRV_DEBUG */

static int LCD_Init(void)
{
	rt_pin_mode(WR_RS_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(CS_PIN, PIN_MODE_OUTPUT);

    spi_dev_lcd = (struct rt_spi_device *)rt_device_find(LCD_SPI_DEVICE_NAME);
    if (!spi_dev_lcd)
    {
        LOG_E("tft-lcd init failed! can't find %s device!\n", LCD_SPI_DEVICE_NAME);
        return -RT_ERROR;
    }

    ST7735_RegisterBusIO(&st7735_pObj,&st7735_pIO);
	if(ST7735_ERROR == ST7735_LCD_Driver.Init(&st7735_pObj,ST7735_FORMAT_RBG565,ST7735_ORIENTATION_LANDSCAPE_ROT180))
    {
        LOG_E("st7735 init failed!");
        // return ;
    }
    ST7735_LCD_Driver.FillRect(&st7735_pObj,0,0,160,80,BLACK);
	ST7735_LCD_Driver.ReadID(&st7735_pObj,&st7735_id);
    ST7735_LCD_Driver.DisplayOn(&st7735_pObj);
    LOG_D("lcd id:0X%08X", st7735_id);
    LOG_D("chip id:0X%08X", HAL_GetDEVID());

    /* turn on the LCD backlight */
    lcd_pwm_dev = (struct rt_device_pwm *)rt_device_find(LCD_PWM_DEV_NAME);
    if (!lcd_pwm_dev)
    {
        LOG_E("lcd pwm pin init failed! can't find %s device!\n", LCD_SPI_DEVICE_NAME);
        return -RT_ERROR;
    }
    /* pwm frequency:100K = 10000ns */
    rt_pwm_set(lcd_pwm_dev, LCD_PWM_DEV_CHANNEL, LCD_PWM_DEV_TIME, 5000);
    rt_pwm_enable(lcd_pwm_dev, LCD_PWM_DEV_CHANNEL);

    ST7735_LCD_Driver.DrawBitmap(&st7735_pObj,0,0,WeActStudiologo);
    // ST7735_LCD_Driver.FillRGBRect(&st7735_pObj, 0, 0, (uint8_t *)image_rttlogo, 240, 69);
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(LCD_Init);


void LCD_SetBrightness(uint32_t Brightness)
{
    Brightness = ((Brightness >= LCD_PWM_DEV_TIME)?(LCD_PWM_DEV_TIME-1):Brightness);
    rt_pwm_set(lcd_pwm_dev, LCD_PWM_DEV_CHANNEL, LCD_PWM_DEV_TIME, Brightness);
    NowBrightness = Brightness;
}

uint32_t LCD_GetBrightness(void)
{
    return NowBrightness;
}

uint16_t POINT_COLOR=0xFFFF;	//������ɫ
uint16_t BACK_COLOR=BLACK;  //����ɫ
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{
    uint8_t temp,t1,t;
    uint16_t y0=y;
    uint16_t x0=x;
    uint16_t colortemp=POINT_COLOR;
    uint32_t h,w;

    uint16_t write[size][size==12?6:8];
    uint16_t count;

    ST7735_GetXSize(&st7735_pObj,&w);
    ST7735_GetYSize(&st7735_pObj,&h);

    //���ô���
    num=num-' ';//�õ�ƫ�ƺ��ֵ
    count = 0;

	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(t=0;t<size;t++)
		{
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608����

			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)
					POINT_COLOR=(colortemp&0xFF)<<8|colortemp>>8;
				else
					POINT_COLOR=(BACK_COLOR&0xFF)<<8|BACK_COLOR>>8;

				write[count][t/2]=POINT_COLOR;
				count ++;
				if(count >= size) count =0;

				temp<<=1;
				y++;
				if(y>=h){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=w){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}
		}
	}
	else//���ӷ�ʽ
	{
		for(t=0;t<size;t++)
		{
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else temp=asc2_1608[num][t];		 //����1608����
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)
					write[count][t/2]=(POINT_COLOR&0xFF)<<8|POINT_COLOR>>8;
				count ++;
				if(count >= size) count =0;

				temp<<=1;
				y++;
				if(y>=h){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=w){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}
		}
	}
	ST7735_FillRGBRect(&st7735_pObj,x0,y0,(uint8_t *)&write,size==12?6:8,size);
	POINT_COLOR=colortemp;
}

//��ʾ�ַ���
//x,y:�������
//width,height:�����С
//size:�����С
//*p:�ַ�����ʼ��ַ
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }
}

void LCD_FillRGBRect(uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height)
{
    ST7735_LCD_Driver.FillRGBRect(&st7735_pObj, Xpos, Ypos, pData, Width, Height);
}

static int32_t lcd_init(void)
{
	return ST7735_OK;
}

static int32_t lcd_writereg(uint8_t reg,uint8_t* pdata,uint32_t length)
{
    int32_t result;
	LCD_CS_RESET;
	LCD_RS_RESET;
    result = rt_spi_send(spi_dev_lcd, &reg, 1);
	LCD_RS_SET;
	if(length > 0)
        result += rt_spi_send(spi_dev_lcd, pdata, length);
	LCD_CS_SET;
    return ((result == length+1)?0:-1);
}

static int32_t lcd_readreg(uint8_t reg,uint8_t* pdata)
{
    int32_t result;
	LCD_CS_RESET;
	LCD_RS_RESET;

	result = rt_spi_send(spi_dev_lcd, &reg, 1);
	LCD_RS_SET;
	result += rt_spi_recv(spi_dev_lcd, pdata, 1);
	LCD_CS_SET;
    return ((result == 2)?0:-1);
}

static int32_t lcd_senddata(uint8_t* pdata,uint32_t length)
{
    int32_t result;
	LCD_CS_RESET;
	//LCD_RS_SET;
	result =rt_spi_send(spi_dev_lcd, pdata, length);
	LCD_CS_SET;
    return ((result == length)?0:-1);
}

static int32_t lcd_recvdata(uint8_t* pdata,uint32_t length)
{
    int32_t result;
	LCD_CS_RESET;
	//LCD_RS_SET;
	result = rt_spi_recv(spi_dev_lcd, pdata, length);
	LCD_CS_SET;
    return ((result == length)?0:-1);
}

