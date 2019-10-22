/*******************************************************************************************************************************
FileName:                    								    CanIf_Types.h                                      
AUTOSAR Version:          								  4.2.2
******************************************************************************************************************************/
#ifndef CANIF_TYPES_H
#define CANIF_TYPES_H

/******************************************************************************************************************************
 **                                                                        Includes                                                                             **
 ******************************************************************************************************************************/
#include "Can_GeneralTypes.h"
#include "ComStack_Types.h"
/*******************************************************************************************************************************
**                                               						  Defines                                                                                **
********************************************************************************************************************************/



/*******************************************************************************************************************************
**                                               						   Variables                                                                             **
********************************************************************************************************************************/


/*******************************************************************************************************************************
**                                               				    Variables Definations                                                                    **
********************************************************************************************************************************/

/**************************************************************************************************
**
Name:                                     CanIf_ConfigType
Type:                                      Structure
Description:  
                     This type defines a data structure for the post build parameters of the CAN
					 interface for all underlying CAN drivers. At initialization the CanIf gets a
					 pointer to a structure of this type to get access to its configuration data, 
					 which is necessary for initialization.
																																**
**************************************************************************************************/





/**************************************************************************************************
**
Name:                                     CanIf_ControllerModeType
Type:                                      Enumeration
Description:  
                     Operating modes of a CAN controller.
																																**
**************************************************************************************************/


typedef enum {
    CANIF_CS_SLEEP,
    CANIF_CS_STARTED,
    CANIF_CS_STOPPED,
    CANIF_CS_UNINIT,

}CanIf_ControllerModeType;


/**************************************************************************************************
**
Name:                                     CanIf_PduModeType
Type:                                      Enumeration
Description:  
                     The PduMode of a channel defines its transmit or receive activity.
					 Communication direction (transmission and/or reception) of the channel can
					 be controlled separately or together by upper layers.
																																**
**************************************************************************************************/

typedef enum {
    CANIF_OFFLINE,
    CANIF_TX_OFFLINE,
    CANIF_TX_OFFLINE_ACTIVE,
    CANIF_ONLINE,

}CanIf_PduModeType;


/**************************************************************************************************
**
Name:                                   CanIf_NotifStatusType
Type:                                      Enumeration
Description:  
                     Return value of CAN L-PDU notification status.
																																**
**************************************************************************************************/

typedef enum {
    CANIF_TX_RX_NOTIFICATION,
    CANIF_NO_NOTIFICATION,

}CanIf_NotifStatusType;

#endif /*CANIF_TYPES_H*/