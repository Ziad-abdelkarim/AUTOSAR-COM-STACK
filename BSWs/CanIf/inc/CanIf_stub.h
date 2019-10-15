#ifndef CANIF_H
#define CANIF_H
#include "Can_GeneralTypes.h"
void CanIf_ControllerBusOff(uint8 ControllerId);
void CanIf_TxConfirmation(PduIdType CanTxPduId);
void CanIf_RxIndication(const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr);

#endif /*CANIF_H*/
