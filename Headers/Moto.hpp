#ifndef MOTOCPP
#define MOTOCPP

#include "TRON.h"

bool ListMem(Moto m, int x, int y) //Elle rechercher si x est dans lX sauf a la derniere pos et pareil pour y, on ne regarde pas la derniere pour pouvoir l'utiliser pour vérifier que la tete ne rentre pas dans le corps
{
	bool res = false;
	int i = 0;
	while (!res && i < m.Len - 1)
	{
		if (x == m.getX(i) && y == m.getY(i))
		{
			res = true;
		}
		i++;
	}
	return res;
}

int SafeRadius(int x, int y, Moto m) //(fonction auxiliiare de AImove)Cette fonction sert à déterminer la plus grosse boule centrée sur (x,y)  qui ne contient aucune source de danger (moto ennemie ou mur)
{
	int tab[4 + m.Len];
	tab[m.Len] = x;
	tab[m.Len+1] = SCREEN_WIDTH - x;
	tab[m.Len+2] = y;
	tab[m.Len+3] = SCREEN_HEIGHT - y;
	for (int i = 0; i < m.Len; i++) //Ajout des distances avec les textures de la moto ennemie
	{
		tab[i] = sqrt((x - m.getX(i))*(x - m.getX(i)) + (y - m.getY(i))*(y - m.getY(i)));
	}
	int res = tab[0];
	for (int i = 1; i < m.Len + 4; i++) //Recherche du minimum
	{
		if (tab[i] < res)
		{
			res = tab[i];
		}
	}
	return res;
}

void AImoveAUX(int* x, int* y, int directionX, int directionY, int action, Moto player, Enemy bot) //(Deuxieme fonction auxiliiare de AImove)Détermine les directions possibles suivant si le bot agit aléatoirement ou intelligemment de facon défensive ou offensive mais sans se suicider
{
  bool isOK = true; // on évite de faire que le choix aléatoire cause un suicide du bot et donc il ne prend pas de direction qui à cause de l'attente le face se suicider
  for (int i = 1; i < 6; i++) //Prise en compte de son propre corps droit devant soit si jamais on choisit cette direction: maximum la taille du corps car on ne peut pas avoir un morceau du corps a plus grande distance que la taille du corps
  {
    isOK = isOK && !ListMem(bot, bot.getX(bot.Len-1) + (i + 1) * directionX, bot.getY(bot.Len-1) + (i + 1) * directionY) && bot.getX(bot.Len-1) + (i + 1) * directionX < SCREEN_WIDTH && bot.getY(bot.Len-1) + (i + 1) * directionY < SCREEN_HEIGHT
           && !ListMem(player, bot.getX(bot.Len-1) + (i + 1) * directionX, bot.getY(bot.Len-1) + (i + 1) * directionY) && bot.getX(bot.Len-1) + (i + 1) * directionX > 0 && bot.getY(bot.Len-1) + (i + 1) * directionY > 0; // si la case est dans le corps et à distance plus faible que le rayon de sécurité alors ca devient le nouveau danger le plus proche
  }
  if (isOK)
  {
    if (action == 1) //Si on fait une action aléatoire, on compte simplement le nombre de positions accessibles sans se suicider pour pouvoir choisir aléatoirement parmis celles-ci
    {
      (*x)++;
      *y = 0;
    }
    else if (action == 2) //Si on agit défensivement, on considère la direction comme valide si et seulement si elle ne fait pas se suicider le bot et on prend celle qui maximise la distance de sécurité du bot
    {
      *y = SafeRadius(bot.getX(bot.Len-1) + directionX, bot.getY(bot.Len-1) + directionY, player);
      for (int i = 1; i < bot.Len; i++) //Prise en compte de son propre corps droit devant soit si jamais on choisit cette direction: maximum la taille du corps car on ne peut pas avoir un morceau du corps a plus grande distance que la taille du corps
      {
        if (ListMem(bot, bot.getX(bot.Len-1) + (i + 1) * directionX, bot.getY(bot.Len-1) + (i + 1) * directionY) && *y > i * (abs(directionY) + abs(directionX))) // si la case est dans le corps et à distance plus faible que le rayon de sécurité alors ca devient le nouveau danger le plus proche
        {
          *y = i * bot.Moto_WIDTH;
        }
      }
    }
    else if (action == 3) //Si on agit offensivement, on calcul le rayon de sécurité du joueur pour prendre la direction qui le minimise et donc on le calcul en remplacant dans la liste des textures du bot la derniere texture par la potentielle suivante puis on remet l'ancienne car le choix de direction n'est pas encore fait
    {
      int tx = bot.getX(0);
      int ty = bot.getY(0);
      bot.setX(0, bot.getX(bot.Len-1) + directionX);
      bot.setY(0, bot.getY(bot.Len-1) + directionY);
      *y = SafeRadius(player.getX(player.Len-1), player.getY(player.Len-1), bot);
      bot.setX(0, tx);
      bot.setY(0, ty);
    }
		else if (action == 4) //Si on cherche à s'approcher du fruit le plus proche, on calcul la distance au fruit selon la direction potentielle
		{
			*y = abs(bot.getX(bot.Len-1) + 5 * directionX - x[0]) + abs(bot.getY(bot.Len-1) + 5 * directionY - x[1]);
		}
  }
}

