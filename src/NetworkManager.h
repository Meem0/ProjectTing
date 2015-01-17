#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "includes.h"
#include "Command.h"

class NetworkManager
{
public:
	// both
	NetworkManager();
	void initialize();
	void send();
	void receive();

	// client only
	void send(Command*);

	// server only
	void send(sf::Uint32, const std::string&, const sf::Vector2f&); // add sprite
	void send(sf::Uint32); // remove sprite 
	void send(sf::Uint32, const sf::Vector2f&); // update sprite
	void send(sf::Uint16, sf::Uint32); // set view follow
	void update();

private:
	// both
	void initializeUDP();

	struct Peer
	{
		Peer() : tcpSocket(), udpPort(0), id(0) {}
		Peer(sf::TcpSocket* s, unsigned short p, sf::Uint16 i)
			: tcpSocket(s), udpPort(p), id(i) {}

		sf::TcpSocket* tcpSocket;
		unsigned short udpPort;
		sf::Uint16 id;
	};

	sf::UdpSocket _udpSocket;
	sf::Packet _sendPacket;


	// client only
	void clientReceive(sf::Packet&);
	void initializeClient();
	void connectToServer();
	sf::IpAddress getInputAddress();

	Peer _server;
	

	// server only
	void serverReceive(sf::Packet&);
	void registerClient(sf::TcpSocket*);
	void checkDisconnectedClients();

	static const unsigned short CONNECTION_TIMER_MAX = 300;
	unsigned short _connectionTimer;
	sf::TcpListener _listener;
	std::vector<Peer> _clients;
	sf::Uint16 _currentClientId;
};

#endif