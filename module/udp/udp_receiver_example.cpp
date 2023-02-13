#include "udp.h"

int main()
{
    UDPReceiver receiver(8888);
    for (int i = 0; i < 100; i++)
    {
        RecvHead head;
        receiver.recv(head);
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
    
    return 0;
}