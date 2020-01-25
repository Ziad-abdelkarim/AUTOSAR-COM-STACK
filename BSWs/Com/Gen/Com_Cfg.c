/*******************************************************************************************************************************
FileName:                                               Com_Cfg.c
AUTOSAR Version:                                          4.2.2
******************************************************************************************************************************/
/******************************************************************************************************************************
 **                                                     Includes                                                             **
 ******************************************************************************************************************************/
#include "Com.h"
#include "Com_Cfg.h"
/*****************************************************************************************************************************
 **                                         Post-Build Configuration variables values                                       **
 *****************************************************************************************************************************/

void Com_CbkSignal0TxAck(void)
{
    UARTprintf("Com_CbkSignal0TxAck\n");
}

void Com_CbkSignal1TxAck(void)
{
    UARTprintf("Com_CbkSignal1TxAck\n");
}

void Com_CbkSignal2RxAck(void)
{
    uint8 SignalData;
    if(Com_ReceiveSignal(2, &SignalData) == E_OK)
    {
        UARTprintf("Com_CbkSignal2RxAck = %d\n", SignalData);
    }
}

void Com_CbkSignal3RxAck(void)
{
    uint8 SignalData;
    if(Com_ReceiveSignal(3, &SignalData) == E_OK)
    {
        UARTprintf("Com_CbkSignal3RxAck = %d\n", SignalData);
    }
}

void Com_CbkSignalGroup0TxAck(void)
{
    UARTprintf("Com_CbkSignalGroup0TxAck\n\n");
}

void Com_CbkSignalGroup1RxAck(void)
{
    uint16 SignalData;
    if(Com_ReceiveSignalGroup(1) == E_OK)
    {
        Com_ReceiveShadowSignal(1, &SignalData);
        UARTprintf("Com_CbkSignalGroup1RxAck = %d\n", SignalData);
    }
}

/* ComSignal Buffers */
uint8 ComSignal0Buffer[1];
uint8 ComSignal1Buffer[1];
uint8 ComSignal2Buffer[1];
uint8 ComSignal3Buffer[1];

/* ComGroupSignal Buffers */
uint8 ComGroupSignal0Buffer[2];
uint8 ComGroupSignal1Buffer[2];

/* Com IPdu Buffers */
uint8 ComIPdu0Buffer[5];
uint8 ComIPdu1Buffer[5];

