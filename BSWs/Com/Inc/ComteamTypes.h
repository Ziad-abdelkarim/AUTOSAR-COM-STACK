typedef struct {
	uint8  ComTeamTxIPduNumberOfRepetitions;
	sint16 ComTeamTxModeRepetitionPeriod;
	sint16 ComTeamTxModeTimePeriod;
	sint16 ComTeamMinimumDelayTimer;
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

