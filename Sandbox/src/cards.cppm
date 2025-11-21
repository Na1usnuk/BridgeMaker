export module cards;

import BridgeMaker;

import std;



//std::shared_ptr<bm::gfx::Cube> makeCard()
//{
//	static std::default_random_engine re;
//	std::uniform_real_distribution<float> d(0.2f, 0.8f);
//
//	auto card = bm::gfx::Object::make<bm::gfx::Cube>();
//
//	card->scale({ 0.6f / 2.f, 0.02f / 2.f, 0.9f / 2.f });
//	card->setPosition({ 0.f, .0f, 0.f });
//	card->rotateX(90);
//	card->apply();
//
//	const auto color = d(re);
//	card->setColor({ color, color, color });
//
//	return card;
//}


//void applyAlgorithm(std::vector<std::shared_ptr<bm::gfx::Cube>>& cards)
//{
//	const int count = cards.size();
//	const float card_width = 0.3f;
//	const float gap = -0.2f;
//	const float step = card_width + gap;
//	const float overall_width = step * count;
//
//	const float max_x = overall_width / 2.f;
//	const float min_x = overall_width / -2.f;
//
//	//Function: y = -x ^ 2
//	//Parabola
//	for (float x = min_x, z = 0.f, i = 0;
//		x <= max_x and i < count;
//		x += step, i++, z -= 0.00001)
//	{
//		const float y = -std::pow(x / 1.5f, 2) ;
//
//		cards[i]->setPosition({ x * 0.5f, y, z});
//		cards[i]->rotateZ(std::clamp( - x * 100, -65.f, 65.f));
//		cards[i]->apply();
//	}


	//Function: y = 2
	//for (float x = min_x + 0.15f, z = 0.f, i = 0; 
	//	x <= max_x and i < count; 
	//	x += step, i++, z -= 0.001)
	//{
	//	cards[i]->setPosition({ x, 0, z });
	//	cards[i]->rotateZ(std::clamp(-x * 40, -35.f, 35.f));
	//	cards[i]->apply();
	//}




	//Function: y = x ^ 2 + y ^ 2
	//Circle
	//const float radius = .3f;
	//const float start_angle = 90.f;
	//const float end_angle = -90.f;
	//const float angle_step = (end_angle - start_angle) / (count - 1);

	//auto rad = [](float deg)
	//	{
	//		return deg * 3.1415926535f / 180.0f;
	//	};

	//float z = 0.f;
	//for (int i = 0; i < count; ++i)
	//{
	//	float angle_deg = start_angle + angle_step * i;
	//	float angle_rad = rad(angle_deg);

	//	float x = radius * std::sin(angle_rad);
	//	float y = radius * std::cos(angle_rad);

	//	cards[i]->setPosition({ x, y, z });
	//	z += 0.0001f;
	//	cards[i]->rotateZ(-angle_deg);
	//	cards[i]->apply();
	//}


	//Function of golden spiral
	//const float angle_step = 30.f;
	//const float phi = std::numbers::phi_v<float>;
	//const float b = std::log(phi) / (std::numbers::pi_v<float> / 2.f);
	//const float a = .2f;

	//float z = 0.f;
	//for (int i = 0; i < count; ++i)
	//{
	//	float angle_deg = i * angle_step;
	//	float angle_rad = rad(angle_deg - 90.f);

	//	float r = a * std::exp(b * angle_rad);

	//	float x = r * std::cos(angle_rad);
	//	float y = r * std::sin(angle_rad);
	//	z += 0.001f;

	//	cards[i]->setPosition({ x, y, z });
	//	cards[i]->rotateZ(angle_deg);
	//	cards[i]->apply();
	//}


//}


//export bm::gfx::ScenePtr Cards(int count)
//{
//    using namespace bm::gfx;
//
//    std::vector<std::shared_ptr<Cube>> cards;
//    cards.reserve(count);
//
//	while (count--)
//		cards.push_back(makeCard());
//
//	applyAlgorithm(cards);
//
//    auto scene = Scene::make();
//
//	for (auto const& c : cards)
//		scene->add(c);
//
//	//auto grid = Object::make<Grid>(500.f, 1.f);
//	//grid->setColor({ .15f, .15f, .15f });
//	//scene->add(grid);
//
//	//auto space = Object::make<CoordinateSpace>(500.f);
//	//scene->add(space);
//
//	auto x_line = Object::make<Line>();
//	auto y_line = Object::make<Line>();
//	auto z_line = Object::make<Line>();
//
//	x_line->setScale({ 1000.f, 0.01f, 0.01f });
//	y_line->setScale({ 1000.f, 0.01f, 0.01f });
//	z_line->setScale({ 1000.f, 0.01f, 0.01f });
//
//	y_line->rotateZ(90.f);
//	z_line->rotateY(90.f);
//
//	x_line->setColor({ 1.f, 0.f, 0.f });
//	y_line->setColor({ 0.f, 1.f, 0.f });
//	z_line->setColor({ 0.f, 0.f, 1.f });
//
//	x_line->apply();
//	y_line->apply();
//	z_line->apply();
//
//	scene->add(x_line);
//	scene->add(y_line);
//	scene->add(z_line);
//
//	return scene;
//}