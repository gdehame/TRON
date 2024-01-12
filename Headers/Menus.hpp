#ifndef MENUHPP //header guard
#define MENUHPP

#include "TRON.h"

void RenderMain() //affichage du menu principal
{ //on clear le renderer puis on affiche le fond puis les texture de texte et on met à l'échelle en fonction de la taille choisis pas l'utilisateur puis on affiche
	SDL_RenderClear(Rend);
	MainMenuBG.render(0, 0);
	MainMenuTexture.render((SCREEN_WIDTH - MainMenuTexture.getWidth()) / 2, (SCREEN_HEIGHT + 7 * MainMenuTexture.getHeight()) / 2);
	MainMenuTexture2.render((SCREEN_WIDTH - MainMenuTexture2.getWidth()) / 2,(SCREEN_HEIGHT + 3 * MainMenuTexture2.getHeight()) / 2);
	MainMenuTexture3.render((SCREEN_WIDTH - MainMenuTexture3.getWidth()) / 2, (SCREEN_HEIGHT + 5 * MainMenuTexture3.getHeight()) / 2);
	SDL_RenderSetScale(Rend, ScalingX, ScalingY);
	SDL_RenderPresent(Rend);
}

void MainMenu(SDL_Event e, Moto* Motoj, Enemy* EnMoto, bool* end, bool* enemyDead)
{
	FruitReset(); //On enlève les fruits pour que ceux d'une partie n'apparaissent pas dans la suivante
	bool buttonCovered[3] = {false, false, false};
	while ((*end) && !quit) //boucle d'initialisation du jeu (navigation dans les menus avant de commencer une partie)
	{
		if (Mix_PlayingMusic() == 0)
		{
			Mix_PlayMusic(menuMusic, -1);
		}
		RenderMain();
		while (SDL_PollEvent(&e) != 0) //gestion des évènements
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) //Permet de changer la taille de l'affichage lorsque la taille de la fenêtre est changée à la souris
			{
				ScalingX = e.window.data1 / ((float) SCREEN_WIDTH);
				ScalingY = e.window.data2 / ((float) SCREEN_HEIGHT);
			}
			else if (e.type == SDL_MOUSEMOTION) //On affiche que la souris pointe sur un boutton en changeant la couleur du texte
			{
				int x, y;
				SDL_GetMouseState(&x, &y); //On récupère les coordonnées de la souris et si elle est sur un bouton on le met en rouge et remet les autres à leur couleur initiale
				if (x >= MainMenuTexture.getPosX() && x <= MainMenuTexture.getPosX() + MainMenuTexture.getWidth() * ScalingX && y >= MainMenuTexture.getPosY() && y <= MainMenuTexture.getPosY() + MainMenuTexture.getHeight() * ScalingY)
				{
					MainMenuTexture.setColor(255, 0, 0);
					MainMenuTexture2.setColor(255, 255, 255);
					MainMenuTexture3.setColor(255, 255, 255);
					buttonCovered[0] = true;
					buttonCovered[1] = false;
					buttonCovered[2] = false;
				}
				else if (x >= MainMenuTexture2.getPosX() && x <= MainMenuTexture2.getPosX() + MainMenuTexture2.getWidth() * ScalingX && y >= MainMenuTexture2.getPosY() && y <= MainMenuTexture2.getPosY() + MainMenuTexture2.getHeight() * ScalingY)
				{
					MainMenuTexture2.setColor(255, 0, 0);
					MainMenuTexture.setColor(255, 255, 255);
					MainMenuTexture3.setColor(255, 255, 255);
					buttonCovered[1] = true;
					buttonCovered[0] = false;
					buttonCovered[2] = false;
				}
				else if (x >= MainMenuTexture3.getPosX() && x <= MainMenuTexture3.getPosX() + MainMenuTexture3.getWidth() * ScalingX && y >= MainMenuTexture3.getPosY() && y <= MainMenuTexture3.getPosY() + MainMenuTexture3.getHeight() * ScalingY)
				{
					MainMenuTexture3.setColor(255, 0, 0);
					MainMenuTexture.setColor(255, 255, 255);
					MainMenuTexture2.setColor(255, 255, 255);
					buttonCovered[2] = true;
					buttonCovered[1] = false;
					buttonCovered[0] = false;
				}
				else
				{
					MainMenuTexture3.setColor(255, 255, 255);
					MainMenuTexture.setColor(255, 255, 255);
					MainMenuTexture2.setColor(255, 255, 255);
					buttonCovered[2] = false;
					buttonCovered[1] = false;
					buttonCovered[0] = false;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) //On prend en compte le clique sur le bouton actuellement sous la souris et agit en conséquence
			{
				if (buttonCovered[0])
				{
					MainMenuTexture.setColor(255, 255, 255);
					if (!isMultiplayer && (*EnMoto).setDifficulty())
					{
						gameTimer.stop();
						*Motoj = *new Moto();
						*EnMoto = *new Enemy();
						*end = false;
						*enemyDead = false;
						gameTimer.start();
						Mix_HaltMusic();
						Mix_PlayMusic(gameMusic, -1);
					}
				}
				else if (buttonCovered[1])
				{
					MainMenuTexture2.setColor(255, 255, 255);
					ESCMenu();
				}
				else if (buttonCovered[2])
				{
					MainMenuTexture3.setColor(255, 255, 255);
					NetworkMenu();
					if (isMultiplayer) //si toutefois on commence la partie (si on ne quitte pas sans avoir créé de connection) on lance la partie
					{
						gameTimer.stop();
						*Motoj = *new Moto();
						*EnMoto = *new Enemy();
						*end = false;
						*enemyDead = false;
						gameTimer.start();
						Mix_HaltMusic();
						Mix_PlayMusic(gameMusic, -1);
					}
				}
			}
			else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
			{
				if (e.key.keysym.sym == SDLK_q)
				{
					quit = true;
				}
				else if (e.key.keysym.sym == SDLK_SPACE) //début de la partie: on reset les éléments au cas où une partie à déjà eu lieu
				{
					if (!isMultiplayer && (*EnMoto).setDifficulty())
					{
						gameTimer.stop();
						*Motoj = *new Moto();
						*EnMoto = *new Enemy();
						*end = false;
						*enemyDead = false;
						gameTimer.start();
						Mix_HaltMusic();
						Mix_PlayMusic(gameMusic, -1);
					}
				}
				else if (e.key.keysym.sym == SDLK_ESCAPE) //entrée dans le menu des options
				{
					ESCMenu();
				}
				else if (e.key.keysym.sym == SDLK_m) //mode multijoueur: entrée dans le menu de connection hôte/client
				{
					NetworkMenu();
					if (isMultiplayer) //si toutefois on commence la partie (si on ne quitte pas sans avoir créé de connection) on lance la partie
					{
						gameTimer.stop();
						*Motoj = *new Moto();
						*EnMoto = *new Enemy();
						*end = false;
						*enemyDead = false;
						gameTimer.start();
						Mix_HaltMusic();
						Mix_PlayMusic(gameMusic, -1);
					}
				}
			}
		}
	}
}

