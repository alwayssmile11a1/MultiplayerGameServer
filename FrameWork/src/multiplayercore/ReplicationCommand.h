#ifndef REPLICATIONCOMMAND_H
#define REPLICATIONCOMMAND_H

#include<stdint.h>
//Some basic command to replicate world
enum ReplicationAction
{
	RA_Create,
	RA_Update,
	RA_Destroy,
	RA_RPC,
	RA_MAX
};

class ReplicationCommand
{
public:

	ReplicationCommand() {}
	ReplicationCommand(uint32_t inInitialDirtyState) : mAction(RA_Create), mDirtyState(inInitialDirtyState) {}

	void AddDirtyState(uint32_t inState) { mDirtyState |= inState; }
	void SetDestroy() { mAction = RA_Destroy; }

	bool HasDirtyState() const { return (mAction == RA_Destroy) || (mDirtyState != 0); }

	void SetAction(ReplicationAction inAction) { mAction = inAction; }
	ReplicationAction GetAction()	const { return mAction; }
	uint32_t GetDirtyState() const { return mDirtyState; }
	void ClearDirtyState(uint32_t inStateToClear);

private:

	uint32_t mDirtyState;
	ReplicationAction mAction;
};

#endif