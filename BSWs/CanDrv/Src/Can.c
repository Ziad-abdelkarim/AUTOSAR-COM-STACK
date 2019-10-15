/********************************************************************************************************************************
**                                                                       includes                                                                                        															   **
********************************************************************************************************************************/
#include "Can_GeneralTypes.h"
#include "Can_MainFunctionRead.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Std_Types.h"
#include "driverlib/can.h"
#include "Can.h"
#include <inc/hw_can.h>
#include <inc/tm4c123gh6pm.h>
#include "Can_GeneralTypes.h"
#include "Platform_Types.h"
#include "ComStack_Types.h"




/************************************************************************************
Name:                                 MessageObjectType
Description:
                   This container contains the configuration (parameters) of CAN Message Objects
                               (PduId ,Confirmation Flag and Message object Number).
Multiplicity:                                       1..*
Type:                                       Container
 *************************************************************************************/

typedef struct{
    uint8 MessageObjectNumber;
    boolean Confirmation;
    PduIdType PduId;
}MessageObjectType;

MessageObjectType MessageObject[NUMBER_OF_CONTROLLERS][NUMBER_OF_HOH][MAX_NUMBER_OF_HANDLERS];




/*
 * [SWS_Can_00108] The function Can_MainFunction_Read shall perform the polling of RX indications when CanRxProcessing
 *             is set to POLLING or mixed.In case of mixed processing only the hardware objects for which CanHardwareObjectUses
 *              Polling is set to TRUE shall be polled.(SRS_BSW_00432, SRS_SPAL_00157)
 */
 
 
/*********************************************************************************************************************************       
Service name:                                       Can_MainFunction_Read0
Service ID[hex]:                                                   0x08

Parameters (in):                                    				void
Parameters (inout):                                               None
Parameters (out):                                                  void
Return value:                                                		void
Description:   
                                  This function performs the polling of RX indications when CAN_RX_PROCESSING is set to POLLING.
*******************************************************************************************************************************/