void RenderPause(Moto Motoj, Enemy EnMoto) //affichage du menu de pause
{
	SDL_RenderClear(Rend);
	GameBG.render(0,0); // on affiche les textures
	timerTexture.render((SCREEN_WIDTH - timerTexture.getWidth()) / 2, 0);
	Motoj.render();
	EnMoto.render();
	renderFruits();
	PauseTexture1.render((SCREEN_WIDTH - PauseTexture1.getWidth()) / 2, 0);
	PauseTexture2.render((SCREEN_WIDTH - PauseTexture2.getWidth()) / 2, (SCREEN_HEIGHT - PauseTexture2.getHeight()) / 2);
	PauseTexture3.render((SCREEN_WIDTH - PauseTexture3.getWidth()) / 2, (SCREEN_HEIGHT + PauseTexture3.getHeight()) / 2);
	PauseTexture4.render((SCREEN_WIDTH - PauseTexture4.getWidth()) / 2, (SCREEN_HEIGHT + 3 * PauseTexture4.getHeight()) / 2);
	SDL_RenderSetScale(Rend, ScalingX, ScalingY);
	SDL_RenderPresent(Rend);
}

bool PauseMenu(Moto Motoj, Enemy EnMoto, SDL_Event e) //On gère le menu de pause
{
	PauseTexture2.setColor(0, 0, 0);
	PauseTexture3.setColor(0, 0, 0);
	PauseTexture4.setColor(0, 0, 0);
	GameBG.setAlpha(128); //on diminue l'opacité des textures de jeu pour avoir l'état actuel de la partie en fond
	timerTexture.setAlpha(128);
	Motoob.setAlpha(128);
	Fruittexture.setAlpha(128);
	Mix_HaltMusic();
	Mix_PlayMusic(menuMusic, -1);
	int MotoVx = Motoj.getVelX();
	int MotoVy = Motoj.getVelY();
	int EnMotoVx = EnMoto.getVelX();
	int EnMotoVy = EnMoto.getVelY();
	Motoj.resetVel(); //on arrête les motos pour qu'elles ne bougent pas en fond
	EnMoto.resetVel();
	pause = true;
	gameTimer.pause();
	bool end = false;
	bool buttonCovered[3] = {false, false, false};

	while (pause)
	{
		RenderPause(Motoj, EnMoto);
		while (SDL_PollEvent (&e) != 0)
		{
			if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) //Permet de changer la taille de l'affichage lorsque la taille de la fenêtre est changée à la souris
			{
				ScalingX = e.window.data1 / ((float) SCREEN_WIDTH);
				ScalingY = e.window.data2 / ((float) SCREEN_HEIGHT);
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				int x, y;
				SDL_GetMouseState(&x, &y); //On récupère les coordonnées de la souris et colore en rouge le bouton sur lequel la souris est si il y en a un et on remet les autres à leur couleur initiale
				if (x >= PauseTexture2.getPosX() && x <= PauseTexture2.getPosX() + PauseTexture2.getWidth() * ScalingX && y >= PauseTexture2.getPosY() && y <= PauseTexture2.getPosY() + PauseTexture2.getHeight() * ScalingY)
				{
					PauseTexture2.setColor(255, 0, 0);
					PauseTexture3.setColor(0, 0, 0);
					PauseTexture4.setColor(0, 0, 0);
					buttonCovered[0] = true;
					buttonCovered[1] = false;
					buttonCovered[2] = false;
				}
				else if (x >= PauseTexture3.getPosX() && x <= PauseTexture3.getPosX() + PauseTexture3.getWidth() * ScalingX && y >= PauseTexture3.getPosY() && y <= PauseTexture3.getPosY() + PauseTexture3.getHeight() * ScalingY)
				{
					PauseTexture3.setColor(255, 0, 0);
					PauseTexture2.setColor(0, 0, 0);
					PauseTexture4.setColor(0, 0, 0);
					buttonCovered[1] = true;
					buttonCovered[0] = false;
					buttonCovered[2] = false;
				}
				else if (x >= PauseTexture4.getPosX() && x <= PauseTexture4.getPosX() + PauseTexture4.getWidth() * ScalingX && y >= PauseTexture4.getPosY() && y <= PauseTexture4.getPosY() + PauseTexture4.getHeight() * ScalingY)
				{
					PauseTexture4.setColor(255, 0, 0);
					PauseTexture2.setColor(0, 0, 0);
					PauseTexture3.setColor(0, 0, 0);
					buttonCovered[2] = true;
					buttonCovered[1] = false;
					buttonCovered[0] = false;
				}
				else
				{
					PauseTexture4.setColor(0, 0, 0);
					PauseTexture2.setColor(0, 0, 0);
					PauseTexture3.setColor(0, 0, 0);
					buttonCovered[2] = false;
					buttonCovered[1] = false;
					buttonCovered[0] = false;
				}
				RenderPause(Motoj, EnMoto);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (buttonCovered[0])
				{
					end=true;
					pause = false;
				}
				else if (buttonCovered[1])
				{
					pause = false;
				}
				else if (buttonCovered[2])
				{
					quit = true;
					pause = false;
				}
			}
			else if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_ESCAPE)
			{
				pause = false;
			}
			else if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_m)
			{
				end=true;
				pause = false;
			}
			else if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_q))
			{
				quit = true;
				pause = false;
			}
		}
	}
	GameBG.setAlpha(255); //on remet les opacités des textures au maximum
	timerTexture.setAlpha(255);
	Motoob.setAlpha(255);
	Fruittexture.setAlpha(255);
	gameTimer.unpause();
	Motoj.setVelX(MotoVx);
	Motoj.setVelY(MotoVy);
	EnMoto.setVelX(EnMotoVx);
	EnMoto.setVelY(EnMotoVy);
	Mix_HaltMusic();
	Mix_PlayMusic(gameMusic, -1);
	return end;
}

