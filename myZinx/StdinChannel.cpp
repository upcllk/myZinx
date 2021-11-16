#include "StdinChannel.h"
#include "StdoutChannel.h"
#include <iostream>
#include <unistd.h>

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

int StdinChannel::GetFd()
{
	return STDIN_FILENO;
}

void StdinChannel::DataProcess(std::string _input)
{
	// 将数据回显到标准输出 -> 需要调用标准输出通道对象的 SendOut 函数
	m_outChannel->DataSendOut(_input);
}