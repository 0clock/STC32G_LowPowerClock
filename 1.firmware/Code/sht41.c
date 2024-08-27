#include "sht41.h"
#include "iic.h"

#include "stdio.h"

static sht41_state_t sht41_state = SHT41_IDLE;
// CRC 校验多项式
#define SHT41_CRC_POLYNOMIAL 0x31
void sht41_init(void)
{
    // i2c_init(); // 初始化 I2C 接口
    sht41_state = SHT41_IDLE;
    iic_write1bit(SHT41_W, 0x94);
}
// CRC 校验函数
static unsigned char sht41_crc8(const unsigned char *data_buff, int length)
{
    unsigned char crc = 0xFF;
    int i, j;
    for (i = 0; i < length; i++)
    {
        crc ^= data_buff[i];
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ SHT41_CRC_POLYNOMIAL;
            }
            else
            {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}
unsigned int sht41_read_16bit(bit k)
{
    u16 dat = 0;
    dat += RecvData(); // 读取数据2
    SendACK();
    dat = dat << 8;
    dat += RecvData(); // 读取数据2
    SendACK();
    RecvData(); // 读取数据2
    if (!k)
        SendACK();
    if (k)
        SendNAK();
    return dat;
}

// 启动测量，不等待数据
void sht41_start_measurement(void)
{
    unsigned char cmd = SHT41_MEASURE_HIGHREP;

#if 0
    iic_start();
    iic_senddata(SHT41_I2C_ADDRESS << 1);
    iic_senddata(cmd);
    iic_stop();
#endif

    iic_write1bit(SHT41_W,0xFD);
    sht41_state = SHT41_MEASURING;
}
unsigned char i2c_read(bit isSendACK)
{
    unsigned char data_buff = RecvData();
    if (isSendACK)
    {
        SendACK();
    }
    else
    {
        SendNAK();
    }

    return data_buff;
}
// 非阻塞读取数据
int sht41_read_data(sht41_data_t *data_buff)
{
    unsigned char buffer[6];
    unsigned int raw_temperature;
    unsigned int raw_humidity;
    int i;
    if (sht41_state != SHT41_MEASURING)
    {
        printf("sht41:当前没有进行测量\n");
        return -1; // 当前没有进行测量
    }
    printf("sht41:测量中\n");
    iic_start();
    iic_senddata((SHT41_I2C_ADDRESS << 1) | 0x01);

#if 0
    if (iic_senddata((SHT41_I2C_ADDRESS << 1) | 0x01) != 1) //不知道对不对是不是1 
    {
        iic_stop();
        printf("sht41: 读取地址失败\n");
        return -1; // 读取地址失败
    }
#endif
#if 0
    for (i = 0; i < 6; i++)
    {
        buffer[i] = i2c_read(i < 5); // 最后一个字节不需要发送ACK
    }
    iic_stop();

// 校验 CRC

    if (sht41_crc8(buffer, 2) != buffer[2] || sht41_crc8(buffer + 3, 2) != buffer[5] )
    {
        printf("sht41:CRC 校验失败\n");
        return -1; // CRC 校验失败
    }

    // 转换温度和湿度数据
    raw_temperature = (buffer[0] << 8) | buffer[1];
    raw_humidity = (buffer[3] << 8) | buffer[4];

#else
    RecvACK();
    raw_temperature = sht41_read_16bit(0);
    raw_humidity = sht41_read_16bit(1);
    iic_stop();
#endif
    data_buff->temperature = -45.0f + 175.0f * ((float)raw_temperature / 65535.0f);
    data_buff->humidity = 100.0f * ((float)raw_humidity / 65535.0f);

    data_buff->state = SHT41_DATA_READY;
    sht41_state = SHT41_IDLE;

    printf("read sht41 finished: %f℃,%f%\n", data_buff->temperature, data_buff->humidity);
    printf("raw data: %d℃,%d%\n", raw_temperature, raw_humidity);

    return 0; // 成功
}

#if 0

#include "stc32g.h"
#include "sht41.h"
#include "iic.h"

void sht41_init()
{
    iic_write1bit(SHT41_W,0x94);
}


unsigned int sht41_read_16bit(bit k)
{
    u16 dat = 0;
    dat += RecvData();                            //读取数据2
    SendACK();
    dat = dat <<8;
    dat += RecvData();                            //读取数据2
    SendACK();
    RecvData();                            //读取数据2
    if(!k) SendACK();
    if(k) SendNAK();
    return dat;
}

void sht41_r_hignt()
{
    iic_write1bit(SHT41_W,0xFD);
}

void sht41_update()
{
    u32 a = 0;
    u32 b = 0;
    if(Sleep_Queue_task[0] & 0x0004)
    {
        Start(); //发送起始命令
        SendData(SHT41_R);                             //发送设备地址+写命令
        RecvACK();
        a = SHT41_R_16bit(0);
        b = SHT41_R_16bit(1);
        Stop();
        a = a*17500/65535-4500;
        b = b*12500/65535-600;
        JCQ06_00[0] = a;
        JCQ06_00[1] = b;
        Sleep_Queue_task[0] &=~0x0004;
    }
}
#endif
#if 0
#include "IIC.h"
#include "SHT41.h"
#include "time.h"
#include "DATA.h"
#include "OLED.h"
#include "UART1.h"

extern u8 xdata DMABuffer[32];
extern u32 xdata JCQ06_00[10];


extern u16 Sleep_Queue_task[Sleep_queue_num];

u8 uuy = 0;
//========================================================================
// 函数: void SHT41_Init()
// 描述: 初始化传感器
// 参数: 
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void SHT41_Init()
{
    IIC_W1bit(SHT41_W,0x94);
}
//========================================================================
// 函数: u16 SHT41_R_16bit(bit k)
// 描述: 读16位数据并返回
// 参数:  k：是否ACK
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
u16 SHT41_R_16bit(bit k)
{
    u16 dat = 0;
    dat += RecvData();                            //读取数据2
    SendACK();
    dat = dat <<8;
    dat += RecvData();                            //读取数据2
    SendACK();
    RecvData();                            //读取数据2
    if(!k) SendACK();
    if(k) SendNAK();
    return dat;
}
//========================================================================
// 函数: void SHT41_R_High()
// 描述: 传感器测量
// 参数:  
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void SHT41_R_High()
{
    IIC_W1bit(SHT41_W,0xFD);
    SleepQueue_aheadW1(28,4,0);
}

//========================================================================
// 函数: void SHT41_falg_0004()
// 描述: 读回温度湿度的值，并计算
// 参数:  
// 返回:
// 版本: V1.0 2023.02.05
//========================================================================
void SHT41_falg_0004()
{
    u32 a = 0;
    u32 b = 0;
    if(Sleep_Queue_task[0] & 0x0004)
    {
        Start(); //发送起始命令
        SendData(SHT41_R);                             //发送设备地址+写命令
        RecvACK();
        a = SHT41_R_16bit(0);
        b = SHT41_R_16bit(1);
        Stop();
        a = a*17500/65535-4500;
        b = b*12500/65535-600;
        JCQ06_00[0] = a;
        JCQ06_00[1] = b;
        Sleep_Queue_task[0] &=~0x0004;
    }
}

#endif