Moto::Moto()
{ //On initie la moto (du joueur) en la mettant au milieu en hauteur et tout à droite de l'écran
	Len = MotoLen;
	mPosX = (int*) malloc (Len * sizeof(int));
	mPosY = (int*) malloc (Len * sizeof(int));
	for (int i = 0; i < Len; i++)
	{
		mPosX[i] = SCREEN_WIDTH - (i + 1) * this->Moto_WIDTH;
		mPosY[i] = SCREEN_HEIGHT / 2;
	}
  mVelX = 0;
  mVelY = 0;
}

void Moto::handleEvent(SDL_Event& e)
{//On regarde si le joueur a appuyé sur une flèche pour diriger le bot
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
  {
    switch(e.key.keysym.sym)
    {//On fait attention à emp$echer le joueur de s'arrêter ou de se suicider en appuyant sur une direction qui l'emmene sur la texture qui suit la tête (grace à la vérification dans la boucle conditionnelle)
      case SDLK_UP: if (mVelY == 0) {mVelY = -Moto_VEL; mVelX = 0;} break;
      case SDLK_DOWN: if (mVelY == 0) {mVelY = Moto_VEL; mVelX = 0;} break;
      case SDLK_LEFT: if (mVelX == 0) {mVelX = -Moto_VEL; mVelY = 0;} break;
      case SDLK_RIGHT: if (mVelX == 0 && moving()) {mVelX = Moto_VEL; mVelY = 0;} break;
    }
  }
}

bool Moto::move(Moto EnMoto)
{
	bool res = false;
	if (isOnFruit(mPosX[Len-1] + mVelX, mPosY[Len-1] + mVelY))
	{
		destroyFruit(mPosX[Len-1] + mVelX, mPosY[Len-1] + mVelY);
		grow();
		mPosX[Len-1] = mPosX[Len-2] + mVelX;
		mPosY[Len-1] = mPosY[Len-2] + mVelY;
		if (ListMem(EnMoto, mPosX[Len-1], mPosY[Len-1]) || (EnMoto.getX(Len-1) == mPosX[Len-1] && EnMoto.getY(Len-1) == mPosY[Len-1])) //Si on arrive sur un fruit on n'a pas besoin de vérifier qu'on sort de l'écran car ils n'y sont pas ni qu'on se mort la queue car sinon il n'y aurait pas de fruit
		{
			res = true;
		}
	}
	else
	{
		int xHead = mPosX[Len-1];
		int yHead = mPosY[Len-1];
		mPosX[Len-1] += mVelX;
		mPosY[Len-1] += mVelY;
		for (int i = 0 ; i < Len - 2; i++)
		{
			mPosX[i] = mPosX[i+1];
			mPosY[i] = mPosY[i+1];
		}
		if (ListMem(*this, mPosX[Len-1], mPosY[Len-1]) || ListMem(EnMoto, mPosX[Len-1], mPosY[Len-1]) || (EnMoto.getX(Len-1) == mPosX[Len-1] && EnMoto.getY(Len-1) == mPosY[Len-1])
				|| mPosX[Len-1] < 0 || mPosX[Len-1] + Moto_WIDTH > SCREEN_WIDTH || mPosY[Len-1] < 0 || mPosY[Len-1] + Moto_HEIGHT > SCREEN_HEIGHT)
		{
			res = true;
		}
		mPosX[Len-2] = xHead;
		mPosY[Len-2] = yHead;
	}
	return res;
}

