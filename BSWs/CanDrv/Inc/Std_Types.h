/***************************************************************************************************
**

FileName:                          Std_Types.h                                     
AUTOSAR Version:            4.2.2
																																				  **
***************************************************************************************************/
#ifndef STD_TYPES_H
#define STD_TYPES_H
/***************************************************************************************************
**                                          Includes                                                       					             **
***************************************************************************************************/
#include "Platform_Types.h"


#ifndef			`STATUSTYPEDEFINED
#define		 STATUSTYPEDEFINED
#define					E_OK																		0x00u
#define 				E_NOT_OK 																0x01u
typedef              unsigned char                                                         StatusType;    /* OSEK compliance */
#endif

#define           		 STD_HIGH                                                                 0x01u        /* Physical state 5V or  3.3V*/
#define 				 STD_LOW 															     0x00u        /* Physical state 0V */

#define		       STD_ACTIVE																 0x01u        /* Logical state active */
#define 			     STD_IDLE                                                                  0x00u        /* Logical state idle */

#define		          STD_ON															    	0x01u        /* Logical state ON*/
#define 			     STD_OFF                                                                  0x00u        /* Logical state OFF */
/***************************************************************************************************
**                                        Types Declerations                                                                       **
***************************************************************************************************/


/**************************************************************************************************
**
Name:                                  Std_ReturnType
Type:                                         uint8
Description:  
                     This type can be used as standard API return type which is shared between the
					RTE and the BSW
					                                                                                                                   **
**************************************************************************************************/
typedef        uint8           Std_ReturnType;

#endif /* STD_TYPES_H */