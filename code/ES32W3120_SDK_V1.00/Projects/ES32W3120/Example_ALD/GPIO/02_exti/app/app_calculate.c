#include <math.h>

#include "bsp_motor.h"

#include "app_calculate.h"

/* Private Macros ------------------------------------------------------------ */

/* Private Variables --------------------------------------------------------- */

/* Public Variables ---------------------------------------------------------- */

/* Private Constants --------------------------------------------------------- */

/* Private function prototypes ----------------------------------------------- */

/* Private Function ---------------------------------------------------------- */

/* Exported Variables -------------------------------------------------------- */

void calculate_accelerometer(uint16_t ax, uint16_t ay, uint16_t az)
{
    uint16_t data[3] = {0};
    
    /* 角度算法 */
    data[1] = -atan(ay / ax) * 180 / 3.14;
    data[0] = atan(az / sqrtf(ax * ax + ay * ay)) * 180 / 3.14;
    data[2] = acos(ax / sqrtf(ax * ax + ay * ay + az * az)) * 180 / 3.14;
    
    if(65 >= data[2]){
        if((20 <= data[0]) || (20 <= data[1])){
            motor_start();  //超过阈值，震动提醒
        }
        else{
            motor_stop();   //关闭提醒
        }
    }
    else{
        
    }
}
