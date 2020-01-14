/*******************************************************************************************************************************
FileName:                                        Com_Types.h
AUTOSAR Version:                                    4.2.2
******************************************************************************************************************************/

#ifndef COM_TYPES_H_
#define COM_TYPES_H_

/******************************************************************************************************************************
 **                                               Includes                                                                   **
 ******************************************************************************************************************************/
#include "ComStack_Types.h"

/*******************************************************************************************************************************
**                                   Variables Definitions                                                                    **
********************************************************************************************************************************/
#define COM_MODULE_ID            (uint16)1
#define COM_INSTANCE_ID          (uint8)1
/*********************************************************************************************************************************
**                                                                  Development Errors                                                                           **
*********************************************************************************************************************************/
/* [SWS_Com_00803] ⌈API service
called with wrong parameter⌋(SRS_BSW_00337) */
#define COM_E_PARAM (uint8)0x01

/* [SWS_Com_00804] ⌈Error code if
any other API service, except
Com_GetStatus, is called before the
AUTOSAR COM module was initialized with Com_Init or after a call to
Com_Deinit⌋(SRS_BSW_00337) */
#define COM_E_UNINIT (uint8)0x02

/* [SWS_Com_00805] ⌈NULL pointer
checking⌋(SRS_BSW_00414) */
#define COM_E_PARAM_POINTER (uint8)0x03

/* [SWS_Com_00837] ⌈Invalid configuration set selection (SRS_BSW_00414) */
#define COM_E_INIT_FAILED   (uint8)0x04

/* [SWS_Com_00864] ⌈Transmission request was skipped (SRS_BSW_00452) */
#define COM_E_SKIPPED_TRANSMISSION  (uint8)0x05


/*******************************************************************************************************************************
**                                   Type Definitions                                                                    **
********************************************************************************************************************************/

/*The AUTOSAR COM module's signal object identifier.*/
typedef uint16 Com_SignalIdType;

/*The AUTOSAR COM module's signal group object identifier.*/
typedef uint16 Com_SignalGroupIdType;

/********************************************************************************************************************************

Name:                                Com_StatusType

Type:                                EcucEnumerationParamDef

Description:          This is a status value returned by the API service Com_GetStatus().

*********************************************************************************************************************************/
typedef enum{

    /*The AUTOSAR COM module is not initialized or not usable. This shall be the default value after reset.
     *This status shall have the value 0.*/
    COM_UNINIT,

    /*The AUTOSAR COM module is initialized and usable.*/
    COM_READY
}Com_StateType;


/********************************************************************************************************************************

Name:                                ComDataInvalidAction

Type:                                EcucEnumerationParamDef

Description:          This parameter defines the action performed upon reception of an invalid signal.
                      Relating to signal groups the action in case if one of the included signals is an
                      invalid signal. If Replace is used the ComSignalInitValue will be used for the replacement

*********************************************************************************************************************************/

typedef enum {
    NOTIFY,
    REPLACE
}Com_DataInvalidActionType;


/********************************************************************************************************************************

Name:                                ComSignalEndianness

Type:                                EcucEnumerationParamDef

Description:          Defines the endianness of the signal's network representation.

*********************************************************************************************************************************/
typedef enum{
    BIG_ENDIAN,
    LITTLE_ENDIAN,
    OPAQUE
}Com_SignalEndiannessType;


/********************************************************************************************************************************

Name:                                ComSignalType

Type:                                EcucEnumerationParamDef

Description:        The AUTOSAR type of the signal. Whether or not the signal is signed or unsigned can be found by examining
                    the value of this attribute.This type could also be used to reserved appropriate storage in AUTOSAR COM.

*********************************************************************************************************************************/
typedef enum{
    BOOLEAN,
    FLOAT32,
    FLOAT64,
    SINT16,
    SINT32,
    SINT64,
    SINT8,
    UINT16,
    UINT32,
    UINT64,
    UINT8
}Com_SignalTypeType;


