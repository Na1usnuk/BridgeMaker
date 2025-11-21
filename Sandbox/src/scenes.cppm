export module scenes;

import std;

import BridgeMaker;


namespace scene
{

    export bm::Traits<bm::gfx::Scene>::Ptr test()
    {
        using namespace bm::gfx;
   
        //auto cube = Object::make<Cube>();
        //cube->move({1.f, 0.f, 0.f});
        //cube->apply();

        //auto grid = Object::make<Grid>(500.f, 1.f);
        //grid->setColor({.15f, .15f, .15f});

		//auto plane = Object::make<Square>();
		//plane->rotateX(-90.f);
		//plane->scale({ 10.f, 10.f, 10.f });
		//plane->setColor({ 0.6f, 0.6f, 0.6f });
		//plane->setPositionY(0.1f);
		//plane->apply();

        auto scene = Scene::make();
		//scene->add(plane);
  //      scene->add(cube);
  //      scene->add(grid);

        return scene;
	}

	export bm::Traits<bm::gfx::Scene>::Ptr triangle()
	{
        using namespace bm::gfx;

        auto triangle = Object::make<Triangle>();
        auto grid = Object::make<Grid>();

        auto scene = Scene::make();
        scene->addObject(std::move(triangle));
        scene->addObject(std::move(grid));
        return scene;

	}

	/*export bm::gfx::ScenePtr cube()
	{
        using namespace bm::gfx;

        auto cube = Object::make<Cube>();

        auto grid = Object::make<Grid>(500.f, 1.f);
        grid->setColor({.15f, .15f, .15f});

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
    }*/

}