#include "Int_MPU6050.h"
#include <stdio.h>
/**
 * @brief 读取MPU6050指定寄存器字节的数据
 *
 * @param reg_addr 读取寄存器的地址
 * @param size 要读取的字节数
 * @param rev_byte 接收数据的指针
 */
void Int_MPU6050_ReadByte(uint8_t reg_addr, uint8_t size, uint8_t* rev_byte)
{
    HAL_I2C_Mem_Read(&hi2c2, (MPU_IIC_ADDR << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, rev_byte, size, 1000);
}

/**
 * @brief 向MPU6050指定寄存器写入数据
 *
 * @param reg_addr 要写入的寄存器的地址
 * @param size 要写入的字节数
 * @param send_byte 写入数据的指针
 */
void Int_MPU6050_WriteByte(uint8_t reg_addr, uint8_t size, uint8_t* send_byte)
{
    HAL_I2C_Mem_Write(&hi2c2, (MPU_IIC_ADDR << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, send_byte, size, 1000);
}

//数字低通滤波器DLPF_CFG值和陀螺仪带宽对应的关系
uint8_t bandwidth_arr[6] = { 188,98,42,20,10,15 };
/**
 *@brief 根据采样率设置低通滤波器
 *
 * @param rate  采样率
 */
void Int_MPU6050_SetDLPF_CFG(uint16_t rate)
{
    rate /= 2;
    for (uint8_t i = 0;i < 6;++i)
    {
        if (rate > bandwidth_arr[i])
        {
            uint8_t tmp_index = i + 1;
            Int_MPU6050_WriteByte(MPU_CFG_REG, 1, &tmp_index);
        }
    }
}


/**
 *@brief 设置陀螺仪的采样率
 *
 * @param rate
 */
void Int_MPU6050_SetGyroRate(uint16_t rate)
{
    /* 采样率限幅 */
    if (rate < 4)   rate = 4;
    else if (rate > 1000) rate = 1000;

    /* 采样率=输出频率/(1+分频值) */
    uint8_t sample_div = 1000 / rate - 1;
    Int_MPU6050_WriteByte(MPU_SAMPLE_RATE_REG, 1, &sample_div);

    /* 根据采样率去设置低通滤波器 */
    Int_MPU6050_SetDLPF_CFG(rate);
}

void Int_MPU6050_Init()
{
    uint8_t dev_id = 0;
    /* 初始化I2C */
    HAL_I2C_Init(&hi2c2);

    /* 复位 -> 延迟一会 -> 唤醒 */
    uint8_t tmpByte = 0x80;
    Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 1, &tmpByte);
    HAL_Delay(150);
    tmpByte = 0x00;
    Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 1, &tmpByte);

    /* 陀螺仪量程 +-2000°/s fs_sel=3 */
    tmpByte = 3 << 3;
    Int_MPU6050_WriteByte(MPU_GYRO_CFG_REG, 1, &tmpByte);
    /* 加速度量程 */
    tmpByte = 0 << 3;
    Int_MPU6050_WriteByte(MPU_ACCEL_CFG_REG, 1, &tmpByte);

    /* 其他功能设置：FIFO、第二I2C、中断 */
    tmpByte = 0x00;
    Int_MPU6050_WriteByte(MPU_INT_EN_REG, 1, &tmpByte);     //关闭中断
    Int_MPU6050_WriteByte(MPU_USER_CTRL_REG, 1, &tmpByte);  //关闭第二I2C
    Int_MPU6050_WriteByte(MPU_FIFO_EN_REG, 1, &tmpByte);    //关闭FIFO

    /* 系统时钟源、陀螺仪采样率、低通滤波的设置 */
    /* 先确认有无正产工作 */
    Int_MPU6050_ReadByte(MPU_DEVICE_ID_REG, 1, &dev_id);
    if (dev_id == MPU_IIC_ADDR)
    {
        /* 设置时钟源：x轴上的时钟 */
        tmpByte = 1 << 0;
        Int_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 1, &tmpByte);

        /* 设置陀螺仪采样率&低通滤波 */
        Int_MPU6050_SetGyroRate(100);

        /* 让ACCEL和GYRO退出待机模式 */
        tmpByte = 0x00;
        Int_MPU6050_WriteByte(MPU_PWR_MGMT2_REG, 1, &tmpByte);

    }
}


void Int_MPU6050_GetGyro(int16_t* gx, int16_t* gy, int16_t* gz)
{
    uint8_t GyroBuff[6];

    /* 直接连续读取六个字节 */
    Int_MPU6050_ReadByte(MPU_GYRO_XOUTH_REG, 6, GyroBuff);

    /* 拼接高低字节 */
    *gx = (int16_t)((GyroBuff[0] << 8) | (GyroBuff[1]));
    *gy = (int16_t)((GyroBuff[2] << 8) | (GyroBuff[3]));
    *gz = (int16_t)((GyroBuff[4] << 8) | (GyroBuff[5]));
}


void Int_MPU6050_GetAccel(int16_t* ax, int16_t* ay, int16_t* az)
{
    uint8_t AccelBuff[6];

    /* 直接连续读取六个字节 */
    Int_MPU6050_ReadByte(MPU_ACCEL_XOUTH_REG, 6, AccelBuff);

    /* 拼接高低字节 */
    *ax = (int16_t)((AccelBuff[0] << 8) | (AccelBuff[1]));
    *ay = (int16_t)((AccelBuff[2] << 8) | (AccelBuff[3]));
    *az = (int16_t)((AccelBuff[4] << 8) | (AccelBuff[5]));
}
