#include <math.h>
#include <string.h>
#include <xaxivdma.h>
#include <xgpiops.h>
#include <xiicps.h>
#include <xv_demosaic.h>
#include <xv_gamma_lut.h>
#include <xvtc.h>
#include "sleep.h"
#include "xil_printf.h"
#include "xil_types.h"
#include "xparameters.h"
#include "xstatus.h"

#include "DP_VIDEO/video_modes.h"

#include "menu/menu_calls.h"
#include "platform/platform.h"
#include "system_config_defines.h"



XAxiVdma vdma;
XAxiVdma_DmaSetup vdmaDMA;
XAxiVdma_Config *vdmaConfig;
XVtc	VtcInst;
XVtc_Config *vtc_config ;
VideoMode video;
XV_demosaic cfa;
XV_gamma_lut gamma_inst;



#define DEMO_MAX_FRAME (720*1280)
#define DISPLAY_NUM_FRAMES 1
#define cam_gpio XPAR_XGPIOPS_0_DEVICE_ID
#define IIC_cam 			XPAR_XIICPS_0_DEVICE_ID
#define CAM_ID              0x78
#define IIC_CAM_ADDR  		0x3c
#define IIC_SCLK_RATE		100000
#define SW_IIC_ADDR         0x74//0x75

void detect_camera();
int  Initial_setting_1 ( u32 *cfg_init , int cfg_init_QTY  );
void gamma_calc(float gamma_val);

u16 gamma_reg[1024];
u32 frameBuf[DISPLAY_NUM_FRAMES][DEMO_MAX_FRAME];
u32 *pFrames[DISPLAY_NUM_FRAMES];

