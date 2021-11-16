#include "ProcessFunc.h"
#include <algorithm>

ProcessFunc::ProcessFunc()
{
}

ProcessFunc::~ProcessFunc()
{
}

void ProcessFunc::DataProcess(std::string _input)
{
	if (!_input.empty() && islower(_input[0])) {
		UpperOut(_input);
	}
	else {
		OrigOut(_input);
	}
}

void ProcessFunc::UpperOut(std::string _input)
{
	std::transform(_input.begin(), _input.end(), _input.begin(), ::_toupper);
	m_outChannel->DataSendOut(_input);
}

void ProcessFunc::OrigOut(std::string _input)
{
	m_outChannel->DataSendOut(_input);
}
