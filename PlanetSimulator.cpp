#include <SDL2/SDL.h>
#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>
#include <cmath>

const double PI = 3.14159265358979;
const double G = 0.1;
const double T = 0.5;
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
const int BODYNUMS = 800;
const int MAXLENGTH = 100;

SDL_Window *wnd;
SDL_Renderer *ren;

void DrawCircle(int x, int y, int r)
{
	int xc = 0, yc = r, i, d;
	d = 3 - 2 * r;
	while (xc <= yc)
	{
		for (i = xc; i <= yc; i++)
		{
			SDL_RenderDrawPoint(ren, x + xc, y + i);
			SDL_RenderDrawPoint(ren, x - xc, y + i);
			SDL_RenderDrawPoint(ren, x + xc, y - i);
			SDL_RenderDrawPoint(ren, x - xc, y - i);
			SDL_RenderDrawPoint(ren, x + i, y + xc);
			SDL_RenderDrawPoint(ren, x - i, y + xc);
			SDL_RenderDrawPoint(ren, x + i, y - xc);
			SDL_RenderDrawPoint(ren, x - i, y - xc);
		}
		if (d < 0) d += 4 * xc + 6;
		else
		{
			d += 4 * (xc - yc) + 10;
			yc--;
		}
		xc++;
	}
}

class Body
{
	public:
		Body()
		{
			x = rand() % SCREEN_WIDTH;
			y = rand() % SCREEN_HEIGHT;
			m = rand() % 3000 + 1000.;
			r = pow((m / PI) * (3. / 4.), 1. / 3.);
			vx = (rand() & 1 ? 1 : -1) * (rand() % 401 / 200.);
			vy = (rand() & 1 ? 1 : -1) * (rand() % 401 / 200.);
			red = rand() % 128 + 128;
			green = rand() % 128 + 128;
			blue = rand() % 128 + 128;
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
			SDL_SetRenderDrawColor(ren, red, green, blue, 255);
			DrawCircle(x, y, r);
			for (std::list<int>::iterator itx1 = listx.begin(), std::list<int>::iterator itx2 = listx.begin() + 1, std::list<int>::iterator ity1 = listy.begin(), std::list<int>::iterator ity2 = listy.begin() + 1; itx2 != listx.end(), ity2 != listy.end(); itx1++, itx2++, ity1++, ity2++)
			{
				SDL_RenderDrawLine(ren, *itx1, *ity1, *itx2, *ity2);
			}
		}
		static void gravitation(Body &a, Body &b)
		{
			double ax, ay;
			if (a.m > 0 && b.m > 0)
			{
				if ((a.r + b.r) * (a.r + b.r) < (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y))
				{
					ax = G * b.m / ((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)) * cos(atan2(b.y - a.y, b.x - a.x));
					ay = G * b.m / ((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)) * sin(atan2(b.y - a.y, b.x - a.x));
					if (b.x > a.x) a.vx += ax * T;
					else if (b.x < a.x) a.vx -= ax * T;
					if (b.y > a.y) a.vy += ay * T;
					else if (b.y < a.y) a.vy -= ay * T;
				}
				else
				{
					if (a.m >= b.m)
					{
						a.vx = (a.m * a.vx + b.m * b.vx) / (a.m + b.m);
						a.vy = (a.m * a.vy + b.m * b.vy) / (a.m + b.m);
						a.m += b.m;
						a.r = pow((a.m / PI) * (3. / 4.), 1. / 3.);
						b.m = b.r = b.vx = b.vy = 0;
					}
					else
					{
						b.vx = (a.m * a.vx + b.m * b.vx) / (a.m + b.m);
						b.vy = (a.m * a.vy + b.m * b.vy) / (a.m + b.m);
						b.m += a.m;
						b.r = pow((b.m / PI) * (3. / 4.), 1. / 3.);
						a.m = a.r = a.vx = a.vy = 0;
					}
				}
			}
		}
	private:
		double x;
		double y;
		double m;
		double r;
		double vx;
		double vy;
		unsigned int red;
		unsigned int green;
		unsigned int blue;
		std::list<int> listx;
		std::list<int> listy;
}

int main(int argc, char *argv[])
{
	wnd = NULL;
	ren = NULL;
	SDL_Event e;
	bool run = true;
	int i, j;
	srand(time(NULL));
	Body bodys[BODYNUMS];
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		std::cerr << "Error: " << SDL_GetError();
		return -1;
	}
	wnd = SDL_CreateWindow("Planet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!wnd)
	{
		std::cerr << "Error: " << SDL_GetError();
		SDL_Quit();
		return -1;
	}
	ren = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!ren)
	{
		std::cerr << "Error: " << SDL_GetError();
		SDL_DestroyWindow(wnd);
		SDL_Quit();
		return -1;
	}
	while (run)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) run = false;
		}
		for (i = 0; i < BODYNUMS; i++)
		{
			for (j = 0; j < BODYNUMS; j++)
			{
				if (i != j) Body::gravitation(bodys[i], bodys[j]);
			}
			bodys[i].move();
			bodys[i].show();
		}
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(wnd);
	SDL_Quit();
	return 0;
}
