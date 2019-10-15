
/***************************************************************************************************
**

FileName:                          ComStack_Types.h                                      
AUTOSAR Version:            4.2.2
																																				  **
***************************************************************************************************/
#ifndef COMSTACK_TYPES_H
#define COMSTACK_TYPES_H
/***************************************************************************************************
**                                          Includes                                                       					             **
***************************************************************************************************/
#include "Std_Types.h"

/***************************************************************************************************
**                                        Types Declerations                                                                       **
***************************************************************************************************/

/**************************************************************************************************
**
Name:                                    PduIdType
Type:                                     uint8, uint16
Description:  
                     Variables of this type serve as a unique identifier of a
					PDU within a software module or a set thereof, and also for interaction of two
					software modules where the PduId of the corresponding target module is being used
					for referencing.
																																				**
**************************************************************************************************/

#if    ( PDUIDTYPE == UINT8)
typedef	uint8  PduIdType;
#elif (PDUIDTYPE == UINT16)
typedef	uint16  PduIdType;
#else // default is uint8
typedef	uint16  PduIdType;
#endif	

/**************************************************************************************************
**
Name:                                    PduLengthType
Type:                                     uint8, uint16, uint32
Description:  
                    Variables of this type serve as length information of a PDU. The
					length information is provided in number of bytes.
					                                                                                                                   **
**************************************************************************************************/

#if    ( PDULENGTHTYPE == UINT8)
typedef	uint8  PduLengthType;
#elif (PDULENGTHTYPE == UINT16)
typedef	uint16  PduLengthType;
#elif (PDULENGTHTYPE == UINT32)
typedef	uint16  PduLengthType;
#else // default is uint8
typedef	uint16  PduLengthType;
#endif	

/**************************************************************************************************
**
Name:                                     PduInfoType
Type:                                      Structure
Description:  
                     Variables of this type shall be used to store the basic information about a PDU of
					any type, namely a pointer variable pointing to its SDU (payload), and the
					corresponding length of the SDU in bytes.
																																				**
**************************************************************************************************/
typedef struct {
	PduLengthType                     SduLength;
	uint8*                                   SduDataPtr;
}PduInfoType;

/**************************************************************************************************
**
Name:                                     BufReq_ReturnType
Type:                                      Enumeration
Description:  
                    Variables of this type shall be used to store the result of a buffer request.
																																				**
**************************************************************************************************/	

typedef enum {
	BUFREQ_OK,
	BUFREQ_E_NOT_OK,
	BUFREQ_E_BUSY,
	BUFREQ_E_OVFL
}BufReq_ReturnType;

/**************************************************************************************************
**
Name:                                     NetworkHandleType
Type:                                      uint8
Description:  
                     Variables of the type NetworkHandleType shall be used to store the identifier of a
                     communication channel.
																																				**
**************************************************************************************************/
typedef  uint8  NetworkHandleType;

#endif /* COMSTACK_TYPES_H */
