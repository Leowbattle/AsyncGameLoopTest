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
		if (target.has_value()) {
			vec2 t = target.value();

			game->drawRect(DrawRectCommand{
			.rect = {(int)t.x, (int)t.y, 15, 15},
			.colour = {0, 255, 0}
				});
		}

		game->drawRect(DrawRectCommand{
			.rect = {(int)pos.x, (int)pos.y, 15, 15},
			.colour = {255, 0, 0}
			});
	}

	Script<> mainAI() {
		CancellationToken cancellationToken;
		Script<> mover;

		while (true) {
			SDL_Point clickPos = co_await game->mouseClicked();
			cancellationToken.cancel();
			cancellationToken = CancellationToken();

			mover = moveTo({ .x = (float)clickPos.x, .y = (float)clickPos.y }, cancellationToken);
		}
	}

	Script<> moveTo(vec2 point, CancellationToken cancelled) {
		target = point;

		vec2 diff = point - pos;
		float distance = diff.length();

		vec2 dir = diff / distance;

		int steps = distance / speed;
		for (int i = 0; i < steps; i++) {
			if (cancelled) {
				co_return;
			}

			pos += dir * speed;

			co_await game->nextFrame();
		}
		pos = point;

		target.reset();
	}

	float speed = 15;
	vec2 pos = { .x = Game::Width / 2, .y = Game::Height / 2 };
	
	std::optional<vec2> target;
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