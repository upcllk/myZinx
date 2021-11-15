#include "StdinChannel.h"
#include <iostream>

StdinChannel::StdinChannel()
{
}

StdinChannel::~StdinChannel()
{
}

std::string StdinChannel::ReadFd()
{
	std::string ret;
	getline(std::cin, ret);
	return ret;
}
