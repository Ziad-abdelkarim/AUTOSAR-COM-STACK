/*******************************************************************************************************************************
FileName:                                               PduR_PBCfg.h
AUTOSAR Version:                                          4.2.2
******************************************************************************************************************************/
#ifndef PDUR_PBCFG_H
#define PDUR_PBCFG_H

/*******************************************************************************************************************************
**                                                                        Defines                                                                                **
********************************************************************************************************************************/
/**************************************************************************************************
**
Name:                       PduRTxConfirmation
Type:                       EcucBooleanParamDef
Description:                Specifies if the BSW module supports the TxConfirmation API or not.
							Value true the API is supported.                          			**
**************************************************************************************************/
#define PduRTxConfirmation	true

/**************************************************************************************************
**
Name:                       PduRTxConfirmation
Type:                       EcucBooleanParamDef
Description:                Switches the Default Error Tracer (Det) detection and notification ON or OFF.
							 true: enabled (ON).
							 false: disabled (OFF).                        					**
**************************************************************************************************/
#define PduRDevErrorDetect	true

/**************************************************************************************************
**
Name:                       PduRTxConfirmation
Type:                       EcucIntegerParamDef
Description:                Maximum number of RoutingPaths in all RoutingTables. This parameter is
							needed only in case of post-build loadable implementation using static
							memory allocation.													**
**************************************************************************************************/
#define PduRMaxRoutingPathCnt	(uint16)2

/**************************************************************************************************
**
Name:                       PduRMaxRoutingTableCnt
Type:                       EcucIntegerParamDef
Description:                Maximum number of RoutingTables.
							This parameter is needed only in case of post-build loadable
							implementation using static memory allocation.						**
**************************************************************************************************/
#define PduRMaxRoutingTableCnt	(uint16)1

#endif /*PDUR_PBCFG_H*/