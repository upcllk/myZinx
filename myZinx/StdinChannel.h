#pragma once
#include <string>
#include "IChannel.h"
#include "ProcessFunc.h"

class StdoutChannel;

class StdinChannel :
	public IChannel
{
public:
	StdinChannel();
	virtual ~StdinChannel();

	// ͨ�� IChannel �̳�
	virtual int GetFd() override;

public:
	// ͨ�� IChannel �̳�
	virtual bool Init() override;
	virtual void Fini() override;

	// ͨ�� IChannel �̳�
	virtual void DataProcess(std::string _input) override;

	// ͨ�� IChannel �̳�
	virtual AZinxHandler* GetInputNextStage() override;
};

