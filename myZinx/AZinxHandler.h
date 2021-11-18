#pragma once

class ZinxMessage {
public:
	ZinxMessage();
	virtual ~ZinxMessage();
};

class AZinxHandler
{
private:
	AZinxHandler* m_nextHandler = nullptr;

public:
	AZinxHandler();
	virtual ~AZinxHandler();

	void Handle(ZinxMessage* _inputMsg);

	void SetNextHandler(AZinxHandler* _pNextHandler) {
		m_nextHandler = _pNextHandler;
	}
	virtual AZinxHandler* GetNextHandler() {
		return m_nextHandler;
	}

	// �ú������ص���һ���Ѷ���, Handle ��������delete, ���һ�����ڲ�������Ϣ, ���� nullptr
	virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) = 0;
};

