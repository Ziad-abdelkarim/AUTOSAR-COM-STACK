

uint8_t CanDevolpmentErrorType;
static uint8_t CanStateType= CAN_UNINIT;
static uint8_t InterruptDisableCount[NUMBER_OF_CAN_CONTROLLER ];
static uint8_t InterruptEnableCount[NUMBER_OF_CAN_CONTROLLER ];

/***************************************************************************************/
/* Description! This function disables all interrupts for this CAN controller.         */
/* Input      !number of  Controllers                                                  */
/* Output     ! Nothing                                                                */
/***************************************************************************************/
void Can_DisableControllerInterrupts(uint8_t Controller)
{
    if (CanStateType!= CAN_READY) //check if can driver is not ready
    {
        CanDevolpmentErrorType = CAN_E_UNINIT; // raise the error CAN_E_UNINIT if the driver not yet initialized
    }
    else
    {
        if (Controller >= NUMBER_OF_CAN_CONTROLLER) //check parameter Controller is out of range
        {
            //CanDevolpmentErrorType = CAN_E_PARAM_CONTROLLER; //raise the error CAN_E_PARAM_CONTROLLER if the parameter Controller is out of range
        }
        else
        { //not found any error {the driver not yet initialized or parameter Controller is out of range}
            InterruptDisableCount[Controller]++; // increase a counter to make disable Controller Interrupts
            if (Controller == CAN_CONTROLLERS_ONE) //check which controllers (Controller0 or Controller1)
            { //Disable the CAN0 interrupt if it is enabled.
                if (IntIsEnabled(INT_CAN0)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_EnableControllerInterrupts has been called several times,
                     *When Can_DisableControllerInterrupts must be called as many times before the interrupts are disabled. */
                    if (InterruptEnableCount[Controller] >= ONE) //check how many times you make to disable Controller Interrupts
                    {
                        InterruptEnableCount[Controller]--; //incremental enable count until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptEnableCount[Controller] == ZERO)
                    { //Disables the specified CAN controller interrupt sources. Only enabled interrupt sources can cause a processor interrupt.
                        CANIntDisable(
                                CAN0_BASE,
                                CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                        // disable interrupts for CAN0
                        IntDisable(INT_CAN0);
                    }
                    else
                    {
                        //must call this function until make InterruptEnableCount[Controller] =ZERO to make disable Controller Interrupts
                    }
                }
                else
                {
                    //it is disabled
                }
            }
            else
            { //we use here controller1
              //Disable the CAN1 interrupt if it is enabled.
                if (IntIsEnabled(INT_CAN1)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_EnableControllerInterrupts has been called several times,
                     *When Can_DisableControllerInterrupts must be called as many times before the interrupts are disabled. */
                    if (InterruptEnableCount[Controller] >= ONE) //check how many times you to make disable Controller Interrupts
                    {
                        InterruptEnableCount[Controller]--; //incremental enable count until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptEnableCount[Controller] == ZERO)
                    { //Disables the specified CAN controller interrupt sources. Only enabled interrupt sources can cause a processor interrupt.
                        CANIntDisable(
                                CAN1_BASE,
                                CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                        // enable to disable interrupts for CAN1
                        IntDisable(INT_CAN1);
                    }
                    else
                    {
                        //must call this function until make InterruptEnableCount[Controller] =ZERO to make disable
                    }
                }
                else
                {
                    //it is disabled
                }
            }
        }
    }
}

/***************************************************************************************/
/* Description! This function enables all allowed interrupts.                          */
/* Input      !number of  Controllers                                                  */
/* Output     ! Nothing                                                                */
/***************************************************************************************/
void Can_EnableControllerInterrupts(uint8_t Controller)
{
    if (CanStateType!= CAN_READY) //check if can driver is not ready
    {
        CanDevolpmentErrorType = CAN_E_UNINIT; // raise the error CAN_E_UNINIT if the driver not yet initialized
    }
    else
    {
        if (Controller >= NUMBER_OF_CAN_CONTROLLER) //check parameter Controller is out of range
        {
            //CanDevolpmentErrorType = CAN_E_PARAM_CONTROLLER; //raise the error CAN_E_PARAM_CONTROLLER if the parameter Controller is out of range
        }
        else
        { //not found any error {the driver not yet initialized or parameter Controller is out of range}
            InterruptEnableCount[Controller]++; // increase a counter to make enable
            if (Controller == CAN_CONTROLLERS_ONE) //check which controllers (Controller0 or Controller1)
            {
                //enable the CAN0 interrupt if it is disabled.
                if (!IntIsEnabled(INT_CAN0)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_DisableControllerInterrupts has been called several times,
                     *When  Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled. */
                    if (InterruptDisableCount[Controller] >= ONE) //check how many times you make to enable Controller Interrupts
                    {
                        InterruptDisableCount[Controller]--; //incremental disable count until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptDisableCount[Controller] == ZERO)
                    { //This function enables specific interrupt sources of the CAN controller. Only enabled sources cause a processor interrupt.
                        CANIntEnable(
                                CAN0_BASE,
                                CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                        // enable interrupts for CAN0
                        IntEnable(INT_CAN0);
                    }
                    else
                    {
                        //must call this function until make InterruptDisableCount[Controller]=ZERO to make enable Controller Interrupts
                    }
                }
                else
                {
                    //it is enabled
                }
            }
            else
            { //we use here controller1
              //enable the CAN1 interrupt if it is disabled.
                if (!IntIsEnabled(INT_CAN1)) //Returns TURE if a peripheral interrupt is enabled.
                {
                    /*When Can_DisableControllerInterrupts has been called several times,
                     *When  Can_EnableControllerInterrupts must be called as many times before the interrupts are re-enabled. */
                    if (InterruptDisableCount[Controller] >= ONE) //check how many times you make to enable Controller Interrupts
                    {
                        InterruptDisableCount[Controller]--; //incremental disable count  until reach to zero
                    }
                    else
                    {
                        //MISRA
                    }
                    if (InterruptDisableCount[Controller] == ZERO)
                    { //This function enables specific interrupt sources of the CAN controller. Only enabled sources cause a processor interrupt.
                        CANIntEnable(
                                CAN1_BASE,
                                CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
                        // enable  interrupts for CAN1
                        IntEnable(INT_CAN1);
                    }
                    else
                    {
                        //must call this function until make InterruptDisableCount[Controller] =ZERO to make enable Controller Interrupts
                    }
                }
                else
                {
                    //it is enabled
                }
            }
        }
    }
}