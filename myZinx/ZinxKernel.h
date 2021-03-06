#pragma once
#include "StdinChannel.h"
#include "IChannel.h"
#include <list>

// 全局单例
class ZinxKernel
{
private:
	std::list<IChannel*> m_channelList;
	int m_epollFd = -1;
	ZinxKernel();
	virtual ~ZinxKernel();
	static ZinxKernel g_kernel;
public:
	static ZinxKernel& GetInstance() {
		return g_kernel;
	}
	void Run();
	// 添加通道顺便将通道 fd 和 epoll 关联起来
	void AddChannel(IChannel* _pChannel);
	// 删除通道顺便将通道 fd 移除 epoll
	void DelChannel(IChannel* _pChannel);
	// 修改 epoll 的监听类型
	// 添加输出的监听方向
	void ModChannelAddOut(IChannel* _pChannel);
	// 删除输出的监听方向
	void ModChannelDelOut(IChannel* _pChannel);

	// 将数据流方向的修改封装，不必手动修改后在调用 Handle,
	// 除了输出通道好像也没有其他的要求 out 方向所以只抽象到通道类的话也不是不可以 ? 
	// 就是 GetNextHandler 返回值要转换一下类型
	// 这个函数在内部改变数据方向，在内部调用 _pNextHandler->Handle  --- _pNextHandler->InterHandle --- 
	// 因为此时这里 AzinxHandler 一定是 IChannel 类型，内部调用 IChannel->DataSendOut 写入缓存，添加 epoll 监听
	void ZinxSendOut(std::string _output, AZinxHandler* _pNextHandler);
	// void ZinxSendOut(std::string _output, IChannel* _pChannel);
};