void ESCMenu() //Gère le menu des options
{
	SDL_Event e;
	bool stop = false;
	bool buttonCovered[3];
	int x, y;

	while (!stop) //Boucle du menu
	{//On commence par afficher les textures du menu
		SDL_RenderClear(Rend);
		GameBG.render(0,0);
		int s = SCREEN_HEIGHT / 4;
		ESCMenutexture1.render((SCREEN_WIDTH - ESCMenutexture1.getWidth()) / 2, s);
		s += ESCMenutexture1.getHeight();
		ESCMenutexture2.render((SCREEN_WIDTH - ESCMenutexture2.getWidth()) / 2, s);
		s += ESCMenutexture2.getHeight();
		ESCMenutexture5.render((SCREEN_WIDTH - ESCMenutexture5.getWidth()) / 2, s);
		s += 2 * ESCMenutexture5.getHeight();
		ESCMenutexture3.render((SCREEN_WIDTH - ESCMenutexture3.getWidth()) / 2, s);
		s += ESCMenutexture3.getHeight();
		ESCMenutexture4.render((SCREEN_WIDTH - ESCMenutexture4.getWidth()) / 2, s);
		s += ESCMenutexture4.getHeight();
		ESCMenutexture6.render((SCREEN_WIDTH - ESCMenutexture6.getWidth()) / 2, s);
		SDL_RenderSetScale(Rend, ScalingX, ScalingY);
		SDL_RenderPresent(Rend);
		while (SDL_PollEvent(&e) && !stop) //Gestion des évènements qui redirige vers d'autres menus
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
				stop = true;
			}
			else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) //Permet de changer la taille de l'affichage lorsque la taille de la fenêtre est changée à la souris
			{
				ScalingX = e.window.data1 / ((float) SCREEN_WIDTH);
				ScalingY = e.window.data2 / ((float) SCREEN_HEIGHT);
			}
			else if (e.type == SDL_MOUSEMOTION) //On affiche que la souris pointe sur un boutton en changeant la couleur du texte
			{
				SDL_GetMouseState(&x, &y); //On récupère les coordonnées de la souris puis on regarde si elle est sur un des boutons et on le colore en rouge et remet les autres à leur couleur initiale
				if (x >= ESCMenutexture3.getPosX() && x <= ESCMenutexture3.getPosX() + ESCMenutexture3.getWidth() * ScalingX && y >= ESCMenutexture3.getPosY() && y <= ESCMenutexture3.getPosY() + ESCMenutexture3.getHeight() * ScalingY)
				{
					ESCMenutexture3.setColor(255, 0, 0);
					ESCMenutexture4.setColor(255, 255, 255);
					ESCMenutexture6.setColor(255, 255, 255);
					buttonCovered[0] = true;
					buttonCovered[1] = false;
					buttonCovered[2] = false;
				}
				else if (x >= ESCMenutexture4.getPosX() && x <= ESCMenutexture4.getPosX() + ESCMenutexture4.getWidth() * ScalingX && y >= ESCMenutexture4.getPosY() && y <= ESCMenutexture4.getPosY() + ESCMenutexture4.getHeight() * ScalingY)
				{
					ESCMenutexture4.setColor(255, 0, 0);
					ESCMenutexture3.setColor(255, 255, 255);
					ESCMenutexture6.setColor(255, 255, 255);
					buttonCovered[1] = true;
					buttonCovered[0] = false;
					buttonCovered[2] = false;
				}
				else if (x >= ESCMenutexture6.getPosX() && x <= ESCMenutexture6.getPosX() + ESCMenutexture6.getWidth() * ScalingX && y >= ESCMenutexture6.getPosY() && y <= ESCMenutexture6.getPosY() + ESCMenutexture6.getHeight() * ScalingY)
				{
					ESCMenutexture6.setColor(255, 0, 0);
					ESCMenutexture3.setColor(255, 255, 255);
					ESCMenutexture4.setColor(255, 255, 255);
					buttonCovered[2] = true;
					buttonCovered[1] = false;
					buttonCovered[0] = false;
				}
				else
				{
					ESCMenutexture6.setColor(255, 255, 255);
					ESCMenutexture3.setColor(255, 255, 255);
					ESCMenutexture4.setColor(255, 255, 255);
					buttonCovered[2] = false;
					buttonCovered[1] = false;
					buttonCovered[0] = false;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) //On prend en compte le clique sur le bouton actuellement sous la souris et agit en conséquence
			{
				ESCMenutexture3.setColor(255, 255, 255);
				if (buttonCovered[0])
				{
					if (windowed)
					{
						SDL_SetWindowFullscreen(Win, SDL_WINDOW_FULLSCREEN);
						SDL_SetWindowSize(Win, SCREEN_WIDTH * ScalingX, SCREEN_HEIGHT * ScalingY); //On multiplie par des variables de Scaling pour adapter à toutes tailles d'écrans et notamment si jamais le joueur veut jouer sur une petite fenêtre
					}
					else
					{
						SDL_SetWindowFullscreen(Win, 0);
						SDL_SetWindowSize(Win, SCREEN_WIDTH * ScalingX, SCREEN_HEIGHT * ScalingY);
					}
					windowed = 1 - windowed;
				}
				else if (buttonCovered[1])
				{
					ESCMenutexture4.setColor(255, 255, 255);
					colorChooser();
				}
				else if (buttonCovered[2])
				{
					ESCMenutexture6.setColor(255, 255, 255);
					stop = screenResizer(e);
				}
			}
			else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
			{
				if (e.key.keysym.sym == SDLK_c) //Envoi sur le selectionneur de couleur des motos
				{
					colorChooser();
				}
				else if (e.key.keysym.sym == SDLK_q)
				{
					stop = true;
				}
				else if (e.key.keysym.sym == SDLK_f) //Changement Fenêtré/Plein écran
				{
					if (windowed)
					{
						SDL_SetWindowFullscreen(Win, SDL_WINDOW_FULLSCREEN);
						SDL_SetWindowSize(Win, SCREEN_WIDTH * ScalingX, SCREEN_HEIGHT * ScalingY); //On multiplie par des variables de Scaling pour adapter à toutes tailles d'écrans et notamment si jamais le joueur veut jouer sur une petite fenêtre
					}
					else
					{
						SDL_SetWindowFullscreen(Win, 0);
						SDL_SetWindowSize(Win, SCREEN_WIDTH * ScalingX, SCREEN_HEIGHT * ScalingY);
					}
					windowed = 1 - windowed;
				}
				else if (e.key.keysym.sym == SDLK_s) //Choix de la taille de la fenêtre de jeu
				{
					stop = screenResizer(e);
				}
			}
		}
	}
}

