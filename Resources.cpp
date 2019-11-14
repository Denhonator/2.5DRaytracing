#include "Resources.h"

sf::Texture* Resources::GetTexture(std::string name)
{
	if(textures.find(name)==textures.end()){
		sf::Texture tex;
		if (tex.loadFromFile(name))
			textures.insert(std::make_pair(name, tex));
	}
	return &textures[name];
}
