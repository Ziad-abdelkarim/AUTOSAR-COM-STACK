/*******************************************************************************************************************************
FileName:                    							CanIf_Cfg.c                                     
AUTOSAR Version:          								  4.2.2
******************************************************************************************************************************/
/******************************************************************************************************************************
 **                                                     Includes                                                             **
 ******************************************************************************************************************************/
#include "CanIf.h"
#include "Can_Cfg.h"
#include "PduR_PBCfg.h"
#include "CanTp_Cfg.h"
/*****************************************************************************************************************************
 **                      					Post-Build Configuration variables values                                       **
 *****************************************************************************************************************************/
extern Can_ConfigType Can;

CanIf_ConfigType CanIf = 
{
	.CanIfCtrlDrvCfg = 
	{
		.CanIfCtrlDrvInitHohConfigRef = &CanIf.CanIfInitCfg.CanIfInitHohCfg[0U],
		.CanIfCtrlCfg = 
		{
			{
				.CanIfCtrlCanCtrlRef = &Can.CanConfigSet.CanController[0U],
				.CanIfCtrlId = 0x0,
				.CanIfCtrlWakeupSupport = false
			}
		 }
	 },
	 
	.CanIfInitCfg =
	{
		.CanIfBufferCfg =
		{
			{
				.CanIfBufferHthRef = &CanIf.CanIfInitCfg.CanIfInitHohCfg[0U].CanIfHthCfg[0U],
				.CanIfBufferSize = 1
			}
		}, 
		.CanIfInitHohCfg = 
		{
			{
				.CanIfHthCfg =
				{
					{
						.CanIfHthCanCtrlIdRef = &CanIf.CanIfCtrlDrvCfg.CanIfCtrlCfg[0U],
						.CanIfHthIdSymRef = &Can.CanConfigSet.CanHardwareObject[0U]
					}
				},
				 
				.CanIfHrhCfg =
				{
					{
						.CanIfHrhCanCtrlIdRef = &CanIf.CanIfCtrlDrvCfg.CanIfCtrlCfg[0U],
						.CanIfHrhIdSymRef = &Can.CanConfigSet.CanHardwareObject[1U]
					}
				}
			}
		},
		 
		.CanIfRxPduCfg = 
		{
			{
				.CanIfRxPduCanId = 0x0,
				.CanIfRxPduCanIdMask = 0x0,
				.CanIfRxPduCanIdType = STANDARD_CAN,
				.CanIfRxPduDlc = 40,
				.CanIfRxPduHrhIdRef = &CanIf.CanIfInitCfg.CanIfInitHohCfg[0U].CanIfHrhCfg[0U],
				.CanIfRxPduId = 0x0,
				.CanIfRxPduReadData = true,
				.CanIfRxPduReadNotifyStatus = false,
				.CanIfRxPduUserRxIndicationUL = PDUR,
				.CanIfRxPduCanIdRange = 
				{
					.CanIfRxPduCanIdRangeLowerCanId = 0,
					.CanIfRxPduCanIdRangeUpperCanId = 2000
				}
			}
		},
		 
		.CanIfTxPduCfg =
		{
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