/********************************************************************************************************************************

Name:                                ComTransferProperty

Type:                                EcucEnumerationParamDef

Description:           Defines if a write access to this signal can trigger the transmission of the corresponding I-PDU.
                       If the I-PDU is triggered, depends also on the transmission mode of the corresponding I-PDU.
*********************************************************************************************************************************/
typedef enum{
    /*A write access to this signal never triggers the transmission of the cor-responding I-PDU.*/
    PENDING,

    /*Depending on the transmission mode, a write access to this signal can trigger the transmission of the corresponding I-PDU.*/
    TRIGGERED,

    /*Depending on the transmission mode, a write access to this signal can trigger the transmission of the corresponding I-PDU,
     *but only in case the written value is different to the locally stored (last sent or initial value) in length or value.*/

    TRIGGERED_ON_CHANGE,
    /*Depending on the transmission mode, a write access to this signal can trigger the transmission of the corresponding I-PDU
     *just once without a repetition, but only in case the written value is different to the locally stored (last sent or initial value)
     *in length or value.*/
    TRIGGERED_ON_CHANGE_WITHOUT_REPETITION,

    /*Depending on the transmission mode, a write access to this signal can trigger the transmission of the corresponding I-PDU
     *just once without a repetition.*/
    TRIGGERED_WITHOUT_REPETITION

}Com_TransferPropertyType;


/********************************************************************************************************************************

Name:                                ComTxModeMode

Type:                                EcucEnumerationParamDef

Description:           The transmission mode None shall not have any further sub-attributes in the ComTxMode object.

Range:          DIRECT:    0x00
                MIXED:     0x01
                NONE:      0x02: Literal for TxMode.
                PERIODIC:  0x03
*********************************************************************************************************************************/
typedef enum{
    DIRECT,
    MIXED,
    NONE,
    PERIODIC
}Com_TxModeModeType;


/********************************************************************************************************************************

Name:                                ComTxIPduClearUpdateBit

Type:                                EcucEnumerationParamDef

Description:    Defines when the update-bits of signals or signal groups, contained in this I-PDU, will be cleared.

Range:          Confirmation:      The update-bits are cleared when the transmission of the I-PDU was confirmed.
                                   In case of Direct/N-Times transmission mode the update bits will be cleared with respect to
                                   the confirmation behavior of ECUC_Com_00305.
                Transmit:          The update-bits are cleared directly after the invocation of PduR_ComTransmit.
                TriggerTransmit:   The update-bits are cleared after the I-PDU was fetched via Com_TriggerTransmit.
*********************************************************************************************************************************/
typedef enum {
    Confirmation,
    Transmit,
    TriggerTransmit
}Com_TxIPduClearUpdateBitType;


/********************************************************************************************************************************

Name:                                ComIPduDirection

Type:                                EcucEnumerationParamDef

Description:    The direction defines if this I-PDU, and therefore the contributing signals and signal groups,shall be sent or received.
*********************************************************************************************************************************/
typedef enum{
    RECEIVE,
    SEND
}Com_IPduDirectionType;

/********************************************************************************************************************************

Name:                                ComIPduSignalProcessing

Type:                                EcucEnumerationParamDef

Description:             For the definition of the two modes Immediate and Deferred.

Range:            DEFERRED:         signal indication / confirmations are defer-red for example to a cyclic task
                  IMMEDIATE:        the signal indications / confirmations are performed in Com_RxIndication/ Com_TxConfirmation
*********************************************************************************************************************************/
typedef enum{
    DEFERRED,
    IMMEDIATE
}Com_IPduSignalProcessingType;


/********************************************************************************************************************************

Name:                                ComIPduType

Type:                                EcucEnumerationParamDef

Description:             Defines if this I-PDU is a normal I-PDU that can be sent unfragmented or if this is a large I-PDU
                         that shall be sent via the Transport Protocol of the underlying bus.

Range:            NORMAL:         sent or received via normal L-PDU
                  TP:             sent or received via TP
*********************************************************************************************************************************/
typedef enum{
    NORMAL,
    TP
}Com_IPduTypeType;



