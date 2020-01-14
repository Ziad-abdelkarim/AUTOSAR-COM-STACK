/*******************************************************************************************************************************
FileName:                                        PduR_Types.h
AUTOSAR Version:                                    4.2.2
******************************************************************************************************************************/
#ifndef PDUR_TYPES_H_
#define PDUR_TYPES_H_
/******************************************************************************************************************************
 **                                               Includes                                                                   **
 ******************************************************************************************************************************/
#include "ComStack_Types.h"
/*******************************************************************************************************************************
**                                   		Variables Definitions                                                             **
********************************************************************************************************************************/
#define PDUR_MODULE_ID            (uint16)1
#define PDUR_INSTANCE_ID          (uint8)1
/*******************************************************************************************************************************
**                                   			Type Definitions                                                              **
********************************************************************************************************************************/
/********************************************************************************************************************************

Name:                                PduR_StateType

Type:                                Enumeration

Description:           				 Status of the PDU Router
*********************************************************************************************************************************/
typedef enum{
	/* PDU Router not initialised */
	PDUR_UNINIT,
	/* PDU Router initialized successfully */
	PDUR_ONLINE
}PduR_StateType;
/********************************************************************************************************************************

Name:                                PduRDestPduDataProvision

Type:                                Enumeration

Description:           	Specifies how data are provided: direct (as part of the Transmit call) or via
						the TriggerTransmit callback function. Only required for non-TP gatewayed I-PDUs.
*********************************************************************************************************************************/
typedef enum{
	/* The PDU Router module shall call the transmit function in the
	destination module and not buffer the I-PDU */
	PDUR_DIRECT,
	
	/* The PDU Router module shall call the transmit function in the destination module. The destination
	module will request the I-PDU using the triggerTransmit function. The IPDU is shall be buffered. */
	PDUR_TRIGGERTRANSMIT
}PduRDestPduDataProvision_Type;


/********************************************************************************************************************************

Name:                                PduRDestPdu

Type:                                Structure

Description:           	This container is a subcontainer of PduRRoutingPath and specifies one
						destination for the PDU to be routed.
*********************************************************************************************************************************/
typedef struct{
	/* Specifies how data are provided: direct (as part of the Transmit call) or via
	the TriggerTransmit callback function. Only required for non-TP gatewayed I-PDUs. */
	PduRDestPduDataProvision_Type PduRDestPduDataProvision;
	
	/* PDU identifier assigned by PDU Router. Used by communication interface
	and transport protocol modules for confirmation
	(PduR_<Lo>TxConfirmation) and for TriggerTransmit
	(PduR_<Lo>TriggerTransmit).*/ 
	uint16 PduRDestPduHandleId;
	
	/* This parameter is only for communication interfaces. Transport protocol
	modules will always call the TxConfirmation function.
	If set the destination communication interface module will call the
	TxConfirmation. However the TxConfirmation may be not called due to
	error. So the PduR shall not block until the TxConfirmation is called.
	One background for this parameter is for the PduR to know when all
	modules have confirmed a multicast operation.*/
	boolean PduRTransmissionConfirmation;
}PduRDestPdu_Type;

/********************************************************************************************************************************

Name:                                PduRSrcPdu

Type:                                Structure

Description:           	This container is a subcontainer of PduRRoutingPath and specifies the
						source of the PDU to be routed.
*********************************************************************************************************************************/
typedef struct{
	/* PDU identifier assigned by PDU Router. */
	uint16 PduRSourcePduHandleId;
	
	/* When enabled, the TxConfirmation will be forwarded to the upper layer.
	Prerequisites: Lower layer and upper layer support TxConfirmation. */
	boolean PduRSrcPduUpTxConf;
}PduRSrcPdu_Type;

/********************************************************************************************************************************

Name:                                PduRRoutingPath

Type:                                Structure

Description:           	This container is a subcontainer of PduRRoutingTable and specifies the
						routing path of a PDU.
*********************************************************************************************************************************/
typedef struct{
	/* This container is a subcontainer of PduRRoutingPath and
	specifies one destination for the PDU to be routed. */
	PduRDestPdu_Type PduRDestPdu[1];
	/*This container is a subcontainer of PduRRoutingPath and specifies the
	source of the PDU to be routed. */
	PduRSrcPdu_Type PduRSrcPdu;
}PduRRoutingPath_Type;

/********************************************************************************************************************************

Name:                                PduRRoutingTable

Type:                                Structure

Description:           	Represents one container of routing paths
*********************************************************************************************************************************/
typedef struct{
	PduRRoutingPath_Type PduRRoutingPath[PduRMaxRoutingPathCnt];
}PduRRoutingTable_Type;

/********************************************************************************************************************************

Name:                                PduRRoutingTables

Type:                                Structure

Description:           	Represents one table of routing paths.
						This routing table allows multiple configurations that can be used to create
						several routing tables in the same configuration. This is mainly used for
						post-build (e.g. post-build selectable) but can be used by pre-compile and
						link-time for variant handling.
*********************************************************************************************************************************/
typedef struct{
	PduRRoutingTable_Type PduRRoutingTable;
}PduRRoutingTables_Type;

/********************************************************************************************************************************

Name:                                PduR_PBConfigType

Type:                                Structure

Description:           	Configuration of the PduR (PDU Router) module.
*********************************************************************************************************************************/
typedef struct{
	PduRRoutingTables_Type PduRRoutingTables;
}PduR_PBConfigType;

#endif /* PDUR_TYPES_H_ */