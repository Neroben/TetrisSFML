#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include "windows.h"

using namespace sf;
using namespace std;

class Background {
	Sprite Back;
	Texture* p;

public:
	Background(string image) {
		p = new Texture();
		p->loadFromFile(image);
		Back.setTexture(*p);
		Back.setPosition(0, 0);
	}

	void draw(RenderWindow *window) {
		window->draw(Back);
	}

	~Background() {
		delete p;
	}

};

class MicroFigure {
	Image img;
	Texture tex;

public:

	MicroFigure(string image) {
		img.loadFromFile(image);
		tex.loadFromImage(img);
	}

	Sprite* setGreen() {
		Sprite *green = new Sprite();
		green->setTexture(tex);
		green->setTextureRect(IntRect(6, 6, 46, 46));
		return green;
	}

	Sprite* setPurple() {
		Sprite *purple = new Sprite();
		purple->setTexture(tex);
		purple->setTextureRect(IntRect(61, 6, 46, 46));
		return purple;
	}

	Sprite* setOrange() {
		Sprite *orange = new Sprite();
		orange->setTexture(tex);
		orange->setTextureRect(IntRect(116, 6, 46, 46));
		return orange;
	}

	Sprite* setRed() {
		Sprite *red = new Sprite();
		red->setTexture(tex);
		red->setTextureRect(IntRect(171, 6, 46, 46));
		return red;
	}

	Sprite* setBlue() {
		Sprite *blue = new Sprite();
		blue->setTexture(tex);
		blue->setTextureRect(IntRect(226, 6, 46, 46));
		return blue;
	}

	Sprite* setYelow() {
		Sprite* yelow = new Sprite();
		yelow->setTexture(tex);
		yelow->setTextureRect(IntRect(281, 6, 46, 46));
		return yelow;
	}

	Sprite* setNeutral() {
		Sprite *neutral = new Sprite();
		neutral->setTexture(tex);
		neutral->setTextureRect(IntRect(336, 6, 46, 46));
		return neutral;
	}
};

class FieldTetris {
	char** arr;
	const int length;
	const int width;
	Sprite*** SpriteArr;

public:
	FieldTetris(int length, int width) : length(length), width(width) {
		arr = new char* [width + 2];
		for (int i = 0; i < width + 2; i++) {
			arr[i] = new char[length + 3];
			for (int j = 0; j < length + 3; j++)
				if (i == 0 || (i == width + 1) || (j == length + 2))
					arr[i][j] = 'x';
				else
					arr[i][j] = 'n';
		}
	}

	~FieldTetris() {
		for (int i = 0; i < width + 2; i++)
			delete arr[i];
		delete[] arr;
	}

	int getLength() {
		return length;
	}

	int getWidth() {
		return width;
	}

	void draw(RenderWindow* window, MicroFigure* micro, float pointx, float pointy) {
		float x = pointx;
		float y = pointy;
		SpriteArr = new Sprite * *[width + 2];
		for (int i = 0; i < width + 2; i++) {
			y = pointy;
			SpriteArr[i] = new Sprite * [length + 3];
			for (int j = 0; j < length + 3; j++) {
				switch (arr[i][j]) {
				case('n'):
					SpriteArr[i][j] = micro->setNeutral();
					break;
				case('g'):
					SpriteArr[i][j] = micro->setGreen();
					break;
				case('p'):
					SpriteArr[i][j] = micro->setPurple();
					break;
				case('o'):
					SpriteArr[i][j] = micro->setOrange();
					break;
				case('r'):
					SpriteArr[i][j] = micro->setRed();
					break;
				case('b'):
					SpriteArr[i][j] = micro->setBlue();
					break;
				case('y'):
					SpriteArr[i][j] = micro->setYelow();
					break;
				default:
					SpriteArr[i][j] = new Sprite();
				}
				if (j > 1) {
					SpriteArr[i][j]->setPosition(x, y);
					window->draw(*SpriteArr[i][j]);
					y += 46;
				}
			}
			x += 46;
		}
	}

	void setColor(int i, int j, char color) {
		arr[i][j] = color;
	}