int main()
{

	XVtc_Timing vtcTiming;
	XVtc_SourceSelect SourceSelect;
	VideoMode video;
	int Status;
    init_platform();
    print("Hello World\n\r");
    vtc_config = XVtc_LookupConfig(XPAR_VTC_0_DEVICE_ID);
    XVtc_CfgInitialize(&VtcInst, vtc_config, vtc_config->BaseAddress);
    init_camera();
	xil_printf("Configuration Complete\n\r");

	//set up the video timing controller
  	video = VMODE_1280x720;
	vtcTiming.HActiveVideo = video.width;	/**< Horizontal Active Video Size */
	vtcTiming.HFrontPorch = video.hps - video.width;	/**< Horizontal Front Porch Size */
	vtcTiming.HSyncWidth = video.hpe - video.hps;		/**< Horizontal Sync Width */
	vtcTiming.HBackPorch = video.hmax - video.hpe + 1;		/**< Horizontal Back Porch Size */
	vtcTiming.HSyncPolarity = video.hpol;	/**< Horizontal Sync Polarity */
	vtcTiming.VActiveVideo = video.height;	/**< Vertical Active Video Size */
	vtcTiming.V0FrontPorch = video.vps - video.height;	/**< Vertical Front Porch Size */
	vtcTiming.V0SyncWidth = video.vpe - video.vps;	/**< Vertical Sync Width */
	vtcTiming.V0BackPorch = video.vmax - video.vpe + 1;;	/**< Horizontal Back Porch Size */
	vtcTiming.V1FrontPorch = video.vps - video.height;	/**< Vertical Front Porch Size */
	vtcTiming.V1SyncWidth = video.vpe - video.vps;	/**< Vertical Sync Width */
	vtcTiming.V1BackPorch = video.vmax - video.vpe + 1;;	/**< Horizontal Back Porch Size */
	vtcTiming.VSyncPolarity = video.vpol;	/**< Vertical Sync Polarity */
	vtcTiming.Interlaced = 0;

    memset((void *)&SourceSelect, 0, sizeof(SourceSelect));
	SourceSelect.VBlankPolSrc = 1;
	SourceSelect.VSyncPolSrc = 1;
	SourceSelect.HBlankPolSrc = 1;
	SourceSelect.HSyncPolSrc = 1;
	SourceSelect.ActiveVideoPolSrc = 1;
	SourceSelect.ActiveChromaPolSrc= 1;
	SourceSelect.VChromaSrc = 1;
	SourceSelect.VActiveSrc = 1;
	SourceSelect.VBackPorchSrc = 1;
	SourceSelect.VSyncSrc = 1;
	SourceSelect.VFrontPorchSrc = 1;
	SourceSelect.VTotalSrc = 1;
	SourceSelect.HActiveSrc = 1;
	SourceSelect.HBackPorchSrc = 1;
	SourceSelect.HSyncSrc = 1;
	SourceSelect.HFrontPorchSrc = 1;
	SourceSelect.HTotalSrc = 1;
	XVtc_RegUpdateEnable(&VtcInst);
	XVtc_SetGeneratorTiming(&VtcInst, &vtcTiming);
	XVtc_SetSource(&VtcInst, &SourceSelect);
	XVtc_EnableGenerator(&VtcInst);
	XVtc_Enable(&VtcInst);




	for (int i = 0; i < 1; i++)
	{
		pFrames[i] = frameBuf[i];
	}

	vdmaConfig = XAxiVdma_LookupConfig(XPAR_AXIVDMA_0_DEVICE_ID);
	XAxiVdma_CfgInitialize(&vdma, vdmaConfig, vdmaConfig->BaseAddress);
	//video = VMODE_1280x720;
	vdmaDMA.FrameDelay = 0;
	vdmaDMA.EnableCircularBuf = 1;
	vdmaDMA.EnableSync = 0;
	vdmaDMA.PointNum = 0;
	vdmaDMA.EnableFrameCounter = 0;

	vdmaDMA.VertSizeInput = video.height;
	vdmaDMA.HoriSizeInput = (video.width)*3;
	vdmaDMA.FixedFrameStoreAddr = 0;
	vdmaDMA.FrameStoreStartAddr[0] = (u32)  pFrames[0];
	vdmaDMA.Stride = (video.width)*3;

	XAxiVdma_DmaConfig(&vdma, XAXIVDMA_WRITE, &(vdmaDMA));
	Status = XAxiVdma_DmaSetBufferAddr(&vdma, XAXIVDMA_WRITE,vdmaDMA.FrameStoreStartAddr);

	XAxiVdma_DmaConfig(&vdma, XAXIVDMA_READ, &(vdmaDMA));
	XAxiVdma_DmaSetBufferAddr(&vdma, XAXIVDMA_READ,vdmaDMA.FrameStoreStartAddr);
	xil_printf("frame addr %x\n\r",vdmaDMA.FrameStoreStartAddr[0]);

	XV_demosaic_Initialize(&cfa, XPAR_PS_VIDEO_RX_VIDEO_V_DEMOSAIC_0_DEVICE_ID);
	XV_demosaic_Set_HwReg_width(&cfa, video.width);
	XV_demosaic_Set_HwReg_height(&cfa, video.height);
	XV_demosaic_Set_HwReg_bayer_phase(&cfa, 0x03);
	XV_demosaic_EnableAutoRestart(&cfa);
	XV_demosaic_Start(&cfa);

	gamma_calc(1.2);
	XV_gamma_lut_Initialize(&gamma_inst, XPAR_PS_VIDEO_RX_VIDEO_V_GAMMA_LUT_0_DEVICE_ID);
	XV_gamma_lut_Set_HwReg_width(&gamma_inst, video.width);
	XV_gamma_lut_Set_HwReg_height(&gamma_inst, video.height);
	XV_gamma_lut_Set_HwReg_video_format(&gamma_inst, 0x00);
	XV_gamma_lut_Write_HwReg_gamma_lut_0_Bytes(&gamma_inst, 0,(int *) gamma_reg, 512);
	XV_gamma_lut_Write_HwReg_gamma_lut_1_Bytes(&gamma_inst, 0,(int *) gamma_reg, 512);
	XV_gamma_lut_Write_HwReg_gamma_lut_2_Bytes(&gamma_inst, 0,(int *) gamma_reg, 512);
	XV_gamma_lut_Start(&gamma_inst);
	XV_gamma_lut_EnableAutoRestart(&gamma_inst);





	xil_printf("Configuration Complete\n\r");

	Status = XAxiVdma_DmaStart(&vdma, XAXIVDMA_WRITE);
	Status = XAxiVdma_StartParking(&vdma, 0, XAXIVDMA_WRITE);
	XAxiVdma_DmaStart(&vdma, XAXIVDMA_READ);
	XAxiVdma_StartParking(&vdma, 0, XAXIVDMA_READ);




	run_dppsu();

	 while(1){
		 menu_calls(TRUE);
	 }


    cleanup_platform();
    return 0;
}







void gamma_calc(float gamma_val)
{
	int i;
	for(i = 0; i<256; i++){
		gamma_reg[i] = (pow((i / 256.0), (1/gamma_val)) * 256.0);
	}
}

