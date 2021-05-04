#include "pc_server.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void PCServer_server(void);

void PCServer_init(void)
{
    TCLauncher_addLaunchFunction(PCServer_server, "process_comm_server");

    TCDirectory_mkdir("", "parameter");

    TCShare_create("parameter/", "value", CCInt64_create(100));

    TCDirectory_mkdir("parameter/", "motor");

    TCDirectory_mkdir("parameter/motor/", "front_right");
    TCDirectory_mkdir("parameter/motor/", "front_left");
    TCDirectory_mkdir("parameter/motor/", "back_right");
    TCDirectory_mkdir("parameter/motor/", "back_left");

    TCShare_create("parameter/motor/front_right/", "power", CCInt64_create(90));
    TCShare_create("parameter/motor/front_left/", "power", CCInt64_create(80));
    TCShare_create("parameter/motor/back_right/", "power", CCInt64_create(70));
    TCShare_create("parameter/motor/back_left/", "power", CCInt64_create(60));

}


void PCServer_server(void)
{
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    CCAutoRelease_startScope();
    {
        struct sockaddr_in ws_address;
        ws_address.sin_family = AF_INET;
        ws_address.sin_port = htons(9000);
        ws_address.sin_addr.s_addr = inet_addr("10.0.128.121");
        
        CC_obj init_header = CCAutoRelease_add(CCDictionary_create());
        CCDictionary_setObject(init_header, CCString_create("init"), "type");
        CCDictionary_setObject(init_header, CCString_create("ogi"), "id");
        CCDictionary_setObject(init_header, CCString_create("10.0.128.122"), "address");
        CCDictionary_setObject(init_header, CCInt32_create(12345), "port");

        CC_obj init_json = CCJsonSerializer_dumps(init_header, CC_BOOL_FALSE);
        sendto(sock, CCString_getCString(init_json), CCString_length(init_json), 0, (struct sockaddr *)&ws_address,  sizeof(ws_address));
    }
    CCAutoRelease_doneScope();

    char buff[2048];
    memset(buff, 0, sizeof(buff));

	struct sockaddr_in client_address;
	socklen_t client_address_len = 0;

    while(1)
    {
        ssize_t recv_len = recvfrom(sock, buff, sizeof(buff), 0, (struct sockaddr *)&client_address, &client_address_len);
        if(0 < recv_len)
        {
            buff[recv_len] = '\0';
            CCLOG_INFO("received: '%s' from client %s[%d]\n", buff, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

            CCAutoRelease_startScope();
            {
                CC_obj js_string = CCAutoRelease_add(CCString_createWithLength(buff, recv_len));
                CC_obj order = CCAutoRelease_add(CCDictionary_create());
                CCDictionary_setObject(order, CCString_create(""), "type");
                CCDictionary_setObject(order, CCString_create(""), "path");
                CCDictionary_setObject(order, CCString_create(""), "json");
                CCJsonSerializer_staticLoads(js_string, order);
                if(CCObject_isObject(order))
                {
                    CC_obj type_string = CCAutoRelease_add(CCDictionary_objectForKey(order, "type"));
                    CC_obj path_string = CCAutoRelease_add(CCDictionary_objectForKey(order, "path"));

                    if(strncmp(CCString_getCString(type_string), "POST", strlen("POST")) == 0)
                    {
                        CC_obj split_path = CCAutoRelease_add(TCDirectory_splitPath(CCString_getCString(path_string)));
                        CC_obj only_path = CCAutoRelease_add(CCString_create(""));
                        for(size_t i = 0; i < CCArray_count(split_path) - 1; i++)
                        {
                            CCString_add(only_path, CCString_getCString(CCArray_objectAtIndex(split_path, i)));
                            CCString_add(only_path, "/");
                        }

                        CC_obj post_string = CCAutoRelease_add(CCDictionary_objectForKey(order, "json"));
                        CC_obj posted_obj = TCDirectory_openObj(CCString_getCString(only_path), CCString_getCString(CCArray_objectAtIndex(split_path, CCArray_count(split_path) - 1)));
                        CCJsonSerializer_staticLoads(post_string, posted_obj);
                    }
                    else if(strncmp(CCString_getCString(type_string), "GET", strlen("GET")) == 0)
                    {
                        CC_obj res_js = CCAutoRelease_add(TCDirectory_ls(CCString_getCString(path_string)));
                        if(0 < CCString_length(res_js))
                        {
                            CCLOG_INFO_NOFMT(CCString_getCString(res_js));
                            sendto(sock, CCString_getCString(res_js), CCString_length(res_js), 0, (struct sockaddr *)&client_address,  sizeof(client_address));
                        }
                    }   
                }
            }
            CCAutoRelease_doneScope();
        }
    }

    close(sock);
}
