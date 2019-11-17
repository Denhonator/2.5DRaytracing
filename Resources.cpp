#include "Resources.h"

sf::Texture* Resources::GetTexture(std::string name)
{
	if(textures.find(name)==textures.end()){
		sf::Texture tex;
		tex.setRepeated(true);
		if (tex.loadFromFile(name))
			textures.insert(std::make_pair(name, tex));
		else
			std::cout << "Failed to load texture: " + name + "\n";
	}
	return &textures[name];
}
