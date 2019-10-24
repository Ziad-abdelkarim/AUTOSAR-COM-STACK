/********************************************************************************************************************************
 **
 FileName:                   Det.c
 AUTOSAR Version:            4.2.2
                                                                                                                                                                          **
 ********************************************************************************************************************************/
/********************************************************************************************************************************
 **                                                          Includes                                                                                                **
 ********************************************************************************************************************************/
#include <Det.h>

/********************************************************************************************************************************
 **                                                       Global Variables                                                                                       **
 ********************************************************************************************************************************/










 /********************************************************************************************************************************
**                                                        Local Functions                                                                                        **
*********************************************************************************************************************************/












/********************************************************************************************************************************
**                                                      Functions  defination                                                                        **
*********************************************************************************************************************************/
/*********************************************************************************************************************************
 Service name:                                                  Det_ReportError
 Service ID[hex]:                                               0x01
 Sync/Async:                                                    Depending on implemented functionality: 1. Breakpoint set: no return 2. Internal
                                                                error counting/logging in RAM: synchronous 3. External error logging via
                                                                communication interface: asynchronous
 Reentrancy:                                                    Reentrant
 Parameters (in):                                               ModuleId      --> Module ID of calling module.
                                                                InstanceId    --> The identifier of the index based instance of a module, starting
                                                                                  from 0, If the module is a single instance module it shall pass 0
                                                                                  as the InstanceId.
                                                                ApiId         --> id of API service in which error is detected (defined in SWS of calling module)
                                                                ErrorId       --> ID of detected development error (defined in SWS of calling module).
 Parameters (out):                                              None
 Return value:                                                  Std_ReturnType -->  returns always E_OK (is required for services)
 Description:                                                   Service to report development errors.
 *******************************************************************************************************************************/


 Std_ReturnType Det_ReportError(uint16 ModuleId,uint8 InstanceId,uint8 ApiId,uint8 ErrorId)
 {

 }
