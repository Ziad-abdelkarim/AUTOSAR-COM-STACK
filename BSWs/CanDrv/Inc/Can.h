
Std_ReturnType Can_SetBaudrate( uint8 Controller, uint16 BaudRateConfigID );
/* comment!
					[SWS_CAN_00491]
					
Sync/Async:  		Synchronous
Reentrancy:			Reentrant for different Controllers. Non reentrant for the same Controller.					

Parameters (in):	Controller CAN controller, whose baud rate shall be set.
                    BaudRateConfigID references a baud rate configuration by ID (see CanControllerBaudRateConfigID)
					
Parameters (out): 	None	

Return value:		Std_ReturnType E_OK: Service request accepted, setting of (new) baud rate started 
                                   E_NOT_OK: Service request not accepted	

Description: 		This service shall set the baud rate configuration of the CAN controller.
					Depending on necessary baud rate modifications the controller might have to reset.								   

 */
