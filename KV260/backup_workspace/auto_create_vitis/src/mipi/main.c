#include <math.h>
#include <string.h>
#include <xaxivdma.h>
#include <xil_printf.h>
#include <xparameters.h>
#include <xv_demosaic.h>
#include <xv_gamma_lut.h>
#include <xvtc.h>

#include "xil_types.h"
#include "DP_VIDEO/video_modes.h"
#include "DP_VIDEO/xdpdma_video.h"
#include "MENU/menu_calls.h"
#include "PLATFORM/platform.h"
#include "config.h"


XAxiVdma vdma;
XAxiVdma_DmaSetup vdmaDMA;
XAxiVdma_Config *vdmaConfig;
XVtc    VtcInst;
XVtc_Config *vtc_config ;
VideoMode video;

#define VDMA_DEVICE_ID XPAR_AXIVDMA_0_DEVICE_ID

u32 frameBuf[DISPLAY_NUM_FRAMES][DEMO_MAX_FRAME];
u32 *pFrames[DISPLAY_NUM_FRAMES];

int main()
{

    XVtc_Timing vtcTiming;
    XVtc_SourceSelect SourceSelect;
    VideoMode video;
    int Status;
    init_platform();
    vtc_config = XVtc_LookupConfig(XPAR_VTC_0_DEVICE_ID);
    XVtc_CfgInitialize(&VtcInst, vtc_config, vtc_config->BaseAddress);
    init_camera();
    per_write_reg(REG0,16);
    displayport_init();
    displayport_setup_interrupts();
    print("Configuration Complete\n\r");

    //**************************************************************************************
    // VIDEO TIMINF CONTROLLER
    //**************************************************************************************
      video = VMODE_1280x720;
    vtcTiming.HActiveVideo          = video.width;    /**< Horizontal Active Video Size */
    vtcTiming.HFrontPorch           = video.hps - video.width;    /**< Horizontal Front Porch Size */
    vtcTiming.HSyncWidth            = video.hpe - video.hps;        /**< Horizontal Sync Width */
    vtcTiming.HBackPorch            = video.hmax - video.hpe + 1;        /**< Horizontal Back Porch Size */
    vtcTiming.HSyncPolarity         = video.hpol;    /**< Horizontal Sync Polarity */
    vtcTiming.VActiveVideo          = video.height;    /**< Vertical Active Video Size */
    vtcTiming.V0FrontPorch          = video.vps - video.height;    /**< Vertical Front Porch Size */
    vtcTiming.V0SyncWidth           = video.vpe - video.vps;    /**< Vertical Sync Width */
    vtcTiming.V0BackPorch           = video.vmax - video.vpe + 1;;    /**< Horizontal Back Porch Size */
    vtcTiming.V1FrontPorch          = video.vps - video.height;    /**< Vertical Front Porch Size */
    vtcTiming.V1SyncWidth           = video.vpe - video.vps;    /**< Vertical Sync Width */
    vtcTiming.V1BackPorch           = video.vmax - video.vpe + 1;;    /**< Horizontal Back Porch Size */
    vtcTiming.VSyncPolarity         = video.vpol;    /**< Vertical Sync Polarity */
    vtcTiming.Interlaced            = 0;
    memset((void *)&SourceSelect, 0, sizeof(SourceSelect));
    SourceSelect.VBlankPolSrc       = 1;
    SourceSelect.VSyncPolSrc        = 1;
    SourceSelect.HBlankPolSrc       = 1;
    SourceSelect.HSyncPolSrc        = 1;
    SourceSelect.ActiveVideoPolSrc  = 1;
    SourceSelect.ActiveChromaPolSrc = 1;
    SourceSelect.VChromaSrc         = 1;
    SourceSelect.VActiveSrc         = 1;
    SourceSelect.VBackPorchSrc      = 1;
    SourceSelect.VSyncSrc           = 1;
    SourceSelect.VFrontPorchSrc     = 1;
    SourceSelect.VTotalSrc          = 1;
    SourceSelect.HActiveSrc         = 1;
    SourceSelect.HBackPorchSrc      = 1;
    SourceSelect.HSyncSrc           = 1;
    SourceSelect.HFrontPorchSrc     = 1;
    SourceSelect.HTotalSrc          = 1;
    XVtc_RegUpdateEnable(&VtcInst);
    XVtc_SetGeneratorTiming(&VtcInst, &vtcTiming);
    XVtc_SetSource(&VtcInst, &SourceSelect);
    XVtc_EnableGenerator(&VtcInst);
    XVtc_Enable(&VtcInst);
    //**************************************************************************************



    //**************************************************************************************
    // VMDA
    //**************************************************************************************
    for (int i = 0; i < 1; i++)
    {
        pFrames[i] = frameBuf[i];
    }
    vdmaConfig = XAxiVdma_LookupConfig(XPAR_AXIVDMA_0_DEVICE_ID);
    XAxiVdma_CfgInitialize(&vdma, vdmaConfig, vdmaConfig->BaseAddress);
    vdmaDMA.FrameDelay              = 0;
    vdmaDMA.EnableCircularBuf       = 1;
    vdmaDMA.EnableSync              = 0;
    vdmaDMA.PointNum                = 0;
    vdmaDMA.EnableFrameCounter      = 0;
    vdmaDMA.VertSizeInput           = video.height;
    vdmaDMA.HoriSizeInput           = (video.width)*3;
    vdmaDMA.FixedFrameStoreAddr     = 0;
    vdmaDMA.FrameStoreStartAddr[0]  = (u32) pFrames[0];
    vdmaDMA.Stride                  = (video.width)*3;
    XAxiVdma_DmaConfig(&vdma, XAXIVDMA_WRITE, &(vdmaDMA));
    Status = XAxiVdma_DmaSetBufferAddr(&vdma, XAXIVDMA_WRITE,vdmaDMA.FrameStoreStartAddr);
    XAxiVdma_DmaConfig(&vdma, XAXIVDMA_READ, &(vdmaDMA));
    XAxiVdma_DmaSetBufferAddr(&vdma, XAXIVDMA_READ,vdmaDMA.FrameStoreStartAddr);
    xil_printf("frame addr %x\n\r",vdmaDMA.FrameStoreStartAddr[0]);
    Status = XAxiVdma_DmaStart(&vdma, XAXIVDMA_WRITE);
    Status = XAxiVdma_StartParking(&vdma, 0, XAXIVDMA_WRITE);
    XAxiVdma_DmaStart(&vdma, XAXIVDMA_READ);
    XAxiVdma_StartParking(&vdma, 0, XAXIVDMA_READ);
    print("Configuration Complete\n\r");

    //**************************************************************************************
    // GAMMA LUT
    //**************************************************************************************
    gamma_lut_init();

    //**************************************************************************************
    // DEMOSAIC
    //**************************************************************************************
    demosaic_init();




    run_dppsu();

    print("Entire video pipeline activated\r\n");

     while(1){
         menu_calls(TRUE);
     }
    cleanup_platform();
    return 0;
}
