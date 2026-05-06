#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "Camera.h"
#include"Player.h"
#include "Game.h"
#include "Level.h"

using namespace sf;
using namespace std;

int screen_x = 1600;
int screen_y = 900;

int main()
{
	Game game;
	game.run();

	return 0;
}