	char getColor(int i, int j) {
		return arr[i][j];
	}

	void deleteSprite() {
		for (int i = 0; i < getWidth() + 2; i++) {
			for (int j = 0; j < getLength() + 3; j++) {
				delete SpriteArr[i][j];
			}
			delete SpriteArr[i];
		}
		delete SpriteArr;
	}

	void clear() {
		for (int i = 1; i < getWidth() + 1; i++)
			for (int j = 0; j < getLength(); j++)
				arr[i][j] = 'n';
	}

	bool isFullLine(int j) {
		for (int i = 0; i < width + 2; i++)
			if (arr[i][j] == 'n')
				return false;
		return true;
	}

	void deleteFullLine(int a) {
		for (int j = a; j > 0; j--)
			for (int i = 0; i < width + 2; i++)
				arr[i][j] = arr[i][j - 1];
	}

	void clearAllFullLine(){
		for (int i = 0; i < width + 2; i++)
			for (int j = 0; j < length + 3; j++ )
				if (isFullLine(j)) {
					deleteFullLine(j);
				}
	}

};

class FigureTetris {
protected:
	FieldTetris* field;
	int x;
	int y;
	int rotat;
	char color;
public:
	FigureTetris(FieldTetris* field, int x, int y, char color):field(field), x(x + 1), y(y + 2), color(color) {}

	void setX(int a) {
		x = a;
	}

	void setY(int a) {
		y = a + 1;
	}

	int getX() {
		return x;
	}

	int getY() {
		return y - 1;
	}

	virtual bool draw() = 0;
	virtual void rotation() = 0;
	virtual void clear() = 0;
	
	bool down() {
		if (isTrue(x, y + 1)) {
			y += 1;
			return true;
		}
		return false;
	}

	bool left() {
		if (isTrue(x - 1, y)) {
			x -= 1;
			return true;
		}
		return false;
	}

	bool right() {
		if (isTrue(x + 1, y)) {
			x += 1;
			return true;
		}
		return false;
	}

	virtual bool isTrue(int x, int y) = 0;


};

class FigureLine: public FigureTetris {
public:
	FigureLine(FieldTetris *field, int x, int y, char color): FigureTetris(field, x, y, color) {
		rotat = 0;
	}

	bool draw() {
		if (!isTrue(x, y))
			return false;
		if (rotat == 0) {
			field->setColor(x - 1, y, color);
			field->setColor(x, y, color);
			field->setColor(x + 1, y, color);
			field->setColor(x + 2, y, color);
			return true;
		}
		else {
			field->setColor(x, y - 1, color);
			field->setColor(x, y, color);
			field->setColor(x, y + 1, color);
			field->setColor(x, y + 2, color);
			return true;
		}
	}

	void rotation() {
		rotat = !rotat;
	}

	void clear() {
		if (rotat == 0) {
			field->setColor(x - 1, y, 'n');
			field->setColor(x, y, 'n');
			field->setColor(x + 1, y, 'n');
			field->setColor(x + 2, y, 'n');
		}
		else {
			field->setColor(x, y - 1, 'n');
			field->setColor(x, y, 'n');
			field->setColor(x, y + 1, 'n');
			field->setColor(x, y + 2, 'n');
		}
	}

	bool isTrue(int x, int y) {
		if (rotat == 0)
			if ((field->getColor(x - 1, y) == 'n') && (field->getColor(x, y) == 'n') && (field->getColor(x + 1, y) == 'n') && (field->getColor(x + 2, y) == 'n'))
				return true;
			else
				return false;
		else {
			if ((field->getColor(x, y - 1) == 'n') && (field->getColor(x, y) == 'n') && (field->getColor(x, y + 1) == 'n') && (field->getColor(x, y + 2) == 'n'))
				return true;
			else
				return false;
		}
	}

};

class FigureL:public FigureTetris {
public:
	FigureL(FieldTetris* field, int x, int y, char color) :FigureTetris(field, x, y, color) {
		rotat = 0;
	}