bool screenResizer(SDL_Event e)
{
	bool stop = false;
	string input;
	float conv;
	LTexture output;
	LTexture error;
	if (!error.loadFromRenderedText("please enter an integer", {180, 180, 180, 255}, Font))
	{
		printf("Unable to load error texture in size changer\n");
	}
	else
	{
		bool renderText = false;
		bool modifying = false;
		bool modifX = false;
		bool end = false;
		while (!end && !quit) //Boucle du menu de choix de taille de la fenêtre
		{
			SDL_RenderClear(Rend);
			GameBG.render(0,0);
			ScreenResize1.render((SCREEN_WIDTH - ScreenResize1.getWidth()) / 2, SCREEN_HEIGHT / 4);
			SDL_RenderSetScale(Rend, ScalingX, ScalingY);
			SDL_RenderPresent(Rend);
			while (SDL_PollEvent(&e) && !modifying && !end)
			{
				if (e.type == SDL_QUIT)
				{
					stop = true;
					end = true;
					quit = true;
				}
				else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) //Permet de changer la taille de l'affichage lorsque la taille de la fenêtre est changée à la souris
				{
					ScalingX = e.window.data1 / ((float) SCREEN_WIDTH);
					ScalingY = e.window.data2 / ((float) SCREEN_HEIGHT);
				}
				else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
				{
					if (e.key.keysym.sym == SDLK_x)
					{
						modifX = true;
						modifying = true;
					}
					else if (e.key.keysym.sym == SDLK_y)
					{
						modifX = false;
						modifying = true;
					}
					else if (e.key.keysym.sym == SDLK_q)
					{
						end = true;
					}
				}
			}
			SDL_StartTextInput();//A partir d'ici on récupère le texte écrit par l'utilisateur pour modifier la taille en conséquence
			while (SDL_PollEvent(&e))
			{}
			input = "";
			while (modifying && !stop && !end)
			{
				while (SDL_PollEvent(&e) && !quit && !stop)
				{
					if (e.type == SDL_QUIT)
					{
						stop = true;
						quit = true;
					}
					else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						ScalingX = e.window.data1 / ((float) SCREEN_WIDTH);
						ScalingY = e.window.data2 / ((float) SCREEN_HEIGHT);
						renderText = true;
					}
					else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
					{
						if (e.key.keysym.sym == SDLK_q)
						{
							stop = true;
						}
						else if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER)
						{
							modifying = false;
							renderText = true;
						}
						else if (e.key.keysym.sym == SDLK_BACKSPACE && input.length() > 0) //On retire le dernier caractère en cas de Backspace
						{
							input.pop_back();
							renderText = true;
						}
					}
					else if (e.type == SDL_TEXTINPUT)
					{
						input += e.text.text;
						renderText = true;
					}
				}
				if (renderText && !stop) //On affiche le texte de l'utilisateur sauf si il a quitté
				{
					renderText = false;
					SDL_RenderClear(Rend);
					GameBG.render(0,0);
					if (input.length() > 0 && !output.loadFromRenderedText(input.c_str(), {180, 180, 180, 255}, Font)) //On charge dans la texture output le texte à afficher
					{
						printf("Failed to load output in network menu!\n");
					}
					else if (input.length() == 0 && !output.loadFromRenderedText(" ", {180, 180, 180, 255}, Font)) //Afficher une chaine de caractère vide provoque une erreur, dans le cas ou l'input est vide on afficher un esoace
					{
						printf("Failed to load output in network menu!\n");
					}//On affiche le tout
					SDL_RenderClear(Rend);
					GameBG.render(0,0);
					ScreenResize1.render((SCREEN_WIDTH - ScreenResize1.getWidth()) / 2, SCREEN_HEIGHT / 4);
					output.render((SCREEN_WIDTH - output.getWidth()) / 2, ScreenResize1.getHeight() + SCREEN_HEIGHT / 4);
					SDL_RenderSetScale(Rend, ScalingX, ScalingY);
					SDL_RenderPresent(Rend);
				}
			}
			SDL_StopTextInput();
			if (!modifying && input.length() > 0) //Si on ne modifie plus mais que l'input est non vide c'est qu'il a été validé par l'utilisateur, on midifie donc notre taille
			{
				conv = 0.f;
				int n = input.length();
				int i = 0;
				bool isOK = true;

				while (i < n && isOK)
				{
					if ((int) input[i] > (int) '9' || (int) input[i] < (int) '0') //On vérifie que le texte entré soit bien un entier
					{
						isOK = false;
					}
					else
					{
						conv *= 10;
						conv += (float) input[i] - (float)'0';
						i++;
					}
				}
				if (isOK)
				{
					if (modifX) //On met à jour le Scaling pour afficher la fenêtre comme il faut par la suite
					{
						ScalingX = conv / ((float) SCREEN_WIDTH);
					}
					else
					{
						ScalingY = conv / ((float) SCREEN_HEIGHT);
					}
					SDL_SetWindowSize(Win, SCREEN_WIDTH * ScalingX, SCREEN_HEIGHT * ScalingY);
				}
				else
				{
					error.render((SCREEN_WIDTH - error.getWidth()) / 2, 3 * SCREEN_HEIGHT / 4);
				}
			}
		}
	}
	return stop;
}

