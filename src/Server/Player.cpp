#include "Player.h"
#include "../Common/Event/SnapshotEvent.h"
#include <iostream>
#include <utility>
#include "../Server/Model/Car/Car.h"

Player::Player(std::shared_ptr<Car> car, int& inRoomId) :
                        _car(std::move(car)),
                        inRoomId(inRoomId),
                        _modifierToAdd(false),
                        done(false){}

int Player::update(){
    int numberOfLaps = _car->update();

    std::vector<Effect*> aux;
    for (auto & _effect : _effects){
        _effect->timeOfAction --;
        if (_effect->timeOfAction == 0){
            _car->stopEffect(_effect->type);
            delete _effect;
        } else {
            aux.push_back(_effect);
        }
    }
    _effects.swap(aux);
    return numberOfLaps;
}


void Player::handleInput(const InputEnum& input){
    _car->handleInput(input);
}

void Player::receive(std::string& received, Protocol& protocol){
    received = protocol.receive();
}

void Player::_addEffect(const int& effectType, const int& timeOfAction){
    auto* effect = new Effect;
    effect->type = effectType;
    effect->timeOfAction = timeOfAction;
    _effects.push_back(effect);
}

void Player::createModifier(const size_t& type, const size_t& id, const float& x, const float& y, const float& angle){
    _modifierToAdd = true;
    _modifierDTO.id = id;
    _modifierDTO.type = type;
    _modifierDTO.x = x;
    _modifierDTO.y = y;
    _modifierDTO.angle = angle;
}


void Player::modifySnapshot(const std::shared_ptr<SnapshotEvent>& snapshot){
    std::vector<std::shared_ptr<Status>> status = _car->getStatus();

    for (auto & statu : status){
        switch (statu->status) {
            case NOTHING :
                break;
            case EXPLODED:
                snapshot->addGameItem(TYPE_EXPLOSION, _car->x(), _car->y(), _car->angle(), inRoomId);
                break;
            case GRABBED_HEALTH_POWERUP :
                snapshot->removeGameItem(TYPE_HEALTH_POWERUP, statu->id);
                break;
            case GRABBED_BOOST_POWERUP :
                _addEffect(TYPE_BOOST_POWERUP, statu->timeOfAction);
                snapshot->removeGameItem(TYPE_BOOST_POWERUP, statu->id);
                break;
            case GRABBED_MUD :
                snapshot->setMudSplatEvent(this->inRoomId);
                snapshot->removeGameItem(TYPE_MUD, statu->id);
                break;
            case GRABBED_ROCK :
                snapshot->removeGameItem(TYPE_ROCK, statu->id);
                break;
            case GRABBED_OIL :
                _addEffect(TYPE_OIL, statu->timeOfAction);
                snapshot->removeGameItem(TYPE_OIL, statu->id);
                break;
            case WINNED :
                done = true;
                snapshot->setGameOver(inRoomId);
                break;
        }
    }

    _car->resetStatus();

    if (_modifierToAdd){
        snapshot->addGameItem(_modifierDTO.type, _modifierDTO.x, _modifierDTO.y, _modifierDTO.angle, _modifierDTO.id);
        _modifierToAdd = false;
    }

    snapshot->setCar(_car->x(), _car->y(), _car->angle() * RADTODEG, _car->health(), inRoomId);
}

std::shared_ptr<SnapshotEvent> Player::sendStart(json j) {
    std::shared_ptr<SnapshotEvent> snap(new SnapshotEvent);
    snap->setMap(j);
    snap->signalMapReady();
    return snap;
}

void Player::assignCarAndId(std::shared_ptr<Car> newCar) {
    newCar->assignId(this->inRoomId);
    this->_car = std::move(newCar);
}

bool Player::finishedPlaying() {
    return done;
}

void Player::setPlayerReadyToPlayAgain() {
    this->done = false;
}
