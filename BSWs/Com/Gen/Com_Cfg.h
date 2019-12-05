/*******************************************************************************************************************************
FileName:                                               Com_Cfg.h
AUTOSAR Version:                                          4.2.2
******************************************************************************************************************************/
#ifndef COM_CFG_H
#define COM_CFG_H



/*******************************************************************************************************************************
**                                                                        Defines                                                                                **
********************************************************************************************************************************/
#define   COM_TX_IPDU_UNUSED_AREAS_DEFAULT   (uint8)255
#define   COM_MINIMUM_DELAY_TIME (float32)0.01

#define ComMaxSignalGroupCnt   (uint8)2
#define ComMaxGroupSignalCnt   (uint8)2
#define ComMaxSignalCnt   (uint8)4
#define ComMaxIPduGroupCnt (uint8)2



/**************************************************************************************************
**
Name:                                   ComConfigurationUseDet
Type:                                   EcucBooleanParamDef
Description:            The error hook shall contain code to call the Det.
                        If this parameter is configured COM_DEV_ERROR_DETECT shall be set
                        to ON as output of the configuration tool. (as input for the source code)                                           **
**************************************************************************************************/
#define ComConfigurationUseDet false


/**************************************************************************************************
**
Name:                                   ComEnableMDTForCyclicTransmission
Type:                                   EcucBooleanParamDef
Description:            Enables globally for the whole Com module the minimum delay time monitoring
                        for cyclic and repeated transmissions (ComTxModeMode=PERIODIC or ComTxModeMode=MIXED
                        for the cyclic transmissions,
                        ComTxModeNumberOfRepetitions > 0 for repeated transmissions).                                         **
**************************************************************************************************/
#define ComEnableMDTForCyclicTransmission false


/**************************************************************************************************
**
Name:                                   ComRetryFailedTransmitRequests
Type:                                   EcucBooleanParamDef
Description:                If this Parameter is set to true, retry of failed transmission requests
                            is enabled. If this Parameter is not present, the default value is assumed                              **
**************************************************************************************************/
#define ComRetryFailedTransmitRequests  false



/**************************************************************************************************

Name:                                ComSupportedIPduGroups
Type:                                EcucIntegerParamDef
Description:                Defines the maximum number of supported I-PDU groups.
Range:                               0 ---> 65535
**************************************************************************************************/
#define ComSupportedIPduGroups  (uint16)    2


/**************************************************************************************************
**
Name:                                   ComRxTimeBase
Type:                                   EcucFloatParamDef
Description:                The period between successive calls to Com_MainFunctionRx in seconds.
                            This parameter may be used by the COM generator to transform the values
                            of the reception related timing configuration parameters of the COM
                            module to internal implementation specific counter or tick values. The
                            COM module's internal timing handling is implementation specific.
                            The COM module (generator) may rely on the fact that
                            Com_MainFunctionRx is scheduled according to the value configuredhere
Range:                                  0 ---> 3600                                         **
**************************************************************************************************/
#define ComRxTimeBase   (float32)    0.1


/**************************************************************************************************
**
Name:                                   ComTxTimeBase

Type:                                   EcucFloatParamDef

Description:                The period between successive calls to Com_MainFunctionTx in seconds.
                            This parameter may be used by the COM generator to transform
                            the values of the transmission related timing configuration parameters of the COM
                            module to internal implementation specific counter or tick values. The
                            COM module's internal timing handling is implementation specific.
                            The COM module (generator) may rely on the fact that
                            Com_MainFunctionTx is scheduled according to the value configured here
Range:                                  0 ---> 3600
**************************************************************************************************/
#define ComTxTimeBase   (float32)    0.1


/**************************************************************************************************

Name:                                ComMaxIPduCnt
Type:                                EcucIntegerParamDef
Description:                Defines the maximum number of supported I-PDU groups.
Range:                               0 ---> 18446744073709551615
**************************************************************************************************/
#define ComMaxIPduCnt  (uint64)    2



#endif /*COM_CFG_H*/
