#include "StdoutChannel.h"
#include <iostream>
#include <unistd.h>

StdoutChannel::StdoutChannel()
{
}

StdoutChannel::~StdoutChannel()
{
}

std::string StdoutChannel::ReadFd()
{
	return std::string();
}

int StdoutChannel::GetFd()
{
	return STDOUT_FILENO;
}

void StdoutChannel::DataProcess(std::string _input)
{
}
