#include "tc_initializer.h"

#include <collection_class.h>
#include "tc_launcher.h"
#include "tc_directory.h"

void TCInitializer_init(void)
{
    TCDirectory_init();
    TCLauncher_init();
}
