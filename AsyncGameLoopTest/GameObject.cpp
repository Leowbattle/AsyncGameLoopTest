#include "GameObject.hpp"
#include "Game.hpp"

GameObject::GameObject(std::shared_ptr<Game> game) : game(game) {}