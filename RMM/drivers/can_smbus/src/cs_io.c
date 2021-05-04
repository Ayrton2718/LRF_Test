#include "cs_io.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// #define CSIO_LOCAL_DEBUG (1)

struct CSIo_info_t
{
    int sock;
    pthread_mutex_t locker;
    struct sockaddr_in send_addr;
};

struct CSIo_ethFrame_t
{
    uint8_t id_high;
    uint8_t id_low;
    uint8_t reg;
    uint8_t len;
    uint8_t data[4];
} __attribute__((__packed__));


static struct CSIo_info_t g_root;


void CSIo_initializer(void)
{
    CCLocker_lockerInit(&g_root.locker);

    g_root.sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in recv_addr;

    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(6001);
    recv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(g_root.sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr));

    g_root.send_addr.sin_family = AF_INET;
    g_root.send_addr.sin_port = htons(7001);
#ifdef CSIO_LOCAL_DEBUG
    g_root.send_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
#else /*CSIO_LOCAL_DEBUG*/
    g_root.send_addr.sin_addr.s_addr = inet_addr("172.22.4.11");
#endif /*CSIO_LOCAL_DEBUG*/
}


CC_bool_t CSIo_read(CSType_canId_t* id, CSType_canReg_t* reg, size_t* len, uint8_t* data)
{
    char buff[128];
    memset(buff, 0, sizeof(buff));

    if(recv(g_root.sock, buff, sizeof(buff), 0) == sizeof(struct CSIo_ethFrame_t))
    {
        struct CSIo_ethFrame_t* usb_frame = (struct CSIo_ethFrame_t*)buff;
        *id = ((uint16_t)usb_frame->id_high << 8) | usb_frame->id_low;
        *reg = usb_frame->reg;
        *len = usb_frame->len;
        memcpy(data, usb_frame->data, usb_frame->len);

        return CC_BOOL_TRUE;
    }else{
        return CC_BOOL_FALSE;
    }
}


void CSIo_send(CSType_canId_t id, CSType_canReg_t reg, size_t len, uint8_t* data)
{
    struct CSIo_ethFrame_t usb_frame;
    usb_frame.id_high = id >> 8;
    usb_frame.id_low = id & 0x00FF;
    usb_frame.reg = reg;
    usb_frame.len = len;
    memcpy(usb_frame.data, data, usb_frame.len);
    sendto(g_root.sock, &usb_frame, sizeof(struct CSIo_ethFrame_t), 0, (struct sockaddr *)&g_root.send_addr, sizeof(g_root.send_addr));
}
