#include <Base/Game.h>

using namespace MyEngine;

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<Game> game = std::make_unique<Game>();

	game->Run();

	return 0;
}