void Moto::render() //On parcourt la liste des positions des textures de la moto pour les afficher
{
	for (int i = 0; i < Len; i++)
	{
		Motoob.render(mPosX[i], mPosY[i]);
	}
}

bool Moto::moving() //Renvoie un booléen spécifiant si la moto se déplace ou non
{
	return mVelX!=0 || mVelY!=0;
}

void Moto::resetVel() //Arrête la moto
{
	mVelX = 0;
	mVelY = 0;
}

int Moto::getVelX()
{
	return mVelX;
}

int Moto::getVelY()
{
	return mVelY;
}

void Moto::setVelX(int x)
{
	mVelX = x;
}

void Moto::setVelY(int x)
{
	mVelY = x;
}

void Moto::sendPos(TCPsocket s) //Envoie une chaine de caractère codant la position de la moto comme-suit: taille|X1#Y1%X2#Y2...|nbFruits|Xf1#Yf1%Xf2#Yf2...
{
	int nbFruits = 0;
	for (int i = 0; i < 96; i++)
	{
		for (int j = 0; j < 54; j++)
		{
			if (gameFruits[i][j] == 1)
			{
				nbFruits++;
			}
		}
	}
	string msg = to_string(Len) + "|" + to_string(nbFruits) + "|";
	for (int i=0; i < Len; i++) //code la position
	{
		msg += to_string(mPosX[i]);
		msg += "#";
		msg += to_string(mPosY[i]);
		msg += "%";
	}
	if (nbFruits > 0) //Si il y a des fruits, on envoie également la position des fruits
	{
		msg += "|";
		for (int i = 0; i < 96; i++)
		{
			for (int j = 0; j < 54; j++)
			{
				if (gameFruits[i][j] == 1)
				{
					msg += to_string(i);
					msg += "#";
					msg += to_string(j);
					msg += "%";
				}
			}
		}
	}
	int l = msg.length();
	char message[l];
	for (int i=0; i < l; i++) //le traduit en char * pour pouvoir être envoyé via SDL_net
	{
		message[i] = msg[i];
	}
	if (SDLNet_TCP_Send(s, message, l) < l)
	{
		printf("Error in sending pos: %s\n", SDLNet_GetError());
	}
}

void Moto::grow()
{
	Len +=1;
	int * x = (int*) malloc (Len * sizeof(int));
	int * y = (int*) malloc (Len * sizeof(int));
	for (int i = 0; i < Len-1; i++)
	{
		x[i] = mPosX[i];
		y[i] = mPosY[i];
	}
	x[Len-1] = 0;
	y[Len-1] = 0;
	free(mPosX);
	free(mPosY);
	mPosX = x;
	mPosY = y;
}

int Moto::getX(int i)
{
	return mPosX[i];
}

int Moto::getY(int i)
{
	return mPosY[i];
}

void Moto::setX(int i, int e)
{
	mPosX[i] = e;
}

void Moto::setY(int i, int e)
{
	mPosY[i] = e;
}

Enemy::Enemy() //Initie l'ennemie au milieu de l'écran en hauteur et à gauche de l'écran
{
	mWait = 2;
	mchaseFruit = 0;
	mDifficulty = 20;
	for (int i=0; i < Len; i++)
	{
		mPosX[i] = i * this->Moto_WIDTH;
		mPosY[i] = SCREEN_HEIGHT / 2;
	}
}

void Enemy::render() //Affiche la moto ennemie (on utilise une méthode différente de Moto::render() car on n'utilise pas la même texture afin que les deux aient des couleurs différentes et on a choisit d'utiliser deux variabels globales pour simplifier le reste plutôt que de mettre en paramètre la texture)
{
	for (int i=0;i<Len;i++)
	{
		EnemyMotoob.render(mPosX[i], mPosY[i]);
	}
}

