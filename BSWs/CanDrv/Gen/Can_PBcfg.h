/*************************************************************************************
FileName:                   Can_PBcfg.c                                        
AUTOSAR Version:            4.2.2
*************************************************************************************/
/*************************************************************************************

**                                   Includes                                       **  
                     
*************************************************************************************/
#include "Can_Cfg.h"
/************************************************************************************
**                      Post-Build Configuration variables values                  **
************************************************************************************/
Can Can = 
{
	.CanGeneral = 
	{
		.CanMainFunctionRWPeriods = 
		{
			{
				.CanMainFunctionPeriod = 0.01
			},
			{
				.CanMainFunctionPeriod = 0.01
			}
		}
	},
	
	.CanConfigSet = 
	{
		.CanController = 
		{
			{
				.CanControllerId = 0U,
				.CanControllerBaudrateConfig = 
				{
					{
						.CanControllerIdBaudRate = 200U,
						.CanControllerSyncJumpWidth = 2U,
						.CanControllerPropSeg = 2U,
						.CanControllerSeg1 = 3U,
						.CanControllerSeg2 = 4U,
						.CanControllerBaudRateConfigId = 0U
					},
					{
						.CanControllerIdBaudRate = 500U,
						.CanControllerSyncJumpWidth = 4U,
						.CanControllerPropSeg = 2U,
						.CanControllerSeg1 = 6U,
						.CanControllerSeg2 = 7U,
						.CanControllerBaudRateConfigId = 1U
					}
				}
				.CanControllerActivation = true,
				.CanControllerBaseAddress = 0x40040000U,
				.CanControllerDefaultBaudrate = &CanContainer.CanConfigSet.CanController[0U].CanControllerBaudrateConfig[0U]
			},
			
			{
				.CanControllerId = 1U,
				.CanControllerBaudrateConfig = 
				{
					{
						.CanControllerIdBaudRate = 200U,
						.CanControllerSyncJumpWidth = 2U,
						.CanControllerPropSeg = 2U,
						.CanControllerSeg1 = 3U,
						.CanControllerSeg2 = 4U,
						.CanControllerBaudRateConfigId = 0U
					},
					{
						.CanControllerIdBaudRate = 500U,
						.CanControllerSyncJumpWidth = 4U,
						.CanControllerPropSeg = 2U,
						.CanControllerSeg1 = 6U,
						.CanControllerSeg2 = 7U,
						.CanControllerBaudRateConfigId = 1U
					}
				}
				.CanControllerActivation = false,
				.CanControllerBaseAddress = 0x40041000U,
				.CanControllerDefaultBaudrate = &CanContainer.CanConfigSet.CanController[1U].CanControllerBaudrateConfig[0U]
			}
		},
		
		.CanHardwareObject =
		{
			{
				.CanTriggerTransmitEnable = false,
				.CanHandleType = FULL,
				.CanObjectType = TRANSMIT,
				.CanControllerRef = &CanContainer.CanConfigSet.CanController[0]
				.CanIdType = STANDARD,
				.CanObjectId = 1U,
				CanHwObjectCount = 1U
				// We don't need to set CanHwFilter as it is a transmit HOH
			},
			
			{
				.CanTriggerTransmitEnable = false,
				.CanHwFilter = 
				{
					.CanHwFilterMask = 0x1U,
					.CanHwFilterCode = 0x0U,
				},
				.CanHandleType = FULL,
				.CanObjectType = RECEIVE,
				.CanControllerRef = &CanContainer.CanConfigSet.CanController[0]
				.CanIdType = STANDARD,
				.CanObjectId = 1U,
				CanHwObjectCount = 1U
			}
		}
	}
};
/**************************************************************************************/