//
// Created by alvaro on 4/11/19.
//

#ifndef MICROMACHINES_EVENTRECEIVER_H
#define MICROMACHINES_EVENTRECEIVER_H


#include <memory>
#include "../../Common/Protocol.h"
#include "../../Common/Event/Event.h"
#include "../../Common/SafeQueue.h"
#include "../../Common/Thread.h"

class EventReceiver : public Thread{
private:
    Protocol& protocol;
    SafeQueue<std::shared_ptr<Event>>& receivingNonBlockingQueue;
    bool& keepTalking;

public:
    EventReceiver(Protocol& protocol, SafeQueue<std::shared_ptr<Event>>& nonBlockingQueue, bool& keepTalking);
    void run() override;
    void joinThread() ;
};


#endif //MICROMACHINES_EVENTRECEIVER_H
