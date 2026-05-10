#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

#include"Utility.h"

#include "Survival.h"

#include"Projectile.h"

#include"Weapon.h"



#include"Soldier.h"
#include"Enemy.h"

#include"PlayerSoldier.h"

#include"Spawn.h"

#include"Game.h"

using namespace sf;
using namespace std;

int main() {

	srand(time(0));
	Game game;
	game.run();
	return 0;
}