#include "ReplicationCommand.h"

void ReplicationCommand::ClearDirtyState(uint32_t inStateToClear)
{
	mDirtyState &= ~inStateToClear;

	if (mAction == RA_Destroy)
	{
		mAction = RA_Update;
	}
}