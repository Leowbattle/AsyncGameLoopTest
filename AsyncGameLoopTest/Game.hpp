#ifndef GAME_HPP
#define GAME_HPP

#include <stdint.h>

#include <vector>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "Script.hpp"
#include "Event.hpp"

struct Colour {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct DrawRectCommand {
	SDL_Rect rect;
	Colour colour;
};

class Game {
public:
	Game();

	void run(Script&& script);

	Event<> nextFrame();
	Event<SDL_Point> mouseClicked();

	void drawRect(const DrawRectCommand& cmd);

	inline static const int Width = 1920;
	inline static const int Height = 1080;
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	Event<> nextFrameEvent;
	Event<SDL_Point> mouseClickedEvent;

	std::vector<DrawRectCommand> drawRectCommands;

	void draw();
};

#endif