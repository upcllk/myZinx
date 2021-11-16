#include "AZinxHandler.h"

ZinxMessage::ZinxMessage()
{
}

ZinxMessage::~ZinxMessage()
{
}

AZinxHandler::AZinxHandler()
{
}

AZinxHandler::~AZinxHandler()
{
}

void AZinxHandler::Handle(ZinxMessage* _inputMsg)
{
	ZinxMessage* pNextMsg = InternelHandle(_inputMsg);
	// 最后一个环节不产生输出消息，即有输出就继续处理
	if (pNextMsg != nullptr) {
		AZinxHandler* pNextHandler = GetNextHandler();
		if (pNextHandler != nullptr) {
			pNextHandler->Handle(pNextMsg);
		}
		delete pNextMsg;
	}
}
