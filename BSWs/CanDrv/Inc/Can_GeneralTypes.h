/*************************************************************************************
**

FileName:                          Can_GeneralTypes.h                                        
AUTOSAR Version:            4.2.2
                                                                                                                     **
*************************************************************************************/
#ifndef CAN_GENERALTYPES_H
#define CAN_GENERALTYPES_H
/*************************************************************************************
**                                          Includes                                                          **
*************************************************************************************/

#define  NUMBER_OF_BAUDRATES           2
#define  NUMBER_OF_CONTROLLERS         2
#define  NUMBER_OF_HOH				   2
#define POLLING 2U
#define	INTERRUPT 0U
#define EXTENDED 1U
#define STANDARD 2U

/************************************************************************************
**                           Post-Build Configuration variables                    **
************************************************************************************/
/************************************************************************************
Name:                                 CanHandleType
Description: 
                    Specifies the type (Full-CAN or Basic-CAN) of a hardware object. 
Multiplicity:                                       1
Type:                          EcucEnumerationParamDef
*************************************************************************************/
typedef enum  {
    Basic,
	Full
}CanHandleType;

/************************************************************************************
Name:                                 CanObjectType
Description: 
                    Specifies if the HardwareObject is used as Transmit or as Receive object. 
Multiplicity:                                       1
Type:                          EcucEnumerationParamDef
*************************************************************************************/


typedef enum {
	Recieve,
	Transmit
}CanObjectType;

/************************************************************************************
Name:                                 CanHwFilter
Description: 
                     This container is only valid for HRHs and contains the configuration
                      (parameters) of one hardware filter.
Multiplicity:                                       1
Type:                                     Container
*************************************************************************************/

typedef struct {
	uint32  CanHwFilterMask;
	uint32  CanHwFilterCode;
}CanHwFilter;

/************************************************************************************
Name:                                 CanControllerBaudrateConfig
Description: 
                    This container contains bit timing related configuration parameters of the
                     CAN controller(s).
Multiplicity:                                             1
Type:                                               Container
*************************************************************************************/

typedef struct {
	
	uint32 CanControllerBaudRate;
	uint8  CanControllerSyncJumpWidth;
	uint8  CanControllerPropSeg;
	uint8  CanControllerSeg1;
	uint8  CanControllerSeg2;
	uint16 CanContollerBaudRateConfigId;
	
}CanControllerBaudrateConfig;

/************************************************************************************
Name:                                 CanMainFunctionRWPeriods
Description: 
                    This container contains the parameter for configuring the period for cyclic
					call to Can_MainFunction_Read or Can_MainFunction_Write depending
					on the referring item.
Multiplicity:                                     	  1
Type:                          					Container
*************************************************************************************/

typedef struct{	
	float CanMainFunctionPeriod;	
}CanMainFunctionRWPeriods;

/************************************************************************************
Name:                                 CanHardwareObject
Description: 
                   This container contains the configuration (parameters) of CAN Hardware Objects.
Multiplicity:                                       1..*
Type:                          				Container
*************************************************************************************/

typedef struct {
	bool ConTriggerTransmitEnable;
	CanHwFilter CanHwFilter;
	CanHandleType CanHandleType;
	CanObjectType CanObjectType;
	CanController* CanControllerRef;
	CanIdType CanIdType;
	uint16 CanObjectId;
	uint8 CanHwObjectCount;
}CanHardwareObject;

/************************************************************************************
Name:                              	   CanController
Description: 
                    This container contains the configuration parameters of the CAN controller(s). 
Multiplicity:                                       2
Type:                        				  Container
*************************************************************************************/

typedef struct {
	uint8  CanControllerId;
    CanControllerBaudrateConfig  CanControllerBaudrateConfig[MAX_NUMBER_OF_BAUDRATE];
	bool  CanControllerActivation;
	uint32  CanControllerBaseAddress ;
	CanControllerBaudrateConfig *CanControllerDefaultBaudrate;
}CanController;

