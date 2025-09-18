export module scenes;

import std;

import BridgeMaker;


namespace scene
{

	export bm::gfx::ScenePtr triangle()
	{
        using namespace bm::gfx;

        auto triangle1 = Object::make<Triangle>();
        auto triangle2 = Object::make<Triangle>();

        triangle1->move({1.f, 1.f, 0.f});
        triangle1->apply();

        auto scene = Scene::make();
        scene->add(triangle1);
        scene->add(triangle2);
        return scene;

	}

	export bm::gfx::ScenePtr cube()
	{
        using namespace bm::gfx;

        auto cube = Object::make<Cube>();

        auto grid = Object::make<Grid>(500.f, 1.f);
        grid->setColor({.15f, .15f, .15f, 1.f});

        auto scene = Scene::make();
        scene->add(cube);
        scene->add(grid);

        return scene;
	}

    export bm::gfx::ScenePtr square()
    {
        using namespace bm::gfx;

        auto square1 = Object::make<Square>();
        auto square2 = Object::make<Square>();

        square1->moveY(2.f);
        square1->apply();

        auto scene = Scene::make();
        scene->add(square1);
        scene->add(square2);

        return scene;
    }

}