Com_Type Com =
{
    .ComConfig=
    {
        .ComSignal=
        {
            {
             .ComBitPosition = 0,
             .ComBitSize= 8,
             .ComHandleId=0,
             .ComNotification = &Com_CbkSignal0TxAck,
             .ComSignalEndianness = LITTLE_ENDIAN,
             .ComSignalLength = 1,
             .ComSignalType= UINT8,
             .ComTransferProperty =  TRIGGERED,
             .ComUpdateBitPosition = 8,
             .ComSignalInitValue = COM_SIGNAL_INIT_VALUE,
             .ComBufferRef = ComSignal0Buffer
            },
            {
             .ComBitPosition = 9,
             .ComBitSize= 8,
             .ComHandleId=1,
             .ComNotification = &Com_CbkSignal1TxAck,
             .ComSignalEndianness = LITTLE_ENDIAN,
             .ComSignalLength = 1,
             .ComSignalType= UINT8,
             .ComTransferProperty =  PENDING,
             .ComUpdateBitPosition = 17,
             .ComSignalInitValue = COM_SIGNAL_INIT_VALUE,
             .ComBufferRef = ComSignal1Buffer
            },
            {
             .ComBitPosition = 0,
             .ComBitSize= 8,
             .ComHandleId=2,
             .ComNotification = &Com_CbkSignal2RxAck,
             .ComSignalEndianness = LITTLE_ENDIAN,
             .ComSignalLength = 1,
             .ComSignalType= UINT8,
             .ComTransferProperty =  TRIGGERED,
             .ComUpdateBitPosition = 8,
             .ComSignalInitValue = COM_SIGNAL_INIT_VALUE,
             .ComBufferRef = ComSignal2Buffer
            },
            {
             .ComBitPosition = 9,
             .ComBitSize= 8,
             .ComHandleId=3,
             .ComNotification = &Com_CbkSignal3RxAck,
             .ComSignalEndianness = LITTLE_ENDIAN,
             .ComSignalLength = 1,
             .ComSignalType= UINT8,
             .ComTransferProperty =  PENDING,
             .ComUpdateBitPosition = 17,
             .ComSignalInitValue = COM_SIGNAL_INIT_VALUE,
             .ComBufferRef = ComSignal3Buffer
            }
        },
        .ComIPduGroup=
        {
           {
                .ComIPduGroupHandleId=0
           },
           {
                .ComIPduGroupHandleId=1
           }
        },
        .ComGroupSignal=
        {
            {
                .ComBitPosition=18,
                .ComBitSize=16,
                .ComHandleId=0,
                .ComSignalEndianness=LITTLE_ENDIAN,
                .ComSignalLength=2,
                .ComSignalType= UINT16,
                .ComTransferProperty = PENDING,
                .ComSignalInitValue=COM_SIGNAL_INIT_VALUE,
                .ComBufferRef = ComGroupSignal0Buffer
            },
            {
                .ComBitPosition=18,
                .ComBitSize=16,
                .ComHandleId=1,
                .ComSignalEndianness=LITTLE_ENDIAN,
                .ComSignalLength=2,
                .ComSignalType= UINT16,
                .ComTransferProperty = PENDING,
                .ComSignalInitValue=COM_SIGNAL_INIT_VALUE,
                .ComBufferRef = ComGroupSignal1Buffer
            }
        },
        .ComSignalGroup=
        {
            {
                .ComHandleId=0,
                .ComNotification = &Com_CbkSignalGroup0TxAck,
                .ComTransferProperty = PENDING,
                .ComUpdateBitPosition =34,
                /*.ComInitialValueOnly= false,*/
                .ComGroupSignalRef=
                {
                    &Com.ComConfig.ComGroupSignal[0],
                    NULL
                }
            },
            {
                .ComHandleId=1,
                .ComNotification = &Com_CbkSignalGroup1RxAck,
                .ComTransferProperty = PENDING,
                .ComUpdateBitPosition =34,
                /*.ComInitialValueOnly= false,*/
                .ComGroupSignalRef=
                {
                    &Com.ComConfig.ComGroupSignal[1],
                    NULL
                }
            }
        },
        .ComIPdu=
        {
            {
               .ComIPduLength = 5,
               .ComIPduDirection = Send,
               .ComIPduHandleId= 0,
               .ComIPduSignalProcessing = IMMEDIATE,
               .ComIPduType = NORMAL,
               .ComIPduGroupRef=
                {
                    &Com.ComConfig.ComIPduGroup[0],
                    NULL
                },
               .ComIPduSignalGroupRef=
                {
                    &Com.ComConfig.ComSignalGroup[0],
                    NULL
                },
               .ComIPduSignalRef=
                {
                    &Com.ComConfig.ComSignal[0],
                    &Com.ComConfig.ComSignal[1],
                    NULL
                },
               .ComTxIPdu=
                {
                    .ComMinimumDelayTime=0.5,
                    .ComTxIPduClearUpdateBit= Confirmation,
                    .ComTxIPduUnusedAreasDefault=255,
                    .ComTxModeFalse=
                    {
                        .ComTxMode=
                        {
                            .ComTxModeMode = PERIODIC,
                            .ComTxModeNumberOfRepetitions = 2,
                            .ComTxModeRepetitionPeriod = 2,
                            .ComTxModeTimePeriod=2
                        }
                    }
                },
                .ComBufferRef = ComIPdu0Buffer
            },
            {
                .ComIPduLength = 5,
                .ComIPduDirection = Receive,
                .ComIPduHandleId= 1,
                .ComIPduSignalProcessing = DEFERRED,
                .ComIPduType = NORMAL,
                .ComIPduGroupRef=
                {
                    &Com.ComConfig.ComIPduGroup[1],
                    NULL
                },
                .ComIPduSignalGroupRef=
                {
                    &Com.ComConfig.ComSignalGroup[1],
                    NULL
                },
                .ComIPduSignalRef=
                {
                    &Com.ComConfig.ComSignal[2],
                    &Com.ComConfig.ComSignal[3],
                    NULL
                },
                .ComBufferRef = ComIPdu1Buffer
            }
        }
    }
};
