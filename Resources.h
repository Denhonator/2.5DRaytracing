#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

struct Light {
	sf::Vector2f pos = sf::Vector2f(0, 0);
	float strength = 1.0f;
};

class Resources
{
public:
	sf::Texture* GetTexture(std::string name);

private:
	std::unordered_map<std::string, sf::Texture> textures;
};

