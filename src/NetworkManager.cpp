#include "NetworkManager.h"
#include "Game.h"
#include "Command.h"
#include "Player.h"

NetworkManager::NetworkManager() : _currentClientId(sf::Uint16(1)) { }

void NetworkManager::initialize()
{
	switch (Game::getGameType())
	{
	case Game::TYPE_CLIENT:
		initializeUDP();
		initializeClient();
		break;
	case Game::TYPE_SERVER:
		Game::setGameId(0);
		initializeUDP();
		
		_listener.setBlocking(false);
		_listener.listen(Constants::SERVER_TCP_PORT);
		std::cout << "Listening for TCP on port " << _listener.getLocalPort() << "\n";
		break;
	case Game::TYPE_SINGLE:
		Game::setGameId(0);
		break;
	}
}

void NetworkManager::initializeUDP()
{
	unsigned short i = 0;
	while (_udpSocket.bind(Constants::SERVER_UDP_PORT + i) != sf::Socket::Done)
	{
		i++;
		
		if (i == 100)
		{
			std::cout << "Failed to bind a UDP port 100 times, giving up.\n";
			std::cin >> i;
			return;
		}
	}
	std::cout << "Listening for UDP on port " << _udpSocket.getLocalPort() << "\n";
	_udpSocket.setBlocking(false);
}

void NetworkManager::send()
{
	if (!_sendPacket.endOfPacket())
	{
		if (Game::getGameType() == Game::TYPE_CLIENT)
			_udpSocket.send(_sendPacket, _server.tcpSocket->getRemoteAddress(), _server.udpPort);
		else
		{
			for (std::vector<Peer>::const_iterator itr = _clients.cbegin();
					itr != _clients.end(); itr++)
			{
				_udpSocket.send(_sendPacket, itr->tcpSocket->getRemoteAddress(), itr->udpPort);
			}
		}

		_sendPacket.clear();
	}
}

void NetworkManager::send(Command* command)
{
	_sendPacket << Game::getGameId() << command;
}

void NetworkManager::send(sf::Uint32 spriteId, const std::string& name, const sf::Vector2f& pos)
{
	_sendPacket << sf::Uint8(0) << spriteId << name << pos.x << pos.y;
}

void NetworkManager::send(sf::Uint32 spriteId)
{
	_sendPacket << sf::Uint8(1) << spriteId;
}

void NetworkManager::send(sf::Uint32 spriteId, const sf::Vector2f& pos)
{
	_sendPacket << sf::Uint8(2) << spriteId << pos.x << pos.y;
}

void NetworkManager::send(sf::Uint16 clientId, sf::Uint32 spriteId)
{
	_sendPacket << sf::Uint8(3) << clientId << spriteId;
}

void NetworkManager::receive()
{
	sf::Packet receivePacket;
	sf::IpAddress senderAddress;
	unsigned short senderPort;

	while (_udpSocket.receive(receivePacket, senderAddress, senderPort) == sf::Socket::Done)
	{
		if (Game::getGameType() == Game::TYPE_SERVER)
			serverReceive(receivePacket);
		else
			clientReceive(receivePacket);
	}
}

void NetworkManager::clientReceive(sf::Packet& receivePacket)
{
	sf::Uint8 action;
	sf::Uint16 gameId;
	sf::Uint32 spriteId;
	std::string spriteName;
	sf::Vector2f spritePos;

	while (!receivePacket.endOfPacket())
	{
		if (!(receivePacket >> action))
			std::cout << "Error extracting sprite operation flag from packet\n";
		else
		{
			switch (action)
			{
			case sf::Uint8(0):
				receivePacket >> spriteId >> spriteName >> spritePos.x >> spritePos.y;
				Game::getSpriteManager().registerSprite(spriteId, spriteName, spritePos);
				break;
			case sf::Uint8(1):
				receivePacket >> spriteId;
				Game::getSpriteManager().removeSprite(spriteId);
				break;
			case sf::Uint8(2):
				receivePacket >> spriteId >> spritePos.x >> spritePos.y;
				Game::getSpriteManager().updateSprite(spriteId, spritePos);
				break;
			case sf::Uint8(3):
				receivePacket >> gameId >> spriteId;
				if (Game::getGameId() == gameId)
					Game::getViewManager().setFollow(spriteId);
				break;
			default: std::cout << "Invalid sprite operation flag\n"; break;
			}
		}
	}
}

void NetworkManager::serverReceive(sf::Packet& receivePacket)
{
	Command* receiveCommand = NULL;
	sf::Uint16 senderId;
	
	while (!receivePacket.endOfPacket())
	{
		if (!(receivePacket >> senderId))
			std::cout << "Error extracting id from packet\n";

		if (!(receivePacket >> receiveCommand))
			std::cout << "Error extracting command from packet\n";

		auto playerId = Game::_gameObjectManager.getPlayer(senderId);
		if (playerId != Game::_gameObjectManager.playerEnd())
			Game::_gameObjectManager.get(playerId->second)->addCommand(receiveCommand);
	}
}

