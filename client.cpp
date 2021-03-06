#include "GameClient.hpp"
#include <SFML/System/Clock.hpp>
#include <enet/enet.h>
#include <iostream>

int main(int argc, char** argv)
{
        Uint16 port = PORT_CL;
        GameClient client;

        if (enet_initialize() != 0)
        {
                std::cerr << "Could not initialize enet." << std::endl;
                return EXIT_FAILURE;
        }

        if (argc == 2)
        {
                port = std::atoi(argv[1]);
                std::cout << "Port " << port << std::endl;
        }

        if (!client.create(port))
        {
                enet_deinitialize();
                return EXIT_FAILURE;
        }

        if (!client.connect("localhost", PORT_SV))
        {
                enet_deinitialize();
                return EXIT_FAILURE;
        }

        sf::Clock clock;
        sf::Time accumulator;

        while (client.isRunning())
        {
                accumulator += clock.restart();

                while (accumulator >= TIME_PER_TICK)
                {
                        accumulator -= TIME_PER_TICK;

                        client.update(TIME_PER_TICK);
                }

                client.draw();
        }

        enet_deinitialize();

        return EXIT_SUCCESS;
}
