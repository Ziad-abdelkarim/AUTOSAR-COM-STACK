/********************************************************************************************************************************
**                                                                       includes                                                                                         **
********************************************************************************************************************************/
#include "Can_GeneralTypes.h"
/********************************************************************************************************************************
**                                                     Type definitions                                                                                         **
********************************************************************************************************************************/
typedef struct{
	uint8_t MessageObjectNumber,
	bool Confirmation,
	PduIdType PduId
}MessageObjectType;

MessageObjectType MessageObject[NUMBER_OF_CONTROLLERS][NUM_OF_HOH][MAX_NUMBER_OF_HANDLERS];

/********************************************************************************************************************************
**                                                     Global Variables                                                                                         **
********************************************************************************************************************************/
static MessageObjectType MessageObject[NUMBER_OF_CONTROLLERS][NUM_OF_HOH][MAX_NUMBER_OF_HANDLERS];
/*********************************************************************************************************************************       
Service name:                                       Can_Init
Service ID[hex]:                                    0x00
Sync/Async:                                         Synchronous
Reentrancy:                                         Non Reentrant
Parameters (in):                                    Config ---> Pointer to driver configuration.                                            
Parameters (inout):                                 None
Parameters (out):                                   None
Return value:                                       None
Description:										This function initializes the module.
*********************************************************************************************************************************/
void Can_Init(const Can_ConfigType *Config)
{
	uint32_t CanSysCtlPeripheral = {
		SYSCTL_PERIPH_CAN0,
		SYSCTL_PERIPH_CAN1
	}
	uint8_t ControllerIndex, HardwareObjectIndex, ObjectIndex;
	uint8_t MessageObjectNumber[NUMBER_OF_CONTROLLERS] = {1,1};
	bool UninitCheck = true;
	tCANBitClkParms CANBitClkParms;
	tCANMsgObject CANMsgObject;
	CanObjectType HardwareObjectType;
	
	/* [SWS_Can_00174] If default error detection for the Can module is enabled: The
		function Can_Init shall raise the error CAN_E_TRANSITION if the driver is not in
		state CAN_UNINIT. */
	if(CanDriverState != CAN_UNINIT)
	{
		CanDevelopmentError = CAN_E_TRANSITION;
	}
	else
	{
		/* [SWS_Can_00408] If default error detection for the Can module is enabled: The
			function Can_Init shall raise the error CAN_E_TRANSITION if the CAN controllers
			are not in state UNINIT. */
		for(ControllerIndex=0U; ControllerIndex < NUMBER_OF_CONTROLLERS; ControllerIndex++)
		{
			if(CanControllerState[ControllerIndex] != CAN_CS_UNINIT)
			{
				UninitCheck = false;
			}
			else
			{
				//MISRA
			}
		}

		if(UninitCheck == true)
		{
			/* [SWS_Can_00245] The function Can_Init shall initialize all CAN controllers
				according to their configuration. (SRS_SPAL_12057, SRS_Can_01041) */
				
				
			// Loop over all controllers
			for(ControllerIndex=0U; ControllerIndex < NUMBER_OF_CONTROLLERS; ControllerIndex++)
			{
				// Check if controller needs to be activated
				if(Config->CanConfigSet.CanController[ControllerIndex].CanControllerActivation)
				{	
					// Enable Can peripheral clock
					SysCtlPeripheralEnable(CanSysCtlPeripheral[ControllerIndex]);
					// Wait for clock to be enabled
					while(!SysCtlPeripheralReady(CanSysCtlPeripheral[ControllerIndex]));
					
					// Initialize Can controller
					CANInit(Config->CanConfigSet.CanController[ControllerIndex].CanControllerBaseAddress);
					
					// Set bit timing values
					CANBitClkParms.ui32SyncPropPhase1Seg = Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudRate->CanControllerPropSeg +\
																Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudRate->CanControllerSeg1 + (uint32_t)1;
					CANBitClkParms.ui32Phase2Seg = Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudRate->CanControllerSeg2;
					CANBitClkParms.ui32SJW = Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudRate->CanControllerSyncJumpWidth;
					CANBitClkParms.ui32QuantumPrescaler =  SysCtlClockGet() / (CANBitClkParms.ui32SyncPropPhase1Seg + CANBitClkParms.ui32Phase2Seg) *\
																	(Config->CanConfigSet.CanController[ControllerIndex].CanControllerDefaultBaudRate->CanControllerBaudRate * 1000U);
					// Set bit timing
					CANBitTimingSet(Config->CanConfigSet.CanController[ControllerIndex].CanControllerBaseAddress, CANBitClkParms);
					
					/* [SWS_Can_00259] The function Can_Init shall set all CAN controllers in the state STOPPED. */
					Can_SetControllerMode(ControllerIndex, CAN_T_STOP);
				}
				else
				{
					//MISRA
				}
				
			}
			
			// Loop over all HOH
			for(HardwareObjectIndex = 0; HardwareObjectIndex < NUMBER_OF_HOH ; HardwareObjectIndex++)
			{
				// Get HOH's Controller ID
				ControllerIndex = Config->CanConfigSet.CanHardwareObject[HardwareObject].CanControllerRef->CanControllerId;
				// Get HOH's Object Type
				HardwareObjectType = Config->CanConfigSet.CanHardwareObject[HardwareObject].CanObjectType;
				
				// Loop over all HTH or HRH
				for(ObjectIndex = 0; ObjectIndex < Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHwObjectCount; ObjectIndex++)
				{
					// Assign a message object to the HTH or HRH
					MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].MessageObjectNumber = MessageObjectNumber[ControllerIndex];
					// Set Message Objects as Free
					MessageObject[ControllerIndex][HardwareObjectIndex][ObjectIndex].Confirmation = true;
					// If the HOH type was receive then additional operations must be done
					if(HardwareObjectType == receive)
					{
						// Set HRH values
						CANMsgObject.ui32MsgID = Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHWFilter.CanHwFilterCode;
						CANMsgObject.ui32MsgIDMask = Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanHWFilter.CanHwFilterMask;
						CANMsgObject.ui32MsgLen = 8U;
						
						if(Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex]CanIdType == STANDARD)
						{
							CANMsgObject.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER | MSG_OBJ_FIFO;
						}
						else if(Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanIdType == EXTENDED)
						{
							CANMsgObject.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER | MSG_OBJ_EXTENDED_ID | MSG_OBJ_USE_EXT_FILTER | MSG_OBJ_FIFO;
						}
						else
						{
							//MISRA
						}
						
						// Make controller receive on this message object
						CANMessageSet(Config->CanConfigSet.CanHardwareObject[HardwareObjectIndex].CanControllerRef->CanControllerBaseAddress,
									MessageObjectNumber[ControllerIndex],
									&CANMsgObject, MSG_OBJ_TYPE_RX);	
					}
					else
					{
						//MISRA
					}
					
					// Increment message object number for next HTH or HRH
					MessageObjectNumber[ControllerIndex] ++;
				}
			}
			
			/* [SWS_Can_00246] The function Can_Init shall change the module state to
				CAN_READY, after initializing all controllers inside the HW
				Unit.(SRS_SPAL_12057, SRS_Can_01041) */
			CanDriverState = CAN_READY;
		}
		else
		{
			CanDevelopmentError = CAN_E_TRANSITION;
		}
	}
}












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
