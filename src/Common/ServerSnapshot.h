#ifndef SERVER_SNAPSHOT_H
#define SERVER_SNAPSHOT_H

#include <vector>
#include <map>
#include <string>
#include "Protocol.h"

struct CarStruct {
	float x;
	float y;
	int angle;
	int health;
	int id;
};

typedef std::vector<CarStruct> CarList;

//TODO: protocolo/socket por argumento. Se sabe enviar y recibir

class ServerSnapshot {
private:
	CarList carList;
public:
	ServerSnapshot() = default;
    //Hace un receive interno.
    explicit ServerSnapshot(Protocol& protocol);

	void setCar(float x, float y, int angle, int health, int id);

	CarList& getCars();

	//Envia el estado de todos los autos.
	void send(Protocol& protocol);
};

#endif // SERVER_SNAPSHOT_H