SDL_Color GetRGBColor(int x, int y) //Fonction auxiliaire de colorChooser pour récupérer la couleur d'un pixel
{//On commence par récupérer la surface de référence et le format des pixels
	SDL_Surface* surface = IMG_Load("./obj/Colorpicker.png");
	Uint32 pixelFormat = SDL_GetWindowPixelFormat(Win);
	int width = 0;

  Uint8* pixels = 0;
  SDL_Surface* tmpSurface = 0;
  SDL_Texture* texture = 0;
  int sizeInBytes = 0;
  void* tmpPixels = 0;
  int tmpPitch = 0;

  tmpSurface = SDL_ConvertSurfaceFormat(surface, pixelFormat, 0);
  if (tmpSurface)
	{//On copie la surface dans une texture que l'on peut modifier grace à SDL_TEXTUREACCESS_STREAMING
    texture = SDL_CreateTexture(Rend, pixelFormat, SDL_TEXTUREACCESS_STREAMING, tmpSurface->w, tmpSurface->h );
  }

  if (texture)
	{
    width = tmpSurface->w;
    sizeInBytes = tmpSurface->pitch * tmpSurface->h;
    pixels = (Uint8*)malloc(sizeInBytes);
    SDL_LockTexture(texture, 0, &tmpPixels, &tmpPitch); //On autorise l'accès aux pixels de la texture
    memcpy(pixels, tmpSurface->pixels, sizeInBytes);
    SDL_UnlockTexture(texture); //On le bloque
  }

  // On delete les éléments utilisés
  if (texture)
	{
    SDL_DestroyTexture(texture);
  }
  if (tmpSurface)
	{
    SDL_FreeSurface(tmpSurface);
  }
	//Les pixels ont pour format BGRA donc on récupère le bleu en premier puis le vert, le rouge, l'alpha
	int b = pixels[4 * (y * width + x) + 0]; // Bleu
	int g = pixels[4 * (y * width + x) + 1]; // Vert
	int r = pixels[4 * (y * width + x) + 2]; // Rouge
	int a = pixels[4 * (y * width + x) + 3]; // Alpha
	free(pixels);
	SDL_Color res;
	res.r = r;
	res.g = g;
	res.b = b;
	res.a = a;
  return res;
}

