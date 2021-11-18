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

	// 该函数返回的是一个堆对象, Handle 函数负责delete, 最后一个环节不产生消息, 返回 nullptr
	virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) = 0;
};

