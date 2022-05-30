// LAST TESTED : 09/08/2019
#ifndef __SYSTEM_CONFIG_DEFINES_H__
#define __SYSTEM_CONFIG_DEFINES_H__
#include "xparameters.h"
#define ONESECOND    0x13D92D3F
#define ONE_SECOND 3250000000
#define READ_WRITE_MUL_FACTOR 0x10
#define DEFAULT_HYPERTERMINAL_UART_ID 	XPAR_PS7_UART_1_DEVICE_ID
#define UART_BASEADDR        			XPAR_PSU_UART_1_BASEADDR
#define D5M_BASE        			XPAR_PS_VIDEO_RX_VIDEO_VFP_0_VFPCONFIG_BASEADDR

/*****************************************************************************************************************/
#define D5M_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define D5M_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
/*****************************************************************************************************************/


/****************************************************************
KERNEL
*****************************************************************/
#define w_kernal_1_reg_08           32
#define w_kernal_2_reg_09           36
#define w_kernal_3_reg_10           40
#define w_kernal_4_reg_11           44
#define w_kernal_5_reg_12           48
#define w_kernal_6_reg_13           52
#define w_kernal_7_reg_14           56
#define w_kernal_8_reg_15           60
#define w_kernal_9_reg_16           64
#define w_kernalconfig_reg_17       68
#define w_kSet_reg_17               68
/****************************************************************
UNSED KERNEL
*****************************************************************/
#define w_a11fl_reg_21              84
#define w_a12fl_reg_22              88
#define w_a13fl_reg_23              92
#define w_a21fl_reg_24              96
#define w_a22fl_reg_25              100
#define w_a23fl_reg_26              104
#define w_a31fl_reg_27              108
#define w_a32fl_reg_28              112
#define w_a33fl_reg_29              116
#define w_aconfig_reg_30            120
#define w_pointinterest_reg_31      124
#define r_pointinterest_reg_31      124
#define w_deltaconfig_reg_32        128
#define w_cpuackgoagain_reg_33      132
#define w_cpuwgridlock_reg_34       136
#define w_cpuackoffframe_reg_35     140
#define w_gridlockaddress_reg_36    144
#define r_readaddress_reg_36        144
#define w_clearfifodata_reg_37      148
#define r_gridlockdatai_reg_38      152
#define r_fifofullh_reg_39          156
#define r_fifoemptyh_reg_40         160
#define r_fifofullh_reg_41          164
#define r_cpugridcont_reg_42        168
#define w_empty_reg_43              172
#define w_empty_reg_44              176
#define w_empty_reg_45              180
#define w_empty_reg_46              184
#define w_empty_reg_47              188
#define w_empty_reg_48              192
#define w_empty_reg_49              196
#define w_rh_reg_50                 200
#define w_rl_reg_51                 204
#define w_gh_reg_52                 208
#define w_gl_reg_53                 212
#define w_bh_reg_54                 216
#define w_bl_reg_55                 220
#define w_lumTh_reg_56              224
#define w_hsvPerCh_reg_57           228
#define w_yccPerCh_reg_58           232
#define w_empty_reg_59              236
#define r_sec_reg_60                240
#define r_min_reg_61                244
#define r_hour_reg_62               248
#define r_revisionnumber_reg_63     252
/*****************************************************************************************************************/
#define SCREEN_WIDTH_HORIZONTAL     1920
#define SCREEN_HEIGHT_VERTICAL      1080
#define VIDEO_RESOLUTION_VGA        0
#define VIDEO_RESOLUTION_480P       1
#define VIDEO_RESOLUTION_576P       2
#define VIDEO_RESOLUTION_SVGA       3
#define VIDEO_RESOLUTION_XGA        4
#define VIDEO_RESOLUTION_720P       5
#define VIDEO_RESOLUTION_SXGA       6
#define VIDEO_RESOLUTION_1080P      7
#define VIDEO_RESOLUTION_UXGA       8
#define NUM_VIDEO_RESOLUTIONS       9
#define ADV7511_ADDR                0x72
#define H_STRIDE                    3840
#define H_ACTIVE                    1920
#define V_ACTIVE                    1080
/*****************************************************************************************************************/
#ifndef uartPRINTF
#define uartPRINTF    printf
#endif
#define uart_Black_Text() uartPRINTF("\x1B[30m")
#define uart_Red_Text() uartPRINTF("\x1B[31m")
#define uart_Green_Text() uartPRINTF("\x1B[32m")
#define uart_Yellow_Text() uartPRINTF("\x1B[33m")
#define uart_Blue_Text() uartPRINTF("\x1B[34m")
#define uart_Magenta_Text() uartPRINTF("\x1B[35m")
#define uart_Cyan_Text() uartPRINTF("\x1B[36m")
#define uart_White_Text() uartPRINTF("\x1B[37m")
#define uart_Default_Text() uartPRINTF("\x1B[39m")
#define print_prompt() uartPRINTF("\x1B[31m")
#define print_prompt_Default_Text() uartPRINTF("\x1B[39m")
#define uart_Black_Background() uartPRINTF("\x1B[40m")
#define uart_Red_Background() uartPRINTF("\x1B[41m")
#define uart_Green_Background() uartPRINTF("\x1B[42m")
#define uart_Yellow_Background() uartPRINTF("\x1B[43m")
#define uart_Blue_Background() uartPRINTF("\x1B[44m")
#define uart_Magenta_Background() uartPRINTF("\x1B[45m")
#define uart_Cyan_Background() uartPRINTF("\x1B[46m")
#define uart_White_Background() uartPRINTF("\x1B[47m")
#define uart_Default_Background() uartPRINTF("\x1B[49m")
#define uart_Cursor_Home() uartPRINTF("\x1B[H")
#define uart_Cursor_Set_XY_Position(X,Y) uartPRINTF("\x1B[%d;%dH",X,Y)
#define uart_Cursor_Show() uartPRINTF("\x1B[?25h")
#define uart_Cursor_Hide() uartPRINTF("\x1B[?25l")
#define uart_Cursor_Up(Rows) uartPRINTF("\x1B[%dA",Rows)
#define uart_Cursor_Down(Rows) uartPRINTF("\x1B[%dB",Rows)
#define uart_Cursor_Left(Columns) uartPRINTF("\x1B[%dC",Columns)
#define uart_Cursor_Right(Columns) uartPRINTF("\x1B[%dD",Columns)
#define uart_Clear_Line_To_End() uartPRINTF("\x1B[0K")
#define uart_Clear_Line_Up_To_Cursor() uartPRINTF("\x1B[1K")
#define uart_Clear_Line() uartPRINTF("\x1B[2K")
#define uart_Clear_Screen() uartPRINTF("\x1B[2J")
#define uart_Clear_Screen_To_End() uartPRINTF("\x1B[J")
#define uart_Clear_Screen_Up_To_Cursor() uartPRINTF("\x1B[1J")
#define uart_Text_Attributes_None() uartPRINTF("\x1B[0m")
#define uart_Text_Attributes_Bold() uartPRINTF("\x1B[1m")
#define uart_Text_Attributes_italic() uartPRINTF("\x1B[3m")
#define uart_Text_Attributes_underline() uartPRINTF("\x1B[4m")
#define uart_Text_Attributes_strike() uartPRINTF("\x1B[9m")
#define uart_Text_Attributes_Dim() uartPRINTF("\x1B[2m")
#define uart_Text_Attributes_Blink() uartPRINTF("\x1B[5m")
#define uart_Text_Attributes_Reverse() uartPRINTF("\x1B[7m")
#define uart_Text_Attributes_Hidden() uartPRINTF("\x1B[8m")
#define uart_Mode_Insert() uartPRINTF("\x1B[4h")
#define uart_Mode_Overwrite() uartPRINTF("\x1B[4l")
#define uart_Mode_Scrolling_Smooth() uartPRINTF("\x1B[?4h")
#define uart_Mode_Scrolling_Jump() uartPRINTF("\x1B[?4l")
#define uart_Editing_Insert_Lines(Lines) uartPRINTF("\x1B[%dL",Lines)
#define uart_Editing_Delete_Lines(Lines) uartPRINTF("\x1B[%dM",Lines)
#define uart_Editing_Delete_Characters(Characters) uartPRINTF("\x1B[%dP",Characters)
/*****************************************************************************************************************/
#define KEYPRESS_ARROW_UP       240
#define KEYPRESS_ARROW_DOWN     241
#define KEYPRESS_ARROW_LEFT     242
#define KEYPRESS_ARROW_RIGHT    243
#define KEYPRESS_END            244
/*****************************************************************************************************************/
#define menu_select             1
#define menuCheck               19
#define kernalCoef              0
/*****************************************************************************************************************/