/********************************************************************************************************************************

Name:                                ComTxMode

Type:                                Structure

Description:       This container contains the configuration parameters of the AUTOSAR COM module's transmission modes.
*********************************************************************************************************************************/
typedef struct{

   /* The available transmission modes. */
   Com_TxModeModeType  ComTxModeMode;

   /* Defines the number of repetitions for the transmission mode DIRECT and
    the event driven part of transmission mode MIXED */
   uint8 ComTxModeNumberOfRepetitions;

   /* Defines the repetition period in seconds of the multiple transmissions in
    case ComTxModeNumberOfRepetitions is configured greater than or
    equal to 1 and ComTxModeMode is configured to DIRECT or MIXED. In
    case of the mixed transmission mode only the event driven part is affected. */
   float32 ComTxModeRepetitionPeriod ;

   /* Defines the repetition period in seconds of the periodic transmission requests in case ComTxModeMode is configured to PERIODIC or MIXED. In
    case of the mixed transmission mode only the periodic part is affected. */
   float32 ComTxModeTimePeriod ;

}Com_TxModeType;



/********************************************************************************************************************************

Name:                                ComTxModeFalse

Type:                                Structure

Description:       This container contains the configuration parameters of the AUTOSAR COM module's transmission modes in the case
                   the ComFilter evaluates to false.
*********************************************************************************************************************************/
typedef struct{
    /* This container contains the configuration parameters of the
    AUTOSAR COM module's transmission modes. */
    Com_TxModeType ComTxMode;
}Com_TxModeFalseType;


/********************************************************************************************************************************

Name:                                ComTxModeTrue

Type:                                Structure

Description:       This container contains the configuration parameters of the AUTOSAR COM module's transmission modes in the case
                   the ComFilter evaluates to true..
*********************************************************************************************************************************/
typedef struct{
    /* This container contains the configuration parameters of the
    AUTOSAR COM module's transmission modes. */
    Com_TxModeType ComTxMode;
}Com_TxModeTrueType;


/********************************************************************************************************************************

Name:                                ComTxIPdu

Type:                                Structure

Description:    This container contains additional transmission related configuration parameters of the AUTOSAR COM module's I-PDUs.
*********************************************************************************************************************************/
typedef struct{

    /* Defines the Minimum Delay Time (MDT) between successive transmissions of this I-PDU in seconds. The MDT is independent of the possible
    different transmission modes. There is only one minimum delay time parameter for one I-PDU. The minimum delay timer is not reset by changing
    the transmission mode. Hence, it is not allowed to violate the minimum
    delay time by transmission mode changes. It is not possible to monitor the
    minimum delay time for I-PDUs that are requested using the
    Com_TriggerTransmit API. */
    float32 ComMinimumDelayTime;

    /* Defines when the update-bits of signals or signal groups, contained in this I-PDU,
    will be cleared. */
    Com_TxIPduClearUpdateBitType ComTxIPduClearUpdateBit;

    /*The AUTOSAR COM module fills not used areas of an I-PDU with this byte
    pattern. This attribute is mandatory to avoid undefined behaviour. This
    byte-pattern will be repeated throughout the I-PDU before any init-values
    or update-bits were set.*/
    uint8 ComTxIPduUnusedAreasDefault;

    /*The referenced transmission mode object that is used when the filtering state for this I-PDU evaluates to false.
     *  The default is transmission mode None.*/
    Com_TxModeFalseType ComTxModeFalse;

    /*The referenced transmission mode object that is used when the filtering state for this I-PDU evaluates to true.*/
    Com_TxModeTrueType ComTxModeTrue;

}Com_TxIPduType;




