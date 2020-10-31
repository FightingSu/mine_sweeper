#include "blockPrivate.h"

blockPrivate::blockPrivate():
	status(mineNormal), sweeped(false),
	flaged(false), nearBy(0), isMine(false)
{
	
}


blockPrivate::~blockPrivate()
{
}

void blockPrivate::setStatus(mineStatus _status)
{
	status = _status;
}

blockPrivate::mineStatus blockPrivate::getStatus()
{
	return status;
}

void blockPrivate::setSweep(bool _sweep)
{
	sweeped = _sweep;
}

bool blockPrivate::getSweeped()
{
	return sweeped;
}

void blockPrivate::setFlag(bool _flag)
{
	flaged = _flag;
}
bool blockPrivate::getFlag()
{
	return flaged;
}

void blockPrivate::setNearBy(int _nearBy)
{
	nearBy = _nearBy;
}

int blockPrivate::getNearBy() 
{
	return nearBy;
}

void blockPrivate::setMine(bool _isMine)
{
	isMine = _isMine;
}

bool blockPrivate::getMine()
{
	return isMine;
}