	bool draw() {
		if (!isTrue(x, y))
			return false;
		switch (rotat) {
		case(0):
			field->setColor(x, y, color);
			field->setColor(x + 1, y, color);
			field->setColor(x, y - 1, color);
			field->setColor(x, y - 2, color);
			break;
		case(1):
			field->setColor(x, y, color);
			field->setColor(x, y + 1, color);
			field->setColor(x + 1, y, color);
			field->setColor(x + 2, y, color);
			break;
		case(2):
			field->setColor(x, y, color);
			field->setColor(x - 1, y, color);
			field->setColor(x, y + 1, color);
			field->setColor(x, y + 2, color);
			break;
		case(3):
			field->setColor(x, y, color);
			field->setColor(x, y - 1, color);
			field->setColor(x - 1, y, color);
			field->setColor(x - 2, y, color);
			break;
		}
		return true;
	}

	void rotation() {
		rotat += 1;
		if (rotat > 3)
			rotat = 0;
	}

	void clear() {
		switch (rotat) {
		case(0):
			field->setColor(x, y, 'n');
			field->setColor(x + 1, y, 'n');
			field->setColor(x, y - 1, 'n');
			field->setColor(x, y - 2, 'n');
			break;
		case(1):
			field->setColor(x, y, 'n');
			field->setColor(x, y + 1, 'n');
			field->setColor(x + 1, y, 'n');
			field->setColor(x + 2, y, 'n');
			break;
		case(2):
			field->setColor(x, y, 'n');
			field->setColor(x - 1, y, 'n');
			field->setColor(x, y + 1, 'n');
			field->setColor(x, y + 2, 'n');
			break;
		case(3):
			field->setColor(x, y, 'n');
			field->setColor(x, y - 1, 'n');
			field->setColor(x - 1, y, 'n');
			field->setColor(x - 2, y, 'n');
			break;
		}
	}

	bool isTrue(int x, int y) {
		switch (rotat) {
		case(0):
			if ((field->getColor(x, y) == 'n') && (field->getColor(x + 1, y) == 'n') && (field->getColor(x, y - 1) == 'n') && (field->getColor(x, y - 2) == 'n'))
				return true;
			else
				return false;
			break;
		case(1):
			if ((field->getColor(x, y) == 'n') && (field->getColor(x, y + 1) == 'n') && (field->getColor(x + 1, y) == 'n') && (field->getColor(x + 2, y) == 'n'))
				return true;
			else
				return false;
			break;
		case(2):
			if ((field->getColor(x, y) == 'n') && (field->getColor(x - 1, y) == 'n') && (field->getColor(x, y + 1) == 'n') && (field->getColor(x, y + 2) == 'n'))
				return true;
			else
				return false;
			break;
		case(3):
			if ((field->getColor(x, y) == 'n') && (field->getColor(x, y - 1) == 'n') && (field->getColor(x - 1, y) == 'n') && (field->getColor(x - 2, y) == 'n'))
				return true;
			else
				return false;
			break;
		}
	}

};

class FigureTriangle:public FigureTetris {
public:
	FigureTriangle(FieldTetris* field, int x, int y, char color) : FigureTetris(field, x, y, color) {
		rotat = 0;
	}

	bool draw() {
		if (!isTrue(x, y))
			return false;
		switch (rotat) {
		case(0):
			field->setColor(x, y, color);
			field->setColor(x - 1, y, color);
			field->setColor(x + 1, y, color);
			field->setColor(x, y - 1, color);
			break;
		case(1):
			field->setColor(x, y, color);
			field->setColor(x + 1, y, color);
			field->setColor(x, y - 1, color);
			field->setColor(x, y + 1, color);
			break;
		case(2):
			field->setColor(x, y, color);
			field->setColor(x - 1, y, color);
			field->setColor(x + 1, y, color);
			field->setColor(x, y + 1, color);
			break;
		case(3):
			field->setColor(x, y, color);
			field->setColor(x, y - 1, color);
			field->setColor(x, y + 1, color);
			field->setColor(x - 1, y, color);
			break;
		}
		return true;
	}

	void rotation() {
		rotat += 1;
		if (rotat > 3)
			rotat = 0;
	}