#if ( Can0RxProcessing == POLLING )
void Can_MainFunction_Read0(void)
{

    /*************************************************************************************
     *                                    Local Variables                                *
     *************************************************************************************/

    uint8 HOH_Num_Iterator = 0;
    uint8 HW_Obj_Counter   = 0;
    uint8 ReceivedMsgArray0[16U];
    tCANMsgObject CanMsgReceived ;                          /* The structure used for encapsulating all the items associated with
                                                               a CAN message object in the CAN controller.*/

    Can_HwType ReceiverMailBox ;                            /*A data structure(MailBox) which clearly provides an Hardware Object Handle
                                                              including its corresponding CAN Controller and therefore CanDrv
                                                              as well as the specific CanId.*/
    PduInfoType ReceiverPduInfo;                            /*used to store the basic information about a PDU of any type,
                                                              namely a pointer variable pointing to its SDU (Payload),
                                                              and the corresponding length of the SDU in bytes.*/

    CanMsgReceived.pui8MsgData=ReceivedMsgArray0;           /*pointer of Data in a message is pointing to an Array where data will be stored*/

    extern CAN Can;
    /*
     *check if if the CAN controller 0 is used in the configuration.
     */
    if(Can.CanConfigSet.CanController[CanController0].CanControllerActivation)
    {

        /*
         * Reset 7th bit in CANIF1CMSK to Transfer the data in the CAN message object specified by
           the MNUM field in the CANIFnCRQ register into the CANIFn registers.
         */
        RstBit((CAN0_IF1CMSK_R) ,WRNRD_Bit);
        RstBit((CAN0_IF1CMSK_R),CANIF1CMSK_TXRQST_Bit);

        /*
         * loop for the number of hardware objects which are 32 in TIVA C
         */

        for(HOH_Num_Iterator= 0U ; HOH_Num_Iterator < NUMBER_OF_HOH ; HOH_Num_Iterator++)
        {
            /*
             * check if status of the HardwareObject is used as Receive object.
             */

            if(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanObjectType == RECEIVE)
            {
                /*
                 * check if the Reference of CAN Controller 0 to which the HOH is associated to is the same as
                 * Configurations of can controller 0.
                 */

                if(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef == & Can.CanConfigSet.CanController[CanController0])
                {
                    /*
                     *iterate on number of Message objects  in each hardware object
                     */

                    for(HW_Obj_Counter = 0U ; HW_Obj_Counter < Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanHwObjectCount;HW_Obj_Counter++)
                    {
                        /*
                         *Selects one of the 32 message objects in the message RAM for data transfer. The message objects are numbered from 1 to 32.
                         */
                        (CAN0_IF1CRQ_R)= MessageObject[CanController0][HOH_Num_Iterator][HW_Obj_Counter].MessageObjectNumber ;

                        /*
                         * check if NEWDAT bit may have two values :
                         * bit == 0 :No new data has been written into the data portion of this message object by the message handler,
                         *  since the last time this flag was cleared by the CPU.
                         * bit == 1 :The message handler or the CPU has written new data into the data portion of this message object.
                         */
                        if(GetBit((CAN0_IF1MCTL_R),NEWDAT_Bit) == STD_HIGH)
                        {
                            /*
                             * [SWS_Can_00279] On L-PDU reception, the Can module shall call the RX indication
                             * callback function CanIf_RxIndication with ID, HOH, abstract CanIf ControllerId in parameter Mailbox,
                             * and the DLC and pointer to the L-SDU buffer in parameter PduInfoPtr.(SRS_Can_01045)
                             */


                            /*******************************************************************************************************|
                                 |* Name:                                 CanMessageGet                                                *|
                                 |* Description:                                                                                       *|
                                 |* Reads a CAN message from one of the message object buffers.                                        *|
                                 |* Prototype:                                                                                         *|
                                 |*                   void CANMessageGet(uint32_t ui32Base,uint32_t ui32ObjID,                         *|
                                 |*                   tCANMsgObject *psMsgObject,bool bClrPendingInt)                                  *|
                                 |*                                                                                                    *|
                                 |* INPUT Parameters:                                                                                  *|
                                 |*                  ui32Base:           is the base address of the CAN controller.                    *|
                                 |*                  ui32ObjID:          is the object number to read (1-32).                          *|
                                 |*                  psMsgObject:        points to a structure containing message object fields.       *|
                                 |*                  bClrPendingInt:     indicates whether an associated interrupt should be cleared.  *|
                                 |*                                                                                                    *|
                                 |******************************************************************************************************/
                            CANMessageGet(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef ->CanControllerBaseAddress,
                                          MessageObject[CanController0][HOH_Num_Iterator][HW_Obj_Counter].MessageObjectNumber,
                                          &CanMsgReceived, STD_OFF);

                            ReceiverMailBox.CanId= CanMsgReceived.ui32MsgID;
                            ReceiverMailBox.ControllerId=Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef -> CanControllerId;
                            ReceiverMailBox.Hoh=Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanObjectId;
                            ReceiverPduInfo.SduDataPtr=CanMsgReceived.pui8MsgData;
                            ReceiverPduInfo.SduLength=CanMsgReceived.ui32MsgLen;

                            CanIf_RxIndication(&ReceiverMailBox, &ReceiverPduInfo);

                        }
                        else
                        {
                            /*MISRA*/
                        }
                    }
                }
                else
                {
                    /*MISRA*/
                }
            }
            else
            {
                /*MISRA*/
            }
        }
    }
    else
    {
        /*MISRA*/
    }
}

#endif






/*
 * Description:[SWS_Can_00108] The function Can_MainFunction_Read shall perform the polling of RX indications when CanRxProcessing
                is set to POLLING or mixed.In case of mixed processing only the hardware objects for which CanHardwareObjectUses
                Polling is set to TRUE shall be polled.(SRS_BSW_00432, SRS_SPAL_00157)
 * 
 */

/*********************************************************************************************************************************       
Service name:                                       Can_MainFunction_Read1
Service ID[hex]:                                                   0x08

Parameters (in):                                    				void
Parameters (inout):                                               None
Parameters (out):                                                  void
Return value:                                                		void
Description:   
                                  This function performs the polling of RX indications when CAN_RX_PROCESSING is set to POLLING.
*******************************************************************************************************************************/



#if ( Can1RxProcessing == POLLING )

