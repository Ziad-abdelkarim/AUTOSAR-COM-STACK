/*******************************************************************************************************************************
FileName:                    							Com_Cfg.c
AUTOSAR Version:          								  4.2.2
******************************************************************************************************************************/
/******************************************************************************************************************************
 **                                                     Includes                                                             **
 ******************************************************************************************************************************/
#include "Com.h"
#include "Com_Cfg.h"
/*****************************************************************************************************************************
 **                      					Post-Build Configuration variables values                                       **
 *****************************************************************************************************************************/

Com_Type Com = {
                .ComConfig={
                            .ComIPdu={
                                      {.ComIPduDirection = RECEIVE ,
                                       .ComIPduSignalProcessing = IMMEDIATE,
                                       .ComIPduType = NORMAL,
                                       .ComTxIPdu={
                                                    .ComTxIPduClearUpdateBit= Transmit,
                                                    .ComTxModeFalse ={
                                                                      .ComTxMode = {
                                                                                    .ComTxModeMode=DIRECT
                                                                                   }
                                                                      },
                                                    .ComTxModeTrue ={
                                                                     .ComTxMode = {
                                                                                   .ComTxModeMode=DIRECT
                                                                                  }
                                                                    },
                                                   .ComMinimumDelayTime= COMMINIMUMDELAYTIME
                                                   },
                                       .ComIPduCancellationSupport=false,
                                       .ComIPduHandleId=0,

                                      },{0,0,0}
                            }

                }
};






























/*
CanIf_ConfigType CanIf = {
	 
	 .CanIfCtrlDrvCfg = {
		 .CanIfCtrlDrvInitHohConfigRef = &CanIf.CanIfInitCfg.CanIfInitHohCfg[0U],
		 
		 .CanIfCtrlCfg = {
			{
				.CanIfCtrlCanCtrlRef = &Can.CanConfigSet.CanController[0U],
				.CanIfCtrlId = 0x0,
				.CanIfCtrlWakeupSupport = false
			}
		 }
	 },
	 
	 .CanIfInitCfg = {
		 .CanIfBufferCfg = {
			 {
				 .CanIfBufferHthRef = &CanIf.CanIfInitCfg.CanIfInitHohCfg[0U].CanIfHthCfg[0U],
				 .CanIfBufferSize = 1
			 }
		 },
		 
		 .CanIfInitHohCfg = {
			 {
				 				 
				 .CanIfHthCfg = {
					 {
						 .CanIfHthCanCtrlIdRef = &CanIf.CanIfCtrlDrvCfg.CanIfCtrlCfg[0U],
						 .CanIfHthIdSymRef = &Can.CanConfigSet.CanHardwareObject[0U]
					 }
				 },
				 
				 .CanIfHrhCfg = {
					 {
						.CanIfHrhCanCtrlIdRef = &CanIf.CanIfCtrlDrvCfg.CanIfCtrlCfg[0U],
						.CanIfHrhIdSymRef = &Can.CanConfigSet.CanHardwareObject[1U]
					 }
				 }
			 }
		 },
		 
		 .CanIfRxPduCfg = {
			 {
				 .CanIfRxPduCanId = 0x0,
				 .CanIfRxPduCanIdMask = 0x0,
				 .CanIfRxPduCanIdType = STANDARD_CAN,
				 .CanIfRxPduDlc = 0x1,
				 .CanIfRxPduHrhIdRef = &CanIf.CanIfInitCfg.CanIfInitHohCfg[0U].CanIfHrhCfg[0U],
				 .CanIfRxPduId = 0x0,
				 .CanIfRxPduReadData = true,
				 .CanIfRxPduReadNotifyStatus = false,
				 .CanIfRxPduUserRxIndicationUL = PDUR,
				 .CanIfRxPduCanIdRange = {
				                          .CanIfRxPduCanIdRangeLowerCanId = 0,
				                          .CanIfRxPduCanIdRangeUpperCanId = 2000
				 }
			 }
		 },
		 
		 .CanIfTxPduCfg = {
			 {
				 .CanIfTxPduBufferRef = &CanIf.CanIfInitCfg.CanIfBufferCfg[0U],
				 .CanIfTxPduCanId = 0x0,
				 .CanIfTxPduCanIdType = STANDARD_CAN,
				 .CanIfTxPduId = 0x0,
				 .CanIfTxPduReadNotifyStatus = true,
				 .CanIfTxPduType = STATIC,
				 .CanIfTxPduUserTxConfirmationUL = PDUR
			 }
		 }
	 }
 };
*/
