#include "Com_PID.h"



/**
 *@brief 角度环PD控制
 *
 * @param kp p系数
 * @param kd d系数
 * @param kalman_angle 卡尔曼滤波后的角度
 * @param balace_angle  期望平衡时的角度
 * @param gy y轴角速度
 * @return int PID后的PWM输出结果
 */
int Com_PID_Angle(float kp, float kd, float kalman_angle, float balace_angle, int16_t gy)
{
    /* 计算角度偏差值 */
    float bias_angle = kalman_angle - balace_angle;
    /* 进行PID计算 */
    return kp * bias_angle + kd * gy;
}

/**
 * @brief 速度环PI控制
 *
 * @param kp p系数
 * @param ki i系数
 * @param encoder_a 编码器A值
 * @param encoder_b 编码器B值
 * @param move_distance 蓝牙遥控控制移动的值
 * @return int
 */
int Com_PID_Velocity(float kp, float ki, int16_t encoder_a, int16_t encoder_b, int16_t move_distance)
{
    static int integral_val = 0;    //速度误差积分值
    static int last_bias_val = 0;    //上一次速度偏差值

    /* 计算速度偏差 */
    int bias_velocity = encoder_a + encoder_b - 0;

    /* 对速度偏差做一个一阶低通滤波，避免其发生突变；减轻速度环的影响，只是辅助作用 */
    /* 最终本次的值=k*上次的值 +（1-k）*本次的值 */
    bias_velocity = 0.8f * last_bias_val + 0.2f * bias_velocity;
    /* 保存本次的偏差值，下一次计算用 */
    last_bias_val = bias_velocity;
    /* 计算积分 */
    integral_val += bias_velocity;
    /* +-所需要的移动值 */

    integral_val -= move_distance;
    /* 积分限幅 */
    if (integral_val > 10000)
        integral_val = 10000;
    else if (integral_val < -10000)
        integral_val = -10000;
    /* 进行PID计算 */
    return kp * bias_velocity + ki * integral_val;

}

/**
 *@brief 转向环P控制
 *
 * @param kp p系数
 * @param gz z轴角速度
 * @return int
 */
int Com_PID_Turn(float kp, short gz)
{
    return kp * (gz - 0);
}
