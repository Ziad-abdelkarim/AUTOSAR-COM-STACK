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
**                                               				    Variables Definitions                                                                    **
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
typedef struct{
	/* Configuration parameters for all the underlying CAN
	Driver modules are aggregated under this container.
	For each CAN Driver module a seperate instance of
	this container has to be provided. */
	CanIfCtrlDrvCfg CanIfCtrlDrvCfg;
	
	/* This container contains the init parameters of the CAN
	Interface. */
	CanIfInitCfg CanIfInitCfg;
	
}CanIf_ConfigType;

/**************************************************************************************************
**
Name:                                     CanIfInitCfg
Type:                                      Structure
Description:  			This container contains the init parameters of the CAN Interface.		**
**************************************************************************************************/
typedef struct
{
	/* This container contains the Txbuffer configuration.
	Multiple buffers with different sizes could be configured.
	If CanIfBufferSize (ECUC_CanIf_00834) equals 0, the
	CanIf Tx L-PDU only refers via this CanIfBufferCfg the
	corresponding CanIfHthCfg. */
	CanIfBufferCfg	CanIfBufferCfg;
	
	/* This container contains the references to the
	configuration setup of each underlying CAN Driver. */
	CanIfInitHohCfg	CanIfInitHohCfg[NUMBER_OF_CANDRIVERS];
	
	/* This container contains the configuration (parameters)
	of each receive CAN L-PDU.
	The SHORT-NAME of "CanIfRxPduConfig" container
	itself represents the symolic name of Receive L-PDU. */
	CanIfRxPduCfg	CanIfRxPduCfg[CanIfMaxRxPduCfg];
	
	/* This container contains the configuration (parameters)
	of a transmit CAN L-PDU. It has to be configured as
	often as a transmit CAN L-PDU is needed.
	The SHORT-NAME of "CanIfTxPduConfig" container
	represents the symolic name of Transmit L-PDU. */
	CanIfTxPduCfg	CanIfTxPduCfg[CanIfMaxTxPduCfg];
	
}CanIfInitCfg;

/**************************************************************************************************
**
Name:                                	  CanIfCtrlDrvCfg
Type:                                 		Structure
Description:  			Configuration parameters for all the underlying CAN Driver modules
						are aggregated under this container. For each CAN Driver module a
						seperate instance of this container has to be provided.											**
**************************************************************************************************/
typedef struct{
	/* Reference to the Init Hoh Configuration */
	CanIfInitHohCfg* CanIfCtrlDrvInitHohConfigRef;
	
	/* This container contains the configuration (parameters)
	of an adressed CAN controller by an underlying CAN
	Driver module. This container is configurable per CAN
	controller. */
	CanIfCtrlCfg CanIfCtrlCfg[NUMBER_OF_CONTROLLERS];
	
}CanIfCtrlDrvCfg;

/**************************************************************************************************
**
Name:                                	  CanIfCtrlCfg
Type:                                      Structure
Description:  			This container contains the configuration (parameters) of an adressed
						CAN controller by an underlying CAN Driver module. This container is
						configurable per CAN controller											**
**************************************************************************************************/
typedef struct
{
	/* This parameter references to the logical handle of the underlying CAN
	controller from the CAN Driver module to be served by the CAN
	Interface module. The following parameters of CanController config
	container shall be referenced by this link: CanControllerId,
	CanWakeupSourceRef
	Range: 0..max. number of underlying supported CAN controllers */
	CanController *CanIfCtrlCanCtrlRef;
	
	/* This parameter abstracts from the CAN Driver specific parameter
	Controller. Each controller of all connected CAN Driver modules shall
	be assigned to one specific ControllerId of the CanIf. Range:
	0..number of configured controllers of all CAN Driver modules */
	uint8 CanIfCtrlId;
	
	/* This parameter defines if a respective controller of the referenced CAN
	Driver modules is queriable for wake up events.
	True: Enabled False: Disabled */
	bool CanIfCtrlWakeupSupport;
	
}CanIfCtrlCfg;

