#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <memory>

class Game;

class GameObject {
public:
	GameObject(std::shared_ptr<Game> game);

	virtual void update() = 0;
	virtual void draw() const = 0;

protected:
	const std::shared_ptr<Game> game;
};

#endif