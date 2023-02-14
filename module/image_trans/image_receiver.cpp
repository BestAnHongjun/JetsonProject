#include "image_trans.h"
#include <unistd.h>
#include <sys/time.h>


IMGReceiver::IMGReceiver(uint16_t local_port, const char* target_ip, uint16_t target_port, 
void (*callback_func)(cv::Mat, double, double))
: udp_receiver(local_port), udp_sender(target_ip, target_port)
{
    time_cnt = 0;
    callback = callback_func;
    
    ttl_handle = true;
    ttl_thread = std::thread(&IMGReceiver::ttl_func, this);

    recv_handle = true;
    recv_thread = std::thread(&IMGReceiver::recv_func, this);
}

IMGReceiver::~IMGReceiver()
{
    ttl_handle = false;
    recv_handle = false;
    ttl_thread.join();
    recv_thread.join();
}

void IMGReceiver::ttl_func()
{
    struct timeval tp;
    double t;
    const char ident[] = "TIMETIME";
    uint16_t buf_len = strlen(ident) + sizeof(time_cnt);
    uint8_t* buffer = new uint8_t[buf_len];
    memcpy(buffer, ident, strlen(ident));
    
    while (ttl_handle)
    {
        usleep(1000 * 500); // 500ms
        
        time_cnt = (time_cnt + 1) % 50;
        memcpy(buffer + strlen(ident), &time_cnt, sizeof(time_cnt));
        
        gettimeofday(&tp, NULL);
        t = (double)tp.tv_sec * 1e3 + (double)tp.tv_usec * 1e-3;
        send_time[time_cnt] = t;

        udp_sender.send(buffer, buf_len);
    }

    delete[] buffer;
}

void IMGReceiver::recv_func()
{
    RecvHead head;

    struct timeval tp;
    double t1, t2;
    const char ident[] = "TIMETIME";
    char recv_ident[strlen(ident) + 1];
    uint16_t buf_len = strlen(ident) + sizeof(double) + sizeof(time_cnt);
    uint8_t* buffer = new uint8_t[buf_len];

    while (recv_handle)
    {
        udp_receiver.recv(head);
        recv += 1;
        if (head.buffer == NULL)
        {
            lost += 1;
            continue;
        }

        if (head.size == buf_len)
        {   // 可能是用于测定传输延时的包
            memcpy(recv_ident, head.buffer, strlen(ident));
            recv_ident[strlen(ident)] = '\0';
            if (strcmp(ident, recv_ident) == 0)
            {   // 确实是用于测定传输延时的包
                gettimeofday(&tp, NULL);
                t2 = (double)tp.tv_sec * 1e3 + (double)tp.tv_usec * 1e-3;
                memcpy(buffer, head.buffer, buf_len);
                uint16_t index;
                memcpy(&index, buffer + strlen(ident), sizeof(time_cnt));
                t1 = send_time[index];
                trans_delay = (t2 - t1) / 2.0;
                delete[] head.buffer;
                continue;
            }
        }

        // 是图像
        std::vector<uint8_t> buffer(head.buffer, head.buffer + head.size);
        cv::Mat mat = cv::imdecode(buffer, CV_LOAD_IMAGE_COLOR);
        delete[] head.buffer;
        if (mat.empty()) 
        {
            lost += 1;
            continue;
        }
        double total_delay = head.delay + trans_delay;
        callback(mat, total_delay, (double)lost / recv);
    }
}