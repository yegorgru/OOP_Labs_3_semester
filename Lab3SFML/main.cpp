#include "GameModel.h"
#include "GameController.h"
#include "GameRender.h"

//����������:
//https://habr.com/ru/post/449596/
//https://habr.com/ru/post/480710/
//https://ravesli.com/uroki-po-sfml/

int main(){
	GameModel game;
	GameRender render(game);
	GameController controller(game, render);
	controller.Run();
	return 0;
}