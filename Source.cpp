#include <iostream>
#include "Resources.h"
#include <math.h>

sf::Vector2f lineLineIntersection(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D);
float vectorAngle(sf::Vector2f a);
float vectorAngle(float x, float y);
float distanceS(sf::Vector2f a);

int main()
{
	unsigned int width = 1920;
	unsigned int height = 1080;
	float FOV = 70;
	Resources res;
	sf::RenderWindow window(sf::VideoMode(width, height), "Rays");
	window.setVerticalSyncEnabled(true);
	sf::View mapView = sf::View(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080));
	window.setView(mapView);

	sf::VertexArray map;
	map.setPrimitiveType(sf::PrimitiveType::Lines);

	map.append(sf::Vertex(sf::Vector2f(0, 0)));
	map.append(sf::Vertex(sf::Vector2f(300, 0)));

	map.append(sf::Vertex(sf::Vector2f(0, 100)));
	map.append(sf::Vertex(sf::Vector2f(300, 500)));

	map.append(sf::Vertex(sf::Vector2f(-400, -100)));
	map.append(sf::Vertex(sf::Vector2f(0, 0)));

	sf::VertexArray playerForward;
	playerForward.setPrimitiveType(sf::PrimitiveType::Lines);

	sf::Sprite player;
	player.setTexture(*res.GetTexture("player.png"));
	player.setOrigin(player.getTexture()->getSize().x / 2, player.getTexture()->getSize().y / 2);
	sf::Vector2f move;
	sf::Vector2f rotation;
	float playerSpeed = 3;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
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

		for (unsigned int i = 0; i < width; i++) {
			float rayAngle = player.getRotation() - FOV / 2 + FOV * ((float)i / (float)width) + 90;
			rotation.x = -std::cos((rayAngle) / 180 * 3.14f);
			rotation.y = -std::sin((rayAngle) / 180 * 3.14f);

			for (unsigned int j = 0; j < map.getVertexCount(); j+=2) {
				sf::Vector2f intersection = lineLineIntersection(player.getPosition(), player.getPosition() + rotation * 100.0f, map[j].position, map[j+1].position);
				if (intersection.x != FLT_MAX) {
					playerForward.append(player.getPosition());
					playerForward.append(intersection);
					break;
				}
			}
		}

		/*if (move.x != 0 || move.y != 0) {
			player.setRotation(std::atan2(move.y, move.x)*180/3.14f+90);
		}*/

		window.clear();
		window.draw(map);
		window.draw(player);
		window.draw(playerForward);
		window.display();
	}

	return 0;
}

sf::Vector2f lineLineIntersection(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D)
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
		if ((((x >= A.x && x <= B.x) || (x <= A.x && x >= B.x)) && ((x >= C.x && x <= D.x) || (x <= C.x && x >= D.x))) &&		//Only return points between the points in the given lines
			(((y >= A.y && y <= B.y) || (y <= A.y && y >= B.y)) && ((y >= C.y && y <= D.y) || (y <= C.y && y >= D.y)))) {
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