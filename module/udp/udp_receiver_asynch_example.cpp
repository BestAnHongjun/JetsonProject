#include "udp.h"
#include <unistd.h>

void callback(RecvHead &head)
{
    if (head.buffer == NULL) 
    {
        printf("NULL\n");
    }
    else
    {
        printf("delay:%.5lf ms, size=%d bytes.\n", head.delay, head.size);
        delete[] head.buffer;
    }
}

int main()
{
    UDPReceiver receiver(8888, callback);
    usleep(10 * 1000 * 1000);
    return 0;
}