/**************************************************************************************************
**
Name:                                     CanIfTxPduCfg
Type:                                      Structure
Description:  			This container contains the configuration (parameters) of a transmit
						CAN L-PDU. It has to be configured as often as a transmit CAN L-PDU is needed.
						The SHORT-NAME of "CanIfTxPduConfig" container represents the
						symolic name of Transmit L-PDU.											**
**************************************************************************************************/
typedef struct{
	/* Configurable reference to a CanIf buffer configuration. */
	CanIfBufferCfg* CanIfTxPduBufferRef;
	
	/* CAN Identifier of transmit CAN L-PDUs used by the CAN Driver for
	CAN L-PDU transmission. Range: 11 Bit For Standard CAN Identifier
	... 29 Bit For Extended CAN identifier
	The CAN Identifier may be omitted for dynamic transmit L-PDUs. */
	uint32 CanIfTxPduCanId;
	
	/* Type of CAN Identifier of the transmit CAN L-PDU used by the CAN
	Driver module for CAN L-PDU transmission. */
	CanIfTxPduCanIdType CanIfTxPduCanIdType;
	
	/* ECU wide unique, symbolic handle for transmit CAN L-SDU.
	Range: 0..max. number of CantTxPduIds */
	uint32 CanIfTxPduId;
	
	/* Enables and disables transmit confirmation for each transmit CAN
	L-SDU for reading its notification status.
	True: Enabled False: Disabled */
	boolean CanIfTxPduReadNotifyStatus;
	
	/* Reference to the "global" Pdu structure to allow harmonization of
	handle IDs in the COM-Stack. */
	/*CanHardwareObject* CanIfTxPduRef;*/
	
	/* Defines the type of each transmit CAN L-PDU. */
	CanIfTxPduType	CanIfTxPduType;
	
	/* This parameter defines the upper layer (UL) module to which the
	confirmation of the successfully transmitted CANTXPDUID has to be
	routed via the <User_TxConfirmation>. This <User_TxConfirmation>
	has to be invoked when the confirmation of the configured
	CANTXPDUID will be received by a Tx confirmation event from the
	CAN Driver module. If no upper layer (UL) module is configured, no
	<User_TxConfirmation> has to be called in case of a Tx confirmation
	event of the CANTXPDUID from the CAN Driver module. */
	CanIfTxPduUserTxConfirmationUL	CanIfTxPduUserTxConfirmationUL;
	
}CanIfTxPduCfg;


/**************************************************************************************************
**
Name:                             CanIfTxPduUserTxConfirmationUL
Type:                                EcucEnumerationParamDef
Description:  			This parameter defines the upper layer (UL) module to which the
						confirmation of the successfully transmitted CANTXPDUID has to be
						routed via the <User_TxConfirmation>. This <User_TxConfirmation>
						has to be invoked when the confirmation of the configured
						CANTXPDUID will be received by a Tx confirmation event from the
						CAN Driver module. If no upper layer (UL) module is configured, no
						<User_TxConfirmation> has to be called in case of a Tx confirmation
						event of the CANTXPDUID from the CAN Driver module.						**
**************************************************************************************************/
typedef enum{
	CAN_NM,
	CAN_TP,
	CAN_TSYN,
	CDD,
	J1939NM,
	J1939TP,
	PDUR,
	XCP
}CanIfTxPduUserTxConfirmationUL;

/**************************************************************************************************
**
Name:                                  CanIfTxPduType
Type:                                EcucEnumerationParamDef
Description:  			Defines the type of each transmit CAN L-PDU.							**
**************************************************************************************************/
typedef enum{
	/* CAN ID is defined at runtime */
	DYNAMIC,
	/* CAN ID is defined at compile-time. */
	STATIC
}CanIfTxPduType;
/**************************************************************************************************
**
Name:                                  CanIfTxPduCanIdType
Type:                                EcucEnumerationParamDef
Description:  			Type of CAN Identifier of the transmit CAN L-PDU used by the CAN
								Driver module for CAN L-PDU transmission.						**
**************************************************************************************************/
typedef enum{
	/* CAN frame with extended identifier (29bits) */
	EXTENDED_CAN,
	/* CAN FD frame with extended identifier (29 bits) */
	EXTENDED_FD_CAN,
	/* CAN frame with standard identifier (11 bits) */
	STANDARD_CAN,
	/* CAN FD frame with standard identifier (11 bits) */
	STANDARD_FD_CAN
}CanIfTxPduCanIdType;


