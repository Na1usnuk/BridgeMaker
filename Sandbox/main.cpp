import sandbox;
import std;

int main(int argc, char** argv)
{
	std::cout << "Main start\n";
	bm::init();

	SandBox SandBox;

	return SandBox.run(argc, argv);
}
