#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <chrono>
#include <memory>

#include "vec2.hpp"
#include "Game.hpp"

class MyGameObject : public GameObject {
public:
	MyGameObject(std::shared_ptr<Game> game) : GameObject(game) {
		mainAI();
	}

	virtual void update() override {
		
	}

	virtual void draw() const override {
		game->drawRect(DrawRectCommand{
			.rect = {(int)pos.x, (int)pos.y, 15, 15},
			.colour = {255, 0, 0}
			});
	}

	Script<> mainAI() {
		while (true) {
			SDL_Point clickPos = co_await game->mouseClicked();
			co_await moveTo({ .x = (float)clickPos.x, .y = (float)clickPos.y });
		}
	}

	Script<> moveTo(vec2 point) {
		vec2 diff = point - pos;
		float distance = diff.length();

		vec2 dir = diff / distance;

		int steps = distance / speed;
		for (int i = 0; i < steps; i++) {
			pos += dir * speed;

			co_await game->nextFrame();
		}
		pos = point;
	}

	float speed = 4;
	vec2 pos = { .x = Game::Width / 2, .y = Game::Height / 2 };
};

Script<> gameScript(std::shared_ptr<Game> game) {
	game->addGameObject(std::make_shared<MyGameObject>(game));

	while (true) {
		co_await game->nextFrame();
	}
}

int main() {
	std::shared_ptr<Game> game = std::make_shared<Game>();
	game->run(gameScript(game));

	return 0;
}