bool Enemy::setDifficulty() //Règle la difficulté de la partie en cas de jeu contre un bot en ajustant la probabilité que le bot fasse un déplacement aléatoire
{
	bool stop = false;
	SDL_Event e;
	while (!quit && !stop)
	{//On affiche l'écran de choix de difficulté
		SDL_RenderClear(Rend);
		GameBG.render(0,0);
		DifficultyTexture.render((SCREEN_WIDTH - DifficultyTexture.getWidth()) / 2, SCREEN_HEIGHT / 4);
		SDL_RenderSetScale(Rend, ScalingX, ScalingY);
		SDL_RenderPresent(Rend);
		while (!quit && !stop && SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
				stop = true;
				return false;
			}
			else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) //Permet de changer la taille de l'affichage lorsque la taille de la fenêtre est changée à la souris
			{
				ScalingX = e.window.data1 / ((float) SCREEN_WIDTH);
				ScalingY = e.window.data2 / ((float) SCREEN_HEIGHT);
			}
			else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
			{
				if (e.key.keysym.sym == SDLK_q)
				{
					return false;
				}
				if (e.key.keysym.sym == SDLK_e)
				{
					stop = true;
					mDifficulty = Easy;
				}
				else if (e.key.keysym.sym == SDLK_m)
				{
					stop = true;
					mDifficulty = Medium;
				}
				else if (e.key.keysym.sym == SDLK_h)
				{
					stop = true;
					mDifficulty = Hard;
				}
			}
		}
	}
	return true;
}

int Enemy::getDifficulty()
{
	return mDifficulty;
}

bool Enemy::receivePos(TCPsocket s, Moto player) //Recoit la position du joueur ennemi puis appel moveEnemy qui s'occupe de traduire le message et de modifier les positions de l'ennemi
{
	int l = (4*2+2)*(Len+1)+3+1+6*96*54;
	char msg[l];
	if (SDLNet_TCP_Recv(s, msg, l) <= 0)
	{
		printf("Error in receiving message: %s\n", SDLNet_GetError());
		return true;
	}
	return moveEnemy(msg, player);
}

bool Enemy::moveEnemy(char * s, Moto player)
{
	for (int i = 0; i < 96; i++) //On fait un pseudo-reset pour récupérer les fruits présents pour l'autre joueur à l'exception de celui que vient d'écraser la moto du joueur local
	{
		for (int j = 0; j < 54; j++)
		{
			if (gameFruits[i][j] != -1)
			{
				gameFruits[i][j] = 0;
			}
		}
	}
	bool changed = false;
	int x = 0;
	int y = 0;
	int i = 0;
	int j = 0;
	int l = strlen(s);
	int newLen = 0;
	while (i < l && s[i] != '|') //On récupère la taille de l'ennemi, si jamais il est passé sur un fruit
	{
		newLen *= 10;
		newLen += (int) s[i] - (int) '0';
		i++;
	}
	i++;
	int nbFruits = 0;
	while (i < l && s[i] != '|')
	{
		if ((int) s[i] <= 57 && (int) s[i] >= 48)
		{
			nbFruits *= 10;
			nbFruits += (int) s[i] - (int) '0'; //Traduit via le code ASCII, il faut donc retirer le code de '0'
		}
		i++;
	}
	i++;
	if (newLen != Len)
	{
		changed = true;
		grow();
	}
	while (i < l && j < Len && s[i] != '|') //Traduit le message
	{
		x = 0;
		y = 0;
		while (s[i] != '#') //Tant qu'on ne croise pas de '#' on récupère la prochaine position en abscisse
		{
			if ((int) s[i] <= 57 && (int) s[i] >= 48)
			{
				x *= 10;
				x += (int) s[i] - (int) '0'; //Traduit via le code ASCII, il faut donc retirer le code de '0'
			}
			i++;
		}
		i++;
		mPosX[j] = SCREEN_WIDTH - x - Moto_WIDTH;
		while (s[i] != '%') //Pareil avec la position en ordonnée tant qu'on ne croise pas le prochain '%'
		{
			if ((int) s[i] <= 57 && (int) s[i] >= 48)
			{
				y *= 10;
				y += (int) s[i] - (int) '0'; //Traduit via le code ASCII, il faut donc retirer le code de '0'
			}
			i++;
		}
		mPosY[j] = SCREEN_HEIGHT - y;
		j++;
		i++;
	}
	if (i < l && s[i] == '|') //Si on croise un '|' c'est qu'on est client en multijoueur et on doit récupérer la position des fruits
	{
		i++;
		while (i < l && nbFruits > 0)
		{
			int x = 0;
			while (s[i] != '#')
			{
				if ((int) s[i] <= 57 && (int) s[i] >= 48)
				{
					x *= 10;
					x += (int) s[i] - (int) '0'; //Traduit via le code ASCII, il faut donc retirer le code de '0'
				}
				i++;
			}
			int y = 0;
			i++;
			while (s[i] != '%')
			{
				if ((int) s[i] <= 57 && (int) s[i] >= 48)
				{
					y *= 10;
					y += (int) s[i] - (int) '0'; //Traduit via le code ASCII, il faut donc retirer le code de '0'
				}
				i++;
			}
			if (gameFruits[96-x][54-y] == -1)
			{
				gameFruits[96-x][54-y] = 0;
			}
			else if (gameFruits[96-x][54-y] == 0)
			{
				gameFruits[96-x][54-y] = 1;
			}
			nbFruits--;
			i++;
		}
	}
	bool res = false;
	if (j != Len) //Si on n'a pas récupérer toutes les positions alors le message a mal été reçu, on décide donc de faire gagner le joueur qui a mal reçu au cas où l'adversaire aurait abandonné
	{
		printf("Error in Enemy Position code!\n");
		res = true;
	}
	i = 0;
	while (i < Len && !res)
	{ //On regarde si l'adversaire est mort d'un mur
		if (mPosX[i] < 0 || (mPosX[i] + Moto_WIDTH > SCREEN_WIDTH) || mPosY[i] < 0 || (mPosY[i] + Moto_HEIGHT > SCREEN_HEIGHT))
		{
			res = true;
		}
		i++;
	}
	if (ListMem(*this, mPosX[Len-1], mPosY[Len-1]) || ListMem(player, mPosX[Len-1], mPosY[Len-1]) || (player.getX(Len-1) == mPosX[Len-1] && player.getY(Len-1) == mPosY[Len-1]))
	{//On regarde si l'adversaire est mort dans le joueur
		res = true;
	}
	if (changed)
	{
		destroyFruit(mPosX[Len-1], mPosY[Len-1]);
	}
	return res;
}

