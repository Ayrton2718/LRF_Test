#ifndef RMM_CAN_SMBUS_CS_REG_SIGNAL_HPP
#define RMM_CAN_SMBUS_CS_REG_SIGNAL_HPP

#include "../rmm_can_smbus.h"

namespace can_smbus
{

template<typename T>
class RegSignal
{
private:
    TCSignal_t m_sock;
    CC_obj m_requester;

public:
    RegSignal(const char* dev_name, const char* reg_name, const char* instance_name)
    {
        CSRegSignal_bind(dev_name, reg_name);
        this->m_sock = CSRegSignal_open(dev_name, reg_name, instance_name);

        this->m_requester = CSRequester_open(dev_name, reg_name);
    }

    ~RegSignal()
    {
        CSRegSignal_close(this->m_sock);
        CSRequester_close(this->m_requester);
    }

    void request(void)
    {
        CSRequester_request(this->m_requester);
    }

    T recv(void)
    {
        T var = CSRegSignal_recv(this->m_sock).Uint32;
        return var;
    }
};

template<>
class RegSignal<float>
{
private:
    TCSignal_t m_sock;
    CC_obj m_requester;

public:
    RegSignal(const char* dev_name, const char* reg_name, const char* instance_name)
    {
        CSRegSignal_bind(dev_name, reg_name);
        this->m_sock = CSRegSignal_open(dev_name, reg_name, instance_name);

        this->m_requester = CSRequester_open(dev_name, reg_name);
    }

    ~RegSignal()
    {
        CSRegSignal_close(this->m_sock);
        CSRequester_close(this->m_requester);
    }

    void request(void)
    {
        CSRequester_request(this->m_requester);
    }

    float recv(void)
    {
        return CSRegSignal_recv(this->m_sock).Float;
    }
};

}

#endif /*RMM_CAN_SMBUS_CS_REG_SIGNAL_HPP*/
