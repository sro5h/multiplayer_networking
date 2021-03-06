#ifndef HOST_HPP_INCLUDED
#define HOST_HPP_INCLUDED

#include "Event.hpp"
#include <unordered_map>

/**
 * Manages connections and sends Packets to connected Peers.
 */
class Host
{
public:
        /**
         * Initializes the Host to an invalid state.
         */
        explicit Host();

        /**
         * Cleans up the ENetHost and its connected ENetPeers if needed.
         */
        ~Host();

        /**
         * Creates the Host on address:port with a maximum of connections
         * imcoming peer connections.
         * @param address The address to create the Host on
         * @param port The port to create the Host on
         * @param connections The maximum amount of connections to accept
         * @return True if successful
         */
        bool create(const std::string& address, const Uint16 port, const std::size_t connections);

        /**
         * Cleans up the ENetHost and its connected ENetPeers if needed and
         * leaves the Host in an invalid state.
         */
        void destroy();

        /**
         * Attempts a connection with a Peer on address:port.
         * @param address The address to connect to
         * @param port The port to connect to
         * return True if an attempt was made
         */
        bool connect(const std::string& address, const Uint16 port);

        /**
         * Attempts a disconnection from a connected Peer.
         * @param peer The Peer to disconnect from
         */
        void disconnect(const Peer& peer);

        /**
         * Attempts a disconnection from all connected Peers.
         */
        void disconnectAll();

        /**
         * Polls the Host for incoming Events.
         * @param event The Event
         * @return True if an Event was found
         */
        virtual bool pollEvent(Event& event);

        /**
         * Sends any queued packets on the host to its connected peers.
         */
        void flush();

        /**
         * Get the number of connected Peers.
         * @return The number of connected Peers
         */
        std::size_t getConnectionCount() const;

        /**
         * Send a Packet to all connected Peers.
         * @param packet The Packet to send
         */
        void broadcast(const Packet& packet);

        /**
         * Sends a Packet to all connected Peers except one.
         * @param peer The Peer to ignore
         * @param packet The Packet to send
         */
        void broadcastExcept(const Peer& peer, const Packet& packet);

        /**
         * Sends a Packet to the specified Peer.
         * @param peer The Peer to send the Packet to
         * @param packet The Packet to send
         */
        virtual bool send(const Peer& peer, const Packet& packet);

public:
        /**
         * Returns whether the Host is in a valid, initialized state.
         * @return True if the Host is valid
         */
        explicit operator bool() const;

private:
        struct PeerData
        {
                Uint16 outgoingId;
                Uint32 connectId;
        };

private:
        /**
         * Converts an ENetEvent to an Event.
         * @param enetEvent The ENetEvent to convert
         * @param event The Event to store the data in
         */
        void convertENetEvent(const ENetEvent& enetEvent, Event& event);

        /**
         * Converts an ENetPeer to a Peer.
         * @param enetPeer The ENetPeer to convert
         * @param peer The Peer to store the data in
         */
        void convertENetPeer(ENetPeer& enetPeer, Peer& peer);

private:
        ENetHost* mHost;
        std::unordered_map<ENetPeer*, PeerData> mPeerData;
};

#endif // HOST_HPP_INCLUDED
