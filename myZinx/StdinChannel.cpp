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

int StdinChannel::GetFd()
{
	return STDIN_FILENO;
}

void StdinChannel::DataProcess(std::string _input)
{
	// 将数据回显到标准输出 -> 需要调用标准输出通道对象的 SendOut 函数
	m_ProcFunc->DataProcess(_input);
}

bool StdinChannel::Init()
{
	return true;
}

void StdinChannel::Fini()
{
}
