#ifndef MICROMACHINES_CONTACTLISTENER_H
#define MICROMACHINES_CONTACTLISTENER_H

#include <Box2D/Box2D.h>

class ContactListener : public b2ContactListener{
private:
    std::shared_ptr<b2World> _world;

public:
    explicit ContactListener(std::shared_ptr<b2World> world);
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;

    void carVsGroundArea(b2Fixture* carFixture, b2Fixture* groundAreaFixture, bool began);
    void handleContact(b2Contact* contact, bool began);
};

#endif //MICROMACHINES_CONTACTLISTENER_H