/**************************************************************************************************
**
Name:                                     CanIfRxPduCfg
Type:                                      Structure
Description:  			This container contains the configuration (parameters) of each receive
						CAN L-PDU.
						The SHORT-NAME of "CanIfRxPduConfig" container itself represents
						the symolic name of Receive L-PDU.											**
**************************************************************************************************/
typedef struct{
	/* CAN Identifier of Receive CAN L-PDUs used by the CAN Interface.
	Exa: Software Filtering. This parameter is used if exactly one Can
	Identifier is assigned to the Pdu. If a range is assigned then the
	CanIfRxPduCanIdRange parameter shall be used.
	Range: 11 Bit For Standard CAN Identifier ... 29 Bit For Extended CAN
	identifier */
	uint32 CanIfRxPduCanId;
	
	/* Identifier mask which denotes relevant bits in the CAN Identifier. This
	parameter defines a CAN Identifier range in an alternative way to
	CanIfRxPduCanIdRange. It identifies the bits of the configured CAN
	Identifier that must match the received CAN Identifier. Range: 11 bits
	for Standard CAN Identifier, 29 bits for Extended CAN Identifier. */
	uint32 CanIfRxPduCanIdMask;
	
	/* CAN Identifier of receive CAN L-PDUs used by the CAN Driver for CAN
	L-PDU reception. */
	CanIfRxPduCanIdType CanIfRxPduCanIdType;
	
	/* Data length of the received CAN L-PDUs used by the CAN Interface.
	This information is used for DLC checking. Additionally it might specify
	the valid bits in case of the discrete DLC for CAN FD L-PDUs > 8 bytes.
	The data area size of a CAN L-PDU can have a range from 0 to 64
	bytes. */
	uint8 CanIfRxPduDlc;
	
	/* The HRH to which Rx L-PDU belongs to, is referred through this
	parameter. */
	CanIfHrhCfg *CanIfRxPduHrhIdRef;
	
	/* ECU wide unique, symbolic handle for receive CAN L-SDU. It shall
	fulfill ANSI/AUTOSAR definitions for constant defines.
	Range: 0..max. number of defined CanRxPduIds */
	uint32 CanIfRxPduId;
	
	/* Enables and disables the Rx buffering for reading of received L-SDU data.
	True: Enabled False: Disabled */
	boolean CanIfRxPduReadData;
	
	/* Enables and disables receive indication for each receive CAN L-SDU
	for reading its notification status.
	True: Enabled False: Disabled */
	boolean CanIfRxPduReadNotifyStatus;
	
	/* Reference to the "global" Pdu structure to allow harmonization of
	handle IDs in the COM-Stack */
	/*CanHardwareObject* CanIfRxPduRef;*/
	
	/* This parameter defines the upper layer (UL) module to which the
	indication of the successfully received CANRXPDUID has to be routed
	via <User_RxIndication>. This <User_RxIndication> has to be invoked
	when the indication of the configured CANRXPDUID will be received
	by an Rx indication event from the CAN Driver module. If no upper
	layer (UL) module is configured, no <User_RxIndication> has to be
	called in case of an Rx indication event of the CANRXPDUID from the
	CAN Driver module. */
	CanIfRxPduUserRxIndicationUL CanIfRxPduUserRxIndicationUL ;
	
}CanIfRxPduCfg;

/**************************************************************************************************
**
Name:                              CanIfRxPduUserRxIndicationUL
Type:                                 EcucEnumerationParamDef
Description:  			This parameter defines the upper layer (UL) module to which the
						indication of the successfully received CANRXPDUID has to be routed
						via <User_RxIndication>. This <User_RxIndication> has to be invoked
						when the indication of the configured CANRXPDUID will be received
						by an Rx indication event from the CAN Driver module. If no upper
						layer (UL) module is configured, no <User_RxIndication> has to be
						called in case of an Rx indication event of the CANRXPDUID from the
						CAN Driver module.														**
**************************************************************************************************/
typedef enum{
	CAN_NM,
	CAN_TP,
	CAN_TSYN,
	CDD,
	J1939NM,
	J1939TP,
	PDUR,
	XCP
}CanIfRxPduUserRxIndicationUL;

/**************************************************************************************************
**
Name:                                	CanIfRxPduCanIdType
Type:                                 EcucEnumerationParamDef
Description:  			CAN Identifier of receive CAN L-PDUs used by the CAN Driver for CAN
									L-PDU reception.											**
**************************************************************************************************/
typedef enum{
	/* CAN 2.0 or CAN FD frame with extended identifier (29 bits) */
	EXTENDED_CAN,
	/* CAN FD frame with extended identifier (29 bits) */
	EXTENDED_FD_CAN,
	/* CAN 2.0 frame with extended identifier (29 bits) */
	EXTENDED_NO_FD_CAN,
	/* CAN 2.0 or CAN FD frame with standard identifier (11 bits) */
	STANDARD_CAN,
	/* CAN FD frame with standard identifier (11 bits) */
	STANDARD_FD_CAN,
	/* CAN 2.0 frame with standard identifier (11 bits) */
	STANDARD_NO_FD_CAN
}CanIfRxPduCanIdType;