/************************************************************************************
Name:                                 CanConfigSet
Description: 
                    This container contains the configuration parameters and sub containers of
                    the AUTOSAR Can module.
Multiplicity:                                       1
Type:                         				 Container
*************************************************************************************/

typedef struct {
	CanController CanController[NUMBER_OF_CAN_CONTROLLER];
	CanHardwareObject CanHardwareObject[NUMBER_OF_HOH];
}CanConfigSet;

/************************************************************************************
Name:                                 CanGeneral
Description: 
                    This container contains the parameters related each CAN Driver Unit. 
Multiplicity:                                       1
Type:                         				 Container
*************************************************************************************/

typedef struct{
	CanMainFunctionRWPeriods CanMainFunctionRWPeriods[2U];
}CanGeneral;

/************************************************************************************
Name:                                           Can
Description: 
                    This container holds the configuration of a single CAN Driver. 
Multiplicity:                                       1
Type:                                        container
*************************************************************************************/

typedef struct {
	CanGeneral  CanGeneral;
	CanConfigSet  CanConfigSet;
}Can;



/***************************************************************************************************
**                                        Types Declerations                                                             **
***************************************************************************************************/

/**************************************************************************************************
**
Name:                                     Can_IdType
Type:                                      uint16, uint32
Description:  
                     Represents the Identifier of an L-PDU. The two most significant bits specify the frame type:
                     00 CAN message with Standard CAN ID
                     01 CAN FD frame with Standard CAN ID                                             **
**************************************************************************************************/
#if    ( CAN_IDTYPE == UINT16)
typedef	uint16  Can_IdType;
#elif (CAN_IDTYPE == UINT32)
typedef	uint32  Can_IdType;
#else // default is uint16
typedef	uint16  Can_IdType;
#endif
/**************************************************************************************************
**
Name:                                    Can_HwHandleType
Type:                                     uint8, uint16
Description:  
                     Represents the hardware object handles of a CAN hardware unit. For CAN
                      hardware units with more than 255 HW objects use extended range. 
					                                                                                                                   **
**************************************************************************************************/
#if    ( CAN_HWHANDLETYPE == UINT8)
typedef	uint8  Can_HwHandleType;
#elif (CAN_HWHANDLETYPE == UINT16)
typedef	uint16  Can_HwHandleType;
#else // default is uint8
typedef	uint16  Can_HwHandleType;
#endif	
/**************************************************************************************************
**
Name:                                     Can_PduType
Type:                                      Structure
Description:  
                     This type unites PduId (swPduHandle), SduLength (length), SduData (sdu), and
                     CanId (id) for any CAN L-SDU.
                                                                                                                                       **
**************************************************************************************************/
typedef struct {
	PduIdType                              swPduHandle;
	uint8                                     length;
	Can_IdType                           id;
	uint8*                                   sdu;
}Can_PduType;
/**************************************************************************************************
**
Name:                                     Can_HwType
Type:                                      Structure
Description:  
                     This type defines a data structure which clearly provides an Hardware Object
                     Handle including its corresponding CAN Controller and therefore CanDrv as well
                     as the specific CanId.
                                                                                                                                       **
**************************************************************************************************/	

typedef struct {
	Can_IdType                           CanId;
	Can_HwHandleType            Hoh;
	uint8                                    ControllerId;
}Can_HwType;
/**************************************************************************************************
**
Name:                                     Can_StateTransitionType
Type:                                      Enumeration
Description:  
                    State transitions that are used by the function CAN_SetControllerMode.
                                                                                                                                       **
**************************************************************************************************/	

typedef enum {
	CAN_T_START,
	CAN_T_STOP,
	CAN_T_SLEEP,
	CAN_T_WAKEUP
}Can_StateTransitionType;

/**************************************************************************************************
**
Name:                                     Can_ReturnType
Type:                                      Enumeration
Description:  
                    Return values of CAN driver API .
                                                                                                                                       **
**************************************************************************************************/		
typedef enum {
	CAN_OK,
	CAN_NOT_OK,
	CAN_BUSY
}Can_ReturnType;

#endif /** (CAN_GENERALTYPES_H	) **/