/********************************************************************************************************************************

Name:                                ComGroupSignal

Type:                                Structure

Description:          This container contains the configuration parameters of group signals.
                      I.e. signals that are included within a signal group.

*********************************************************************************************************************************/
typedef struct{
    /*Starting position within the I-PDU. This parameter refers to the position in the I-PDU and not in the shadow buffer.
     *If the endianness conversion is configured to Opaque the parameter ComBitPosition shall define the bit0 of the first byte
     *like in little endian byte order*/
    uint32 ComBitPosition;

    /*Size in bits, for integer signal types. For ComSignalType UINT8_N and UINT8_DYN, the size shall be configured by ComSignalLength.
     *For ComSignalTypes FLOAT32 and FLOAT64 the size is already defined by the signal type and therefore may be omitted.*/
    /*Range: from 0 to 64*/
    uint8  ComBitSize;

    /*The numerical value used as the ID. For signals it is required by the API calls Com_UpdateShadowSignal,
     *Com_ReceiveShadowSignal and Com_InvalidateShadowSignal. For sig-nals groups it is required by the Com_SendSignalGroup
     *and Com_ReceiveSignalGroup calls.*/
    uint16  ComHandleId;

    /* Defines the endianness of the signal's network representation. */
    Com_SignalEndiannessType ComSignalEndianness;

    /*For ComSignalType UINT8_N this parameter specifies the length n in bytes. For Type UINT8_DYN it specifies the maximum
     * length in bytes. For all other types this parameter shall be ignored.
     *Range: 0..8 for normal CAN/ LIN I-PDUs*/
    uint8 ComSignalLength;

    /* The AUTOSAR type of the signal. Whether or not the signal is signed or unsigned
    can be found by examining the value of this attribute.
    This type could also be used to reserved appropriate storage in AUTOSAR COM. */
    Com_SignalTypeType ComSignalType;

    /* Optionally defines whether this group signal shall contribute to the TRIGGERED_ON_CHANGE transfer property of the signal group. If at least one group signal
    of a signal group has the "ComTransferProperty" configured all other group signals
    of that signal group shall have the attribute configured as well. */
    Com_TransferPropertyType ComTransferProperty;

    /* Reference to the group signal buffer */
    /* Note: Autosar extension */
    uint8* ComBufferRef;


}Com_GroupSignalType;



/********************************************************************************************************************************

Name:                                ComSignalGroup

Type:                                Structure

Description:           Contains the configuration parameters of the AUTOSAR COM module's signal groups.

*********************************************************************************************************************************/
typedef struct{

    /*Only valid on sender side: Name of Com_CbkTxErr callback function to be called.
     If this parameter is omitted no error notification shall take place.*/
    /* void (* ComErrorNotification )(void); */

    /*The numerical value used as the ID.For signals it is required by the API calls Com_UpdateShadowSignal,
     *Com_ReceiveShadowSignal and Com_InvalidateShadowSignal. For sig-nals groups it is required by the Com_SendSignalGroup
     *and Com_ReceiveSignalGroup calls.*/
    uint16 ComHandleId ;

    /*On sender side: Name of Com_CbkTxAck callback function to be called.
     *On receiver side: Name of Com_CbkRxAck callback function to be called.
     *If this parameter is omitted no notification shall take place.*/
    void (* ComNotification )(void);

    /* Defines if a write access to this signal can trigger the transmission of the corresponding I-PDU. If the I-PDU is triggered, depends also on the transmission mode of the
    corresponding I-PDU. */
    Com_TransferPropertyType ComTransferProperty;

    /*Bit position of update-bit inside I-PDU.If this attribute is omitted then there is no update-bit. This setting must be
     *consistently on sender and on receiver side.
     *Range: from 0 to 63 for CAN and LIN*/
     uint8 ComUpdateBitPosition;

    /*This parameter defines that the respective signal's initial value shall be put into the respective PDU but there
     *will not be any update of the value through the RTE. Thus the Com implementation does not need to expect any API calls
     *for this signal (group).*/
     /* boolean ComInitialValueOnly; */

    /* References to all group signals contained in this signal group */
    Com_GroupSignalType* ComGroupSignalRef[ComMaxGroupSignalCnt];

}Com_SignalGroupType;


