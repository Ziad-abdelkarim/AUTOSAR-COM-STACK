/*******************************************************************************************************************************
FileName:                    							CanIf_Cfg.h                                      
AUTOSAR Version:          								  4.2.2
******************************************************************************************************************************/
#ifndef CANIF_CFG_H
#define CANIF_CFG_H

/**************************************************************************************************
**
Name:                                   CanIfPrivateCfg
Type:                                      Structure
Description:  			This container contains the private configuration (parameters) of the
										CAN Interface.											**
**************************************************************************************************/
/**************************************************************************************************
**
Name:                                   CanIfPrivateDlcCheck

Type:                                    EcucBooleanParamDef

Description: 				Selects whether the DLC check is supported.
									True: Enabled False: Disabled								**
**************************************************************************************************/
#define CanIfPrivateDlcCheck	true
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
#define CanIfPrivateSoftwareFilter	(CanIfPrivateSoftwareFilterType)LINEAR
/***************************************************************************************************/


/**************************************************************************************************
**
Name:                                   CanIfPublicCfg
Type:                                      Structure
Description:  			This container contains the public configuration (parameters) of the
										CAN Interface.											**
**************************************************************************************************/
/**************************************************************************************************
**
Name:                                   CanIfPublicDevErrorDetect

Type:                                    EcucBooleanParamDef

Description: 				Switches the Default Error Tracer (Det) detection and notification ON or OFF.
										• true: enabled (ON).
										• false: disabled (OFF).								**
**************************************************************************************************/
#define CanIfPublicDevErrorDetect false
/**************************************************************************************************
**
Name:                                   CanIfPublicHandleTypeEnum

Type:                                    EcucEnumerationParamDef

Description: 				This parameter is used to configure the Can_HwHandleType. The
							Can_HwHandleType represents the hardware object handles of a CAN
							hardware unit. For CAN hardware units with more than 255 HW objects
							the extended range shall be used (UINT16).							**
**************************************************************************************************/
#define CanIfPublicHandleTypeEnum	uint8
/**************************************************************************************************
**
Name:                                 CanIfPublicReadRxPduDataApi

Type:                                    EcucBooleanParamDef

Description: 				Enables / Disables the API CanIf_ReadRxPduData() for reading
										received L-SDU data.
									True: Enabled False: Disabled								**
**************************************************************************************************/
#define CanIfPublicReadRxPduDataApi	true
/**************************************************************************************************
**
Name:                              CanIfPublicReadRxPduNotifyStatusApi

Type:                                    EcucBooleanParamDef

Description: 				Enables and disables the API for reading the notification status of
											receive L-PDUs.
									True: Enabled False: Disabled								**
**************************************************************************************************/
#define CanIfPublicReadRxPduNotifyStatusApi	true
/**************************************************************************************************
**
Name:                              CanIfPublicReadTxPduNotifyStatusApi

Type:                                    EcucBooleanParamDef

Description: 				Enables and disables the API for reading the notification status of
											transmit L-PDUs.
									True: Enabled False: Disabled								**
**************************************************************************************************/
#define CanIfPublicReadTxPduNotifyStatusApi	true
/**************************************************************************************************
**
Name:                              		CanIfPublicTxBuffering

Type:                                    EcucBooleanParamDef

Description: 				Enables and disables the buffering of transmit L-PDUs (rejected by the
								CanDrv) within the CAN Interface module.	
									True: Enabled False: Disabled								**
**************************************************************************************************/
#define CanIfPublicTxBuffering	true
/**************************************************************************************************
**
Name:                              CanIfPublicTxConfirmPollingSupport

Type:                                    EcucBooleanParamDef

Description: 				Configuration parameter to enable/disable the API to poll for Tx
										Confirmation state.										**
**************************************************************************************************/
#define CanIfPublicTxConfirmPollingSupport false
/**************************************************************************************************
**
Name:                              		CanIfSetBaudrateApi

Type:                                   EcucBooleanParamDef

Description: 				Configuration parameter to enable/disable the CanIf_SetBaudrate API
							to change the baud rate of a CAN Controller. If this parameter is set to
							true the CanIf_SetBaudrate API shall be supported. Otherwise the API
							is not supported.													**
**************************************************************************************************/
#define CanIfSetBaudrateApi true
/*************************************************************************************************/

/**************************************************************************************************
**
Name:                                     CanIfInitCfg
Type:                                      Structure
Description:  			This container contains the init parameters of the CAN Interface.		**
**************************************************************************************************/
/**************************************************************************************************
**
Name:                                   CanIfMaxBufferSize

Type:                                   EcucIntegerParamDef

Description:  			Maximum total size of all Tx buffers. This parameter is needed only in
						case of post-build loadable implementation using static memory
											allocation.											**
**************************************************************************************************/
#define CanIfMaxBufferSize	(uint32)10
/**************************************************************************************************
**
Name:                                     CanIfMaxRxPduCfg

Type:                                   EcucIntegerParamDef

Description:  			Maximum number of Pdus. This parameter is needed only in case of
						post-build loadable implementation using static memory allocation.		**
**************************************************************************************************/
#define CanIfMaxRxPduCfg	(uint32)1
/**************************************************************************************************
**
Name:                                    CanIfMaxTxPduCfg

Type:                                   EcucIntegerParamDef

Description:  			Maximum number of Pdus. This parameter is needed only in case of
						post-build loadable implementation using static memory allocation.		**
**************************************************************************************************/
#define CanIfMaxTxPduCfg	(uint32)1

#endif /*CANIF_CFG_H*/
