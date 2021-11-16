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
	// ���һ�����ڲ����������Ϣ����������ͼ�������
	if (pNextMsg != nullptr) {
		AZinxHandler* pNextHandler = GetNextHandler();
		if (pNextHandler != nullptr) {
			pNextHandler->Handle(pNextMsg);
		}
		delete pNextMsg;
	}
}
