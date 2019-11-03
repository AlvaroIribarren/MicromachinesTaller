#include <iostream>
#include "Player.h"
#include "../Common/Event/SnapshotEvent.h"

Player::Player(Socket socket, Car* car) : _protocol(std::move(socket)), _car(car){}

void Player::handleInput(const InputEnum& input){
    std::cout << "In handle input: " << input << '\n';
    switch (input) {
        case ACCELERATE:
            _car->handleInput(PRESS_UP, PRESS_NONE);
            break;
        case STOP_ACCELERATING:
            _car->handleInput(RELEASE_UP, PRESS_NONE);
            break;
        case DESACCELERATE:
            _car->handleInput(PRESS_DOWN, PRESS_NONE);
            break;
        case STOP_DESACCELERATING:
            _car->handleInput(RELEASE_DOWN, PRESS_NONE);
            break;
        case TURN_RIGHT:
            _car->handleInput(PRESS_NONE, PRESS_RIGHT);
            break;
        case STOP_TURNING_RIGHT:
            _car->handleInput(PRESS_NONE, RELEASE_RIGHT);
            break;
        case TURN_LEFT:
            _car->handleInput(PRESS_NONE, PRESS_LEFT);
            break;
        case STOP_TURNING_LEFT:
            _car->handleInput(PRESS_NONE, RELEASE_LEFT);
            break;
    }

    _car->update();
}

void Player::receive(std::string& received){
    received = _protocol.receive();
}

void Player::send(){
    std::cout << "Sending\n";
    SnapshotEvent snap;
    snap.addExplosion(_car->x(), _car->y());
    snap.setCar(_car->x(), _car->y(), _car->angle() * RADTODEG, _car->health(), 11);
    snap.send(_protocol);
}