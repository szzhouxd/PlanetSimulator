#include <SDL2/SDL.h>
#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>

const double PI = 3.14159265358979;
const double G = 0.1;
const double T = 0.5;
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
const int BODYNUMS = 800;
const int MAXLENGTH = 100;

SDL_Window *wnd = NULL;
SDL_Renderer *ren = NULL;

class Planet
{
	public:
		Planet()
		{
			x = rand() % SCREEN_WIDTH;
			y = rand() % SCREEN_HEIGHT;
			m = rand() % 3000 + 1000.0;
			r = CalR();
			vx = (rand() & 1 ? 1 : -1) * ((rand() % 401) / 200.0);
			vy = (rand() & 1 ? 1 : -1) * ((rand() % 401) / 200.0);
			color = {rand() % 256, rand() % 256, rand() % 256};
			for (int i = 0; i < MAXLENGTH; i++)
			{
				listx.push_back(x);
				listy.push_back(y);
			}
		}
		void move()
		{
			x += vx * T;
			y += vy * T;
			listx.push_back(x);
			listy.push_back(y);
			listx.pop_front();
			listy.pop_front();
		}
		void show()
		{
			for (std::list<int>::iterator itx = listx.begin(), std::list<int>::iterator ity = listy.begin(); itx != listx.end(), ity != listy.end(); itx++, ity++)
			{
				
			}
		}
}

int main()
{
	
}
