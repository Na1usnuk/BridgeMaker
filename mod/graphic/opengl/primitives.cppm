export module bm.gfx:primitives;

import :buffer;
import :array;
import :shader;
import :mesh;
import :material;
import :scene;



import std;


namespace bm::gfx
{

export class Triangle : public Object
{
public:

    //static constexpr std::array<float, 24> vertices;

    //static constexpr std::array<unsigned int, 3> indices;

public:

    Triangle();
};


export class Square : public Object
{
public:

    Square();

private:
};


export class Cube : public Object
{

public:

    Cube();
};


export class Grid : public Object
{
public:

    Grid(float size_of_line = 100.f, float size_of_square_side = 0.2f);

};


// Sphere
export class Sphere : public Object
{
public:
    Sphere(int sectors = 36, int stacks = 18);
};

// Pyramid (square base)
export class Pyramid : public Object
{
public:
    Pyramid();
};

// Cone
// Cone
export class Cone : public Object
{
public:
    Cone(int segments = 36);
    
};

// Cylinder
export class Cylinder : public Object
{
public:
    Cylinder(int segments = 36);
};




}