/**************************************************************************************************
**
Name:                                	  CanIfInitHohCfg
Type:                                 		Structure
Description:  			This container contains the references to the configuration setup of
										each underlying CAN Driver.											**
**************************************************************************************************/
typedef struct{
	/* This container contains configuration parameters for
	each hardware receive object (HRH) */
	CanIfHrhCfg CanIfHrhCfg[1U];
	
	/* This container contains parameters related to each HTH. */
	CanIfHthCfg CanIfHthCfg[1U];
	
}CanIfInitHohCfg;

/**************************************************************************************************
**
Name:                                	  CanIfHthCfg
Type:                                      Structure
Description:  			This container contains parameters related to each HTH.					**
**************************************************************************************************/
typedef struct{
	/* Reference to controller Id to which the HTH belongs to. A controller
	can contain one or more HTHs. */
	CanIfCtrlCfg* CanIfHthCanCtrlIdRef;
	
	/* The parameter refers to a particular HTH object in the CanDrv
	configuration */
	CanHardwareObject* CanIfHthIdSymRef;
	
}CanIfHthCfg;

/**************************************************************************************************
**
Name:                                	  CanIfHrhCfg
Type:                                      Structure
Description:  			This container contains configuration parameters for each hardware
										receive object (HRH).									**
**************************************************************************************************/
typedef struct{
	/* Reference to controller Id to which the HRH belongs to. A controller
	can contain one or more HRHs. */
	CanIfCtrlCfg* CanIfHrhCanCtrlIdRef;
	
	/* The parameter refers to a particular HRH object in the CanDrv
	configuration */
	CanHardwareObject* CanIfHrhIdSymRef;
	
	/* The parameter refers to a particular HRH object in the CanDrv
	configuration */
	bool CanIfHrhSoftwareFilter;
	
}CanIfHrhCfg;

/**************************************************************************************************
**
Name:                                	 CanIfBufferCfg
Type:                                      Structure
Description:  			This container contains the Txbuffer configuration. Multiple buffers with
						different sizes could be configured. If CanIfBufferSize
						(ECUC_CanIf_00834) equals 0, the CanIf Tx L-PDU only refers via this
						CanIfBufferCfg the corresponding CanIfHthCfg.											**
**************************************************************************************************/
typedef struct{
	/* Reference to HTH, that defines the hardware object or the pool of
	hardware objects configured for transmission. All the CanIf Tx L-PDUs
	refer via the CanIfBufferCfg and this parameter to the HTHs if
	TxBuffering is enabled, or not.
	Each HTH shall not be assigned to more than one buffer.*/
	CanIfHthCfg* CanIfBufferHthRef[CanIfMaxTxPduCfg];
	
	/* This parameter defines the number of CanIf Tx L-PDUs which can be
	buffered in one Txbuffer. If this value equals 0, the CanIf does not
	perform Txbuffering for the CanIf Tx L-PDUs which are assigned to this
	Txbuffer. If CanIfPublicTxBuffering equals False, this parameter equals
	0 for all TxBuffer. If the CanHandleType of the referred HTH equals
	FULL, this parameter equals 0 for this TxBuffer. */
	uint8 CanIfBufferSize;
	
}CanIfBufferCfg;

/**************************************************************************************************
**
Name:                                CanIf_ControllerModeType
Type:                                      Enumeration
Description:  
                     Operating modes of a CAN controller.
																								**
**************************************************************************************************/


typedef enum {
    CANIF_CS_SLEEP,
    CANIF_CS_STARTED,
    CANIF_CS_STOPPED,
    CANIF_CS_UNINIT
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
    CANIF_ONLINE
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
    CANIF_NO_NOTIFICATION
}CanIf_NotifStatusType;

/**************************************************************************************************
**
Name:                                CanIfPrivateSoftwareFilterType

Type:                                    EcucEnumerationParamDef

Description: 				Selects the desired software filter mechanism for reception only.
							Each implemented software filtering method is identified by this enumeration number.

Range: 								Types implemented software filtering methods
									BINARY Selects Binary Filter method.
									INDEX Selects Index Filter method.
									LINEAR Selects Linear Filter method.
									TABLE Selects Table Filter method.							**
**************************************************************************************************/
typedef enum{
	BINARY,
	INDEX,
	LINEAR,
	TABLE
}CanIfPrivateSoftwareFilterType;

#endif /*CANIF_TYPES_H*/