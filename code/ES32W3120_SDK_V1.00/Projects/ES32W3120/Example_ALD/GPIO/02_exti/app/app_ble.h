#ifndef __APP_BLE_H
#define __APP_BLE_H

#include "global.h"

#define SYSTEM_CMD                  0xc1  //ϵͳָ��
#define CONTROL_CMD                 0xc2  //����ָ��
#define SET_DATA_CMD                0xc3  //��������ָ��
#define READ_STATE_CMD              0xc4  //��ȡ״ָ̬��
#define READ_DATA_CMD               0xc5  //��ȡ����ָ��
#define WRITE_DATA_CMD              0xd5  //д����ָ��
#define WXID_CMD                    0xd9  //WXIDָ��

#define SYSTEM_HEART_BEATS          0x01  //����������
#define SYSTEM_ACK                  0x02  //ȷ���յ�����
#define SYSTEM_ERROR                0x03  //����
#define SYSTEM_RESEND               0x04  //�ط�

#define ERROR_IMU                   0x01  //IMU����������
#define ERROR_FLASH                 0x02  //FLASH�ڴ����
#define ERROR_BAT                   0x03  //����¶ȹ���
#define ERROR_COMM                  0x04  //ͨ�Ź���

#define CONTROL_CALIBRATE           0x01  //��̬У׼
#define CONTROL_SCAN                0x02  //����ɨ��
#define CONTROL_SEND_SCAN_DATA      0x03  //�ϴ�ɨ������
#define CONTROL_SEND_MONITOR_DATA   0x04  //�ϴ�ʵʱ�������
#define CONTROL_SEND_FLASH_DATA     0x05  //�ϴ��洢����
#define CONTROL_NO_DATA             0x06  //û����������

#define SEND_FLASH_DATA_START       0x01  //��λ��֪ͨ��λ����ʼ�ϴ�����
#define SEND_FLASH_DATA_FINISH      0x02  //��λ��֪ͨ��λ�������Ѿ��������
#define SEND_FLASH_DATA_DELETE      0x03  //��λ��֪ͨ��λ������ɾ������

#define SET_FREQUENCY               0x01  //������Ƶ��

#define STATE_INFO                  0x01  //��ǰ�������ڴ桢��ǰ��������״̬���豸���к�
#define STATE_SCAN                  0x02  //��λ����ǰ�Ƿ���ɨ�����

#define DATA_MONITOR_DATA           0x01  //����Ʒ����������
#define DATA_UTC                    0x02  //����ʱ��
#define DATA_ONLINE_IMU_DATA        0x01  //ʵʱIMU���������ݼ�ʱ���
#define DATA_OFFLINE_IMU_DATA       0x03  //����IMU���������ݼ�ʱ���
#define DATA_SCAN_DATA              0x04  //�ϴ�ɨ�����ݼ�ʱ���

#define WXID_WRITE                  0x01  //��λ���·�wxid

#define BIND_FLG                    0X01

typedef struct {
    uint8_t tbd0;
    uint8_t tbd1;
    uint8_t tbd2;
    uint8_t tbd3;
    uint8_t tbd4;
    uint8_t tbd5;
    uint8_t utc_y;
    uint8_t utc_m;
    uint8_t utc_d;
    uint8_t utc_h;
    uint8_t utc_f;
    uint8_t utc_s;
    uint8_t utc_5ms;
    uint8_t tbd6;
    uint8_t tbd7;
    
} data_utc_t;

typedef struct {
    uint8_t wxid_0;
    uint8_t wxid_1;
    uint8_t wxid_2;
    uint8_t wxid_3;
    uint8_t bind_flg;
    
} data_wxid_t;

typedef struct {
    uint8_t start;
    uint8_t len;
    uint8_t cmd;
    uint8_t address;
    uint8_t data[15];
    uint8_t sum;
    
} ble_data_t;

int ble_data_decode(void);

#endif


