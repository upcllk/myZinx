#include "StdinChannel.h"
#include "StdoutChannel.h"
#include "ZinxKernel.h"
#include <unistd.h>

StdinChannel::StdinChannel()
{
}

StdinChannel::~StdinChannel()
{
}

int StdinChannel::GetFd()
{
	return STDIN_FILENO;
}

void StdinChannel::DataProcess(std::string _input)
{
}

bool StdinChannel::Init()
{
	return true;
}

void StdinChannel::Fini()
{
}
