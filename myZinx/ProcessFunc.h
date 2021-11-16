#pragma once
#include <string>
#include "IChannel.h"

class ProcessFunc
{
public:
	ProcessFunc();
	virtual ~ProcessFunc();
	void SetOutChannel(IChannel* _pChannel) {
		m_outChannel = _pChannel;
	}
	void DataProcess(std::string _input);


private:
	IChannel* m_outChannel = nullptr;
	void UpperOut(std::string _input);
	void OrigOut(std::string _input);
};

