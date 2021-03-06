#include "TextureCreator.h"
#include <iostream>
#include "../Common/Constants.h"

#include "View/StraightTrackView.h"
#include "View/CurveTrackView.h"
#include "View/HealthPowerupView.h"
#include "View/BoostPowerupView.h"
#include "View/RockView.h"
#include "View/CarView.h"
#include "View/OilView.h"
#include "View/MudView.h"
#include "View/HealthBarBackView.h"
#include "View/HealthBarFrontView.h"
#include "View/ExplosionView.h"
#include "View/MudSplatView.h"
#include "View/RoomView.h"
#include "View/StartLineView.h"
#include "View/ArrowView.h"
#include "View/LapsTextView.h"

TextureCreator::TextureCreator(const SdlWindow& window) :
        straightTrackTex(IMAGES_DIR + std::string("straight_track.png"), window),
        curveTrackTex(IMAGES_DIR + std::string("curve_track.png"), window),
        powerupTex(IMAGES_DIR + std::string("powerups.png"), window),
        rockTex(IMAGES_DIR + std::string("rock.png"), window),
        carRedTex(IMAGES_DIR + std::string("car_red.png"), window),
        carBlueTex(IMAGES_DIR + std::string("car_blue.png"), window),
        carYellowTex(IMAGES_DIR + std::string("car_yellow.png"), window),
        carGreenTex(IMAGES_DIR + std::string("car_green.png"), window),
        oilTex(IMAGES_DIR + std::string("oil.png"), window),
        mudTex(IMAGES_DIR + std::string("mud.png"), window),
        explosionTex(IMAGES_DIR + std::string("explosion.png"), window),
        mudSplatTex(IMAGES_DIR + std::string("mud_splat.png"), window),
        room1Tex(IMAGES_DIR + std::string("room1.png"), window),
        room2Tex(IMAGES_DIR + std::string("room2.png"), window),
        room3Tex(IMAGES_DIR + std::string("room3.png"), window),
        room4Tex(IMAGES_DIR + std::string("room4.png"), window),
        startLineTex(IMAGES_DIR + std::string("start.png"), window),
        arrowTex(IMAGES_DIR + std::string("arrow.png"), window),
        carSelectedTex(IMAGES_DIR + std::string("car_selected.png"), window),
        lapsTextTex(IMAGES_DIR + std::string("laps.png"), window),
        healthBarBackTex(IMAGES_DIR + std::string("health_background.png"), window),
        healthBarFrontTex(IMAGES_DIR + std::string("health_bar.png"), window) {}


ObjectViewPtr TextureCreator::create(ObjectType type, int x, int y, int angle) {
    ObjectViewPtr ov;
    switch (type) {
        case TYPE_STRAIGHT_TRACK: ov.reset(new StraightTrackView(straightTrackTex, angle)); break;
        case TYPE_CURVE_TRACK: ov.reset(new CurveTrackView(curveTrackTex, angle)); break;
        case TYPE_HEALTH_POWERUP: ov.reset(new HealthPowerupView(powerupTex)); break;
        case TYPE_BOOST_POWERUP: ov.reset(new BoostPowerupView(powerupTex)); break;
        case TYPE_ROCK: ov.reset(new RockView(rockTex)); break;
        case TYPE_CAR_RED: ov.reset(new CarView(carRedTex, angle)); break;
        case TYPE_CAR_BLUE: ov.reset(new CarView(carBlueTex, angle)); break;
        case TYPE_CAR_YELLOW: ov.reset(new CarView(carYellowTex, angle)); break;
        case TYPE_CAR_GREEN: ov.reset(new CarView(carGreenTex, angle)); break;
        case TYPE_OIL: ov.reset(new OilView(oilTex, angle)); break;
        case TYPE_MUD: ov.reset(new MudView(mudTex, angle)); break;
        case TYPE_EXPLOSION: ov.reset(new ExplosionView(explosionTex)); break;
        case TYPE_MUD_SPLAT: ov.reset(new MudSplatView(mudSplatTex)); break;
        case TYPE_ROOM_1: ov.reset(new RoomView(room1Tex)); break;
        case TYPE_ROOM_2: ov.reset(new RoomView(room2Tex)); break;
        case TYPE_ROOM_3: ov.reset(new RoomView(room3Tex)); break;
        case TYPE_ROOM_4: ov.reset(new RoomView(room4Tex)); break;
        case TYPE_START_LINE: ov.reset(new StartLineView(startLineTex, angle)); break;
        case TYPE_ARROW: ov.reset(new ArrowView(arrowTex)); break;
        case TYPE_CAR_SELECTED: ov.reset(new CarView(carSelectedTex, angle)); break;
        case TYPE_LAPS_TEXT: ov.reset(new LapsTextView(lapsTextTex)); break;
        case TYPE_HEALTH_BAR_BACK: ov.reset(new HealthBarFrontView(healthBarBackTex)); break;
        case TYPE_HEALTH_BAR_FRONT: ov.reset(new HealthBarFrontView(healthBarFrontTex)); break;
        default: throw std::runtime_error("Texture Creator: Wrong view_id");
    }
    ov->setInitialPos(x, y);
    return ov;
}