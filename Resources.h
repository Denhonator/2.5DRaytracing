#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

class Resources
{
public:
	sf::Texture* GetTexture(std::string name);

private:
	std::unordered_map<std::string, sf::Texture> textures;
};

