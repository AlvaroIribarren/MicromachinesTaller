#ifndef MICROMACHINES_CONTACTLISTENER_H
#define MICROMACHINES_CONTACTLISTENER_H

#include <Box2D/Box2D.h>

class ContactListener : public b2ContactListener{
private:
    b2World *_world;

public:
    explicit ContactListener(b2World *world);
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
};

#endif //MICROMACHINES_CONTACTLISTENER_H