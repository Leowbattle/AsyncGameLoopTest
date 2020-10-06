#include <stdio.h>
#include <math.h>

#include <chrono>
#include <memory>

#include "Game.hpp"

Script<int> what(std::shared_ptr<Game> game) {
	co_await game->nextFrame();
	printf("wow\n");
	co_return 42;
}

Script<> gameScript(std::shared_ptr<Game> game) {
	int whatever = co_await what(game);

	float x = 50;
	float y = 50;

	while (true) {
		SDL_Point clickPos = co_await game->mouseClicked();

		float dx = clickPos.x - x;
		float dy = clickPos.y - y;
		float distance = hypotf(dx, dy);

		dx /= distance;
		dy /= distance;

		float speed = 15;
		int steps = distance / speed;

		for (int i = 0; i < steps; i++) {
			x += dx * speed;
			y += dy * speed;

			game->drawRect(DrawRectCommand{
				.rect = {(int)x, (int)y, 5, 5},
				.colour = {255, 0, 0}
				});

			co_await game->nextFrame();
		}
	}
}

int main() {
	std::shared_ptr<Game> game = std::make_shared<Game>();
	game->run(gameScript(game));

	return 0;
}