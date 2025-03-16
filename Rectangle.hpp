#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include <SFML/Graphics.hpp>

class Rectangle{
    public:
    void setColor(sf::Color color);
    Rectangle(sf::Color color, int xCoord, int yCoord);
    void draw(sf::RenderTarget& window);
    sf::Color getColor();
      
    int _xCoord;
    int _yCoord;
    

    private:
    sf::Color _color;
};

Rectangle::Rectangle(sf::Color color, int xCoord, int yCoord){
    _color = color;
    _xCoord = xCoord;
    _yCoord = yCoord;
}

void Rectangle::draw(sf::RenderTarget& window){
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(15,15));
    rectangle.setFillColor(_color);
    rectangle.setPosition(_xCoord*15, _yCoord*15);
    window.draw(rectangle);
}

void Rectangle::setColor(sf::Color color){
    _color = color;
}

sf::Color Rectangle::getColor(){
    return _color;
}

#endif 
