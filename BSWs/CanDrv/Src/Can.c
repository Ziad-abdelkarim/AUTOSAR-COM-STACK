/******************************************************************************************************************************
Service name:                                             Can_MainFunction_Write
Service ID[hex]:                                                   0x01
Description:                    This function performs the polling of TX indications when CAN_TX_PROCESSING is set to POLLING.
*******************************************************************************************************************************/
/********************************************************************************************************************************
                                                          Type definitions
********************************************************************************************************************************/
#define CHECK_BIT(REG,POS) ((REG) & (1<<(POS)))
/*********************************************************************************************************************************
                                                             includes
*********************************************************************************************************************************/
#include "Can_GeneralTypes.h"
/********************************************************************************************************************************/
#if(CanTxProcessing0==INTERRUPT && CanTxProcessing1==INTERRUPT)
               void Can_MainFunction_Write(){
                    //empty define function in interrupt mode
                }
#elif(CanTxProcessing0==POLLING || CanTxProcessing1==POLLING)

    void Can_MainFunction_Write(){


        uint8 INDEX_HOH;
        uint8 INDEX_MESGOBJ;

        #if(CanTxProcessing0==POLLING)

           if(Can.CanConfigSet.CanController[0].CanControllerActivation){
                for(INDEX_HOH=0;INDEX_HOH<NUMBER_OF_HOH;INDEX_HOH++){
                    if(Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanControllerRef== &Can.CanConfigSet.CanController[0]){
                        if(Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanObjectType==transmit){
                          for(INDEX_MESGOBJ=0;INDEX_MESGOBJ<=Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanHwObjectCount;INDEX_MESGOBJ++){
                             if(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber <= 16){
                                if(!(CHECK_BIT(CAN0_BASE+CAN_O_TXRQ1,(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber-1)))){

                                    /*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                   successful transmission.It shall either called by the TX-interrupt service routine
                                   of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                   polling mode.*/

                                    MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].confirmation=true;
                                    //CanIf_TxConfirmation()

                                }else{

                                    /*7.8 Notification concept*/

                                    /*CanDrv must store the all in HTHs pending L-PDU Ids in an array
                                    organized perTH to avoid new search of the L-PDU ID for call of

                                     */
                                    MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].confirmation=false;
                                    //CanIf_TxConfirmation(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].PduId)

                                }


                            }else{

                                if(!(CHECK_BIT(CAN0_BASE+CAN_O_TXRQ2,(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber)-1))){

                                    /*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                   successful transmission.It shall either called by the TX-interrupt service routine
                                   of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                   polling mode.*/

                                    MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].confirmation=true;
                                    //CanIf_TxConfirmation().

                                }else{

                                    /*7.8 Notification concept*/

                                    /*CanDrv must store the all in HTHs pending L-PDU Ids in an array
                                    organized perTH to avoid new search of the L-PDU ID for call of

                                     */
                                    MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].confirmation=fasle;
                                    //CanIf_TxConfirmation(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].PduId)

                                }

                            }

                          }

                        }else{/*misra error*/}
                    }else{/*misra error*/}
                  }
               }else{/*misra error*/}

               #else //misra error
               #endif


        #if(CanTxProcessing1==POLLING)

           if(Can.CanConfigSet.CanController[1].CanControllerActivation){

                for(INDEX_HOH=0;INDEX_HOH<NUMBER_OF_HOH;INDEX_HOH++){
                    if(Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanControllerRef== &Can.CanConfigSet.CanController[1]){
                        if(Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanObjectType==transmit){
                          for(INDEX_MESGOBJ=0;INDEX_MESGOBJ<=Can.CanConfigSet.CanHardwareObject[INDEX_HOH].CanHwObjectCount;INDEX_MESGOBJ++){
                           //if(MessageObject[0][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber){}
                             if(MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber <= 16){
                                if(!(CHECK_BIT(CAN1_BASE+CAN_O_TXRQ1,(MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber-1)))){

                                    /*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                   successful transmission.It shall either called by the TX-interrupt service routine
                                   of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                   polling mode.*/

                                    MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].confirmation=true;
                                    //CanIf_TxConfirmation().

                                }else{

                                    /*7.8 Notification concept*/

                                    /*CanDrv must store the all in HTHs pending L-PDU Ids in an array
                                    organized perTH to avoid new search of the L-PDU ID for call of

                                     */
                                    MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].confirmation=fasle;
                                    //CanIf_TxConfirmation(MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].PduId)

                                }


                            }else{

                                if(!(CHECK_BIT(CAN1_BASE+CAN_O_TXRQ2,(MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].MessageObjectNumber)-1))){

                                    /*[SWS_Can_00016] ⌈The Can module shall call CanIf_TxConfirmation to indicate a
                                   successful transmission.It shall either called by the TX-interrupt service routine
                                   of the corresponding HW resource or inside the Can_MainFunction_Write in case of
                                   polling mode.*/

                                    MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].confirmation=true;
                                    //CanIf_TxConfirmation().

                                }else{

                                    /*7.8 Notification concept*/

                                    /*CanDrv must store the all in HTHs pending L-PDU Ids in an array
                                    organized perTH to avoid new search of the L-PDU ID for call of

                                     */

                                    MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].confirmation=fasle;
                                    //CanIf_TxConfirmation(MessageObject[1][INDEX_HOH][INDEX_MESGOBJ].PduId)

                                }
                              }
                          }

                        }else{/*misra error*/}
                    }else{/*misra error*/}
                  }
               }else{/*misra error*/}

        #else //misra error
        #endif

        }

#else //misra error
#endif
