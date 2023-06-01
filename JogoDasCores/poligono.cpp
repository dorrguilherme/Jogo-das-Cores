#include "poligono.h"

using namespace std;

void poligono::initPoligono(bool visible1, int red, int green, int blue) {
    visible = visible1;
    this->red = red;
    this->green = green;
    this->blue = blue;
}

void poligono::setVisible(bool visible1) {
    visible = visible1;
}

bool poligono::getVisible() {
    return visible;
}

int poligono::getRed() {
    return red;
}

int poligono::getGreen() {
    return green;
}

int poligono::getBlue() {
    return blue;
}