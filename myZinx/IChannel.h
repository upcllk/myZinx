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

	// 这边 ReadFd 和 WriteFd 写为虚函数的原因是对于 TCP 连接的 fd，读写操作需要进行重写，，比如 accept 啥的
	virtual std::string ReadFd();
	// 不直接调用 WriteFd， 只是存到缓存 m_buffer 并修改 epoll, 等到 epoll 返回再发, 通用函数
	void DataSendOut(std::string _output);
	// 向外刷出缓存数据 内部调用 WriteFd 这个才是真正的输出函数
	void FlushOut();

	virtual void SetNextStage(AZinxHandler* _nextHandler);
	virtual AZinxHandler* GetNextStage();

	/*
	* 设置这个函数的目的是, 在我的代码中虽然跟着视频使用了工厂方法模式，但是视频里的没有什么 SetNextStage 和 GetNextStage
	* 只有下面这个纯虚函数，而在 TCPDataChannel 类中也没有实现，同时我们没法在框架运行时 lfd 检测到连接后创建一个
	* 对象，再去手动调用 SetNextStage， 所以就有了这个纯虚函数
	* 有了这个函数之后由于 TCPDataChannel 类中也没有实现，所以调用者必须继承该类重写该函数
	*/
	virtual AZinxHandler* GetInputNextStage() = 0;

private:
	std::string m_buffer;
	// 通过 AZinxHandler 继承
	// IChannel 通道类只负责输入输出，他的工作很单一
	virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) override;

	virtual void WriteFd(std::string _output);
};

