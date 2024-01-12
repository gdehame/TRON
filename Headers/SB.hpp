#ifndef SBCPP
#define SBCPP

#include "TRON.h"

void initializeSb() //On récupère les tableua des scores actuels (stockés dans des fichiers)
{
	SDL_RWops* fileE = SDL_RWFromFile("obj/Sbe.bin", "r");
	SDL_RWops* fileM = SDL_RWFromFile("obj/Sbm.bin", "r");
	SDL_RWops* fileH = SDL_RWFromFile("obj/Sbh.bin", "r");
	if (fileE == NULL || fileM == NULL || fileH == NULL)
	{
		printf("Warning: Unable to open file in initialization! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		for (int i = 0; i < 10; i++) //On lit les fichiers et ajoute dans le tableau les entrées lues
		{
			SDL_RWread(fileE, &ScoreboardE[i], sizeof(int), 1);
			SDL_RWread(fileM, &ScoreboardM[i], sizeof(int), 1);
			SDL_RWread(fileH, &ScoreboardH[i], sizeof(int), 1);
		}
		SDL_RWclose(fileE);
		SDL_RWclose(fileM);
		SDL_RWclose(fileH);
	}
}

int addtoSb(int x, int Sb[10]) //on ajoute le temps de la partie qui vient de finir au tableau des scores correspondnt à la difficulté
{
	int i = 0;
	while (i < 10 && x > Sb[i] && Sb[i] != 0) //on recherche la position à laquelle on doit insérer le nouveau score, on sort du tableau si toutefois il est trop mauvais
	{
		i++;
	}
	int transi = x;
	int transi2 = x;
	for (int j = i; j < 10; j++) //on décale les scores moins bons pour insérer le nouveau et supprime le dernier
	{
		transi = Sb[j];
		Sb[j] = transi2;
		transi2 = transi;
	}
	return i;
}

void updateSb() //On met à jour les fichiers de tableaux des scores
{
	SDL_RWops* fileE = SDL_RWFromFile("obj/Sbe.bin", "w");
	SDL_RWops* fileM = SDL_RWFromFile("obj/Sbm.bin", "w");
	SDL_RWops* fileH = SDL_RWFromFile("obj/Sbh.bin", "w");
	if (fileE == NULL || fileM == NULL || fileH == NULL)
	{
		printf("Warning: Unable to open file in update! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		for (int i=0; i<10; i++) //SDL détruit tout ce qui est dans le fichier à l'ouverture donc on se contente d'écrire comme si le fichier était vide
		{
			SDL_RWwrite(fileE, &ScoreboardE[i], sizeof(int), 1);
			SDL_RWwrite(fileM, &ScoreboardM[i], sizeof(int), 1);
			SDL_RWwrite(fileH, &ScoreboardH[i], sizeof(int), 1);
		}
	}
	SDL_RWclose(fileE);
	SDL_RWclose(fileM);
	SDL_RWclose(fileH);
}

void renderSb(int k, int Sb[10]) //affichage du tableau des scores
{
	LTexture Sbtexture[10];
	SDL_RenderClear(Rend);
	GameBG.setAlpha(128);
	GameBG.render(0, 0);
	SbTitletexture.render((SCREEN_WIDTH - SbTitletexture.getWidth()) / 2, SCREEN_HEIGHT / 4);
	int s = SCREEN_HEIGHT / 4 + SbTitletexture.getHeight();
	for (int i = 0; i < 10; i++) //on créé et affiche des textures pour chaque ligne du tableau des scores
	{
		if (i == k && !Sbtexture[i].loadFromRenderedText(to_string(Sb[i] / 1000.f), {255, 0, 0, 255}, Font))
		{
			printf("Failed to load Scoreboard Texture for new score!\n");
		}
		else if (i != k && !Sbtexture[i].loadFromRenderedText(to_string(Sb[i] / 1000.f), {0, 0, 0, 255}, Font))
		{
			printf("Failed to load Scoreboard Texture!\n");
		}
		else
		{
			Sbtexture[i].render((SCREEN_WIDTH - Sbtexture[i].getWidth()) / 2, s);
			s += Sbtexture[i].getHeight();
		}
	}//on affiche à l'échelle le tout et on remet l'opacité du fond au maximum
	SDL_RenderSetScale(Rend, ScalingX, ScalingY);
	SDL_RenderPresent(Rend);
	GameBG.setAlpha(255);
}

#endif
