#include "./Headers/TRON.h"
#include "./Headers/Game.hpp"
#include "./Headers/Fruits.hpp"
#include "./Headers/Timer.hpp"
#include "./Headers/Texture.hpp"
#include "./Headers/Moto.hpp"
#include "./Headers/SB.hpp"
#include "./Headers/Menus.hpp"
#include "./Headers/INI.hpp"
#include "./Headers/END.hpp"

int main(int argc, char* args[])
{
	if(!init()) //on initialise et affiche une erreur en cas de problème
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if(!loadMedia()) //on charge les textures et affiche une erreur en cas de problème
		{
			printf("Failed to load media!\n");
		}
		else
		{
			gameTimer.start();
			stringstream timer;
			SDL_Event e;
			SDL_Color textColor = {180, 180, 180, 255};
			Moto Motoj;
			Enemy EnMoto;
			bool end = true; //détermine si une partie est en cours ou non
			bool enemyDead = false;

			while(!quit) //boucle d'application
			{
				MainMenu(e, &Motoj, &EnMoto, &end, &enemyDead);
				GameLoop(e, &Motoj, &EnMoto, &end, &enemyDead, &timer, textColor);
			}
		}
	}
	close();
	return 0;
}
