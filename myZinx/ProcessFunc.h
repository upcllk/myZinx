#pragma once
#include <string>
#include "AZinxHandler.h"
#include "IChannel.h"

class ProcessFunc :
	public AZinxHandler
{
public:
	ProcessFunc();
	virtual ~ProcessFunc();


private:
	std::string UpperOut(std::string _input);
	std::string OrigOut(std::string _input);

	// ͨ�� AZinxHandler �̳�
	virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) override;

};