void Can_MainFunction_Read1(void)
{
    /*************************************************************************************
     **                                    Local Variables                                       *
     *************************************************************************************/

    uint8 HOH_Num_Iterator = 0;
    uint8 HW_Obj_Counter   = 0;
    uint8 ReceivedMsgArray0[16U];
    tCANMsgObject CanMsgReceived ;                          /* The structure used for encapsulating all the items associated with
                                                               a CAN message object in the CAN controller.*/

    Can_HwType ReceiverMailBox ;                            /*A data structure(MailBox) which clearly provides an Hardware Object Handle
                                                              including its corresponding CAN Controller and therefore CanDrv
                                                              as well as the specific CanId.*/
    PduInfoType ReceiverPduInfo;                            /*used to store the basic information about a PDU of any type,
                                                              namely a pointer variable pointing to its SDU (Payload),
                                                              and the corresponding length of the SDU in bytes.*/

    CanMsgReceived.pui8MsgData=ReceivedMsgArray0;           /*pointer of Data in a message is pointing to an Array where data will be stored*/
    extern CAN Can;

    /*
     *check if if the CAN controller 1 is used in the configuration.
     */
    if(Can.CanConfigSet.CanController[CanController1].CanControllerActivation)
    {

        /*
         * Reset 7th bit in CANIF1CMSK to Transfer the data in the CAN message object specified by
           the MNUM field in the CANIFnCRQ register into the CANIFn registers.
         */
        RstBit((CAN0_IF1CMSK_R) ,WRNRD_Bit);
        RstBit((CAN0_IF1CMSK_R),CANIF1CMSK_TXRQST_Bit);

        /*
         * loop for the number of hardware objects which are 32 in TIVA C
         */

        for(HOH_Num_Iterator= 0U ; HOH_Num_Iterator < NUMBER_OF_HOH ; HOH_Num_Iterator++)
        {
            /*
             * check if status of the HardwareObject is used as Receive object.
             */

            if(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanObjectType == RECEIVE)
            {
                /*
                 * check if the Reference of CAN Controller 0 to which the HOH is associated to is the same as
                 * Configurations of can controller 1.
                 */

                if(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef == & Can.CanConfigSet.CanController[CanController1])
                {
                    /*
                     *iterate on number of Message objects  in each hardware object
                     */

                    for(HW_Obj_Counter = 0U ; HW_Obj_Counter < Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanHwObjectCount;HW_Obj_Counter++)
                    {
                        /*
                         *Selects one of the 32 message objects in the message RAM for data transfer. The message objects are numbered from 1 to 32.
                         */
                        (CAN0_IF1CRQ_R)= MessageObject[CanController1][HOH_Num_Iterator][HW_Obj_Counter].MessageObjectNumber ;

                        /*
                         * check if NEWDAT bit may have two values :
                         * bit == 0 :No new data has been written into the data portion of this message object by the message handler,
                         *           since the last time this flag was cleared by the CPU.
                         * bit == 1 :The message handler or the CPU has written new data into the data portion of this message object.
                         */
                        if(GetBit((CAN0_IF1MCTL_R),NEWDAT_Bit) == STD_HIGH)
                        {
                            /*
                             * [SWS_Can_00279] On L-PDU reception, the Can module shall call the RX indication
                             * callback function CanIf_RxIndication with ID, HOH, abstract CanIf ControllerId in parameter Mailbox,
                             * and the DLC and pointer to the L-SDU buffer in parameter PduInfoPtr.(SRS_Can_01045)
                             */


                            /*******************************************************************************************************|
                                 |* Name:                                 CanMessageGet                                                *|
                                 |* Description:                                                                                       *|
                                 |* Reads a CAN message from one of the message object buffers.                                        *|
                                 |* Prototype:                                                                                         *|
                                 |*                   void CANMessageGet(uint32_t ui32Base,uint32_t ui32ObjID,                         *|
                                 |*                   tCANMsgObject *psMsgObject,bool bClrPendingInt)                                  *|
                                 |*                                                                                                    *|
                                 |* INPUT Parameters:                                                                                  *|
                                 |*                  ui32Base:           is the base address of the CAN controller.                    *|
                                 |*                  ui32ObjID:          is the object number to read (1-32).                          *|
                                 |*                  psMsgObject:        points to a structure containing message object fields.       *|
                                 |*                  bClrPendingInt:     indicates whether an associated interrupt should be cleared.  *|
                                 |*                                                                                                    *|
                                 |******************************************************************************************************/
                            CANMessageGet(Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef ->CanControllerBaseAddress,
                                          MessageObject[CanController1][HOH_Num_Iterator][HW_Obj_Counter].MessageObjectNumber,
                                          &CanMsgReceived, STD_OFF);

                            ReceiverMailBox.CanId= CanMsgReceived.ui32MsgID;
                            ReceiverMailBox.ControllerId=Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanControllerRef -> CanControllerId;
                            ReceiverMailBox.Hoh=Can.CanConfigSet.CanHardwareObject[HOH_Num_Iterator].CanObjectId;
                            ReceiverPduInfo.SduDataPtr=CanMsgReceived.pui8MsgData;
                            ReceiverPduInfo.SduLength=CanMsgReceived.ui32MsgLen;

                            CanIf_RxIndication(&ReceiverMailBox, &ReceiverPduInfo);

                        }
                        else
                        {
                            /*MISRA*/
                        }
                    }
                }
                else
                {
                    /*MISRA*/
                }
            }
            else
            {
                /*MISRA*/
            }
        }
    }
    else
    {
        /*MISRA*/
    }


}

#endif












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
