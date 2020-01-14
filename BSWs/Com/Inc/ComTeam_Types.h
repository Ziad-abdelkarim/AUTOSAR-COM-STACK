/*******************************************************************************************************************************
FileName:                                        ComTeam_Types.h
AUTOSAR Version:                                    4.2.2
******************************************************************************************************************************/

#ifndef COMTEAM_TYPES_H_
#define COMTEAM_TYPES_H_

/******************************************************************************************************************************
 **                                               Includes                                                                   **
 ******************************************************************************************************************************/

/*******************************************************************************************************************************
**                                   Variables Definitions                                                                    **
********************************************************************************************************************************/


/*******************************************************************************************************************************
**                                   Type Definitions                                                                    **
********************************************************************************************************************************/
typedef struct {
	uint8  ComTeamTxIPduNumberOfRepetitions;
	float32 ComTeamTxModeRepetitionPeriod;
	float32 ComTeamTxModeTimePeriod;
	float32 ComTeamMinimumDelayTimer;
}ComTeamTxMode_Type;

typedef struct 
{
	ComTeamTxMode_Type ComTeamTxMode;
}ComTeamIPdu_Type;

typedef struct{
	boolean ComTeamSignalUpdated;
	boolean ComTeamSignalConfirmed;
}ComTeamSignal_Type;

typedef struct{
	boolean ComTeamSignalGroupUpdated;
	boolean ComTeamSignalGroupConfirmed;
}ComTeamSignalGroup_Type;

typedef struct {
	ComTeamIPdu_Type ComTeamIPdu[ComMaxIPduCnt];
	ComTeamSignal_Type ComTeamSignal[ComMaxSignalCnt];
	ComTeamSignalGroup_Type ComTeamSignalGroup[ComMaxSignalGroupCnt];
}ComTeamConfig_Type;

#endif /* COMTEAM_TYPES_H_ */
