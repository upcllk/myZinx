#pragma once
#include <string>

class StdinChannel
{
public:
	StdinChannel();
	virtual ~StdinChannel();
	std::string ReadFd();
};

