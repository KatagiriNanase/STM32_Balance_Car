#include "App_Car.h"
#include "Com_PID.h"
#include "Int_TB6612.h"

int16_t gx, gy, gz;
int16_t ax, ay, az;

float accel_angle;  //通过加速度计算出来的倾角
float gyro_y;       //采样值转换后的y轴角速度

int16_t  encoder_a;  //编码器A值
int16_t  encoder_b;  //编码器B值

extern float angle; //卡尔曼滤波后的角度

char batBuff[5];   //电池电压字符串
char encoderBUff[7]; //编码器值字符串
char angleBuff[7];  //倾角字符串

/*->>>>>>>>>> 角度环PID参数 >>>>>>>>>-*/
float angle_kp = -300.0;  //-300
float angle_kd = 0.9f;     //0.9
float balance_angle = 0.6f; //0.6

/*->>>>>>>>>> 速度环PID参数 >>>>>>>>>-*/
float velocity_kp = 70.0f;      //70
float velocity_ki = 0.35f;      //0.35
int16_t move_distance = 0;      //遥控前后的距离
int16_t turn_angle = 0;         //遥控转向的角度

/*->>>>>>>>>> 转向环PID参数 >>>>>>>>>-*/
float turn_kp = 1.0f;

/*->>>>>>>>>> 蓝牙遥控参数 >>>>>>>>>-*/
_Bool flag_up, flag_down, flag_left, flag_right;

void App_Car_GetAngle(void)
{
    Int_MPU6050_GetAccel(&ax, &ay, &az);
    Int_MPU6050_GetGyro(&gx, &gy, &gz);
    /* 通过加速度计算倾角 */
    accel_angle = atan2(ax, az) * 180 / PI;
    /* 角速度 */
    gyro_y = -gy / 16.4;
    /* 卡尔曼滤波 */
    Com_Filter_Kalman(accel_angle, gyro_y);

    /* 获取编码器A，B的值 */
    encoder_a = Int_Encoder_ReadCounter(MOTOR_A);
    encoder_b = Int_Encoder_ReadCounter(MOTOR_B);

    // printf("accel_angle=%.1f\r\n", accel_angle);
    // printf("gyro_y=%.1f\r\n", gyro_y);
    // printf("angle=%.1f\r\n", angle);
}

void App_Car_Display(void)
{
    /* 显示电压值 */
    double bat_v = Dri_Bat_Det();
    sprintf(batBuff, "%3.1f", bat_v);
    OLED_ShowString(1, 5, batBuff);
    /* 显示编码器值 */
    sprintf(encoderBUff, "%6d", encoder_a);
    OLED_ShowString(2, 4, encoderBUff);
    sprintf(encoderBUff, "%6d", encoder_b);
    OLED_ShowString(3, 4, encoderBUff);
    /* 显示倾角*/
    sprintf(angleBuff, "%5.1f", angle);
    OLED_ShowString(4, 7, angleBuff);


}

void App_Car_PID(void)
{
    int angle_out = 0;
    int velocity_out = 0;
    int turn_out = 0;
    int pwma, pwmb;
    /* 角度环控制 */
    angle_out = Com_PID_Angle(angle_kp, angle_kd, angle, balance_angle, gy);
    /* 速度环控制 */
    /* 蓝牙遥控逻辑 */
    if (flag_up)
        move_distance = 20;
    else if (flag_down)
        move_distance = -20;
    else
        move_distance = 0;

    velocity_out = Com_PID_Velocity(velocity_kp, velocity_ki, encoder_a, encoder_b, move_distance);
    /* 转向环控制 */
    if (flag_left)
    {
        turn_out = 0;
        turn_angle += 20;
    }
    else if (flag_right)
    {
        turn_out = 0;
        turn_angle -= 20;
    }
    else
    {
        turn_angle = 0;
        turn_out = Com_PID_Turn(turn_kp, gz);
    }
    /* 转向限幅 */
    if (turn_angle > 50)
        turn_angle = 50;
    else if (turn_angle < -50)
        turn_angle = -50;
    /* 叠加PID结果 */
    pwma = angle_out + velocity_out + turn_out - turn_angle;
    pwmb = angle_out + velocity_out - turn_out + turn_angle;

    /* 设置pwm占空比(电机速度) */
    Int_TB6612_SetPWM(pwma, pwmb);

}
