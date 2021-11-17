#include "StdoutChannel.h"
#include <unistd.h>

StdoutChannel::StdoutChannel()
{
}

StdoutChannel::~StdoutChannel()
{
}

int StdoutChannel::GetFd()
{
	return STDOUT_FILENO;
}

bool StdoutChannel::Init()
{
	return true;
}

void StdoutChannel::Fini()
{
}

void StdoutChannel::DataProcess(std::string _input)
{
}

