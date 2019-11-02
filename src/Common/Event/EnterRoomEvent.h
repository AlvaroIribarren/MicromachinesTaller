//
// Created by alvaro on 1/11/19.
//

#ifndef MICROMACHINES_ENTERROOMEVENT_H
#define MICROMACHINES_ENTERROOMEVENT_H


#include "Event.h"

class EnterRoomEvent: public Event{
public:
	//TODO: falta constructor con argumentos
    virtual void send(Protocol &protocol) override;
    virtual ~EnterRoomEvent() = default;
};


#endif //MICROMACHINES_ENTERROOMEVENT_H