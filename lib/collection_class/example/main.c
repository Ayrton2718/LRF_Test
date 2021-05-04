#include "stdio.h"
#include "collection_class.h"

int main(void)
{
    CCLog_init();
    
    CC_obj obj = CCDictionary_create();
    CCDictionary_setObject(obj, CCString_create("gg"), "gg");
    CCDictionary_setObject(obj, CCString_create("gg"), "gg2");
    // CCDebugger_info(obj);
    return 0;
}
