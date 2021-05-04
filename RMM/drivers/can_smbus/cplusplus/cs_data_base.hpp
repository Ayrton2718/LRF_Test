#ifndef RMM_CAN_SMBUS_CS_DATA_BASE_HPP
#define RMM_CAN_SMBUS_CS_DATA_BASE_HPP

#include "../rmm_can_smbus.h"

namespace can_smbus
{

class RegDataBase
{
private:
    char m_dev_name[128];
    CSType_canId_t m_dev_id;

public:
    RegDataBase(const char* dev_name, CSType_canId_t dev_id)
    {
        size_t len = strlen(dev_name);
        memcpy(this->m_dev_name, dev_name, len);
        this->m_dev_name[len] = '\0';

        this->m_dev_id = dev_id;

        CSDataBase_setDevice(this->m_dev_name, this->m_dev_id);
    }
    
    void setInputRegister(const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type)
    {
        CSDataBase_setRegIn(this->m_dev_name, reg_name, reg_id, data_type);
    }

    void setOutputRegister(const char* reg_name, CSType_canId_t reg_id, CCVariable_data_t data_type)
    {
        CSDataBase_setRegOut(this->m_dev_name, reg_name, reg_id, data_type);
    }
};

}

#endif /*RMM_CAN_SMBUS_CS_DATA_BASE_HPP*/
