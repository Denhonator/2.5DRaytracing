#include "Resources.h"
#include <math.h>

float constexpr eps = 0.0001f;

sf::Vector2f LineIntersection(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D);
float vectorAngle(sf::Vector2f a);
float vectorAngle(float x, float y);
float distanceS(sf::Vector2f a);

int main()
{
	unsigned int width = 1920;
	unsigned int height = 1080;
	float FOV = 70;
	bool mapToggle = true;
	Resources res;
	sf::RenderWindow window(sf::VideoMode(width, height), "Rays");
	window.setVerticalSyncEnabled(true);
	sf::View mapView = sf::View(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080));
	sf::View FPView = sf::View(sf::FloatRect(0, 0, width, height));
	window.setView(mapView);

	sf::VertexArray FP;
	FP.setPrimitiveType(sf::PrimitiveType::Lines);

	sf::VertexArray map;
	map.setPrimitiveType(sf::PrimitiveType::Lines);

	map.append(sf::Vertex(sf::Vector2f(-150, -150)));
	map.append(sf::Vertex(sf::Vector2f(150, -150)));

	map.append(sf::Vertex(sf::Vector2f(150, -150)));
	map.append(sf::Vertex(sf::Vector2f(150, 150)));

	map.append(sf::Vertex(sf::Vector2f(150, 150)));
	map.append(sf::Vertex(sf::Vector2f(-150, 150)));

	map.append(sf::Vertex(sf::Vector2f(-150, 150)));
	map.append(sf::Vertex(sf::Vector2f(-150, -150)));

	std::vector<Light> lights;
	lights.push_back(Light{ sf::Vector2f(120, 120), 1.0f });
	lights.push_back(Light{ sf::Vector2f(-120, 120), 1.0f });
	lights.push_back(Light{ sf::Vector2f(120, -120), 1.0f });
	lights.push_back(Light{ sf::Vector2f(-120, -120), 1.0f });

	sf::VertexArray playerForward;
	playerForward.setPrimitiveType(sf::PrimitiveType::Lines);

	sf::Sprite player;
	player.setTexture(*res.GetTexture("player.png"));
	player.setOrigin(player.getTexture()->getSize().x / 2, player.getTexture()->getSize().y / 2);
	sf::Vector2f move;
	sf::Vector2f rotation;
	float playerSpeed = 1;

	sf::Sprite floor, ceiling;
	floor.setTexture(*res.GetTexture("floor.png"));
	floor.setTextureRect(sf::IntRect(0, 0, width, height / 2));
	floor.setPosition(0, height / 2);
	ceiling.setTexture(*res.GetTexture("ceiling.png"));
	ceiling.setTextureRect(sf::IntRect(0, 0, width, height / 2));
	sf::Shader floorShader, ceilingShader;
	floorShader.loadFromFile("floor.shader", sf::Shader::Fragment);
	ceilingShader.loadFromFile("ceiling.shader", sf::Shader::Fragment);
	floorShader.setUniform("texture", sf::Shader::CurrentTexture);
	ceilingShader.setUniform("texture", sf::Shader::CurrentTexture);
	floorShader.setUniform("tiling", sf::Vector2f(0.3f, 0.3f));
	ceilingShader.setUniform("tiling", sf::Vector2f(0.3f, 0.3f));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Tab) {
					mapToggle = !mapToggle;
					window.setView(mapToggle ? mapView : FPView);
				}
			}
		}

		move = sf::Vector2f(0, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			move.x -= playerSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			move.x += playerSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			move.y -= playerSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			move.y += playerSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			player.rotate(-playerSpeed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			player.rotate(playerSpeed);
		}
		rotation.x = std::cos((player.getRotation()+90) / 180 * 3.14f);
		rotation.y = std::sin((player.getRotation()+90) / 180 * 3.14f);
		player.move(sf::Vector2f(rotation.x * move.y + move.x * rotation.y, rotation.y * move.y - move.x * rotation.x));

		playerForward.clear();
		FP.clear();

		for (unsigned int i = 0; i < width; i++) {
			float rayAngle = player.getRotation() - FOV / 2 + FOV * ((float)i / (float)width) + 90;
			rotation.x = -std::cos(rayAngle / 180 * 3.14f);
			rotation.y = -std::sin(rayAngle / 180 * 3.14f);

			sf::Vector2f intersection = sf::Vector2f(FLT_MAX,FLT_MAX);
			sf::Vector2f test;
			unsigned int wallIndex = 0;
			float distS = FLT_MAX;
			for (unsigned int j = 0; j < map.getVertexCount(); j+=2) {
				test = LineIntersection(player.getPosition(), player.getPosition() + rotation * 400.0f, map[j].position, map[j + 1].position);
				if (test.x != FLT_MAX) {
					float distTest = distanceS(test - player.getPosition());
					if (distTest < distS) {
						intersection = test;
						distS = distTest;
						wallIndex = j;
					}
				}
			}
			if (intersection.x != FLT_MAX) {
				playerForward.append(player.getPosition());
				playerForward.append(intersection);
				float dist = std::sqrt(distS);
				float wallHeight = 25.7f * height / dist;

				float b = 255.0f / dist;
				bool colTest = true;
				for (unsigned int l = 0; l < lights.size(); l++) {
					float lightDistS = distanceS(lights.at(l).pos - intersection);
					for (unsigned int j = 0; j < map.getVertexCount(); j += 2) {
						test = LineIntersection(intersection, lights.at(l).pos, map[j].position, map[j + 1].position);
						if (test.x != FLT_MAX && distanceS(test - intersection) < lightDistS) {
							colTest = false;
							break;
						}
					}
					b += 25500 * lights.at(l).strength / lightDistS;
				}
				b = std::min(255.0f, b);
				float pointPos = std::sqrt(distanceS(map[wallIndex].position - intersection));
				float texX = pointPos * 3;
				FP.append(sf::Vertex(sf::Vector2f(i, (height - wallHeight) / 2), sf::Color(b, b, b, 255), sf::Vector2f(texX,0)));
				FP.append(sf::Vertex(sf::Vector2f(i, (height + wallHeight) / 2), sf::Color(b, b, b, 255), sf::Vector2f(texX,64)));
			}
		}

		window.clear();
		if (mapToggle) {
			window.draw(map);
			window.draw(player);
			window.draw(playerForward);
		}
		else {
			//floor.setRotation(-player.getRotation());
			//ceiling.setRotation(-player.getRotation());
			floorShader.setUniform("offset", player.getPosition());
			ceilingShader.setUniform("offset", player.getPosition());
			floorShader.setUniform("angle", player.getRotation() + 90);
			ceilingShader.setUniform("angle", player.getRotation() + 90);
			window.draw(floor, &floorShader);
			window.draw(ceiling, &ceilingShader);
			window.draw(FP, res.GetTexture("wall.png"));
		}
		window.display();
	}

	return 0;
}

