#pragma once
#include <string>
#include"AZinxHandler.h"


// epoll 与 IChannel 之间传递的 IO 方向的消息
class SysIODirMsg :
	public ZinxMessage
{
public:
	enum IO_DIR {
		IO_IN, IO_OUT
	} m_Dir = IO_IN;

	SysIODirMsg(IO_DIR dir) :m_Dir(dir) {}
};

// IChannel 与 业务层传递的 ByteMsg 类型数据
// 为了让数据保持之前的状态，即包含上一阶段的 IO 方向消息，避免额外的赋值操作，我们继承 SysIODirMsg 类
class ByteMsg :
	public SysIODirMsg
{
public:
	std::string m_content;

	ByteMsg(std::string _content, SysIODirMsg& _IO_Dir) :m_content(_content), SysIODirMsg(_IO_Dir.m_Dir) {}
};

// 对于通道类的 Handle 有统一的处理，即发送出去
class IChannel :
	public AZinxHandler
{
public:
	IChannel();
	virtual ~IChannel();

	// 在 kernel 中 AddChannel() 中调用
	virtual bool Init() = 0;
	// 去初始化， 在 kernel 中 DelChannel() 中调用
	virtual void Fini() = 0;
	virtual int GetFd() = 0;
	// 这个目前来看确实没啥用,不过或许可以留着?
	// 万一以后自定义操作可以在 InternelHandle 里先调用 DataProcess，? 具体操作调用者可以指定
	virtual void DataProcess(std::string _input) = 0;

	std::string ReadFd();
	// 不直接调用 WriteFd， 只是存到缓存 m_buffer 并修改 epoll, 等到 epoll 返回再发, 通用函数
	void DataSendOut(std::string _output);
	// 向外刷出缓存数据 内部调用 WriteFd 这个才是真正的输出函数
	void FlushOut();

private:
	std::string m_buffer;
	// 通过 AZinxHandler 继承
	virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) override;

	void WriteFd(std::string _output);
};