void NetworkManager::initializeClient()
{
	connectToServer();
	
	sf::Packet setupPacket;
	
	// send setup information to server:
	//    - UDP port
	setupPacket << _udpSocket.getLocalPort();
	if (_server.tcpSocket->send(setupPacket) != sf::Socket::Done)
		std::cout << "Error sending my UDP port to server.\n";
	
	// receive setup information from server:
	//    - this client's game ID
	//	  - server's UDP port
	//    - sprite information of all current game objects
	setupPacket.clear();
	if (_server.tcpSocket->receive(setupPacket) == sf::Socket::Done)
	{
		// this client's game ID
		sf::Uint16 gameId;
		if (setupPacket >> gameId)
		{
			Game::setGameId(gameId);
			std::cout << "I am client #" << gameId << '\n';
		}
		else
			std::cout << "Error extracting game ID from setup packet\n";
		
		// server's UDP port
		if (!(setupPacket >> _server.udpPort))
			std::cout << "Error extracting server's UDP port from setup packet\n";
		
		// sprite information of all current game objects
		sf::Uint32   spriteId;
		std::string  spriteName;
		sf::Vector2f spritePos;

		while (!setupPacket.endOfPacket())
		{
			if (setupPacket >> spriteId >> spriteName >> spritePos.x >> spritePos.y)
			{
				Game::getSpriteManager().registerSprite(spriteId, spriteName, spritePos);
				std::cout << "Sprite " << spriteId << "'s position: "
						  << Helpful::toString(spritePos) << '\n';
			}
			else std::cout << "Error extracting sprite data from setup packet\n";
		}
	}
	else
		std::cout << "Error receiving setup packet from server.\n";
}

void NetworkManager::connectToServer()
{
	bool attempt = true;
	_server.tcpSocket = new sf::TcpSocket;
	
	while (	attempt &&
			_server.tcpSocket->connect(getInputAddress(), Constants::SERVER_TCP_PORT)
				!= sf::Socket::Done)
	{ attempt = Helpful::consoleYesNo("Error connecting to server.  Try again?"); }
	
	std::cout << "Connected to server: " << _server.tcpSocket->getRemoteAddress()
			  << ":" << _server.tcpSocket->getRemotePort() << "\n";
}

sf::IpAddress NetworkManager::getInputAddress()
{
	std::string addressInput;
	bool done = false;
	while (!done)
	{
		std::cout << "Input server's IP address: ";
		if (!(std::cin >> addressInput))
		{
			std::cin.clear();
			std::cout << "Invalid input\n";
		}
		else
			done = true;
	}

	if (addressInput[0] == 'j')
		return sf::IpAddress("154.5.170.57");
	else if (addressInput[0] == 'l')
		return sf::IpAddress("192.168.1.66");
	else
		return sf::IpAddress(addressInput);
}

void NetworkManager::update()
{
	if (Game::getGameType() == Game::TYPE_SERVER && CONNECTION_TIMER_MAX < _connectionTimer++)
	{
		_connectionTimer = 0;

		sf::TcpSocket* client = new sf::TcpSocket;
		if (_listener.accept(*client) == sf::Socket::Done)
			registerClient(client);
		else
			delete client;

		checkDisconnectedClients();
	}
}

void NetworkManager::registerClient(sf::TcpSocket* client)
{
	std::cout << "Client connected: " << client->getRemoteAddress() << ":" << client->getRemotePort() << "\n";

	sf::Packet setupPacket;
	
	// receive setup information from client:
	//    - client's UDP port
	unsigned short port;
	if (client->receive(setupPacket) == sf::Socket::Done)
	{
		if (setupPacket >> port)
		{
			_clients.push_back(Peer(client, port, _currentClientId));
			std::cout << "Registered it with id " << _currentClientId << ", using UDP port " << port << "\n";
		}
		else
			std::cout << "Error extracting client's UDP port from setup packet\n";
	}
	else
		std::cout << "Error receiving setup packet from client\n";
	
	// send setup information to client:
	//    - game ID
	//	  - UDP port
	//    - sprite information of all current game objects
	setupPacket.clear();
	// game ID, UDP port
	setupPacket << _currentClientId << _udpSocket.getLocalPort();
	
	// sprite information of all current game objects
	for (auto itr = Game::_gameObjectManager.begin();
		 itr != Game::_gameObjectManager.end(); itr++)
	{
		setupPacket << itr->second->getId() << itr->second->getTextureName()
					<< itr->second->getPos().x << itr->second->getPos().y;
	}
	
	if (client->send(setupPacket) != sf::Socket::Done)
		std::cout << "Error sending setup packet to new client.\n";

	// create the new player
	Player* player = new Player(_currentClientId);
	std::string textureName;

	if (_currentClientId % 3 == 0)
		textureName = "81_";
	else if (_currentClientId % 3 == 2)
		textureName = "330_";
	else
		textureName = "376_";

	player->setTexture(textureName);

	Game::_gameObjectManager.add(player);

	// view
	send(_currentClientId, player->getId());
	
	_currentClientId++;
}

void NetworkManager::checkDisconnectedClients()
{
	std::vector<std::vector<Peer>::iterator> removeIndices;

	for (std::vector<Peer>::iterator itr = _clients.begin();
		 itr != _clients.end(); itr++)
	{
		if (itr->tcpSocket->send(sf::Packet()) == sf::Socket::Disconnected)
		{
			std::cout << "Client " << itr->id << " disconnected\n";
			removeIndices.push_back(itr);

			auto playerId = Game::_gameObjectManager.getPlayer(itr->id);
			Game::_gameObjectManager.remove(playerId->second);
		}
	}

	while (!removeIndices.empty())
	{
		_clients.erase(removeIndices.back());
		removeIndices.pop_back();
	}
}