#ifndef __TASK_COMMON_H
#define __TASK_COMMON_H

#include "global.h"

#include "task_safeguard.h"
#include "task_control.h"
#include "task_measure.h"
#include "task_mem_read.h"
#include "task_mem_write.h"
#include "task_bluetooth.h"
#include "task_communicate.h"
#include "task_other.h"

#define TASK0                         sg_task
#define TASK1                         contrl_task
#define TASK2                         measure_task
#define TASK3                         mem_read_task
#define TASK4                         comm_task
#define TASK5                         bluetooth_task
#define TASK6                         mem_write_task
#define TASK7                         other_task

#define SG                            0                             //系统主任务0 
#define SHUTDOWN_MODE                 0                             //关机模式
#define LOW_POWER_MODE                1                             //低功耗模式
#define ADV_MODE                      2                             //广播模式
#define CONNECT_MODE                  3                             //连接模式

#define CONTROL                       1                             //控制任务1
#define MOTOR_START                   0                             //马达开始震动
#define MOTOR_STOP                    1                             //马达停止
#define BLE_DISCON                    2                             //断开蓝牙连接

#define MEASURE                       2                             //测量任务2
#define IMU_DATA                      0                             //读取6050数据
#define BATT_VOL                      1                             //测量电池电压

#define MEM_READ                      3                             //flash读取任务3
#define FLASH_READ                    0                             //读取flash中的数据
#define FLASH_DATA_SEND               1                             //上传flash中的数据至上位机

#define COMM                          4                             //NB通讯任务4

#define BLUETOOTH                     5                             //蓝牙任务5
#define DATA_DECODE                   0                             //蓝牙数据解析
#define RET_ACK                       1                             //蓝牙数据回执
#define WXID_REQ                      2                             //向小程序请求wxid

#define MEM_WRITE                     6                             //flash存储任务6
#define WRITE_SYSTEM_INFO             0                             //保存系统信息至内部 flash
#define FLASH_DELETE                  1                             //删除外部 flash 中的数据
#define OTHER                         7                             //其他任务7


typedef struct
{
    uint8_t (*function)(uint8_t m_Event);                           //带参数的函数指针 用于执行任务函数
}Task_Type;

extern uint8_t                            g_Maintask;               //系统主任务
extern uint8_t                            ga_Subtask[8];            //系统子任务
extern Task_Type                          Task_Struct[8];
extern const uint8_t                      ga_TaskMapTable[256];

#endif