void RendercolorChooser()
{
	SDL_RenderClear(Rend);
	GameBG.render(0,0);
	colorChooser1.render((SCREEN_WIDTH - colorChooser1.getWidth()) / 2, SCREEN_HEIGHT / 4);
	colorChooser2.render((SCREEN_WIDTH - colorChooser2.getWidth()) / 2, (SCREEN_HEIGHT - 3 * colorChooser2.getHeight()) / 2);
	ColorPicker.render((SCREEN_WIDTH - ColorPicker.getWidth()) / 2, (SCREEN_HEIGHT - colorChooser2.getHeight()) / 2);
	colorChooser3.render((SCREEN_WIDTH - colorChooser3.getWidth()) / 2, ColorPicker.getHeight() + (SCREEN_HEIGHT - colorChooser2.getHeight()) / 2);
	EnemyMotoob.render((SCREEN_WIDTH - EnemyMotoob.getWidth()) / 2, colorChooser3.getHeight() + ColorPicker.getHeight() + (SCREEN_HEIGHT - colorChooser2.getHeight()) / 2);
	Motoob.render((SCREEN_WIDTH - 5 * Motoob.getWidth()) / 2, colorChooser3.getHeight() + ColorPicker.getHeight() + (SCREEN_HEIGHT - colorChooser2.getHeight()) / 2);
	SDL_RenderSetScale(Rend, ScalingX, ScalingY);
	SDL_RenderPresent(Rend);
}

void colorChooser()
{//On commence par afficher les textures du menu
	LTexture output;
	LTexture errormsg;
	if (!errormsg.loadFromRenderedText("please enter an int from 0 to 255", {180, 180, 180, 255}, Font))
	{
		printf("Failed to load error message in color chooser!\n");
	}
	else
	{//On initialise les variables nécessaires
		SDL_Event e;
		bool stop = false; //gère l'arrêt de la sélection
		bool modifEnemy = false; //détermine si la prochaine modification affecte l'ennemi
		bool modifPlayer = false; //détermine si la prochaine modification affecte le joueur
		//On affiche les previews
		while (!stop && !quit)
		{
			RendercolorChooser();
			while (SDL_PollEvent(&e) && !stop && !quit)//On gère les pigmentations à affecter et la moto à affecter via des inputs r/g/b/e/f/v
			{
				if (e.type == SDL_QUIT)
				{
					stop = true;
					quit = true;
				}
				else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) //Permet de changer la taille de l'affichage lorsque la taille de la fenêtre est changée à la souris
				{
					ScalingX = e.window.data1 / ((float) SCREEN_WIDTH);
					ScalingY = e.window.data2 / ((float) SCREEN_HEIGHT);
				}
				else if (e.type == SDL_MOUSEBUTTONDOWN) //Si on détecte un clique, on prend en comptre la couleur du pixel sur lequel on a cliqué et on change la couleur de l'objet sélecionné
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					if (x >= ColorPicker.getPosX() && x <= ColorPicker.getPosX() + ColorPicker.getWidth() * ScalingX && y >= ColorPicker.getPosY() && y <= ColorPicker.getPosY() + ColorPicker.getHeight() * ScalingY) //On vérifie que le clique a lieu sur le cercle chromatique (à noter: getPosX et getPosY renvoie les coordonnées déjà multipliées par ScalingX et ScalingY)
					{
						SDL_Color color = GetRGBColor((x - ColorPicker.getPosX()) / ScalingX, (y - ColorPicker.getPosY()) / ScalingY); //On va chercher la couleur du pixel sur lequel on a cliqué, il faut diviser par ScalingX et ScalingY pour récupérer les coordonnées du pixel si la texture était à sa taille normal car elle le sera dans la fonction
						if (modifEnemy)
						{
							EnemyMotoob.setColor(color.r, color.g, color.b);
							modifEnemy = false;
						}
						else if (modifPlayer)
						{
							Motoob.setColor(color.r, color.g, color.b);
							modifPlayer = false;
						}
					}
				}
				else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
				{
					if (e.key.keysym.sym == SDLK_q)
					{
						stop = true;
					}
					else if (e.key.keysym.sym == SDLK_e)
					{
						modifEnemy = true;
					}
					else if (e.key.keysym.sym == SDLK_p)
					{
						modifPlayer = true;
					}
				}
			}
		}
	}
}

