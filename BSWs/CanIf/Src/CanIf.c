/*********************************************************************************************************************************
 Service name:                                           CanIf_ReadRxPduData
 Service ID[hex]:                                               0x06
 Sync/Async:                                               Synchronous
 Reentrancy:                                                 Non Reentrant
 Parameters (in):                                           CanIfRxSduId    -->Receive L-SDU handle specifying the corresponding 
																								CAN L-SDU ID and implicitly the CAN Driver instance
																								as well as the corresponding CAN controller device.
Parameters (inout):                                          None
 Parameters (out):                                       CanIfRxInfoPtr   -->Pointer to a structure with CAN L-SDU related data:
																								DLC and pointer to CAN L-SDU buffer including the
																								MetaData of dynamic L-PDUs.
 Return value:                                           Std_ReturnType
 Description:
					                    This service provides the CAN DLC and the received data of the requested
					                              CanIfRxSduId to the calling upper layer.
 *******************************************************************************************************************************/
Std_ReturnType CanIf_GetRxPdu(PduIdType CanIfRxSduId, CanIfRxPduCfg* RxPduPtr)
{
	/*[SWS_CANIF_00325]If parameter CanIfRxSduId of CanIf_ReadRxPduData()
	has an invalid value, e.g. not configured to be stored within CanIf via CANIF_READRXPDU_DATA
	(ECUC_CanIf_00600), CanIf shall report development error code CANIF_E_INVALID_RXPDUID
	to the Det_ReportError service of the DET, when CanIf_ReadRxPduData() is called.*/
    uint32 RxPduIndex;
    if(CanIfRxSduId < CanIfMaxRxPduCfg)
    {
        for(RxPduIndex = 0; RxPduIndex < CanIfMaxRxPduCfg ; RxPduIndex++)
        {
            if(CanIf.CanIfInitCfg.CanIfRxPduCfg[RxPduIndex].CanIfRxPduId == CanIfRxSduId)
            {
                RxPduPtr = &CanIf.CanIfInitCfg.CanIfRxPduCfg[RxPduIndex];
                return E_OK;
            }
            else
            {
			 /*misra error*/
            }
        }
    }
    else
    {
        return E_NOT_OK;
    }

    return E_NOT_OK;
}

/*[SWS_CANIF_00330]This API can be enabled or disabled at pre-compile time 
configuration by the configuration parameter CANIF_PUBLIC_READRXPDU_DATA_API*/
#if(CANIF_PUBLIC_READRXPDU_DATA_API)

Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId,PduInfoType* CanIfRxInfoPtr)
{



    uint32 RxPduIndex;
    CanIf_ControllerModeType CanIf_GetControllerModeReturn;
    CanIfRxPduCfg *RxPduPtr = NULL;
    PduInfoType* RxBuffer[CanIfMaxRxPduCfg];

        /*  CanIf must be initialized after Power ON */
        if(CanIfState == CANIF_UNINIT)
        {
            return E_NOT_OK;
        }
        else
        {
            if(CanIf_GetRxPdu(CanIfRxSduId, RxPduPtr) == E_NOT_OK)
            {
                return E_NOT_OK;
            }
            else
            {

                if(CanIfGetControllerMode(RxPduPtr->CanIfRxPduHrhIdRef->CanIfHrhCanCtrlIdRef->CanIfCtrlId, &CanIf_GetControllerModeReturn) == E_NOT_OK)
                {
                    return E_NOT_OK;
                }
                else
                {
					/*[SWS_CANIF_00324] d The function CanIf_ReadRxPduData() shall not accept a
					request and return E_NOT_OK, if the corresponding CCMSM does not equal CANIF_CS_STARTED
					and the channel mode is in the receive path online.*/
                    if(CanIf_GetControllerModeReturn != CANIF_CS_STARTED)
                    {
                        return E_NOT_OK;
                    }
                    else
                     {
					  /*[SWS_CANIF_00326]If parameter CanIfRxInfoPtr of CanIf_ReadRxPduData()
					  has an invalid value, CanIf shall report development error code CANIF_E_PARAM_POINTER 
					  to the Det_ReportError service of the DET module, when CanIf_ReadRxPduData() is called.*/					   
                      if( (CanIfRxInfoPtr->SduLength) > 8 || (CanIfRxInfoPtr->SduDataPtr)==NULL)
                         {
                         CanIfDevelopmentError = CANIF_E_PARAM_POINTER;
                            return E_NOT_OK;
                         }
                         else
                         {

                            CanIfRxInfoPtr = RxBuffer[CanIfRxSduId];

                         }
                     }
                }
            }
        }

}

#else

    /*misra error*/

#endif
