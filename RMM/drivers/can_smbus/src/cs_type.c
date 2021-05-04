#include "cs_type.h"

#include <unistd.h>

void CSType_delay_us(size_t us)
{
    usleep((useconds_t)us);
}

void CSType_delay_ms(size_t ms)
{
    usleep((useconds_t)(ms * 1000));
}

void CSType_delay(float sec)
{
    usleep((useconds_t)(sec * 1000000));
}


// size_t CSType_getDataSize(CSType_data_t data_type)
// {
// 	switch(data_type)
// 	{
// 		case CSType_data_Int32:
// 			return 4;

// 		case CSType_data_UInt32:
// 			return 4;

// 		case CSType_data_Int16:
// 			return 2;

// 		case CSType_data_UInt16:
// 			return 2;

// 		case CSType_data_Int8:
// 			return 1;

// 		case CSType_data_UInt8:
// 			return 1;

// 		case CSType_data_Bin1:
// 			return 1;

// 		case CSType_data_Bin2:
// 			return 2;

// 		case CSType_data_Bin3:
// 			return 3;

// 		case CSType_data_Bin4:
// 			return 4;
		
// 		default:
// 			return 0;
// 	}
// }

// CC_bool_t CSType_isBinarryType(CSType_data_t data_type)
// {
// 	switch(data_type)
// 	{
// 		case CSType_data_Int32:
// 		case CSType_data_UInt32:
// 		case CSType_data_Int16:
// 		case CSType_data_UInt16:
// 		case CSType_data_Int8:
// 		case CSType_data_UInt8:
// 			return CC_BOOL_FALSE;

// 		case CSType_data_Bin1:
// 		case CSType_data_Bin2:
// 		case CSType_data_Bin3:
// 		case CSType_data_Bin4:
// 			return CC_BOOL_TRUE;

		
// 		default:
// 			return CC_BOOL_TRUE;
// 	}
// }


// CC_obj CSType_createtDataObj(CSType_data_t data_type)
// {
// 	switch(data_type)
// 	{
// 		case CSType_data_Int32:
// 			return CCInt32_create(0);

// 		case CSType_data_UInt32:
// 			return CCInt64_create(0);

// 		case CSType_data_Int16:
// 			return CCInt32_create(0);

// 		case CSType_data_UInt16:
// 			return CCInt32_create(0);

// 		case CSType_data_Int8:
// 			return CCInt32_create(0);

// 		case CSType_data_UInt8:
// 			return CCInt32_create(0);

// 		case CSType_data_Bin1:
// 			return CCBytes_create(1);

// 		case CSType_data_Bin2:
// 			return CCBytes_create(2);

// 		case CSType_data_Bin3:
// 			return CCBytes_create(3);

// 		case CSType_data_Bin4:
// 			return CCBytes_create(4);
		
// 		default:
// 			return NULL;
// 	}
// }

// void CSType_setDataObj(CC_obj obj, CSType_data_t data_type, void* data)
// {
// 	switch(data_type)
// 	{
// 		case CSType_data_Int32:
// 			*((int32_t*)data) = CCInt32_get(obj);

// 		case CSType_data_UInt32:
// 			*((uint32_t*)data) = CCInt64_get(obj);

// 		case CSType_data_Int16:
// 			*((int16_t*)data) = CCInt32_get(obj);

// 		case CSType_data_UInt16:
// 			*((uint16_t*)data) = CCInt32_get(obj);

// 		case CSType_data_Int8:
// 			*((uint16_t*)data) = CCInt32_get(obj);

// 		case CSType_data_UInt8:
// 			return CCInt64_create(0);

// 		case CSType_data_Bin1:
// 			return CCBytes_create(1);

// 		case CSType_data_Bin2:
// 			return CCBytes_create(2);

// 		case CSType_data_Bin3:
// 			return CCBytes_create(3);

// 		case CSType_data_Bin4:
// 			return CCBytes_create(4);
		
// 		default:
// 			return NULL;
// 	}
// }