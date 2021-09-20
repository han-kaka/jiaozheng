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

#define SG                            0                             //ϵͳ������0 
#define SHUTDOWN_MODE                 0                             //�ػ�ģʽ
#define LOW_POWER_MODE                1                             //�͹���ģʽ
#define ADV_MODE                      2                             //�㲥ģʽ
#define CONNECT_MODE                  3                             //����ģʽ

#define CONTROL                       1                             //��������1
#define MOTOR_START                   0                             //��￪ʼ��
#define MOTOR_STOP                    1                             //���ֹͣ
#define BLE_DISCON                    2                             //�Ͽ���������

#define MEASURE                       2                             //��������2
#define IMU_DATA                      0                             //��ȡ6050����
#define BATT_VOL                      1                             //������ص�ѹ

#define MEM_READ                      3                             //flash��ȡ����3
#define FLASH_READ                    0                             //��ȡflash�е�����
#define FLASH_DATA_SEND               1                             //�ϴ�flash�е���������λ��

#define COMM                          4                             //NBͨѶ����4

#define BLUETOOTH                     5                             //��������5
#define DATA_DECODE                   0                             //�������ݽ���
#define RET_ACK                       1                             //�������ݻ�ִ
#define WXID_REQ                      2                             //��С��������wxid

#define MEM_WRITE                     6                             //flash�洢����6
#define WRITE_SYSTEM_INFO             0                             //����ϵͳ��Ϣ���ڲ� flash
#define FLASH_DELETE                  1                             //ɾ���ⲿ flash �е�����
#define OTHER                         7                             //��������7


typedef struct
{
    uint8_t (*function)(uint8_t m_Event);                           //�������ĺ���ָ�� ����ִ��������
}Task_Type;

extern uint8_t                            g_Maintask;               //ϵͳ������
extern uint8_t                            ga_Subtask[8];            //ϵͳ������
extern Task_Type                          Task_Struct[8];
extern const uint8_t                      ga_TaskMapTable[256];

#endif

