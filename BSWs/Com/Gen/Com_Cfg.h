/*******************************************************************************************************************************
FileName:                    							Com_Cfg.h
AUTOSAR Version:          								  4.2.2
******************************************************************************************************************************/
#ifndef COM_CFG_H
#define COM_CFG_H



/*******************************************************************************************************************************
**                                                                        Defines                                                                                **
********************************************************************************************************************************/

#define ComMaxSignalGroupCnt   1
#define ComMaxGroupSignal		1
#define ComMaxSignalCnt   2
#define COMMINIMUMDELAYTIME (float32)0.5





/**************************************************************************************************
**
Name:                                   ComConfigurationUseDet
Type:                                   EcucBooleanParamDef
Description:  			The error hook shall contain code to call the Det.
                        If this parameter is configured COM_DEV_ERROR_DETECT shall be set
                        to ON as output of the configuration tool. (as input for the source code)											**
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
Name:                                   ComEnableSignalGroupArrayApi
Type:                                   EcucBooleanParamDef
Description:            Activate/Deactivate the signal group array access APIs
                        (Com_SendSignalGroupArray, Com_ReceiveSignalGroupArray).
                        true: signal group array access APIs activated
                        false: signal group array access APIs deactivated                                         **
**************************************************************************************************/
#define ComEnableSignalGroupArrayApi false


/**************************************************************************************************
**
Name:                                   ComRetryFailedTransmitRequests
Type:                                   EcucBooleanParamDef
Description: 				If this Parameter is set to true, retry of failed transmission requests
                            is enabled. If this Parameter is not present, the default value is assumed								**
**************************************************************************************************/
#define ComRetryFailedTransmitRequests	false


/**************************************************************************************************

Name:                                ComSupportedIPduGroups
Type:                                EcucIntegerParamDef
Description: 				Defines the maximum number of supported I-PDU groups.
Range: 								 0 ---> 65535
**************************************************************************************************/
#define ComSupportedIPduGroups	(uint16)    0


/**************************************************************************************************
**
Name:                                   ComRxTimeBase
Type:                                   EcucFloatParamDef
Description:  			    The period between successive calls to Com_MainFunctionRx in seconds.
                            This parameter may be used by the COM generator to transform the values
                            of the reception related timing configuration parameters of the COM
                            module to internal implementation specific counter or tick values. The
                            COM module's internal timing handling is implementation specific.
                            The COM module (generator) may rely on the fact that
                            Com_MainFunctionRx is scheduled according to the value configuredhere
Range:                                  0 ---> 3600											**
**************************************************************************************************/
#define ComRxTimeBase   (uint16)    0


/**************************************************************************************************
**
Name:                                   ComTxTimeBase

Type:                                   EcucFloatParamDef

Description: 				The period between successive calls to Com_MainFunctionTx in seconds.
                            This parameter may be used by the COM generator to transform the values of the transmission related timing configuration parameters of the COM
                            module to internal implementation specific counter or tick values. The
                            COM module's internal timing handling is implementation specific.
                            The COM module (generator) may rely on the fact that
                            Com_MainFunctionTx is scheduled according to the value configured here
Range:                                  0 ---> 3600
**************************************************************************************************/
#define ComTxTimeBase   (uint16)    0


/**************************************************************************************************

Name:                                ComMaxIPduCnt
Type:                                EcucIntegerParamDef
Description:                Defines the maximum number of supported I-PDU groups.
Range:                               0 ---> 18446744073709551615
**************************************************************************************************/
#define ComMaxIPduCnt  (uint64)    2




/**************************************************************************************************
**
Name:                                   ComTxIPduUnusedAreasDefault
Type:                                   EcucIntegerParamDef
Description: 				The AUTOSAR COM module fills not used areas of an I-PDU with this byte
                            pattern. This attribute is mandatory to avoid undefined behaviour. This
                            byte-pattern will be repeated throughout the I-PDU before any init-values
                            or update-bits were set.
Range:                                  0 ---> 255
**************************************************************************************************/
#define ComTxIPduUnusedAreasDefault     (uint8)     255



#endif /*COM_CFG_H*/
