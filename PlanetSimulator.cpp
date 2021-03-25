#include <SDL2/SDL.h>
#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>
#include <cmath>

const double SK = 0.23873241463784;
const double G = 0.1;
const double T = 0.5;
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;
const int BODYNUMS = 800;
const int MAXLENGTH = 100;

SDL_Window *wnd = NULL;
SDL_Renderer *ren = NULL;

double cbrt(double x)
{
	double x0 = 1, x1 = x0 - (x0 * x0 * x0 - x) / (3 * x0 * x0);
	while (x0 - x1 > 0.00000000000001 || x0 - x1 < -0.00000000000001)
	{
		x0 = x1 - (x1 * x1 * x1 - x) / (3 * x1 * x1);
		x1 = x0 - (x0 * x0 * x0 - x) / (3 * x0 * x0);
	}
	return x1;
}

void DrawCircle(int x, int y, int r)
{
	int xc = 0, yc = r, i, d;
	d = 3 - (r << 1);
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
		if (d < 0) d += (xc << 2) + 6;
		else
		{
			d += ((xc - yc) << 2) + 10;
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
			m = rand() % 500 + 10.;
			r = cbrt(m * SK);
			vx = (rand() & 1 ? 1 : -1) * (rand() % 401 / 200.);
			vy = (rand() & 1 ? 1 : -1) * (rand() % 401 / 200.);
			red = rand() % 256;
			green = rand() % 256;
			blue = rand() % 256;
			for (int i = 0; i < MAXLENGTH; i++)
			{
				listx.push_back(x);
				listy.push_back(y);
			}
		}
		void move()
		{
			if (m > 0)
			{
				x += vx * T;
				y += vy * T;
				listx.push_back(x);
				listy.push_back(y);
				listx.pop_front();
				listy.pop_front();
			}
		}
		void show()
		{
			if (m > 0)
			{
				SDL_SetRenderDrawColor(ren, red, green, blue, 255);
				DrawCircle(x, y, r);
				std::list<int>::iterator itx1 = listx.begin(), itx2 = itx1, ity1 = listy.begin(), ity2 = ity1;
				++itx2;
				++ity2;
				while (itx2 != listx.end())
				{
					SDL_RenderDrawLine(ren, *itx1, *ity1, *itx2, *ity2);
					++itx1;
					++itx2;
					++ity1;
					++ity2;
				}
			}
		}
		static void gravitation(Body &a, Body &b)
		{
			double tmp1, tmp2;
			if (a.m > 0 && b.m > 0)
			{
				if ((a.r + b.r) * (a.r + b.r) < (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y))
				{
					tmp1 = G * b.m / ((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
					tmp2 = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
					a.vx += tmp1 * ((b.x- a.x) / tmp2);
					a.vy += tmp1 * ((b.y - a.y) / tmp2);
				}
				else
				{
					if (a.m >= b.m)
					{
						a.vx = (a.m * a.vx + b.m * b.vx) / (a.m + b.m);
						a.vy = (a.m * a.vy + b.m * b.vy) / (a.m + b.m);
						a.m += b.m;
						a.r = cbrt(a.m * SK);
						b.m = 0;
						b.r = 0;
						b.vx = 0;
						b.vy = 0;
					}
					else
					{
						b.vx = (a.m * a.vx + b.m * b.vx) / (a.m + b.m);
						b.vy = (a.m * a.vy + b.m * b.vy) / (a.m + b.m);
						b.m += a.m;
						b.r = cbrt(b.m * SK);
						a.m = 0;
						a.r = 0;
						a.vx = 0;
						a.vy = 0;
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
};

int main(int argc, char *argv[])
{
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
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);
		for (i = 0; i < BODYNUMS; i++)
		{
			bodys[i].move();
			for (j = 0; j < BODYNUMS; j++)
			{
				if (i != j) Body::gravitation(bodys[i], bodys[j]);
			}
			bodys[i].show();
		}
		SDL_RenderPresent(ren);
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(wnd);
	SDL_Quit();
	return 0;
}