void NetworkMenu()
{//On commence par déclarer les variables à utiliser et afficher le menu
	TCPsocket res;
	IPaddress ip;
	SDL_RenderClear(Rend);
	GameBG.render(0,0);
	int s = SCREEN_HEIGHT / 3;
	NetworkTexture1.render((SCREEN_WIDTH - NetworkTexture1.getWidth()) / 2, s);
	s += NetworkTexture1.getHeight();
	NetworkTexture2.render((SCREEN_WIDTH - NetworkTexture2.getWidth()) / 2, s);
	s += NetworkTexture2.getHeight();
	SDL_RenderSetScale(Rend, ScalingX, ScalingY);
	SDL_RenderPresent(Rend);
	bool stop = false;
	bool client = false;
	bool host = false;
	SDL_Event e;
	while (!stop) //boucle du menu
	{
		while (SDL_PollEvent(&e) && !stop)
		{
			if (e.type == SDL_QUIT)
			{
				stop = true;
				quit = true;
			}
			else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) //Permet de changer la taille de l'affichage lorsque la taille de la fenêtre est changée à la souris
			{
				ScalingX = e.window.data1 / ((float) SCREEN_WIDTH);
				ScalingY = e.window.data2 / ((float) SCREEN_HEIGHT);
				SDL_RenderClear(Rend);
				GameBG.render(0,0);
				int s = SCREEN_HEIGHT / 3;
				NetworkTexture1.render((SCREEN_WIDTH - NetworkTexture1.getWidth()) / 2, s);
				s += NetworkTexture1.getHeight();
				NetworkTexture2.render((SCREEN_WIDTH - NetworkTexture2.getWidth()) / 2, s);
				s += NetworkTexture2.getHeight();
				SDL_RenderSetScale(Rend, ScalingX, ScalingY);
				SDL_RenderPresent(Rend);
			}
			else if (e.type == SDL_KEYDOWN && e.key.repeat == 0) // Sélection de position Hôte/Client
			{
				if (e.key.keysym.sym == SDLK_q)
				{
					stop = true;
				}
				else if (e.key.keysym.sym == SDLK_c)
				{
					client = true;
					stop = true;
				}
				else if (e.key.keysym.sym == SDLK_h)
				{
					host = true;
					stop = true;
				}
			}
		}
	}
	if (client)
	{//On affiche les consignes spécifiques au client
		SDL_RenderClear(Rend);
		GameBG.render(0,0);
		NetworkTexture3.render((SCREEN_WIDTH - NetworkTexture3.getWidth()) / 2, SCREEN_HEIGHT / 3);
		NetworkTexture4.render((SCREEN_WIDTH - NetworkTexture4.getWidth()) / 2, NetworkTexture3.getHeight() + SCREEN_HEIGHT / 3);
		SDL_RenderSetScale(Rend, ScalingX, ScalingY);
		SDL_RenderPresent(Rend);

		string input;
		LTexture output;
		bool renderText = false;
		bool modifying = true;
		bool end = false;
		//On récupère l'adresse ip de l'hôte de la même façon qu'on récupère les inputs pour le choix de couleur/de taille (pas de fonction car il y a de légère différences)
		while (SDL_PollEvent(&e))
		{}
		bool ok = false;
		while (!ok && !end)
		{
			SDL_RenderClear(Rend);
			GameBG.render(0,0);
			NetworkTexture3.render((SCREEN_WIDTH - NetworkTexture3.getWidth()) / 2, SCREEN_HEIGHT / 3);
			NetworkTexture4.render((SCREEN_WIDTH - NetworkTexture4.getWidth()) / 2, NetworkTexture3.getHeight() + SCREEN_HEIGHT / 3);
			SDL_RenderSetScale(Rend, ScalingX, ScalingY);
			SDL_RenderPresent(Rend);

			SDL_StartTextInput();
			input = "";
			modifying = true;
			while (modifying && !end)
			{
				while (SDL_PollEvent(&e) && !end && modifying)
				{
					if (e.type == SDL_QUIT)
					{
						end = true;
						quit = true;
					}
					else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						ScalingX = e.window.data1 / ((float) SCREEN_WIDTH);
						ScalingY = e.window.data2 / ((float) SCREEN_HEIGHT);
						renderText = true;
					}
					else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
					{
						if (e.key.keysym.sym == SDLK_q)
						{
							end = true;
						}
						else if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER)
						{
							modifying = false;
							renderText = true;
						}
						else if (e.key.keysym.sym == SDLK_BACKSPACE && input.length() > 0)
						{
							input.pop_back();
							renderText = true;
						}
					}
					else if (e.type == SDL_TEXTINPUT)
					{
						input += e.text.text;
						renderText = true;
					}
				}
				if (renderText && !end) //On affiche le texte entré
				{
					renderText = false;
					SDL_RenderClear(Rend);
					GameBG.render(0,0);
					NetworkTexture3.render((SCREEN_WIDTH - NetworkTexture3.getWidth()) / 2, SCREEN_HEIGHT / 3);
					NetworkTexture4.render((SCREEN_WIDTH - NetworkTexture4.getWidth()) / 2, NetworkTexture3.getHeight() + SCREEN_HEIGHT / 3);

					if (input.length() > 0 && !output.loadFromRenderedText(input.c_str(), {180, 180, 180, 255}, Font))
					{
						printf("Failed to load output in network menu!\n");
					}
					else if (input.length() == 0 && !output.loadFromRenderedText(" ", {180, 180, 180, 255}, Font))
					{
						printf("Failed to load output in network menu!\n");
					}
					else
					{
						output.render((SCREEN_WIDTH - output.getWidth()) / 2, SCREEN_HEIGHT / 3 + NetworkTexture4.getWidth() + NetworkTexture3.getHeight());
					}
					SDL_RenderSetScale(Rend, ScalingX, ScalingY);
					SDL_RenderPresent(Rend);
				}
			}
			SDL_StopTextInput(); //Une fois l'adresse ip récupérée, on va créer un socket de communication
			if (!modifying)
			{
				if (SDLNet_ResolveHost(&ip, input.c_str(), 1234) == -1) //On gère les erreurs possibles lors de la traduction du texte en adresse ip
				{
					printf("Error while resolving host: %s\n", SDLNet_GetError());
					if (!output.loadFromRenderedText("error, please retry", {180, 180, 180, 255}, Font))
					{
						printf("Failed to load output after host resolving error!\n");
					}
					else
					{
						SDL_RenderClear(Rend);
						GameBG.render(0,0);
						NetworkTexture3.render((SCREEN_WIDTH - NetworkTexture3.getWidth()) / 2, SCREEN_HEIGHT / 3);
						output.render((SCREEN_WIDTH - output.getWidth()) / 2, SCREEN_HEIGHT / 3 + NetworkTexture3.getHeight());
						SDL_RenderSetScale(Rend, ScalingX, ScalingY);
						SDL_RenderPresent(Rend);
					}
				}
				else
				{//Maintenant, on créé la socket et on recoit une réponse de l'hôte pour s'assurer que la socket est fonctionnelle
					res = SDLNet_TCP_Open(&ip); //Création de la socket
					if (res == NULL)
					{
						printf("Unable to open TCP socket: %s\n", SDLNet_GetError());
						if (!output.loadFromRenderedText("wrong ip address", {180, 180, 180, 255}, Font))
						{
							printf("Failed to load 'Wrong IP Address' ouput\n");
						}
						else
						{
							SDL_RenderClear(Rend);
							GameBG.render(0,0);
							output.render((SCREEN_WIDTH - output.getWidth()) / 2, SCREEN_HEIGHT / 2);
						}
					}
					else
					{
						ok = true;
						isMultiplayer = true;
						isClient = true;
						isServer = false;
						char linking[25];
						if (SDLNet_TCP_Recv(res, linking, 25) <= 0) //On récupère un message
						{
							printf("Error in receiving connection established msg: %s\n", SDLNet_GetError());
						}
						SDL_RenderClear(Rend);
						GameBG.render(0,0);
						if (strcmp(linking, "Connection established") == 0)
						{
							NetworkTexture5.render((SCREEN_WIDTH - NetworkTexture5.getWidth()) / 2, SCREEN_HEIGHT / 2);
						}
						else
						{
							NetworkTexture6.render((SCREEN_WIDTH - NetworkTexture6.getWidth()) / 2, SCREEN_HEIGHT / 2);
						}
					}
					SDL_RenderSetScale(Rend, ScalingX, ScalingY);
					SDL_RenderPresent(Rend);
					SDL_Delay(5000);
				}
			}
		}
	}
	else if (host)
	{
		if (SDLNet_ResolveHost(&ip, NULL, 1234) == -1) //Mettre NULL met en mote server et accepte tous les messages
		{
			printf("Error in creating server: %s\n", SDLNet_GetError());
		}
		else
		{
			gen = SDLNet_TCP_Open(&ip); //gen est un socket générateur du socket de cummonication qui nous interesse
			if (gen == NULL)
			{
				printf("Error in opening generator socket: %s\n", SDLNet_GetError());
				LTexture Error;
				if (!Error.loadFromRenderedText("an error occured, returning to main menu", {180, 180, 180, 255}, Font))
				{
					printf("Unable to load error message in Network Menu\n");
				}
				else
				{
					SDL_RenderClear(Rend);
					GameBG.render(0,0);
					Error.render((SCREEN_WIDTH - Error.getWidth()) / 2, SCREEN_HEIGHT / 2);
					SDL_RenderSetScale(Rend, ScalingX, ScalingY);
					SDL_RenderPresent(Rend);
					SDL_Delay(5000);
				}
			}
			else
			{
				isMultiplayer = true;
				isServer = true;
				stop = false;
				SDL_RenderClear(Rend);
				GameBG.render(0,0);
				NetworkTexture7.render((SCREEN_WIDTH - NetworkTexture7.getWidth()) / 2, SCREEN_HEIGHT / 2);
				SDL_RenderSetScale(Rend, ScalingX, ScalingY);
				SDL_RenderPresent(Rend);

				while (!stop) //Attente de la requête du client
				{
					while (SDL_PollEvent(&e) && !stop)
					{
						if (e.type == SDL_QUIT)
						{
							stop = true;
							quit = true;
							isMultiplayer = false;
							isServer = false;
							isClient = false;
							SDLNet_TCP_Close(res);
							SDLNet_TCP_Close(gen);
						}
						else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
						{
							ScalingX = e.window.data1 / ((float) SCREEN_WIDTH);
							ScalingY = e.window.data2 / ((float) SCREEN_HEIGHT);
							SDL_RenderClear(Rend);
							GameBG.render(0,0);
							NetworkTexture7.render((SCREEN_WIDTH - NetworkTexture7.getWidth()) / 2, SCREEN_HEIGHT / 2);
							SDL_RenderSetScale(Rend, ScalingX, ScalingY);
							SDL_RenderPresent(Rend);
						}
						else if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_q)
						{
							stop = true;
							isMultiplayer = false;
							isServer = false;
							isClient = false;
							SDLNet_TCP_Close(res);
							SDLNet_TCP_Close(gen);
						}
					}
					if (!stop)
					{
						res = SDLNet_TCP_Accept(gen); //On accepte la requête
						if (res)
						{
							if (SDLNet_TCP_Send(res, "Connection established", strlen("Connection established")) < (int) strlen("Connection established")) //On envoie une réponse
							{
								printf("Error in sending connection established msg: %s\n", SDLNet_GetError());
							}//On prévient l'utilisateur que tout a bien fonctionné
							SDL_RenderClear(Rend);
							GameBG.render(0,0);
							NetworkTexture5.render((SCREEN_WIDTH - NetworkTexture5.getWidth()) / 2, SCREEN_HEIGHT / 2);
							SDL_RenderSetScale(Rend, ScalingX, ScalingY);
							SDL_RenderPresent(Rend);
							stop = true;
							SDL_Delay(5000);
						}
					}
				}
			}
		}
	}
	if (isMultiplayer)//Si la variable est à false c'est que l'utilisateur a quitté le menu, on n'a donc pas de mode multijoueur, sinon on récupère le socket de communication
	{
		communication = res;
	}
}

#endif
