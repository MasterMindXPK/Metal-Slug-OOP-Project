#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "Game.h"
#include "Camera.h"
#include"Soldier.h"

using namespace sf;
using namespace std;

int main() {

	srand(time(0));
	Game game;
	game.run();
	return 0;
}