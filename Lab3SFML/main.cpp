#include "GameModel.h"
#include "GameController.h"
#include "GameRender.h"

int main(){
	GameModel game;
	GameRender render(game);
	GameController controller(game, render);
	controller.Run();
	return 0;
}