#define kernalconfig            0x6769666E
#define clear                   0x10ECC0
#define mainmenu                0x4773053D
#define uart_0_baseaddr         0xe0000000


#define timex                   0x137C66
#define display1                0x429F579F
#define display2                0x429F57A0
#define display3                0x429F57A1
#define display5                0x429F57A3
#define display6                0x429F57A4
#define quit                    0x1EBAA
#define pl_testpattern          0x65747461
#define hdmi                    0x1C1FB
#define vga                     0x327F
#define xbright                 0xA8B548
#define blur                    0x1AE04
#define emboss                  0xAC6E0D
#define sharp                   0x134D38
#define hsv                     0x2D94
#define hsl                     0x2D8A
#define rgb                     0x30F0
#define rgbpoi                  0xBF59FF
#define sobel                   0x13686E
#define prewit                  0xBE08B2
#define prewitt                 0x76C5768
#define cgain                   0x10D7F4
#define menucall                0x47B89810
/************************************/

#define d5mgain                 0x60FE0B4
/************************************/
#define hsvpervalue             0xF43BADEF
#define yccpervalue             0x5E197EEF
#define lumvalue                0x4812C2CF
/************************************/
#define rgbdetect               0xEB640CB6
#define kernal                  0xB49AAE

#define quit_current_state      0x71



#define cmds_quit               0x95BA400A
#define cmds_space              0x20
#define cmds_uart               0x95BA4834
#define cmds_hdmitx             0x7CB0C08C
#define cmds_videoconfig        0x6F636F65
#define cmds_readfifo           0x66696664
#define cmds_fifomode           0x646F6D6F
#define cmds_fifostatus         0x5B28281D
#define cmds_gridpoint          0x148D411C
#define cmds_griddelta          0x148B46A1
#define sdcard                  0xC06994
#define wsd                     0x335E
#define rsd                     0x316A

/*****************************************************************************************************************/
#endif // __SYSTEM_CONFIG_DEFINES_H__
