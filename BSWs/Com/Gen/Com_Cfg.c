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

void Com_CbkTxAck(void)
{

}

void Com_CbkRxAck(void)
{

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
             .ComNotification = &Com_CbkTxAck,
             .ComSignalEndianness = LITTLE_ENDIAN,
             .ComSignalLength = 1,
             .ComSignalType= UINT8,
             .ComTransferProperty =  PENDING,
             .ComUpdateBitPosition = 8,
             .ComBufferRef = ComSignal0Buffer
            },
            {
             .ComBitPosition = 9,
             .ComBitSize= 8,
             .ComHandleId=1,
             .ComNotification = &Com_CbkTxAck,
             .ComSignalEndianness = LITTLE_ENDIAN,
             .ComSignalLength = 1,
             .ComSignalType= UINT8,
             .ComTransferProperty =  TRIGGERED,
             .ComUpdateBitPosition = 17,
             .ComBufferRef = ComSignal1Buffer
            },
            {
             .ComBitPosition = 0,
             .ComBitSize= 8,
             .ComHandleId=2,
             .ComNotification = &Com_CbkRxAck,
             .ComSignalEndianness = LITTLE_ENDIAN,
             .ComSignalLength = 1,
             .ComSignalType= UINT8,
             .ComTransferProperty =  TRIGGERED_ON_CHANGE,
             .ComUpdateBitPosition = 8,
             .ComBufferRef = ComSignal2Buffer
            },
            {
             .ComBitPosition = 9,
             .ComBitSize= 8,
             .ComHandleId=3,
             .ComNotification = &Com_CbkRxAck,
             .ComSignalEndianness = LITTLE_ENDIAN,
             .ComSignalLength = 1,
             .ComSignalType= UINT8,
             .ComTransferProperty =  TRIGGERED_WITHOUT_REPETITION,
             .ComUpdateBitPosition = 17,
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
                .ComBufferRef = ComGroupSignal1Buffer
            }
        },
        .ComSignalGroup=
        {
            {
                .ComHandleId=0,
                .ComNotification = &Com_CbkTxAck,
                .ComTransferProperty = TRIGGERED_ON_CHANGE,
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
                .ComNotification = &Com_CbkRxAck,
                .ComTransferProperty = TRIGGERED_ON_CHANGE,
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
               .ComIPduDirection = SEND,
               .ComIPduHandleId= 0,
               .ComIPduSignalProcessing = DEFERRED,
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
                    .ComMinimumDelayTime=0.01,
                    .ComTxIPduClearUpdateBit= Confirmation,
                    .ComTxIPduUnusedAreasDefault=255,
                    .ComTxModeFalse=
                    {
                        .ComTxMode=
                        {
                            .ComTxModeMode = DIRECT,
                            .ComTxModeNumberOfRepetitions = 2,
                            .ComTxModeRepetitionPeriod = 0.05,
                            .ComTxModeTimePeriod=0.1
                        }
                    }
                },
                .ComBufferRef = ComIPdu0Buffer
            },
            {
				.ComIPduLength = 5,
                .ComIPduDirection = RECEIVE,
                .ComIPduHandleId= 1,
                .ComIPduSignalProcessing = IMMEDIATE,
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
