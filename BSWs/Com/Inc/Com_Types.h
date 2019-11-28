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


/*******************************************************************************************************************************
**                                   Type Definitions                                                                    **
********************************************************************************************************************************/

/*The AUTOSAR COM module's signal object identifier.*/
typedef uint16 Com_SignalIdType;

/*The AUTOSAR COM module's signal group object identifier.*/
typedef uint16 Com_SignalGroupIdType;

/*The AUTOSAR COM I-PDU module's group object identifier.*/
typedef uint16 Com_IpduGroupIdType;

/*This type can be used to store a flag (bit) for each I-PDU group within the system. It is used for setting the activation
 *state and deadline monitoring state for I-PDU groups within one function call*/
uint8 Com_IpduGroupVector[(ComSupportedIPduGroups-1)/8+1];


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
    COM_INIT
}Com_StatusType;


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

   Com_TxModeModeType  ComTxModeMode;

   /*Defines the number of repetitions for the transmission mode DIRECT and the event driven part of transmission mode MIXED.*/
   uint8 ComTxModeNumberOfRepetitions;

   /*Defines the repetition period in seconds of the multiple transmissions in case ComTxModeNumberOfRepetitions is
    * configured greater than or equal to 1 and ComTxModeMode is configured to DIRECT or MIXED. In case of the mixed
    * transmission mode only the event driven part is affected*/
   /*Range from 0 to 3600*/
   const float32 ComTxModeRepetitionPeriod ;

   /*Defines the period in seconds between the start of the I-PDU by Com_IpduGroupControl and the first transmission request
    *in case ComTxModeMode is configured to PERIODIC or MIXED. In case of the mixed transmission mode only the periodic part is affected.
    *In case ComTxModeTimeOffset is omitted or configured to 0, the first periodic transmission shall be transmitted within
    *the next invocation of Com_MainFunctionTx.*/
   /*Range from 0 to 3600*/
   const float32 ComTxModeTimeOffset ;

   /*Defines the repetition period in seconds of the periodic transmission re-quests in case ComTxModeMode is configured to PERIODIC
    * or MIXED. In case of the mixed transmission mode only the periodic part is affected.*/
   /*Range: from 0 to 3600*/
   const float32 ComTxModeTimePeriod ;

}Com_TxModeType;



/********************************************************************************************************************************

Name:                                ComTxModeFalse

Type:                                Structure

Description:       This container contains the configuration parameters of the AUTOSAR COM module's transmission modes in the case
                   the ComFilter evaluates to false.
*********************************************************************************************************************************/
typedef struct{
    Com_TxModeType ComTxMode;
}Com_TxModeFalseType;


/********************************************************************************************************************************

Name:                                ComTxModeTrue

Type:                                Structure

Description:       This container contains the configuration parameters of the AUTOSAR COM module's transmission modes in the case
                   the ComFilter evaluates to true..
*********************************************************************************************************************************/
typedef struct{
    Com_TxModeType ComTxMode;
}Com_TxModeTrueType;


/********************************************************************************************************************************

Name:                                ComTxIPdu

Type:                                Structure

Description:    This container contains additional transmission related configuration parameters of the AUTOSAR COM module's I-PDUs.
*********************************************************************************************************************************/
typedef struct{

    Com_TxIPduClearUpdateBitType ComTxIPduClearUpdateBit;

    /*The referenced transmission mode object that is used when the filtering state for this I-PDU evaluates to false.
     *  The default is transmission mode None.*/
    Com_TxModeFalseType ComTxModeFalse;

    /*The referenced transmission mode object that is used when the filtering state for this I-PDU evaluates to true.*/
    Com_TxModeTrueType ComTxModeTrue;


    /*Defines the Minimum Delay Time (MDT) between successive transmissions of this I-PDU in seconds. The MDT is independent of
     *the possible different transmission modes. There is only one minimum delay time parameter for one I-PDU. The minimum delay
     *timer is not reset by changing the transmission mode. Hence, it is not allowed to violate the minimum delay time by
     *transmission mode changes. It is not possible to monitor the minimum delay time for I-PDUs that are requested using the
     *transmission Com_TriggerTransmit API.*/
    /*Range: from 0 to 3600 */
    const float32 ComMinimumDelayTime;



}Com_TxIPduType;




