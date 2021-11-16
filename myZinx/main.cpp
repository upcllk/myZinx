#include <iostream>
#include "ZinxKernel.h"
#include "StdinChannel.h"
#include "StdoutChannel.h"

int main()
{
    StdinChannel in_channel;
    StdoutChannel out_channel;
    in_channel.SetOutChannel(&out_channel);

    ZinxKernel& kernel = ZinxKernel::GetInstance();
    kernel.AddChannel(&out_channel);
    kernel.AddChannel(&in_channel);

    kernel.Run();
    return 0;
}