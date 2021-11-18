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

	// 通过 IChannel 继承
	virtual int GetFd() override;

public:
	// 通过 IChannel 继承
	virtual bool Init() override;
	virtual void Fini() override;

	// 通过 IChannel 继承
	virtual void DataProcess(std::string _input) override;

	// 通过 IChannel 继承
	virtual AZinxHandler* GetInputNextStage() override;
};

