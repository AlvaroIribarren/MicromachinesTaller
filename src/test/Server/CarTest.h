#ifndef MICROMACHINES_TESTCAR_H
#define MICROMACHINES_TESTCAR_H

#define DEGTORAD 0.0174532925199432957f

#include <cppunit/extensions/HelperMacros.h>

#include "src/Server/Model/Car/Car.h"

class CarTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE( CarTest );
        CPPUNIT_TEST( testCreateCarInTheExitLine );
        //CPPUNIT_TEST( testCreateMultipleCarsInTheExitLineOneNextToEachOther );
        CPPUNIT_TEST( testStart );
        CPPUNIT_TEST( testIncreaseSpeed );
        CPPUNIT_TEST( testDecreaseSpeed );
        CPPUNIT_TEST( testTurnRightWithHighManeuverabilityAndHighGrab );
        CPPUNIT_TEST( testTurnRightWithHighManeuverabilityAndLowGrab );
        CPPUNIT_TEST( testTurnRightWithLowManeuverabilityAndHighGrab );
        CPPUNIT_TEST( testTurnRightWithLowManeuverabilityAndLowGrab );
        CPPUNIT_TEST( testTurnLeftWithHighManeuverabilityAndHighGrab );
        CPPUNIT_TEST( testTurnLeftWithHighManeuverabilityAndLowGrab );
        CPPUNIT_TEST( testTurnLeftWithLowManeuverabilityAndHighGrab );
        CPPUNIT_TEST( testTurnLeftWithLowManeuverabilityAndLowGrab );
        CPPUNIT_TEST( testExplodesWhenHealthIn0 );
    CPPUNIT_TEST_SUITE_END();

private:
    b2World* world;
    Car *car1, *car2, *car3;
    float _car_x_init = 15;
    float _car_y_init = 7;

public:
    void setUp(){
        b2Vec2 gravity(0, 0);
        world = new b2World(gravity);

        b2BodyDef _carBodyDef;
        _carBodyDef.type = b2_dynamicBody;
        _carBodyDef.position.Set(15, 7);
        _carBodyDef.angle = 180 * DEGTORAD;

        b2Body* carBody = world->CreateBody(&_carBodyDef);

        car1 = new Car(carBody);
    }

    void tearDown(){
        //delete car1;
    }

    void testCreateCarInTheExitLine(){
        std::cout << "TESTS CAR\n";

        std::cout << "TEST create car in the exit line: \n";

        CPPUNIT_ASSERT(car1->x() == _car_x_init);
        CPPUNIT_ASSERT(car1->y() == _car_y_init);
        CPPUNIT_ASSERT(car1->speed() == 0);
        //See how to check state
        std::cout << "OK\n";
    }

    void testCreateMultipleCarsInTheExitLineOneNextToEachOther(){
        std::cout << "TEST create multiple cars in the exit line: \n";

        CPPUNIT_ASSERT(car1->x() == _car_x_init);
        CPPUNIT_ASSERT(car1->y() == _car_y_init);

        //TODO: ver distancias entre autos
        CPPUNIT_ASSERT(car2->x() == _car_x_init + 0.1);
        CPPUNIT_ASSERT(car2->y() == _car_y_init);

        CPPUNIT_ASSERT(car3->x() == _car_x_init + 0.2);
        CPPUNIT_ASSERT(car3->y() == _car_y_init);
        std::cout << "OK\n";
    }

    void testStart(){
        std::cout << "TEST start car: \n";

        car1->handleInput(PRESS_UP);
        car1->update();
        world->Step(1/30.0, 8, 3);
        CPPUNIT_ASSERT(car1->speed() > 0);

        std::cout << "OK\n";
    }

    void testIncreaseSpeed(){
        std::cout << "TEST increase speed: \n";


        //CPPUNIT_ASSERT(car1->speed() == );

        std::cout << "OK\n";
    }

    void testDecreaseSpeed(){
        std::cout << "TEST decrease speed: \n";

        //CPPUNIT_ASSERT(car1->speed() == );

        std::cout << "OK\n";
    }

    void testTurnRightWithLowManeuverabilityAndLowGrab(){
        std::cout << "TEST turn right with low maneuvrability and low grab: \n";

        std::cout << "OK\n";
    }

    void testTurnRightWithLowManeuverabilityAndHighGrab(){
        std::cout << "TEST turn right with low maneuvrability and high grab: \n";

        std::cout << "OK\n";
    }

    void testTurnRightWithHighManeuverabilityAndLowGrab(){
        std::cout << "TEST turn right: \n";
        std::cout << "TEST turn right with high maneuvrability and low grab: \n";

        std::cout << "OK\n";
    }

    void testTurnRightWithHighManeuverabilityAndHighGrab(){
        std::cout << "TEST turn right with high maneuvrability and high grab: \n";

        std::cout << "OK\n";
    }

    void testTurnLeftWithLowManeuverabilityAndLowGrab(){
        std::cout << "TEST turn left with low maneuvrability and low grab: \n";

        std::cout << "OK\n";
    }

    void testTurnLeftWithLowManeuverabilityAndHighGrab(){
        std::cout << "TEST turn left with low maneuvrability and high grab: \n";

        std::cout << "OK\n";
    }

    void testTurnLeftWithHighManeuverabilityAndLowGrab(){
        std::cout << "TEST turn left: \n";
        std::cout << "TEST turn left with high maneuvrability and low grab: \n";

        std::cout << "OK\n";
    }

    void testTurnLeftWithHighManeuverabilityAndHighGrab(){
        std::cout << "TEST turn left with high maneuvrability and high grab: \n";

        std::cout << "OK\n";
    }

    void testExplodesWhenHealthIn0(){
        std::cout << "TEST explodes when health is 0: \n";

        std::cout << "OK\n";
    }

};

#endif //MICROMACHINES_CARTEST_H