/********************************************************************************************************************************

Name:                                ComSignal

Type:                                Structure

Description:          Contains the configuration parameters of the AUTOSAR COM module's signals.

*********************************************************************************************************************************/
typedef struct{

    /*Starting position within the I-PDU. This parameter refers to the position in the I-PDU and not in the shadow buffer.
     *If the endianness conversion is configured to Opaque the parameter ComBitPosition shall define the bit0 of the first byte
     *like in little endian byte order*/
    uint8 ComBitPosition;

    /*Size in bits, for integer signal types. For ComSignalType UINT8_N and UINT8_DYN the size shall be configured by ComSignalLength.
     * For ComSignalTypes FLOAT32 and FLOAT64 the size is already defined by the signal type and therefore may be omitted.*/
    uint8 ComBitSize;

    /*Only valid on sender side: Name of Com_CbkTxErr callback function to be called.
     * If this parameter is omitted no error notification shall take place.*/
    // void (* ComErrorNotification )(void);

    /*The numerical value used as the ID.This ID identifies signals and signal groups in the COM APIs using Com_SignalIdType
     *or Com_SignalGroupIdType parameter respectively.*/
    uint16 ComHandleId;

    /*On sender side: Name of Com_CbkTxAck callback function to be called.
     *On receiver side: Name of Com_CbkRxAck callback function to be called.
     *If this parameter is omitted no notification shall take place.*/
    void (* ComNotification) (void);

    /* Defines the endianness of the signal's network representation. */
    Com_SignalEndiannessType ComSignalEndianness;

    /*For ComSignalType UINT8_N this parameter specifies the length n in bytes. For ComSignalType UINT8_DYN it specifies the maximum
     *length in bytes. For all other types this parameter shall be ignored. The supported maximum length is restricted by the used
     *transportation system. For non TP-PDUs the maximum size of a PDU, and therefore also of any included signal, is limited by
     *the concrete bus characteristic. For example, the limit is 8 bytes for CAN and LIN, 64 bytes for CAN FD and 254 for FlexRay*/
    uint32 ComSignalLength;

    /* The AUTOSAR type of the signal. Whether or not the signal is signed or unsigned
    can be found by examining the value of this attribute.
    This type could also be used to reserved appropriate storage in AUTOSAR COM. */
    Com_SignalTypeType ComSignalType;

    /* Defines if a write access to this signal can trigger the transmission of the corresponding I-PDU. If the I-PDU is triggered, depends also on the transmission mode of the
    corresponding I-PDU. */
    Com_TransferPropertyType ComTransferProperty;

    /*Bit position of update-bit inside I-PDU.If this attribute is omitted then there is no update-bit.
     *This setting must be consistently on sender and on receiver side. Range: 0..63 for CAN and LIN,
     *0..511 for CAN FD, 0..2031 for FlexRay, 0..4294967295 for TP.*/
    uint8 ComUpdateBitPosition;

    /* Reference to the signal buffer */
    /* Note: Autosar extension */
    uint8* ComBufferRef;

}Com_SignalType;



/********************************************************************************************************************************

Name:                                ComSIPduGroup

Type:                                Structure

Description:           Contains the configuration parameters of the AUTOSAR COM module's IPDU groups

*********************************************************************************************************************************/

typedef struct {
    /* The numerical value used as the ID of this I-PDU Group .
    The ComIPduGroupHandleId is required by the API calls to start and stop
    I-PDU Groups. */
    uint16 ComIPduGroupHandleId;
}Com_IPduGroupType;