sf::Vector2f LineIntersection(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D)
{
	// Line AB represented as a1x + b1y = c1 
	float a1 = B.y - A.y;
	float b1 = A.x - B.x;
	float c1 = a1 * (A.x) + b1 * (A.y);

	// Line CD represented as a2x + b2y = c2 
	float a2 = D.y - C.y;
	float b2 = C.x - D.x;
	float c2 = a2 * (C.x) + b2 * (C.y);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		return sf::Vector2f(FLT_MAX, FLT_MAX);
	}
	else
	{
		float x = (b2 * c1 - b1 * c2) / determinant;
		float y = (a1 * c2 - a2 * c1) / determinant;
		if ((((x - A.x >= -eps && x - B.x <= eps) || (x - A.x <= eps && x - B.x >= -eps)) && ((x - C.x >= -eps && x - D.x <= eps) || (x - C.x <= eps && x - D.x >= -eps))) &&		//Only return points between the points in the given lines
			(((y - A.y >= -eps && y - B.y <= eps) || (y - A.y <= eps && y - B.y >= -eps)) && ((y - C.y >= -eps && y - D.y <= eps) || (y - C.y <= eps && y - D.y >= -eps)))) {
			return sf::Vector2f(x, y);
		}
		return sf::Vector2f(FLT_MAX, FLT_MAX);
	}
}

float vectorAngle(sf::Vector2f a) {
	float angle = std::atan2(a.y, a.x) * 180 / 3.14f + 90;
	if (angle > 360)
		return angle - 360;
	if (angle < 0)
		return angle + 360;
	return angle;
}

float vectorAngle(float x, float y) {
	float angle = std::atan2(y, x) * 180 / 3.14f + 90;
	if (angle > 360)
		return angle - 360;
	if (angle < 0)
		return angle + 360;
	return angle;
}
float distanceS(sf::Vector2f a) {
	return a.x * a.x + a.y * a.y;
}


//float vectorAngle(sf::Vector2f a, sf::Vector2f b) {
//	return atan2(a.x * b.y - a.y * b.x, a.x * b.y + a.y * b.y);
//}