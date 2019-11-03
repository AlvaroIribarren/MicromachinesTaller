//
// Created by alvaro on 1/11/19.
//

#ifndef MICROMACHINES_PLAYASUSEREVENT_H
#define MICROMACHINES_PLAYASUSEREVENT_H


#include "Event.h"

class PlayAsUserEvent: public Event{
public:
	//TODO: falta constructor con argumentos
    virtual void send(Protocol &protocol) override;
    virtual ~PlayAsUserEvent() = default;
};


#endif //MICROMACHINES_PLAYASUSEREVENT_H
