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
			.rect = {(int)orbitCentre.x, (int)orbitCentre.y, 15, 15},
			.colour = {0, 255, 0}
			});

		game->drawRect(DrawRectCommand{
			.rect = {(int)pos.x, (int)pos.y, 15, 15},
			.colour = {255, 0, 0}
			});
	}

	Script<> mainAI() {
		float startAngle = M_PI/2;
		float orbitRadius = 150;
		orbitCentre = { .x = Game::Width / 2, .y = Game::Height / 2 };

		while (true) {
			CancellationToken cancellationToken;
			Script<> orbiterScript = orbiterAI(orbitCentre, startAngle, orbitRadius, cancellationToken);
			
			SDL_Point clickPos = co_await game->mouseClicked();
			cancellationToken.cancel();

			orbitCentre = { .x = (float)clickPos.x, .y = (float)clickPos.y };

			vec2 target = orbitCentre + (pos - orbitCentre).normalised() * orbitRadius;
			startAngle = atan2f(target.y - orbitCentre.y, target.x - orbitCentre.x);

			co_await moveTo(target);
		}
	}

	Script<> orbiterAI(vec2 orbitCentre, float startAngle, float orbitRadius, CancellationToken canelled) {
		float orbitSpeed = speed / orbitRadius;

		float angle = startAngle;

		while (true) {
			if (canelled) {
				co_return;
			}

			pos.x = orbitRadius * cos(angle) + orbitCentre.x;
			pos.y = orbitRadius * sin(angle) + orbitCentre.y;

			angle += orbitSpeed;

			co_await game->nextFrame();
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

	float speed = 15;
	vec2 pos = { .x = Game::Width / 2, .y = Game::Height / 2 };
	vec2 orbitCentre;
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