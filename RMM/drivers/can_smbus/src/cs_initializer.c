#include "cs_initializer.h"

#include "cs_io.h"
#include "cs_sender.h"
#include "cs_register.h"
#include "cs_reg_share.h"
#include "cs_reg_signal.h"
#include "cs_data_base.h"

void CSInitializer_init(void)
{
    TCDirectory_mkdir("", "comm");

    CSIo_initializer();

    CSRegister_initializer();
    CSSender_initializer();
    
    CSDataBase_initializer();
    CSRegShare_initializer();
    CSRegSignal_initializer();
}