#include <sleep.h>
#include <xiicps.h>
#include <xil_printf.h>
#include <xil_types.h>
#include <xstatus.h>

#include "ov5640.h"
#include "PS_i2c.h"


XIicPs  iic_cam;
XIicPs ps_i2c;

#define IIC_DEVICEID XPAR_XIICPS_0_DEVICE_ID
#define cam_gpio XPAR_XGPIOPS_0_DEVICE_ID
#define IIC_cam 			XPAR_XIICPS_0_DEVICE_ID
#define CAM_ID              0x78
#define IIC_CAM_ADDR  		0x3c
#define IIC_SCLK_RATE		100000
#define SW_IIC_ADDR         0x74//0x75


u8 SendBuffer [10];
u8 RecvBuffer [10];

int init_camera()
{



	XIicPs_Config *iic_conf;

	int Status;



    print("init_camera\n\r");



	iic_conf = XIicPs_LookupConfig(IIC_cam);
	XIicPs_CfgInitialize(&iic_cam,iic_conf,iic_conf->BaseAddress);
	XIicPs_SetSClk(&iic_cam, IIC_SCLK_RATE);





    i2c_init(&ps_i2c, IIC_DEVICEID,100000);
    ///set the I2C Switch


    SendBuffer[0]= 0x04;
    Status = XIicPs_MasterSendPolled(&iic_cam, SendBuffer, 1, SW_IIC_ADDR);
  	if (Status != XST_SUCCESS) {
  		print("I2C write error\n\r");
  		return XST_FAILURE;
  	}


    //configure camera
	SendBuffer[0]= 0x31;
	SendBuffer[1]= 0x03;
	SendBuffer[2]= 0x11;
	Status = XIicPs_MasterSendPolled(&iic_cam, SendBuffer, 3, IIC_CAM_ADDR);
	//writeReg(0x3103, 0x11);
	//[7]=1 Software reset; [6]=0 Software power down; Default=0x02


	SendBuffer[0]= 0x30;
	SendBuffer[1]= 0x08;
	SendBuffer[2]= 0x82;
	Status = XIicPs_MasterSendPolled(&iic_cam, SendBuffer, 3, IIC_CAM_ADDR);
	//writeReg(0x3008, 0x82);

	usleep(1000000);


	SendBuffer[0]= 0x30;
	SendBuffer[1]= 0x08;
	SendBuffer[2]= 0x82;
	Status = XIicPs_MasterSendPolled(&iic_cam, SendBuffer, 3, IIC_CAM_ADDR);


	sensor_init(&ps_i2c);





    return 0;
}




