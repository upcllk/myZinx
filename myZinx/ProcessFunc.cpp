#include <algorithm>
#include "ProcessFunc.h"
#include "ZinxKernel.h"

ProcessFunc::ProcessFunc()
{
}

ProcessFunc::~ProcessFunc()
{
}

std::string ProcessFunc::UpperOut(std::string _input)
{
	std::string nextString = _input;
	std::transform(nextString.begin(), nextString.end(), nextString.begin(), ::_toupper);
	return nextString;
}

std::string ProcessFunc::OrigOut(std::string _input)
{
	return _input;
}

ZinxMessage* ProcessFunc::InternelHandle(ZinxMessage* _inputMsg)
{
	ByteMsg* pRet = nullptr;
	ByteMsg* pMsg = dynamic_cast<ByteMsg*>(_inputMsg);
	if (pMsg != nullptr) {
		std::string nextString;
		if (!pMsg->m_content.empty() && islower(pMsg->m_content[0])) {
			nextString = UpperOut(pMsg->m_content);
		}
		else {
			nextString = OrigOut(pMsg->m_content);
		}
		// 记得修改数据方向
		//// 好像不太方便
		//pMsg->m_Dir = SysIODirMsg::IO_OUT;
		//pRet = new ByteMsg(nextString, *pMsg);
		ZinxKernel::GetInstance().ZinxSendOut(nextString, GetNextHandler());
	}
	return pRet;
}
