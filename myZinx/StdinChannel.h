#pragma once
#include <string>
#include "IChannel.h"

class StdoutChannel;

class StdinChannel :
	public IChannel
{
private:
	StdoutChannel* m_outChannel = nullptr;
public:
	StdinChannel();
	virtual ~StdinChannel();

	// ͨ�� IChannel �̳�
	virtual int GetFd() override;
	virtual void DataProcess(std::string _input) override;

public:
	void SetOutChannel(StdoutChannel* _pChannel) {
		m_outChannel = _pChannel;
	}
	StdoutChannel* GetOutChannel() {
		return m_outChannel;
	}

	// ͨ�� IChannel �̳�
	virtual bool Init() override;
	virtual void Fini() override;
};

