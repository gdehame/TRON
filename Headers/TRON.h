#ifndef AHPP
#define AHPP //Header Guard
//Ce header sert à déclarer les différentes variables globales et les classes utilisé(e)s dans le code de l'application
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
using namespace std;

SDL_DisplayMode DM;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
float ScalingX = 1;
float ScalingY = 1;
const int MotoLen = 30;
int ScoreboardE[10];
int ScoreboardM[10];
int ScoreboardH[10];

const int Easy = 10;
const int Medium = 15;
const int Hard = 20;

class LTexture
{
	public:
		LTexture();
		~LTexture();
		bool loadFromFile(string path);
		#if defined(SDL_TTF_MAJOR_VERSION)
		bool loadFromRenderedText(string textureText, SDL_Color textColor, TTF_Font* f);
		#endif
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		void setBlendMode(SDL_BlendMode blending);
		void setAlpha(Uint8 alpha);
		void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		int getWidth();
		int getHeight();
		int getPosX();
		int getPosY();

	private:
		SDL_Texture* mTexture;
		int mPosX, mPosY;
		int mWidth;
		int mHeight;
};

class LTimer
{
  public:
		LTimer();
		void start();
		void stop();
		void pause();
		void unpause();
		Uint32 getTicks();
		bool isStarted();
		bool isPaused();

  private:
		Uint32 mStartTicks;
		Uint32 mPausedTicks;
		bool mPaused;
		bool mStarted;
};

class Moto
{
  public:
		static const int Moto_WIDTH = 10;
		static const int Moto_HEIGHT = 10;
		static const int Moto_VEL = 10;
		int Len;

		Moto();
		void handleEvent(SDL_Event& e);
		bool move(Moto m);
		void render();
		bool moving();
		void resetVel();
		int getVelX();
		int getVelY();
		void setVelX(int x);
		void setVelY(int x);
		void sendPos(TCPsocket s);
		void grow();
		int getX(int i);
		int getY(int i);
		void setX(int i, int e);
		void setY(int i, int e);
  protected:
		int * mPosX;
		int * mPosY;
		int mVelX, mVelY;
};

class Enemy : public Moto
{
	public:
		Enemy();
		void render();
		bool setDifficulty();
		int getDifficulty();
		bool receivePos(TCPsocket s, Moto player);
		bool moveEnemy(char* s, Moto player);
		bool AImove(Moto player);
	private:
		int mDifficulty; //règle la difficulté en ajustant la probabilité que le bot agisse intelligemment
		int mWait;
		int mchaseFruit;
};

void GameLoop();

void generateFruit(Moto player, Enemy enemy);

void FruitReset();

void renderFruits();

bool isFruit();

bool isOnFruit(int x, int y);

void destroyFruit(int x, int y);

bool ListMem(Moto m, int x, int y);

int SafeRadius(int x, int y, Moto m);

void AImoveAUX(int* x, int* y, int directionX, int directionY, int action, Moto player, Enemy bot);

bool init();

bool loadMedia();

void RenderMain();

void MainMenu(SDL_Event e, Moto* Motoj, Enemy* EnMoto, bool* end, bool* enemyDead);

void RenderPause(Moto Motoj, Enemy EnMoto);

bool PauseMenu(Moto Motoj, Enemy EnMoto, SDL_Event e);

void ESCMenu();

bool screenResizer(SDL_Event e);

SDL_Color GetRGBColor(int x, int y);

void RendercolorChooser();

void colorChooser();

void NetworkMenu();

void initializeSb();

int addtoSb(int x, int Sb[10]);

void updateSb();

void renderSb(int k, int Sb[10]);

void RenderGameOver(Moto Motoj, Enemy EnMoto);

void RenderWIN(Moto Motoj, Enemy EnMoto);

void close();

SDL_Window* Win = NULL;
SDL_Renderer* Rend = NULL;
int windowed;
TTF_Font* Font;

Mix_Music* gameMusic = NULL;
Mix_Music* menuMusic = NULL;
Mix_Chunk* gameWIN = NULL;
Mix_Chunk* gameLose = NULL;

int gameFruits[96][54];
LTexture Fruittexture;

bool quit = false;
LTexture SbTitletexture;

LTexture ESCMenutexture1;
LTexture ESCMenutexture2;
LTexture ESCMenutexture3;
LTexture ESCMenutexture4;
LTexture ESCMenutexture5;
LTexture ESCMenutexture6;
LTexture ScreenResize1;
LTexture ColorPicker;

LTexture NetworkTexture1;
LTexture NetworkTexture2;
LTexture NetworkTexture3;
LTexture NetworkTexture4;
LTexture NetworkTexture5;
LTexture NetworkTexture6;
LTexture NetworkTexture7;
bool isMultiplayer = false;
bool isServer = false;
bool isClient = false;
TCPsocket gen;
TCPsocket communication;

LTexture MainMenuBG;
LTexture MainMenuTexture;
LTexture MainMenuTexture2;
LTexture MainMenuTexture3;

LTexture colorChooser1;
LTexture colorChooser2;
LTexture colorChooser3;

LTexture PauseTexture1;
LTexture PauseTexture2;
LTexture PauseTexture3;
LTexture PauseTexture4;

LTexture DifficultyTexture;

LTexture GameBG;
LTexture Motoob;
LTexture EnemyMotoob;
LTexture GameOver;
LTexture WinTexture;
LTexture timerTexture;
LTimer gameTimer;
bool pause = true;

#endif
