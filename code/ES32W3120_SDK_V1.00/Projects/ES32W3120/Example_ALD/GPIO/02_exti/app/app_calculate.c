#include <math.h>

#include "bsp_motor.h"
#include "bsp_system.h"
#include "bsp_flash.h"
#include "bsp_time.h"

#include "app_calculate.h"
#include "app_common.h"

#include "task_common.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */
//uint8_t *calibrate_data_p = NULL;
uint8_t calibrate_data_p[15000] = {0};
uint16_t calibrate_packet_cnt = 0;
short last_ax = 0;
short last_ay = 0;
short last_az = 0;
uint32_t lpw_cnt = 0;

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */
extern system_state_t system_state;
extern utc_time_t utc_time;

void calculate_accelerometer(short ax, short ay, short az)
{
    uint16_t data[3] = {0};
    uint8_t save_data_temp[20];
    uint8_t sum = 0;
    uint8_t i = 0;
    
    if((0==last_ax) && (0==last_ay) && (0==last_az)){
        last_ax = ax;
        last_ay = ay;
        last_az = az;
    }       
    else{
        if((1000>(last_ax>=ax?last_ax-ax:ax-last_ax)) && (1000>(last_ay>=ay?last_ay-ay:ay-last_ay))&& (1000>(last_az>=az?last_az-az:az-last_az))){
            lpw_cnt++;
            ES_LOG_PRINT("lpw_cnt: %u\n", lpw_cnt);
            if(120<lpw_cnt){
                set_task(SG, LOW_POWER_MODE);
            }
        }
        else{
            lpw_cnt = 0;
            last_ax = ax;
            last_ay = ay;
            last_az = az;
        }
    }
    
    if(1 == system_state.system_flg.calibrate_mode_flg){
        if(1 == system_state.system_flg.calibrate_key_flg){
            
            memset(save_data_temp, 0, 20);
            save_data_temp[0] = 0xaa;
            save_data_temp[1] = 0x13;
            save_data_temp[2] = 0xd5;
            save_data_temp[3] = 0x04;
            save_data_temp[4] = ax >> 8;
            save_data_temp[5] = ax & 0xff;
            save_data_temp[6] = ay >> 8;
            save_data_temp[7] = ay & 0xff;
            save_data_temp[8] = az >> 8;
            save_data_temp[9] = az & 0xff;
            save_data_temp[10] = utc_time.utc_y;
            save_data_temp[11] = utc_time.utc_m;
            save_data_temp[12] = utc_time.utc_d;
            save_data_temp[13] = utc_time.utc_h;
            save_data_temp[14] = utc_time.utc_f;
            save_data_temp[15] = utc_time.utc_s;
            
            sum = 0;
            for(i=0; i<19; i++){
                sum += save_data_temp[i];
            }
            save_data_temp[19] = sum;
            
//            if(NULL != calibrate_data_p){
                memcpy(calibrate_data_p+calibrate_packet_cnt*20, save_data_temp, 20);
                calibrate_packet_cnt++;
//            }
        }
    }
    else{
        /* ???????? */
        data[1] = -atan(ay / ax) * 180 / 3.14;
        data[0] = atan(az / sqrtf(ax * ax + ay * ay)) * 180 / 3.14;
        data[2] = acos(ax / sqrtf(ax * ax + ay * ay + az * az)) * 180 / 3.14;
        
        if(65 >= data[2]){
            if((25 <= data[0]) || (10 <= data[1])){
                motor_start();  //??????????????????
            }
            else{
                if(1 == system_state.system_flg.motor_start_flg){
                    motor_stop();   //????????
                }
            }
        }
        
        save_accelerometer(ax, ay, az);
    }
}
