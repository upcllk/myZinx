#include <iostream>
#include "ZinxKernel.h"
#include "StdinChannel.h"

int main()
{
    ZinxKernel kernel;
    StdinChannel in_channel;

    kernel.AddChannel(&in_channel);
    kernel.Run();

    return 0;
}