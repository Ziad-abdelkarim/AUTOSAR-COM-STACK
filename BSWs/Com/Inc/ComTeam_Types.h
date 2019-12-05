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
	float32ComTeamMinimumDelayTimer;
}ComTeamTxMode_type;

typedef struct 
{
	ComTeamTxMode_type ComTeamTxMode;
}ComTeamIPdu_type;

typedef struct{
	boolean ComTeamSignalUpdated;
	boolean ComTeamSignalConfirmed;
}ComTeamSignal_type;

typedef struct{
	boolean ComTeamSignalGroupUpdated;
	boolean ComTeamSignalGroupConfirmed;
}ComTeamSignalGroup_type;

typedef struct {
	ComTeamIPdu_type ComTeamIPdu[ComMaxIPduCnt];
	ComTeamSignal_type ComTeamSignal[ComMaxSignalCnt];
	ComTeamSignalGroup_type ComTeamSignalGroup[ComMaxSignalGroupCnt];
}ComTeamConfig_type;

#endif /* COMTEAM_TYPES_H_ */
