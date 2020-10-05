#include <stdio.h>

#include <chrono>
#include <memory>

#include "Game.hpp"

Script gameScript(std::shared_ptr<Game> game) {
	int i = 0;
	
	auto last = std::chrono::steady_clock::now();

	while (true) {
		for (int i = 0; i < 60; i++) {
			co_await game->nextFrame();
		}

		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<float> diff = now - last;

		i++;
		printf("%d seconds have passed, %f\n", i, diff.count());

		last = now;
	}
}

int main() {
	std::shared_ptr<Game> game = std::make_shared<Game>();
	game->run(gameScript(game));

	return 0;
}