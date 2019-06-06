#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace sf;

int main()
{
	RenderWindow *window = new RenderWindow(VideoMode(960, 540), "Tetris");
	MicroFigure *micro = new MicroFigure("Images/square.png");
	Background *back = new Background("Images/fon.jpg");
	FieldTetris *a = new FieldTetris(10, 10);
	GameTetris(window, micro, back, a);
	

	delete window;
	delete micro;
	delete back;
	delete a;

	return 0;
}