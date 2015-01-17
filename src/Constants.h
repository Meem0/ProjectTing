#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML\Graphics.hpp>

namespace Constants
{
	const static float DEFAULT_OBJECT_SPEED = 8.0f;
	const static float MAGIC_BOLT_SPEED = 12.0f;
	const static float ENEMY_SPEED = 7.5f;

	const static sf::Uint32 MAGIC_BOLT_DAMAGE = 5;

	const static sf::Uint32 PLAYER_HP = 50;
	const static sf::Uint32 ENEMY_HP = 7;

	// distance from a player for an object to be considered off-screen
	const static sf::Uint32 OFFSCREEN_DISTANCE_SQRD = 5290000; // 2300^2
	// time spent off-screen for a magic bolt to despawn
	const static sf::Uint32 MAGIC_BOLT_OFFSCREEN_TIME = 15;

	static const unsigned int SERVER_UDP_PORT = 53001;
	static const unsigned int SERVER_TCP_PORT = 53000;

	static const unsigned int SPAWN_CHANCE = 10; // (1 / _spawnChance) chance to spawn each frame
	static const int SPAWN_DISTANCE_MIN = 1000;
	static const int SPAWN_DISTANCE_MAX = 2000; // actual max distance = SPAWN_DISTANCE_MAX + SPAWN_DISTANCE_MIN
	static const unsigned int SPAWN_MOB_CAP = 5;
	static const int DESPAWN_DISTANCE = 5000;
}

#endif