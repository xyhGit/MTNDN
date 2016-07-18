#ifndef CONTROLER_H
#define CONTROLER_H

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <thread>
#include <map>
#include <vector>

#include <QLabel>

#include "consumer.h"
#include "player.h"


class Controler
{
public:
    Controler();

    ~Controler()
    {}

    int addStream(std::string prefix);

    int addConsumer(std::string prefix);

    int startConsumer(int consumerId);

    int stopConsumer(int consumerId);

    Consumer* getConsumer(const int consumerId );

    void addFace(const string host, const int port);


//private:
    int consumerNumber_;
    int faceNumber_;
    map<int,Consumer*> consumersMap_;
    vector<Consumer*> consumersVec_;

    set<std::string> hostSet_;

    //map<int,std::thread*> consumerThreads_;

    boost::shared_ptr<FaceWrapper> FaceWrapper_;

//    Consumer* consumer;
//    Player* player_;

};

#endif // CONTROLER_H
