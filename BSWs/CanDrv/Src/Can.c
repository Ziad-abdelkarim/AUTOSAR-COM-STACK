/********************************************************************************************************************************
**                                                                       includes                                                                                         **
********************************************************************************************************************************/
#include "Can_GeneralTypes.h"














/*********************************************************************************************************************************       
Service name:                                       Can_SetControllerMode
Service ID[hex]:                                                   0x03
Sync/Async:                                                  Asynchronous
Reentrancy:                                                  Non Reentrant
Parameters (in):                                    Controller ---> CAN controller for which the status shall be changed
                                                               Transition ---> Transition value to request new CAN controller state
Parameters (inout):                                               None
Parameters (out):                                                  None
Return value:                                                Can_ReturnType
Description:   
                                  This function performs software triggered state transitions of the CAN controller
                                  State machine.
*******************************************************************************************************************************/
Can_ReturnType Can_SetControllerMode (uint8 Controller,Can_StateTransitionType Transition) {
	
	
}



Std_ReturnType Can_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID )
{   /* comment!*********************************************[SWS_CAN_00491]*********************************************************

    Sync/Async:         Synchronous
    Reentrancy:         Reentrant for different Controllers. Non reentrant for the same Controller.

    Parameters (in):    Controller CAN controller, whose baud rate shall be set.
                        BaudRateConfigID references a baud rate configuration by ID (see CanControllerBaudRateConfigID)

    Parameters (out):   None

    Return value:       Std_ReturnType E_OK: Service request accepted, setting of (new) baud rate started
                                       E_NOT_OK: Service request not accepted

    Description:        This service shall set the baud rate configuration of the CAN controller.
                        Depending on necessary baud rate modifications the controller might have to reset.

     *******************************************************************************************************************************/

    uint8 BaudRateIndex;
    tCANBitClkParms  CanBitTimingParameters;

    if(CanDriverState == CAN_READY)
    {
                if(Controller >= NUMBER_OF_CONTROLLERS)
                {
                        /*[SWS_CAN_00494]? If default error detection for the Can module is enabled
                          the function Can_SetBaudrate shall raise the error CAN_E_PARAM_CONTROLLER
                          and return E_NOT_OK if the parameter Controller is out of range.?()*/

                        CanDevolpmentError = CAN_E_PARAM_CONTROLLER;

                                return E_NOT_OK;
                }
                else
                {
                    if(/*ControllerStatus == STOPPED*/)
                    {
                        if(BaudRateConfigID < NUMBER_OF_BAUDRATES)
                        {
                            for(BaudRateIndex = 0 ; BaudRateIndex < NUMBER_OF_BAUDRATES ; BaudRateIndex++)
                            {
                                    if(Config->CanConfigSet.CanController[Controller].CanControllerBaudrateConfig[BaudRateIndex]->CanControllerBaudRateConfigID == BaudRateConfigID)
                                    {
                                       
                                                CanBitTimingParameters.ui32SyncPropPhase1Seg = Config->CanConfigSet.CanController[Controller].CanControllerBaudrateConfig[BaudRateIndex]->CanControllerPropSeg +\
                                                                                               Config->CanConfigSet.CanController[Controller].CanControllerBaudrateConfig[BaudRateIndex]->CanControllerSeg1 + 1U ;

                                                CanBitTimingParameters.ui32Phase2Seg         = Config->CanConfigSet.CanController[Controller].CanControllerBaudrateConfig[BaudRateIndex]->CanControllerSeg2;

                                                CanBitTimingParameters.ui32SJW               = Config->CanConfigSet.CanController[Controller].CanControllerBaudrateConfig[BaudRateIndex]->CanControllerSyncJumpWidth;

                                                CanBitTimingParameters. ui32QuantumPrescaler = SysCtlClockGet()/(CanBitTimingParameters.ui32SyncPropPhase1Seg + CanBitTimingParameters.ui32Phase2Seg)*\
                                                                                               (Config->CanConfigSet.CanController[index1].CanControllerDefaultBaudRate->CanControllerBaudRate * 1000U);

                                                CANBitTimingSet(Config->CanConfigSet.CanController[Controller].CanControllerBaseAddress , CanBitTimingParameters);

                                                        return E_OK;
                                       
                                    }
                                    else
                                    {
                                        /*erorr not BaudRateConfigID not in range*/
                                                      /*Misra*/
                                    }
                            }
                        }
                        else
                        {
                                /*[SWS_CAN_00493]? If default error detection for the Can module is enabled:
                                  The function Can_SetBaudrate shall raise the error CAN_E_PARAM_BAUDRATE
                                  and return E_NOT_OK if the parameter BaudRateConfigID has an invalid value.?()*/

                                        CanDevolpmentError = BaudRateConfigID;

                                                return E_NOT_OK;
                        }
         
                    }
                    else
                    {
							/*erorr not stopped*/
								return E_NOT_OK;
                    }
                }
    }
    else
    {
            /*comment!
            [SWS_CAN_00492]? If default error detection for the Can module is enabled:
            The function Can_SetBaudrate shall raise the error CAN_E_UNINIT and return
            E_NOT_OK if the driver is not yet initialized.?()*/

            CanDevolpmentError = CAN_E_UNINIT;

                return E_NOT_OK;
    }
}


