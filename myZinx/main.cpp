#include <iostream>
#include <algorithm>
#include <unistd.h>
#include "ZinxKernel.h"
#include "StdinChannel.h"
#include "StdoutChannel.h"
#include "FifoChannel.h"
#include "ProcessFunc.h"
#include "AZinxHandler.h"

using namespace std;

class MessageTest :
    public ZinxMessage
{
public:
    string m_data;
};

class TransUpper :
    public AZinxHandler
{
    // 通过 AZinxHandler 继承
    virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) override
    {
        MessageTest* pMsg = dynamic_cast<MessageTest*>(_inputMsg);
        string outputStr = pMsg->m_data;
        transform(outputStr.begin(), outputStr.end(), outputStr.begin(), ::toupper);
        cout << "TransUpper : " << outputStr << endl;
        // 返回一个堆对象
        MessageTest* pRet = new MessageTest;
        pRet->m_data = outputStr;
        return pRet;
    }
};

class OutputAscii :
    public AZinxHandler
{
    // 通过 AZinxHandler 继承
    virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) override
    {
        MessageTest* pMsg = dynamic_cast<MessageTest*>(_inputMsg);
        if (!pMsg->m_data.empty()) {
            printf("OutputAscii : %d\n", pMsg->m_data[0]);
        }
        // 原样交给下一个环节，但是要 new 一个新的对象
        MessageTest* pRet = new MessageTest;
        pRet->m_data = pMsg->m_data;
        return pRet;
    }
};

class OutputNextLetter :
    public AZinxHandler
{
    // 通过 AZinxHandler 继承
    virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) override
    {
        MessageTest* pMsg = dynamic_cast<MessageTest*>(_inputMsg);
        if (!pMsg->m_data.empty()) {
            printf("OutputNextLetter : %c\n", pMsg->m_data[0] + 1);
        }
        return nullptr;
    }
};


void ChainTest() {
    TransUpper step1;
    OutputAscii step2;
    OutputNextLetter step3;
    step1.SetNextHandler(&step2);
    step2.SetNextHandler(&step3);

    string tmp;
    while (1) {
        getline(cin, tmp);
        if (tmp == "exit") {
            break;
        }
        MessageTest initMsg;
        initMsg.m_data = tmp;
        step1.Handle(&initMsg);
    }
}

// 改变一下数据流向
class NothingTodo :
    public AZinxHandler
{
    // 通过 AZinxHandler 继承
    // 由于它是输出通道前的最后一个，直接调用 kernel 的 sendout 函数，在函数内部修改 IO_DIR 方向输出
    //************************   TO DO : ****************************************************
    // 但是好像不符合开闭原则，或许可以具体通道类做什么写成 IChannel 类中的纯虚函数进行重写 ProcessFunc 也有同样的问题
    // 这样每次往责任链里添加新东西就不用改动里面的代码

    virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) override
    {
        ByteMsg* pMsg = dynamic_cast<ByteMsg*>(_inputMsg);
        if (pMsg != nullptr) {
            ZinxKernel::GetInstance().ZinxSendOut(pMsg->m_content, GetNextHandler());
        }
        return nullptr;
    }
};

int main()
{
    // ChainTest();

    ZinxKernel& kernel = ZinxKernel::GetInstance();

    StdinChannel in_channel;
    StdoutChannel out_channel;
    ProcessFunc pf;

    // in_channel -----> pf -----> out_channel
    //           DataProc  DataSend
    in_channel.SetNextHandler(&pf);
    pf.SetNextHandler(&out_channel);

    kernel.AddChannel(&out_channel);
    kernel.AddChannel(&in_channel);

    FifoChannel fifo_in_channel("input", true);
    FifoChannel fifo_out_channel("output", false);
    NothingTodo nt;

    fifo_in_channel.SetNextHandler(&nt);
    nt.SetNextHandler(&fifo_out_channel);
    // fifo_in_channel.SetOutChannel(&fifo_out_channel);

    kernel.AddChannel(&fifo_in_channel);
    kernel.AddChannel(&fifo_out_channel);

    kernel.Run();
    return 0;
}