	void clear() {
		switch (rotat) {
		case(0):
			field->setColor(x, y, 'n');
			field->setColor(x - 1, y, 'n');
			field->setColor(x + 1, y, 'n');
			field->setColor(x, y - 1, 'n');
			break;
		case(1):
			field->setColor(x, y, 'n');
			field->setColor(x + 1, y, 'n');
			field->setColor(x, y - 1, 'n');
			field->setColor(x, y + 1, 'n');
			break;
		case(2):
			field->setColor(x, y, 'n');
			field->setColor(x - 1, y, 'n');
			field->setColor(x + 1, y, 'n');
			field->setColor(x, y + 1, 'n');
			break;
		case(3):
			field->setColor(x, y, 'n');
			field->setColor(x, y - 1, 'n');
			field->setColor(x, y + 1, 'n');
			field->setColor(x - 1, y, 'n');
			break;
		}
	}

	bool isTrue(int x, int y) {
		switch (rotat) {
		case(0):
			if ((field->getColor(x, y) == 'n') && (field->getColor(x - 1, y) == 'n') && (field->getColor(x + 1, y) == 'n') && (field->getColor(x, y - 1) == 'n'))
				return true;
			else
				return false;
			break;
		case(1):
			if ((field->getColor(x, y) == 'n') && (field->getColor(x + 1, y) == 'n') && (field->getColor(x, y - 1) == 'n') && (field->getColor(x, y + 1) == 'n'))
				return true;
			else
				return false;
			break;
		case(2):
			if ((field->getColor(x, y) == 'n') && (field->getColor(x - 1, y) == 'n') && (field->getColor(x + 1, y) == 'n') && (field->getColor(x, y + 1) == 'n'))
				return true;
			else
				return false;
			break;
		case(3):
			if ((field->getColor(x, y) == 'n') && (field->getColor(x, y - 1) == 'n') && (field->getColor(x, y + 1) == 'n') && (field->getColor(x - 1, y) == 'n'))
				return true;
			else
				return false;
			break;
		}
	}
};

class FigureZ :public FigureTetris {
public:
	FigureZ(FieldTetris* field, int x, int y, char color) : FigureTetris(field, x, y, color) {
		rotat = 0;
	}

	bool draw() {
		if (!isTrue(x, y))
			return false;
		switch (rotat) {
		case(0):
			field->setColor(x, y, color);
			field->setColor(x - 1, y, color);
			field->setColor(x, y + 1, color);
			field->setColor(x + 1, y + 1, color);
			break;
		case(1):
			field->setColor(x, y, color);
			field->setColor(x, y - 1, color);
			field->setColor(x - 1, y, color);
			field->setColor(x - 1, y + 1, color);
			break;
		}
		return true;
	}

	void rotation() {
		rotat = !rotat;
	}

	void clear() {
		switch (rotat) {
		case(0):
			field->setColor(x, y, 'n');
			field->setColor(x - 1, y, 'n');
			field->setColor(x, y + 1, 'n');
			field->setColor(x + 1, y + 1, 'n');
			break;
		case(1):
			field->setColor(x, y, 'n');
			field->setColor(x, y - 1, 'n');
			field->setColor(x - 1, y, 'n');
			field->setColor(x - 1, y + 1, 'n');
			break;
		}
	}

	bool isTrue(int x, int y) {
		switch (rotat) {
		case(0):
			if ((field->getColor(x, y) == 'n') && (field->getColor(x - 1, y) == 'n') && (field->getColor(x, y + 1) == 'n') && (field->getColor(x + 1, y + 1) == 'n'))
				return true;
			else
				return false;
			break;
		case(1):
			if ((field->getColor(x, y) == 'n') && (field->getColor(x, y - 1) == 'n') && (field->getColor(x - 1, y) == 'n') && (field->getColor(x - 1, y + 1) == 'n'))
				return true;
			else
				return false;
			break;
		}
	}
};

class FigureSquare :public FigureTetris {
public:
	FigureSquare(FieldTetris* field, int x, int y, char color) :FigureTetris(field, x, y, color) {}

