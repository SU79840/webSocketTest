#include <iostream>
#include "EasyWsClient.hpp"
#include <assert.h>
#include <string>

int main()
{
    //std::cout << "Hello World!" << std::endl;
    std::unique_ptr<easywsclient::WebSocket> ws(easywsclient::WebSocket::from_url("ws://118.178.135.125:8188/mrtc"));
    assert(ws);
    //ws->send("goodbye");
    ws->send("world");
    while (ws->getReadyState() != easywsclient::WebSocket::readyStateValues::CLOSED) {
        easywsclient::WebSocket::pointer wsp = &*ws; // <-- because a unique_ptr cannot be copied into a lambda
        ws->poll();
           printf(">>> 111\n");
        ws->dispatch([wsp](const std::string & message) {
            printf(">>> %s\n", message.c_str());
            if (1) { wsp->close(); }
        });
    }

    printf(">>> \n");
    return 0;
}
