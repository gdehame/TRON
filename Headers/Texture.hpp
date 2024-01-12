#ifndef TEXTURECPP
#define TEXTURECPP //Header Guard

#include "TRON.h"

LTexture::LTexture() //INitialisation de la texture (vude)
{
	mTexture = NULL;
	mPosX = 0;
	mPosY = 0;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture() //Libération de la mémoire
{
	if(mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mPosX = 0;
		mPosY = 0;
		mWidth = 0;
		mHeight = 0;
	}
}

bool LTexture::loadFromFile(string path) //Charge une texture à partir d'un fichier (bitmap)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL) //Gère les erreurs
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{//Créé la texture
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ));
    newTexture = SDL_CreateTextureFromSurface(Rend, loadedSurface);
		if(newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else //Modifie la taille de la texture en conséquence
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface( loadedSurface );
	}
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(string textureText, SDL_Color textColor, TTF_Font* f) //Charge une texture à partir d'un texte
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(f, textureText.c_str(), textColor);
	if( textSurface != NULL ) //Gère les erruers
	{
    mTexture = SDL_CreateTextureFromSurface(Rend, textSurface);
		if( mTexture == NULL )//Gère les erreurs
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else //Modifie la taille de la texture en conséquence
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	return mTexture != NULL;
}
#endif

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) //Règle la couleur de la texture
{
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) //Règle le Blending de la texture
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) //Règle l'opacité de la texture
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	mPosX = x * ScalingX;
	mPosY = y * ScalingY;
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};
	if(clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(Rend, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() //Renvoie la taille de la texture
{
	return mWidth;
}

int LTexture::getHeight() //Renvoie la hauteur de la texture
{
	return mHeight;
}

int LTexture::getPosX()
{
	return mPosX;
}

int LTexture::getPosY()
{
	return mPosY;
}

#endif
