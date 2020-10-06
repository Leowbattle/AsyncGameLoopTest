#include "Game.hpp"

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Async Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Game::run(Script<>&& script) {
	while (true) {
		// If update returns true the game is over.
		if (update(script)) {
			break;
		}

		draw();
	}
}

void Game::addGameObject(std::shared_ptr<GameObject> gameObject) {
	gameObjects.push_back(gameObject);
}

void Game::drawRect(const DrawRectCommand& cmd) {
	drawRectCommands.push_back(cmd);
}

bool Game::update(Script<>& script) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return true;

		case SDL_MOUSEBUTTONDOWN: {
			SDL_Point clickPoint = { event.button.x, event.button.y };
			mouseClickedEvent.setResult(clickPoint);
			break;
		}

		default:
			break;
		}
	}

	nextFrameEvent.setResult();

	for (auto gameObject : gameObjects) {
		gameObject->update();
	}

	// If the game script returns, the game is over.
	return script.await_ready();
}

void Game::draw() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	for (const auto gameObject : gameObjects) {
		gameObject->draw();
	}

	for (const DrawRectCommand& cmd : drawRectCommands) {
		SDL_SetRenderDrawColor(renderer, cmd.colour.r, cmd.colour.g, cmd.colour.b, 255);
		SDL_RenderFillRect(renderer, &cmd.rect);
	}
	drawRectCommands.clear();

	SDL_RenderPresent(renderer);
}

Event<> Game::nextFrame() {
	return nextFrameEvent;
}

Event<SDL_Point> Game::mouseClicked() {
	return mouseClickedEvent;
}