/********************************************************************************************************************************

Name:                                ComGroupSignal

Type:                                Structure

Description:          This container contains the configuration parameters of group signals.
                      I.e. signals that are included within a signal group.

*********************************************************************************************************************************/
typedef struct{

    /*Size in bits, for integer signal types. For ComSignalType UINT8_N and UINT8_DYN, the size shall be configured by ComSignalLength.
     *For ComSignalTypes FLOAT32 and FLOAT64 the size is already defined by the signal type and therefore may be omitted.*/
    /*Range: from 0 to 64*/
    uint8  ComBitSize;

    /*For ComSignalType UINT8_N this parameter specifies the length n in bytes. For Type UINT8_DYN it specifies the maximum
     * length in bytes. For all other types this parameter shall be ignored.
     *Range: 0..8 for normal CAN/ LIN I-PDUs*/
    uint8 ComSignalLength;

    /*The numerical value used as the ID. For signals it is required by the API calls Com_UpdateShadowSignal,
     *Com_ReceiveShadowSignal and Com_InvalidateShadowSignal. For sig-nals groups it is required by the Com_SendSignalGroup
     *and Com_ReceiveSignalGroup calls.*/
    uint16  ComHandleId;

    /*Starting position within the I-PDU. This parameter refers to the position in the I-PDU and not in the shadow buffer.
     *If the endianness conversion is configured to Opaque the parameter ComBitPosition shall define the bit0 of the first byte
     *like in little endian byte order*/
    uint32 ComBitPosition;

    Com_SignalEndiannessType ComSignalEndianness;
    Com_SignalTypeType ComSignalType;


}Com_GroupSignalType;



/********************************************************************************************************************************

Name:                                ComSignalGroup

Type:                                Structure

Description:           Contains the configuration parameters of the AUTOSAR COM module's signal groups.

*********************************************************************************************************************************/
typedef struct{

    /*Only valid on sender side: Name of Com_CbkTxErr callback function to be called.
     *If this parameter is omitted no error notification shall take place.*/
    void (* ComErrorNotification )(void);

    /*On sender side: Name of Com_CbkTxAck callback function to be called.
     *On receiver side: Name of Com_CbkRxAck callback function to be called.
     *If this parameter is omitted no notification shall take place.*/
    void (* ComNotification )(void);

    /*Bit position of update-bit inside I-PDU.If this attribute is omitted then there is no update-bit. This setting must be
     *consistently on sender and on receiver side.
     *Range: from 0 to 63 for CAN and LIN*/
     uint8 ComUpdateBitPosition;

    /*This parameter defines that the respective signal's initial value shall be put into the respective PDU but there
     *will not be any update of the value through the RTE. Thus the Com implementation does not need to expect any API calls
     *for this signal (group).*/
    boolean ComInitialValueOnly;

    /*Defines whether the uint8-array based access shall be used for this Com-SignalGroup.*/
    boolean ComSignalGroupArrayAccess;

    /*The numerical value used as the ID.For signals it is required by the API calls Com_UpdateShadowSignal,
     *Com_ReceiveShadowSignal and Com_InvalidateShadowSignal. For sig-nals groups it is required by the Com_SendSignalGroup
     *and Com_ReceiveSignalGroup calls.*/
     uint16 ComHandleId ;

    /*Defines the length of the first deadline monitoring timeout period in seconds. This timeout is used immediately
     *after start (or restart) of the dead-line monitoring service. The timeout period of the successive periods is
     *configured by ECUC_Com_00263.*/
    /*Range: from 0 to 3600*/
     float32 ComFirstTimeout;

    /*Defines the length of the deadline monitoring timeout period in seconds. The period for the first timeout
     * period can be configured separately by*/
     float32 EcucFloatParamDef;

    /*Defines the length of the deadline monitoring timeout period in seconds. The period for the first timeout period
     *can be configured separately by*/
    /*Range: From 0 to 3600*/
     float32 ComTimeout ;

    Com_TransferPropertyType ComTransferProperty;
    Com_GroupSignalType ComGroupSignal;

}Com_SignalGroupType;


