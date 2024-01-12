#ifndef INIHPP
#define INIHPP

#include "TRON.h"

bool init()
{
	windowed = 0;
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (SDL_GetCurrentDisplayMode(0,&DM) != 0)
		{
			printf("SDL could not get Display Mode: %s\n", SDL_GetError());
			success=false;
		}
		ScalingX = DM.w / ((float) SCREEN_WIDTH);
		ScalingY = DM.h / ((float) SCREEN_HEIGHT);
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
		Win = SDL_CreateWindow("Tron", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * ScalingX, SCREEN_HEIGHT * ScalingY, SDL_WINDOW_SHOWN);
		if(Win == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			SDL_SetWindowResizable(Win, SDL_TRUE);
			SDL_SetWindowFullscreen(Win, SDL_WINDOW_FULLSCREEN);
			Rend = SDL_CreateRenderer(Win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(Rend == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(Rend, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if(!( IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				if(TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia() //On charge les différentes textures et affiche une erreur si le chargement n'a pas fonctionné
{
	bool success = true;
	if(!Motoob.loadFromFile("obj/Bsquare.bmp") || !EnemyMotoob.loadFromFile("obj/Bsquare.bmp"))
	{
		printf("Failed to load Moto texture!\n");
		success = false;
	}
	else
	{
		EnemyMotoob.setColor(255, 128, 128);
	}
	if (!GameBG.loadFromFile("obj/bg.bmp"))
	{
		printf("Unable to load game background!\n");
	}
	if (!MainMenuBG.loadFromFile("obj/tron.bmp"))
	{
		printf("Unable to load main menu background!\n");
		success = false;
	}
	Font = TTF_OpenFont("obj/Tron2.ttf", 36);
	if(Font == NULL)
	{
		printf("Unable to load Tron font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	if (!Fruittexture.loadFromFile("obj/fruit.png"))
	{
		printf("Unable to load fruit texture");
		success = false;
	}
	if (!ESCMenutexture3.loadFromRenderedText("f to change window mode", {180, 180, 180, 255}, Font) || !ESCMenutexture2.loadFromRenderedText("q to quit at anytime except while playing", {180, 180, 180, 255}, Font) ||
	!ESCMenutexture1.loadFromRenderedText("use arrows to move", {180, 180, 180, 255}, Font) || !ESCMenutexture4.loadFromRenderedText("c to go in color chooser", {180, 180, 180, 255}, Font) ||
	!ESCMenutexture5.loadFromRenderedText("esc to pause during game", {180, 180, 180, 255}, Font) || !ESCMenutexture6.loadFromRenderedText("s to resize window", {180, 180, 180, 255}, Font))
	{
		printf("Unable to load Escape Menu texture!\n");
		success = false;
	}
	if (!ScreenResize1.loadFromRenderedText("x/y to change width/height or use mouse anytime out of game", {180, 180, 180, 255}, Font))
	{
		printf("Unable to load Screen resizer texture!\n");
		success = false;
	}
	if (!NetworkTexture1.loadFromRenderedText("c to be client", {180, 180, 180, 255}, Font) || !NetworkTexture2.loadFromRenderedText("h to be host", {180, 180, 180, 255}, Font) ||
	!NetworkTexture3.loadFromRenderedText("enter your friend's ip address", {180, 180, 180, 255}, Font) || !NetworkTexture4.loadFromRenderedText("ip address", {180, 180, 180, 255}, Font) ||
	!NetworkTexture5.loadFromRenderedText("connection established", {180, 180, 180, 255}, Font) || !NetworkTexture6.loadFromRenderedText("connection not established, return to Main Menu (q)", {180, 180, 180, 255}, Font) ||
	!NetworkTexture7.loadFromRenderedText("trying to establish connection (q to stop)", {180, 180, 180, 255}, Font))
	{
		printf("Unable to load Network Menu texture!\n");
		success = false;
	}
	if (!colorChooser2.loadFromRenderedText("e/p to modify enemy/player then click on the color picker", {180, 180, 180, 255}, Font) || !colorChooser3.loadFromRenderedText("preview (you left, enemy right)", {180, 180, 180, 255}, Font) ||
	!colorChooser1.loadFromRenderedText("color chooser", {180, 180, 180, 255}, Font))
	{
		printf("Unable to load color chooser texture!\n");
		success = false;
	}
	if (!MainMenuTexture.loadFromRenderedText("space to start", {180, 180, 180, 255}, Font) || !MainMenuTexture2.loadFromRenderedText("esc for inputs and options", {180, 180, 180, 255}, Font) ||
	!MainMenuTexture3.loadFromRenderedText("m for lan multiplayer", {180, 180, 180, 255}, Font))
	{
		printf("Unable to load Main Menu texture!\n");
		success = false;
	}
	if (!PauseTexture1.loadFromRenderedText("pause", {0, 0, 0, 255}, Font) || !PauseTexture2.loadFromRenderedText("m to return to main menu", {180, 180, 180, 255}, Font) || !PauseTexture3.loadFromRenderedText("esc to resume", {180, 180, 180, 255}, Font) ||
	!PauseTexture4.loadFromRenderedText("q to quit", {180, 180, 180, 255}, Font))
	{
		printf("Unable to load Pause Menu texture!\n");
		success = false;
	}
	if (!GameOver.loadFromRenderedText("game over!", {0, 0, 0, 255}, Font))
	{
			printf("Unable to load GameOver screen texture!\n");
			success = false;
	}
	if (!WinTexture.loadFromRenderedText("you win!", {0, 0, 0, 255}, Font))
	{
			printf("Unable to load Win screen texture!\n");
			success = false;
	}
	if (!SbTitletexture.loadFromRenderedText("scoreboard", {0, 0, 0, 255}, Font))
	{
		printf("Unable to load Scoreboard title texture!\n");
		success = false;
	}
	if (!DifficultyTexture.loadFromRenderedText("choose your difficulty, e/m/h for easy/medium/hard", {180, 180, 180, 255}, Font))
	{
		printf("Unable to load Difficulty texture!\n");
		success = false;
	}
	gameMusic = Mix_LoadMUS( "obj/gameMusic.wav" );
	if(gameMusic == NULL)
	{
		printf( "Failed to load game music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	menuMusic = Mix_LoadMUS( "obj/menuMusic.wav" );
	if(menuMusic == NULL)
	{
		printf( "Failed to load menu music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gameWIN = Mix_LoadWAV("obj/GW.wav");
	if (gameWIN == NULL)
	{
		printf("Failed to load game won sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	gameLose = Mix_LoadWAV("obj/GO.wav");
	if (gameLose == NULL)
	{
		printf("Failed to load game over sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	if (!ColorPicker.loadFromFile("./obj/Colorpicker.png"))
	{
		printf("Failed to load color picker png\n");
	}
	initializeSb();
	return success;
}

#endif
