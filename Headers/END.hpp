#ifndef ENDHPP
#define ENDHPP

#include "TRON.h"

void RenderGameOver(Moto Motoj, Enemy EnMoto) //affichage de l'écran de défaite
{
	Mix_HaltMusic();
	Mix_PlayChannel(-1, gameLose, 0);
	SDL_RenderClear(Rend);
	GameBG.setAlpha(128);
	GameBG.render(0,0);
	timerTexture.setColor(0,0,0);
	timerTexture.render((SCREEN_WIDTH - timerTexture.getWidth()) / 2, 0);
	Motoob.setAlpha(128);
	Motoj.render();
	EnMoto.render();
	GameOver.render((SCREEN_WIDTH - GameOver.getWidth()) / 2, (SCREEN_HEIGHT - GameOver.getHeight()) / 2);
	SDL_RenderSetScale(Rend, ScalingX, ScalingY);
	SDL_RenderPresent(Rend);
	GameBG.setAlpha(255);
	Motoob.setAlpha(255);
	timerTexture.setColor(180,180,180);
}

void RenderWIN(Moto Motoj, Enemy EnMoto) //affichage de l'écran de victoire
{
	Mix_HaltMusic();
	Mix_PlayChannel(-1, gameWIN, 0);
	SDL_RenderClear(Rend);
	timerTexture.setColor(0,0,0);
	GameBG.setAlpha(128);
	GameBG.render(0,0);
	timerTexture.render((SCREEN_WIDTH - timerTexture.getWidth()) / 2, 0);
	Motoob.setAlpha(128);
	Motoj.render();
	EnMoto.render();
	WinTexture.render((SCREEN_WIDTH - WinTexture.getWidth()) / 2, (SCREEN_HEIGHT - WinTexture.getHeight()) / 2);
	SDL_RenderSetScale(Rend, ScalingX, ScalingY);
	SDL_RenderPresent(Rend);
	GameBG.setAlpha(255);
	Motoob.setAlpha(255);
	timerTexture.setColor(180,180,180);
}

void close() //fermeture des différents objets et mise à jour des tableaux des scores lors de la fermeture de l'application
{
	updateSb();
	Mix_FreeChunk(gameWIN);
	Mix_FreeChunk(gameLose);
	Mix_FreeMusic(gameMusic);
	Mix_FreeMusic(menuMusic);
	gameMusic = NULL;
	menuMusic = NULL;
	SDL_DestroyRenderer(Rend);
	SDL_DestroyWindow(Win);
	Win = NULL;
	Rend = NULL;
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

#endif
