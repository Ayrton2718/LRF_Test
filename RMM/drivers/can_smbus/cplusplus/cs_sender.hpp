#ifndef RMM_CAN_SMBUS_CS_SENDER_HPP
#define RMM_CAN_SMBUS_CS_SENDER_HPP

#include "../rmm_can_smbus.h"

namespace can_smbus
{

template<typename T>
class Sender
{
private:
    CC_obj m_sock;

public:
    Sender(const char* dev_name, const char* reg_name)
    {
        this->m_sock = CSSender_open(dev_name, reg_name);

    }

    ~Sender(void)
    {
        CSSender_close(this->m_sock);
    }

    void write(T var)
    {
        CCVariable_t tmp;
        tmp.Uint32 = (uint32_t)var;
        CSSender_write(this->m_sock, tmp);
    }
};

template<>
class Sender<float>
{
private:
    CC_obj m_sock;

public:
    Sender(const char* dev_name, const char* reg_name)
    {
        this->m_sock = CSSender_open(dev_name, reg_name);

    }

    ~Sender(void)
    {
        CSSender_close(this->m_sock);
    }

    void write(float var)
    {
        CCVariable_t tmp;
        tmp.Float = var;
        CSSender_write(this->m_sock, tmp);
    }
};

}

#endif /*RMM_CAN_SMBUS_CS_SENDER_HPP*/
