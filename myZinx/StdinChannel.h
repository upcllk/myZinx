#pragma once
#include <string>
#include "IChannel.h"
#include "ProcessFunc.h"

class StdoutChannel;

class StdinChannel :
	public IChannel
{
private:
	ProcessFunc* m_ProcFunc = nullptr;
public:
	StdinChannel();
	virtual ~StdinChannel();

	// ͨ�� IChannel �̳�
	virtual int GetFd() override;
	virtual void DataProcess(std::string _input) override;

public:
	void SetOutProcFunc(ProcessFunc* _pProcFunc) {
		m_ProcFunc = _pProcFunc;
	}

	// ͨ�� IChannel �̳�
	virtual bool Init() override;
	virtual void Fini() override;
};