bool Enemy::AImove(Moto player) //Gère les déplacements du bot
{
	bool res = false;
	int xHead = mPosX[Len-1];
	int yHead = mPosY[Len-1];
	int xTail = mPosX[0];
	int yTail = mPosY[0];

	int a = -1;
	int b = -1;
	int c = -1;
	int d = -1;
	int dir = -1;

	if (mWait != 0) //la variable wait empêche l'IA d'agir trop vite
	{
		mWait -= 1;
		mPosX[Len-1] += mVelX;
		mPosY[Len-1] += mVelY;
	}
	else
	{
		mWait = 5;
		if (xHead == mPosX[Len-2]) //On détermine la direction qui ne sera jamais possible
		{
			if (yHead > mPosY[Len-2])
			{
				d = -2;
			}
			else
			{
				c = -2;
			}
		}
		else
		{
			if (xHead > mPosX[Len-2])
			{
				b = -2;
			}
			else
			{
				a = -2;
			}
		}
		if (rand() % mDifficulty == 0) //On met une certaine probabilité que le bot joue aléatoirement pour éviter qu'il ne soit trop fort, auquel cas il choisit aléatoirement une direction qui ne le tue pas (sinon il est trop mauvais) s'il en existe une
		{
			int n = 0;
			if (a != -2 && !ListMem(*this, xHead + Moto_WIDTH, yHead) && xHead + Moto_WIDTH < SCREEN_WIDTH)
			{
				AImoveAUX(&n, &a, Moto_WIDTH, 0, 1, player, *this);
			}
			if (b != -2 && !ListMem(*this, xHead - Moto_WIDTH, yHead) && xHead - Moto_WIDTH > 0)
			{
				AImoveAUX(&n, &b, -Moto_WIDTH, 0, 1, player, *this);
			}
			if (c != -2 && !ListMem(*this, xHead, yHead + Moto_HEIGHT) && yHead + Moto_HEIGHT < SCREEN_HEIGHT)
			{
				AImoveAUX(&n, &c, 0, Moto_HEIGHT, 1, player, *this);
			}
			if (d != -2 && !ListMem(*this, xHead, yHead - Moto_HEIGHT) && yHead - Moto_HEIGHT > 0)
			{
				AImoveAUX(&n, &d, 0, -Moto_HEIGHT, 1, player, *this);
			}
			if (n == 0) //direction arbitraire car le bot est condamné à mourrir peu importe la direction
			{
				dir = 0;
			}
			else
			{
				int tab[4] = {a, b, c, d};
				int r = rand() % n;
				dir = 0;
				while (dir < 4 && r >= 0) //Permet de choisir la r-ième direction qui ne fait pas se suicider le bot
				{
					if (tab[dir] >= 0)
					{
						r--;
					}
					dir++;
				}
				dir--;
			}
		}
		else
		{
			int playerSafeDist = SafeRadius(player.getX(Len-1), player.getY(Len-1), *this);
			int AISafeDist = SafeRadius(xHead, yHead, player);
			int bin = 0; //variable inutile pour compler dans les appels à AImoveAUX

			int min = SCREEN_WIDTH + SCREEN_HEIGHT;
			int mini = 0;
			int minj = 0;
			if (isFruit())
			{
				for (int i = 0; i < 96; i++) // On recherche le fruit le plus proche
				{
					for (int j = 0; j < 54; j++)
					{
						int dist = abs(xHead - i * 20) + abs(yHead - j * 20);
						if (gameFruits[i][j] == 1 && min > dist)
						{
							min = dist;
							mini = i * 20;
							minj = j * 20;
						}
					}
				}
			}

			if (min > 60 && AISafeDist < playerSafeDist) //Si le bot est plus en danger que le joueur, il joue défensivement
			{
				if (a != -2 && !ListMem(*this, xHead + Moto_WIDTH, yHead) && xHead + Moto_WIDTH < SCREEN_WIDTH)
				{
					AImoveAUX(&bin, &a, Moto_WIDTH, 0, 2, player, *this);
				}
				if (b != -2 && !ListMem(*this, xHead - Moto_WIDTH, yHead) && xHead - Moto_WIDTH > 0)
				{
					AImoveAUX(&bin, &b, -Moto_WIDTH, 0, 2, player, *this);
				}
				if (c != -2 && !ListMem(*this, xHead, yHead + Moto_HEIGHT) && yHead + Moto_HEIGHT < SCREEN_HEIGHT)
				{
					AImoveAUX(&bin, &c, 0, Moto_HEIGHT, 2, player, *this);
				}
				if (d != -2 && !ListMem(*this, xHead, yHead - Moto_HEIGHT) && yHead - Moto_HEIGHT > 0)
				{
					AImoveAUX(&bin, &d, 0, -Moto_HEIGHT, 2, player, *this);
				}
				int tab[4] = {a, b, c, d};
				int max = 0;
				for (int i = 0; i < 4; i++) //On choisit donc la direction qui maximise le rayon de sécurité
				{
					if (tab[i] >= 0 && max < tab[i])
					{
						max = tab[i];
						dir = i;
					}
				}
			}
			else //Ici, le bot étant plus en sécurité que le joueur, il va jouer offensivement
			{
				if (min <= 60 || mchaseFruit != 0 || (isFruit() && rand() % ((30 - mDifficulty) * 3 / 5))) // Une certaine probabilité de chercher à se rapprocher d'un fruit s'il y en a un, qui diminue lorsque la difficulté augmente (facile: 12, moyen: 9, difficile: 6)
				{
					if (mchaseFruit != 0)
					{
						mchaseFruit--;
					}
					else
					{
						mchaseFruit = 10;
					}
					if (min <= 60)
					{
						mWait = 1;
					}
					int * mincoord = (int *) malloc (sizeof(int) * 2);
					mincoord[0] = mini;
					mincoord[1] = minj;
					if (a != -2 && !ListMem(*this, xHead + Moto_WIDTH, yHead) && xHead + Moto_WIDTH < SCREEN_WIDTH)
					{
						AImoveAUX(mincoord, &a, Moto_WIDTH, 0, 4, player, *this);
					}
					if (b != -2 && !ListMem(*this, xHead - Moto_WIDTH, yHead) && xHead - Moto_WIDTH > 0)
					{
						AImoveAUX(mincoord, &b, -Moto_WIDTH, 0, 4, player, *this);
					}
					if (c != -2 && !ListMem(*this, xHead, yHead + Moto_HEIGHT) && yHead + Moto_HEIGHT < SCREEN_HEIGHT)
					{
						AImoveAUX(mincoord, &c, 0, Moto_HEIGHT, 4, player, *this);
					}
					if (d != -2 && !ListMem(*this, xHead, yHead - Moto_HEIGHT) && yHead - Moto_HEIGHT > 0)
					{
						AImoveAUX(mincoord, &d, 0, -Moto_HEIGHT, 4, player, *this);
					}
					free(mincoord);
				}
				else
				{
					if (a != -2 && !ListMem(*this, xHead + Moto_WIDTH, yHead) && xHead + Moto_WIDTH < SCREEN_WIDTH)
					{
						AImoveAUX(&bin, &a, Moto_WIDTH, 0, 3, player, *this);
					}
					if (b != -2 && !ListMem(*this, xHead - Moto_WIDTH, yHead) && xHead - Moto_WIDTH > 0)
					{
						AImoveAUX(&bin, &b, -Moto_WIDTH, 0, 3, player, *this);
					}
					if (c != -2 && !ListMem(*this, xHead, yHead + Moto_HEIGHT) && yHead + Moto_HEIGHT < SCREEN_HEIGHT)
					{
						AImoveAUX(&bin, &c, 0, Moto_HEIGHT, 3, player, *this);
					}
					if (d != -2 && !ListMem(*this, xHead, yHead - Moto_HEIGHT) && yHead - Moto_HEIGHT > 0)
					{
						AImoveAUX(&bin, &d, 0, -Moto_HEIGHT, 3, player, *this);
					}
				}
				int tab[4] = {a, b, c, d};
				int min = SCREEN_WIDTH + SCREEN_HEIGHT; // La distance maximale dans la fenetre est max(SCREEN_WIDTH, SCREEN_HEIGHT) qui est plus petit que SCREEN_WIDTH + SCREEN_HEIGHT
				for (int i = 0; i < 4; i++)
				{
					if (tab[i] >= 0 && min > tab[i]) //On choisit donc la direction qui minimise le rayon de sécurité adverse
					{
						min = tab[i];
						dir = i;
					}
				}
			}
		}
		if (dir == -1) //direction arbitraire car le bot est condamné à mourrir	peu importe la direction
		{
			mWait = 5;
			if (a == -2) //On fait en sorte que le bot continue tout droit
			{
				dir = 1;
			}
			else if (b == -2)
			{
				dir = 0;
			}
			else if (c == -2)
			{
				dir = 3;
			}
			else
			{
				dir = 2;
			}
		}//Maintenant, on déplace en fonction de la direction choisie
		if (dir == 0)
		{
			mPosX[Len - 1] += Moto_WIDTH;
			mVelX = Moto_VEL;
			mVelY = 0;
		}
		else if (dir == 1)
		{
			mPosX[Len - 1] -= Moto_WIDTH;
			mVelX = -Moto_VEL;
			mVelY = 0;
		}
		else if (dir == 2)
		{
			mPosY[Len - 1] += Moto_HEIGHT;
			mVelY = Moto_VEL;
			mVelX = 0;
		}
		else if (dir == 3)
		{
			mPosY[Len - 1] -= Moto_HEIGHT;
			mVelY = -Moto_VEL;
			mVelX = 0;
		}
	}
	for (int i = 0; i < Len - 2; i++) //On décale le reste de la moto
	{
		mPosX[i] = mPosX[i+1];
		mPosY[i] = mPosY[i+1];
	}
	mPosX[Len-2] = xHead;
	mPosY[Len-2] = yHead;
	if (ListMem(player, mPosX[Len-1], mPosY[Len-1]) || (player.getX(Len-1) == mPosX[Len-1] && player.getY(Len-1) == mPosY[Len-1]) || mPosX[Len - 1] > SCREEN_WIDTH
			|| mPosX[Len - 1] < 0 || mPosY[Len - 1] > SCREEN_HEIGHT || mPosY[Len - 1] < 0 || ListMem(*this, mPosX[Len-1], mPosY[Len-1]))
	{//On test si le bot s'est suicidé ou non (ce qui arrive en cas d'abscence de choix convenable)
		res = true;
	}
	if (isOnFruit(mPosX[Len-1], mPosY[Len-1]))
	{
		destroyFruit(mPosX[Len-1], mPosY[Len-1]);
		grow();
		for (int i = Len - 1; i > 0; i--)
		{
			mPosX[i] = mPosX[i-1];
			mPosY[i] = mPosY[i-1];
		}
		mPosX[0] = xTail;
		mPosY[0] = yTail;
	}
	return res;
}

#endif
