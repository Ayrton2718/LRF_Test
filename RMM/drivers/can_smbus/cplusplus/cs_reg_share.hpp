#ifndef RMM_CAN_SMBUS_CS_REG_SHARE_HPP
#define RMM_CAN_SMBUS_CS_REG_SHARE_HPP

#include "../rmm_can_smbus.h"

namespace can_smbus
{

template<typename T>
class RegShare
{
private:
    TCShare_t m_sock;
    CC_obj m_requester;

public:
    RegShare(const char* dev_name, const char* reg_name)
    {
        CSRegShare_bind(dev_name, reg_name);
        this->m_sock = CSRegShare_open(dev_name, reg_name);

        this->m_requester = CSRequester_open(dev_name, reg_name);
    }

    ~RegShare()
    {
        CSRegShare_close(this->m_sock);
        CSRequester_close(this->m_requester);
    }

    void request(void)
    {
        CSRequester_request(this->m_requester);
    }


    T read(void)
    {
        T var = (T)CSRegShare_read(this->m_sock).Uint32;
        return var;
    }
};


template<>
class RegShare<float>
{
private:
    TCShare_t m_sock;
    CC_obj m_requester;

public:
    RegShare(const char* dev_name, const char* reg_name)
    {
        CSRegShare_bind(dev_name, reg_name);
        this->m_sock = CSRegShare_open(dev_name, reg_name);

        this->m_requester = CSRequester_open(dev_name, reg_name);
    }

    ~RegShare()
    {
        CSRegShare_close(this->m_sock);
        CSRequester_close(this->m_requester);
    }

    void request(void)
    {
        CSRequester_request(this->m_requester);
    }


    float read(void)
    {
        return CSRegShare_read(this->m_sock).Float;
    }
};

}

#endif /*RMM_CAN_SMBUS_CS_REG_SHARE_HPP*/
