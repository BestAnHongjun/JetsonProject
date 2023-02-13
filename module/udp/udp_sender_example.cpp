#include "udp.h"
#include <unistd.h>

int main()
{
    char test[512 * 1000 + 53];
    UDPSender sender("192.168.137.97", 8888);
    for (int i = 0; i < 110; i++)
    {
        printf("%d:\n", i);
        sender.send((uint8_t*)test, 512 * 1000 + 53);
    }
    usleep(10 * 1000 * 1000);
    printf("end.\n");
    return 0;
}