#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <unistd.h>

#include "EasyWsClient.hpp"
#include <assert.h>
#include <string>
#include <thread>
#include <memory>

#include <stdlib.h>
#include <stdio.h>
#include "Audiofifo.hpp"
bool closeFlag = false;

void checkArgs(){
    while(1){
        if(closeFlag == false) {
            printf("closeFlag false\n");
        } else {
            printf("closeFlag true\n");
            break;
        }
        usleep(100);
    }

}

int main()
{
    //std::cout << "Hello World!" << std::endl;
//while(1) {
//for(int i=0; i<3; i++) {
    std::cout<<sizeof(int)<<std::endl;
    std::cout<<sizeof(char)<<std::endl;
    // std::cout<<sizeof(char*)<<std::endl;
    FILE *fp = NULL;
    fp = fopen("/Users/su/Documents/mudu/webSocketTest/pcm.pcm", "rb+");
    FILE *outfile = NULL;
    outfile = fopen("/Users/su/Documents/mudu/webSocketTest/out.pcm", "wb+");
    if(NULL == fp) {
        return -1;
    }
    //AudioFifo *fifo = new AudioFifo();
    //std::unique_ptr<AudioFifo> fifo= std::make_unique<AudioFifo>();
    std::unique_ptr<AudioFifo> fifo(new AudioFifo());
    fifo->initFifo(1024);
//    char buf[44100*10*5] ={0};
//    char out[44100*10*5] ={0};
    char *buf = (char*)malloc(sizeof(char*)* 44100*10*5);
    char *out = (char*)malloc(sizeof(char*)* 44100*10*5);
    fread(buf,1,44100*5,fp);
    fifo->writeData(buf, 44100*5);
    //fseek(fp, 44100*5);
    fread(buf,1,44100*5,fp);
    //buf[44100*10*5] ={0};
    fifo->writeData(buf, 44100*5);
    for(int i=0; i<5;i++){
        fifo->readData(out,44100*2);
        fwrite(out, 1,  44100*2, outfile);
    }

    fclose(fp);
    fclose(outfile);
    free(buf);
    free(out);

//    char data[100] = {127, 127,127,127,122, 98,80,111,79};
//    char *p = data;
//    fifo->writeData(p, 9);
//    fifo->writeData(p, 5);
//    fifo->writeData(p, 5);
//    char out[4];
//    int size = fifo->getBufferSampleSize();
//    //out = (char *)malloc(sizeof(char*)*5);
//    fifo->readData(out, 4);
//    fifo->writeData(p, 5);
//     char out2[4];
//     fifo->readData(out2, 11);
//    //std::cout<<out[3]<<std::endl;
//    //free(out);
//    bool flag = fifo->isReadEmpty();
//    std::cout<<flag<<std::endl;

#if 0
    std::thread checkThread(checkArgs);
    checkThread.detach();
    std::string result;
    int flag = 0;
    std::unique_ptr<easywsclient::WebSocket> ws(easywsclient::WebSocket::from_url("ws://118.178.135.125:8188/mrtc"));
    assert(ws);
    ws->send("{\"mrtc\": \"create\", \"transaction\": \"tXGnh0l7EsA1\"}");
    //ws->send("\"mrtc\":\"attach\",\"plugin\":\"mrtc.plugin.videoroom\",\"transaction\":\"Zaw1eWM42gU2\",\"force-bundle\":true,\"force-rtcp-mux\":true,\"session_id\":1679135881862937");
    //ws->send("\"mrtc\":\"keepalive\",\"session_id\":4130559979094865,\"transaction\":\"YL1l3S6eSqp7\"");
    printf(">>> 222\n");
    while (ws->getReadyState() != easywsclient::WebSocket::readyStateValues::CLOSED && flag == 0) {
        easywsclient::WebSocket::pointer wsp = &*ws; // <-- because a unique_ptr cannot be copied into a lambda
        ws->poll();
        //   printf(">>> 111\n");
        ws->dispatch([wsp, &result, &flag](const std::string & message) {
            printf(">>> %s\n", message.c_str());
            result = message;
            flag = 1;
            if (0) { wsp->close(); }
        });
    }


//}
    flag = 0;
    printf(">>>%s\n", result.c_str());

    rapidjson::Document d;
    d.Parse(result.c_str());
    // 2. 利用 DOM 作出修改。
    rapidjson::Value& s = d["data"];


    rapidjson::Value &sid = s["id"];

    std::cout << sid.GetInt64() << std::endl;

    const char* json = "{\"mrtc\":\"attach\",\"plugin\":\"mrtc.plugin.videoroom\",\"transaction\":\"Zaw1eWM42gU2\",\"force-bundle\":true,\"force-rtcp-mux\":true,\"session_id\":1679135881862937}";
    printf(">>>%s\n", json);
    rapidjson::Document d2;
    d2.Parse(json);

    rapidjson::Value& s2 = d2["session_id"];
    s2.SetInt64(sid.GetInt64());
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d2.Accept(writer);
    std::cout << buffer.GetString() << std::endl;


    closeFlag = true;
    ws->send(buffer.GetString());
    while (ws->getReadyState() != easywsclient::WebSocket::readyStateValues::CLOSED && flag == 0) {
        easywsclient::WebSocket::pointer wsp = &*ws; // <-- because a unique_ptr cannot be copied into a lambda
        ws->poll();
        //   printf(">>> 111\n");
        ws->dispatch([wsp, &result, &flag](const std::string & message) {
            printf(">>> %s\n", message.c_str());
            result = message;
            flag = 1;
            if (0) { wsp->close(); }
        });
    }

    flag = 0;

     std::cout << result.c_str() << std::endl;

     rapidjson::Document handIdDoc;
     handIdDoc.Parse(result.c_str());

     rapidjson::Value& tmp = handIdDoc["data"];
      rapidjson::Value& handId = tmp["id"];

    const char* json_2 = "{\"mrtc\":\"message\",\"body\":{\"request\":\"joinandconfigure\",\"room\":317231034,\"ptype\":\"publisher\",\"display\":\"pgm\",\"bitrate\":128000},\"transaction\":\"zFOZPy473Vwu\",\"session_id\":1773955743750197,\"handle_id\":3421669225717217}";
    printf(">>>%s\n", json_2);
    rapidjson::Document d3;
    d3.Parse(json_2);

    rapidjson::Value& s3 = d3["session_id"];
    s3.SetInt64(sid.GetInt64());
    rapidjson::Value& s31 = d3["handle_id"];
    s31.SetInt64(handId.GetInt64());

    rapidjson::StringBuffer buffer3;
    rapidjson::Writer<rapidjson::StringBuffer> writer3(buffer3);
    d3.Accept(writer3);
    std::cout << buffer3.GetString() << std::endl;


    ws->send(buffer3.GetString());
    while (ws->getReadyState() != easywsclient::WebSocket::readyStateValues::CLOSED && flag == 0) {
        easywsclient::WebSocket::pointer wsp = &*ws; // <-- because a unique_ptr cannot be copied into a lambda
        ws->poll();
        //   printf(">>> 111\n");
        ws->dispatch([wsp, &result, &flag](const std::string & message) {
            printf(">>> %s\n", message.c_str());
            result = message;
            flag = 1;
            if (0) { wsp->close(); }
        });
    }

    flag = 0;
   //  std::cout << buffer3.GetString() << std::endl;


    const char *keepalive = "{\"mrtc\":\"keepalive\",\"session_id\":1773955743750197,\"transaction\":\"lj9760io7AsD\"}";

    rapidjson::Document keepaliveDoc;
    keepaliveDoc.Parse(keepalive);
    rapidjson::Value& keepalivetmp = keepaliveDoc["session_id"];
    keepalivetmp.SetInt64(sid.GetInt64());

    rapidjson::StringBuffer buffer4;
    rapidjson::Writer<rapidjson::StringBuffer> writer4(buffer4);
    keepaliveDoc.Accept(writer4);
    std::cout << buffer4.GetString() << std::endl;

for (int i=0; i<=10; i++) {
    ws->send (buffer4.GetString());

    while (ws->getReadyState() != easywsclient::WebSocket::readyStateValues::CLOSED && flag == 0) {
        easywsclient::WebSocket::pointer wsp = &*ws; // <-- because a unique_ptr cannot be copied into a lambda
        ws->poll();
        //   printf(">>> 111\n");
        ws->dispatch([wsp, &result, &flag](const std::string & message) {
            printf(">>> %s\n", message.c_str());
            result = message;
            flag = 1;
            if (0) { wsp->close(); }
        });
    }
    flag = 0;
    if (i == 5)
        ws->close();
    if (ws->getReadyState() == easywsclient::WebSocket::readyStateValues::CLOSED)
        printf(">>> %s\n", "closed");
    sleep(1);
}

#endif
    return 0;
}