/********************************************************************************************************************************

Name:                                ComSignal

Type:                                Structure

Description:          Contains the configuration parameters of the AUTOSAR COM module's signals.

*********************************************************************************************************************************/
typedef struct{

    Com_SignalEndiannessType ComSignalEndianness;
    Com_TransferPropertyType ComTransferProperty;
    Com_SignalTypeType ComSignalType;

    /*Only valid on sender side: Name of Com_CbkTxErr callback function to be called.
     * If this parameter is omitted no error notification shall take place.*/
    void (* ComErrorNotification )(void);

    /*On sender side: Name of Com_CbkTxAck callback function to be called.
     *On receiver side: Name of Com_CbkRxAck callback function to be called.
     *If this parameter is omitted no notification shall take place.*/
    void (* ComNotification) (void);

    /*On sender side: Name of Com_CbkTxTOut callback function to be called.
     *On receiver side: Name of Com_CbkRxTOut callback function to be called.*/
    void (* ComTimeoutNotification )(void);


    /*Bit position of update-bit inside I-PDU.If this attribute is omitted then there is no update-bit.
     *This setting must be consistently on sender and on receiver side. Range: 0..63 for CAN and LIN,
     *0..511 for CAN FD, 0..2031 for FlexRay, 0..4294967295 for TP.*/
    uint32 ComUpdateBitPosition;

    /*Starting position within the I-PDU. This parameter refers to the position in the I-PDU and not in the shadow buffer.
     *If the endianness conversion is configured to Opaque the parameter ComBitPosition shall define the bit0 of the first byte
     *like in little endian byte order*/
    uint32 ComBitPosition;

    /*For ComSignalType UINT8_N this parameter specifies the length n in bytes. For ComSignalType UINT8_DYN it specifies the maximum
     *length in bytes. For all other types this parameter shall be ignored. The supported maximum length is restricted by the used
     *transportation system. For non TP-PDUs the maximum size of a PDU, and therefore also of any included signal, is limited by
     *the concrete bus characteristic. For example, the limit is 8 bytes for CAN and LIN, 64 bytes for CAN FD and 254 for FlexRay*/
    uint32 ComSignalLength;

    /*The numerical value used as the ID.This ID identifies signals and signal groups in the COM APIs using Com_SignalIdType
     *or Com_SignalGroupIdType parameter respectively.*/
    uint16 ComHandleId;

    /*Size in bits, for integer signal types. For ComSignalType UINT8_N and UINT8_DYN the size shall be configured by ComSignalLength.
     * For ComSignalTypes FLOAT32 and FLOAT64 the size is already defined by the signal type and therefore may be omitted.*/
    uint8 ComBitSize;

    /*Defines the length of the first deadline monitoring timeout period in se-conds. This timeout is used immediately after
     *start (or restart) of the dead-line monitoring service. The timeout period of the successive periods is configured by
     *start ECUC_Com_00263.*/
    float32 ComFirstTimeout;

    /*Defines the length of the deadline monitoring timeout period in seconds.
     *the period for the first timeout period can be configured separately by ECUC_Com_00183.*/
    float32 ComTimeout;

}Com_SignalType;


/********************************************************************************************************************************

Name:                                ComIPdu

Type:                                Structure

Description:             Contains the configuration parameters of the AUTOSAR COM module's I-PDUs.
*********************************************************************************************************************************/
typedef struct{


    Com_IPduDirectionType ComIPduDirection;
    Com_IPduSignalProcessingType ComIPduSignalProcessing;
    Com_IPduTypeType ComIPduType;

    Com_TxIPduType ComTxIPdu;

    Com_SignalGroupType * ComIPduSignalGroupRef;
    Com_SignalType * ComIPduSignalRef;

    /*Defines for I-PDUs with ComIPduType NORMAL: If the underlying IF-module supports cancellation of transmit requests.
      Defines for I-PDUs with ComIPduType TP: If the underlying TP-module supports RX and TX cancellation of ongoing requests.*/
    /*dependency: This parameter shall not be set to true if ComCancellation-Support is set to false*/
    boolean ComIPduCancellationSupport;


    /*The numerical value used as the ID of this I-PDU. The ComIPduHandleId is required by the API calls Com_RxIndication,
     *Com_TpRxIndication Com_StartOfReception and Com_CopyRxData to receive I-PDUs from the PduR (ComIP-duDirection: Receive),
     *as well as the PduId passed to an Rx-I-PDU-callout. For Tx-I-PDUs (ComIPduDirection: Send), this handle Id is used for
     *the APIs calls Com_TxConfirmation, Com_TriggerTransmit, Com_TriggerIPDUSend or Com_TriggerIPDUSendWithMetaData,
     *Com_CopyTxData and Com_TpTxConfirmation to transmit respectively confirm transmissions of I-PDUs, as well as
     *the PduId passed to the Tx-I-PDU-callout configured with ComIPduCallout and/or ComIPduTrigger-TransmitCallout.*/
    /*Range: from 0 to 3600*/
    uint16 ComIPduHandleId;

}Com_IPduType;





/********************************************************************************************************************************

Name:                                ComConfig

Type:                                Structure

Description:          This container contains the configuration parameters and sub containers of the AUTOSAR COM module.

*********************************************************************************************************************************/
typedef struct{
    Com_IPduType ComIPdu[ComMaxIPduCnt];
    Com_SignalGroupType ComSignalGroup[ComMaxSignalGrpCnt];
    Com_SignalType ComSignal[ComMaxSignalCnt];


}Com_ConfigType;


/********************************************************************************************************************************

Name:                                Com

Type:                                Structure

Description:          This container contains the configuration parameters and sub containers of the AUTOSAR COM module.

*********************************************************************************************************************************/
typedef struct {
    Com_ConfigType ComConfig;
}Com_Type;




#endif /* COM_TYPES_H_ */
