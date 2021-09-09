/**
  ******************************************************************************
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  * Copyright (C) 2021 Shanghai Eastsoft Microelectronics Co., Ltd. ALL rights reserved.
  *******************************************************************************
  */

<--------------------------------------------------------------------------------
Version history:
--------------------------------------------------------------------------------->
2021-3-16	V1.0.0	AE TEAM

<--------------------------------------------------------------------------------
Demo introduction:
--------------------------------------------------------------------------------->
本示例功能：INPUT端口产生上升沿，触发外部中断，在中断服务程序中翻转OUTPUT端口电平。

<--------------------------------------------------------------------------------
测试环境:
--------------------------------------------------------------------------------->
测试用板；
ES-DEV-ES32W3120

辅助工具：
多功能接口转换工具ESBridge

辅助软件：
ESBridge上位机软件

<--------------------------------------------------------------------------------
测试步骤：
--------------------------------------------------------------------------------->
1) 编译工程，编译通过后将程序下载到目标芯片；
2) 将程序配置的OUTPUT对应的引脚连接ESBridge的逻辑笔通道，INPUT端口连接PWM通道；
3) ESBridge通过USB线缆连接PC，打开上位机软件，“设备操作”选项里选择“打开”；
4) 上位机切换到PWM签页，配置通道波形后选择“开始”；上位机切换到逻辑笔标签页，勾选相应通道选择“开始”；
5) 在线调试，运行程序；
6) 输入到INPUT端口的波形产生上升沿的时候OUTPUT端口电平会相应翻转。

<--------------------------------------------------------------------------------
注意：
--------------------------------------------------------------------------------->
1) 测试板与ESBridge需共GND。

<--------------------------------------------------------------------------------
System clock:
--------------------------------------------------------------------------------->
48MHz.

<--------------------------------------------------------------------------------
Pin connection: 
--------------------------------------------------------------------------------->
INPUT  ---- PA6[GPIO_FUNC_1]
OUTPUT  ---- PA7[GPIO_FUNC_1]