/********************************************************************************************************************************

Name:                                ComIPdu

Type:                                Structure

Description:             Contains the configuration parameters of the AUTOSAR COM module's I-PDUs.
*********************************************************************************************************************************/
typedef struct{
    /* The direction defines if this I-PDU, and therefore the contributing signals and signal
    groups, shall be sent or received. */
    Com_IPduDirectionType ComIPduDirection;

    /* The numerical value used as the ID of this I-PDU. The ComIPduHandleId
    is required by the API calls Com_RxIndication, Com_TpRxIndication, Com_StartOfReception
    and Com_CopyRxData to receive I-PDUs from the PduR (ComIP-duDirection: Receive), as well
    as the PduId passed to an Rx-I-PDU-callout. For Tx-I-PDUs (ComIPduDirection: Send),
    this handle Id is used for the APIs calls Com_TxConfirmation, Com_TriggerTransmit,
    Com_TriggerIPDUSend or Com_TriggerIPDUSendWithMetaData,
    Com_CopyTxData and Com_TpTxConfirmation to transmit respectively
    confirm transmissions of I-PDUs, as well as the PduId passed to the Tx-IPDU-callout
    configured with ComIPduCallout and/or ComIPduTriggerTransmitCallout.*/
    uint16 ComIPduHandleId;

    /* For the definition of the two modes Immediate and Deferred. */
    Com_IPduSignalProcessingType ComIPduSignalProcessing;

    /* Defines if this I-PDU is a normal I-PDU that can be sent unfragmented or if this is a
    large I-PDU that shall be sent via the Transport Protocol of the underlying bus. */
    Com_IPduTypeType ComIPduType;

    /* Reference to the I-PDU groups this I-PDU belongs to. */
    Com_IPduGroupType * ComIPduGroupRef[ComSupportedIPduGroups];

    /* References to all signal groups contained in this I-Pdu */
    Com_SignalGroupType * ComIPduSignalGroupRef[ComMaxSignalGroupCnt];

    /* References to all signals contained in this I-PDU. */
    Com_SignalType * ComIPduSignalRef[ComMaxSignalCnt];

    /* This container must be included if COM_IPDU_DIRECTION is
    configured to SEND. */
    Com_TxIPduType ComTxIPdu;

    /* Reference to the IPdu buffer */
    /* Note: Autosar extension */
    uint8* ComBufferRef;
	
	/* Autosar Extension */
	uint8 ComIPduLength;

}Com_IPduType;


/********************************************************************************************************************************

Name:                                ComConfig

Type:                                Structure

Description:          This container contains the configuration parameters and sub containers of the AUTOSAR COM module.

*********************************************************************************************************************************/
typedef struct{
    /* Contains the configuration parameters of the AUTOSAR COM module's IPDUs. */
    Com_IPduType ComIPdu[ComMaxIPduCnt];

    /* Contains the configuration parameters of the AUTOSAR COM module's IPDU groups. */
    Com_IPduGroupType ComIPduGroup[ComMaxIPduGroupCnt];

    /* Contains the configuration parameters of the AUTOSAR COM module's signals. */
    Com_SignalType ComSignal[ComMaxSignalCnt];

    /* Contains the configuration parameters of the AUTOSAR COM module's signal groups. */
    Com_SignalGroupType ComSignalGroup[ComMaxSignalGroupCnt];

    /* This container contains the configuration parameters of group signals. I.e. signals that are included within a signal group */
    /* Note: Autosar violation, Moved from inside ComSignalGroup to ComConfig */
    Com_GroupSignalType ComGroupSignal[ComMaxGroupSignalCnt];
	
}Com_ConfigType;


/********************************************************************************************************************************

Name:                                Com

Type:                                Structure

Description:          This container contains the configuration parameters and sub containers of the AUTOSAR COM module.

*********************************************************************************************************************************/
typedef struct {
    /*This container contains the configuration parameters and sub containers of
    the AUTOSAR COM module.*/
    Com_ConfigType ComConfig;
}Com_Type;


#endif /* COM_TYPES_H_ */
