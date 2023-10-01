#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

class Body {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float mass;

    Body(float radius, float mass, sf::Vector2f position, sf::Vector2f velocity)
        : shape(radius), mass(mass), velocity(velocity) {
        shape.setPosition(position);
    }

    void applyForce(sf::Vector2f force) {
        velocity += force / mass;
    }

    void update() {
        shape.move(velocity);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gravity Simulator");

    // Create earth and moon bodies with scaled masses
    Body earth(10.f, 5.972f, sf::Vector2f(400.f, 300.f), sf::Vector2f(0.f, 0.f));
    Body moon(5.f, 0.07348f, sf::Vector2f(400.f + 384.f, 300.f), sf::Vector2f(0.f, 0.1f));

    // Create a font and text for displaying the distance and force
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Error loading font" << std::endl;
        return 1;
    }
    sf::Text distanceText;
    distanceText.setFont(font);
    distanceText.setCharacterSize(24);
    distanceText.setFillColor(sf::Color::White);
    distanceText.setPosition(10.f, 10.f);

    sf::Text forceText;
    forceText.setFont(font);
    forceText.setCharacterSize(24);
    forceText.setFillColor(sf::Color::White);
    forceText.setPosition(10.f, 30.f);

    sf::Text earthInfoText;
    earthInfoText.setFont(font);
    earthInfoText.setCharacterSize(16);
    earthInfoText.setFillColor(sf::Color::White);
    earthInfoText.setPosition(10.f, 540.f);

    sf::Text moonInfoText;
    moonInfoText.setFont(font);
    moonInfoText.setCharacterSize(16);
    moonInfoText.setFillColor(sf::Color::White);
    moonInfoText.setPosition(10.f, 560.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Vector2f direction = earth.shape.getPosition() - moon.shape.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f force = direction / (distance * distance * distance) * earth.mass * moon.mass;

        earth.applyForce(-force);
        moon.applyForce(force);

        earth.update();
        moon.update();

        // Update the text with the current distance and force
        std::stringstream distanceSS;
        distanceSS << "Distance: " << distance;
        distanceText.setString(distanceSS.str());

        std::stringstream forceSS;
        forceSS << "Force: " << std::sqrt(force.x * force.x + force.y * force.y);
        forceText.setString(forceSS.str());

        std::stringstream earthInfoSS;
        earthInfoSS << std::fixed << std::setprecision(4); // Set precision to 4 decimal places
        earthInfoSS << "Earth - Position: (" << earth.shape.getPosition().x << ", " << earth.shape.getPosition().y << ") Velocity: (" << earth.velocity.x << ", " << earth.velocity.y << ")";
        earthInfoText.setString(earthInfoSS.str());

        std::stringstream moonInfoSS;
        moonInfoSS << std::fixed << std::setprecision(4); // Set precision to 4 decimal places
        moonInfoSS << "Moon - Position: (" << moon.shape.getPosition().x << ", " << moon.shape.getPosition().y << ") Velocity: (" << moon.velocity.x << ", " << moon.velocity.y << ")";
        moonInfoText.setString(moonInfoSS.str());

        // Draw a line between the earth and the moon
        sf::Vertex line[] = {
            sf::Vertex(earth.shape.getPosition() + sf::Vector2f(earth.shape.getRadius(), earth.shape.getRadius())),
            sf::Vertex(moon.shape.getPosition() + sf::Vector2f(moon.shape.getRadius(), moon.shape.getRadius()))
        };

        window.clear();
        window.draw(line, 2, sf::Lines);
        window.draw(earth.shape);
        window.draw(moon.shape);
        window.draw(distanceText);
        window.draw(forceText);
        window.draw(earthInfoText);
        window.draw(moonInfoText);
        window.display();
    }

    return 0;
}
