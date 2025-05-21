#include "App_Task.h"

/* 启动任务配置 */
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
TaskHandle_t start_task_handle;
void App_Task_Start(void* pvParameters);

/* 姿态解算任务配置 */
#define POSE_TASK_STACK 128
#define POSE_TASK_PRIORITY 4
TaskHandle_t pose_task_handle;
void App_Task_Pose(void* pvParameters);

/* PID控制任务配置 */
#define PID_TASK_STACK 128
#define PID_TASK_PRIORITY 4
TaskHandle_t pid_task_handle;
void App_Task_PID(void* pvParameters);

/* OLED显示任务配置 */
#define OLED_TASK_STACK 128
#define OLED_TASK_PRIORITY 4
TaskHandle_t oled_task_handle;
void App_Task_OLED(void* pvParameters);

void App_Task_Init(void)
{
    /* 创建启动任务 */
    xTaskCreate((TaskFunction_t)App_Task_Start,
        "App_Task_Start",
        START_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        (TaskHandle_t*)&start_task_handle
    );

    /* 启动调度器 */
    vTaskStartScheduler();
}

/**
 *@brief FreeRTOS启动任务，用于创建其他任务
 *
 * @param pvParameters 无参数
 */
void App_Task_Start(void* pvParameters)
{
    taskENTER_CRITICAL();

    xTaskCreate((TaskFunction_t)App_Task_Pose,
        "App_Task_Pose",
        POSE_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)POSE_TASK_PRIORITY,
        (TaskHandle_t*)&pose_task_handle
    );

    xTaskCreate((TaskFunction_t)App_Task_PID,
        "App_Task_PID",
        PID_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)PID_TASK_PRIORITY,
        (TaskHandle_t*)&pid_task_handle
    );

    xTaskCreate((TaskFunction_t)App_Task_OLED,
        "App_Task_OLED",
        OLED_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)OLED_TASK_PRIORITY,
        (TaskHandle_t*)&oled_task_handle
    );

    vTaskDelete(NULL);

    taskEXIT_CRITICAL();
}


/**
 * @brief 解算姿态
 *
 * @param pvParameters
 */
void App_Task_Pose(void* pvParameters)
{
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
    while (1)
    {
        App_Car_GetAngle();
        /* 通知PID进行控制 */
        xTaskNotifyGive(pid_task_handle);
        vTaskDelayUntil(&pxPreviousWakeTime, 10);
    }

}

/**
 *@brief PID控制任务
 *
 * @param pvParameters
 */
void App_Task_PID(void* pvParameters)
{
    while (1)
    {
        /* 等待姿态数据解算 */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        App_Car_PID();
    }

}

/**
 *@brief OLED显示任务
 *
 * @param pvParameters
 */
void App_Task_OLED(void* pvParameters)
{
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
    while (1)
    {
        App_Car_Display();
        xTaskDelayUntil(&pxPreviousWakeTime, 50);
    }
}
