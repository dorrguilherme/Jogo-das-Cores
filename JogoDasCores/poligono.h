#ifndef POLIGONO_H
#define POLIGONO_H

class poligono {
private:
    bool visible;
    int red;
    int green;
    int blue;

public:
    void initPoligono(bool visible1, int red, int green, int blue);
    void setVisible(bool visible1);
    bool getVisible();
    int getRed();
    int getGreen();
    int getBlue();
};

#endif