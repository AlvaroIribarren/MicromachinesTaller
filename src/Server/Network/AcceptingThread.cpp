//
// Created by alvaro on 30/9/19.
//


#include <future>
#include "AcceptingThread.h"
#include "../../Common/Protocol.h"
#include "../../Common/SocketError.h"

AcceptingThread::AcceptingThread(Socket &acceptSocket):
        acceptSocket(acceptSocket), roomController(running), running(true){}


 void AcceptingThread::run() {
    std::cout << "Starting acceptor" << std::endl;
    try {
        roomController.start();
        while (running) {
            try {
                Protocol newProtocol(this->acceptSocket.accept());
                std::cout << "Client connected" << std::endl;
                int clientId = clientCounter.addOneAndReturn();

                std::shared_ptr<ClientThread> newClientThread
                        (new ClientThread(std::move(newProtocol), roomController, clientId));

                roomController.addClient(clientId, newClientThread);

            } catch (SocketError &e){
                running = false;
            }
        }
    } catch(const std::exception &e) {
        printf("ERROR from accepting Thread: %s \n", e.what());
    } catch(...) {
        printf("Unknown error from accepting thread.");
    }
}

AcceptingThread::~AcceptingThread()= default;