	bool draw() {
		if (!isTrue(x, y))
			return false;
		field->setColor(x, y, color);
		field->setColor(x + 1, y, color);
		field->setColor(x, y + 1, color);
		field->setColor(x + 1, y + 1, color);
		return true;
	}

	void rotation() {}

	void clear() {
		field->setColor(x, y, 'n');
		field->setColor(x + 1, y, 'n');
		field->setColor(x, y + 1, 'n');
		field->setColor(x + 1, y + 1, 'n');
	}

	bool isTrue(int x, int y) {
		if ((field->getColor(x, y) == 'n') && (field->getColor(x + 1, y) == 'n') && (field->getColor(x, y + 1) == 'n') && (field->getColor(x + 1, y + 1) == 'n'))
			return true;
		else
			return false;
	}
};

void Fdown(FigureTetris* figure, mutex* a, bool* flag, int speed) {//1000
	while (*flag) {
		a->lock();
		figure->clear();
		*flag = figure->down();
		if (!(*flag))
			figure->draw();
		a->unlock();
		Sleep(speed);
	}
}

void Fcontrol(FigureTetris* figure, mutex* a, bool *flag, int speed) {//100
	while (*flag) {
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			a->lock();
			figure->clear();
			figure->left();
			a->unlock();
			Sleep(speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			a->lock();
			figure->clear();
			figure->right();
			a->unlock();
			Sleep(speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			a->lock();
			figure->clear();
			figure->rotation();
			a->unlock();
			Sleep(speed);
		}
	}
}

void none() {}

class Motion {
	FieldTetris* field;
	FigureTetris* figure;
	mutex* a;
	int speed;
	thread* thr1;
	thread* thr2;

public:
	Motion(FieldTetris* field, FigureTetris* figure, mutex *a, int speed, bool *flag) : field(field), figure(figure), a(a), speed(speed) {
		figure->setX(field->getWidth() / 2);
		figure->setY(0);
		
		void (*down)(FigureTetris*, mutex*, bool*, int);
		down = Fdown;
		void (*control)(FigureTetris*, mutex*, bool*, int);
		control = Fcontrol;
		thr1 = new thread(down, figure, a, flag, speed);
		thr2 = new thread(control, figure, a, flag, 100);
	}

	Motion() {
		void (*non)();
		non = none;
		thr1 = new thread(non);
		thr2 = new thread(non);
	}

	~Motion() {
		thr1->join();
		thr2->join();
		delete thr1;
		delete thr2;
	}

};

class GameTetris {
	Background *back;
	FieldTetris *field;
	FigureTetris* figure;
	RenderWindow *window;
	MicroFigure* micro;
	Motion* motion;
	mutex* mutx;
public:
	GameTetris(RenderWindow *w, MicroFigure *m, Background *b, FieldTetris *f):window(w), micro(m), back(b), field(f) {
		mutx = new mutex();
		figure = NULL;
		bool flag = false;
		motion = new Motion();
		int i = 0;
		while (w->isOpen()) {
			sf::Event event;
			while (w->pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					flag = false;
					w->close();
				}
			}
			
			if (!flag) {
				delete motion;
				field->clearAllFullLine();
				randomFigure();
				flag = true;
				motion = new Motion(field, figure, mutx, 1000, &flag);
			}

			w->clear();
			b->draw(w);
			mutx->lock();
			field->draw(w, m, 240, 0);
			figure->draw();
			w->display();
			mutx->unlock();
			field->deleteSprite();
		}
	}

	void randomFigure() {
		int r = rand();
		switch (r%5) {
		case(0):
			figure = new FigureLine(field, 0,0, 'r');
			break;
		case(1):
			figure = new FigureL(field, 0, 0, 'p');
			break;
		case(2):
			figure = new FigureTriangle(field, 0, 0, 'b');
			break;
		case(3):
			figure = new FigureZ(field, 0, 0, 'y');
			break;
		case(4):
			figure = new FigureSquare(field, 0, 0, 'g');
			break;
		}
	}

	~GameTetris() {
		delete mutx;
		delete motion;
	}
};
