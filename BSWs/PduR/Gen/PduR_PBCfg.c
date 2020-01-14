/*******************************************************************************************************************************
FileName:                    							PduR_PBcfg.c
AUTOSAR Version:          								  4.2.2
******************************************************************************************************************************/
/******************************************************************************************************************************
 **                                                     Includes                                                             **
 ******************************************************************************************************************************/
 #include " PduR.h"
 #include " PduR_PBCfg.h"

 /*****************************************************************************************************************************
  **                      					Post-Build Configuration variables values                                       **
  *****************************************************************************************************************************/
PduR_PBConfigType PduR = {

                            .PduRRoutingTables=
                            {
                              .PduRRoutingTable=
                              {
                                {
                                  .PduRRoutingPath=
                                  {
                                    {
                                      .PduRDestPdu=
                                      {
                                          {
                                          .PduRDestPduDataProvision=PDUR_DIRECT;
                                          //.PduRDestPduHandleId=
                                          .PduRTransmissionConfirmation= true;

                                          }
                                      },
                                      .PduRSrcPdu=
                                        {
                                          //.PduRSourcePduHandleId=
                                          .PduRSrcPduUpTxConf=true;

                                        }
                                     },
                                    {
                                      .PduRDestPdu={
                                        {
                                          .PduRDestPduDataProvision=PDUR_DIRECT;
                                          //.PduRDestPduHandleId=
                                          .PduRTransmissionConfirmation= true;

                                        }
                                      },
                                      .PduRSrcPdu=
                                      {
                                        //.PduRSourcePduHandleId=
                                        .PduRSrcPduUpTxConf=true;

                                      }
                                    }
                                  }
                                  }
                                }
                              }
                          }
