/*******************************************************************************************************************************
FileName:                    							PduR_PBCfg.c
AUTOSAR Version:          								  4.2.2
******************************************************************************************************************************/
/******************************************************************************************************************************
 **                                                     Includes                                                             **
 ******************************************************************************************************************************/
 #include "PduR.h"

 /*****************************************************************************************************************************
  **                      					Post-Build Configuration variables values                                       **
  *****************************************************************************************************************************/
PduR_PBConfigType PduR = 
{
	.PduRRoutingTables=
	{
		.PduRRoutingTable=
		{
			{
				.PduRRoutingPath=
				{
					{
						.PduRDestPdu=
						{
							{
								.PduRDestPduDataProvision=PDUR_DIRECT,
								.PduRDestPduHandleId=0,
								.PduRTransmissionConfirmation= true
							}
						},
						.PduRSrcPdu=
						{
							.PduRSourcePduHandleId=0,
							.PduRSrcPduUpTxConf=true
						}
					},
					{
						.PduRDestPdu=
						{
							{
								.PduRDestPduDataProvision=PDUR_DIRECT,
								.PduRDestPduHandleId=1,
								.PduRTransmissionConfirmation= true
							}
						},
						.PduRSrcPdu=
						{
								.PduRSourcePduHandleId=1,
								.PduRSrcPduUpTxConf=true
						}
					}
				}
			}
		}
	}
};
