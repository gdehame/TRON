#ifndef GAMEHPP
#define GAMEHPP

#include "TRON.h"

void GameLoop(SDL_Event e, Moto* Motoj, Enemy* EnMoto, bool* end, bool* enemyDead, stringstream* timer, SDL_Color textColor)
{
  while (!(*end) && !quit) //boucle de jeu
  {
    while(SDL_PollEvent(&e) != 0)
    {
      if(e.type == SDL_QUIT)
      {
        quit = true;
      }
      else if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_ESCAPE && !isMultiplayer) //menu pause
      {
        (*end) = PauseMenu((*Motoj), (*EnMoto), e);
      }
      (*Motoj).handleEvent(e);
    }
    (*timer).str("");
    (*timer) << ((int)(gameTimer.getTicks() / 1000.f)); //on récupère le timer en secondes (de base en ms)
    if(!timerTexture.loadFromRenderedText((*timer).str().c_str(), textColor, Font)) //on charge la texture du timer avec le nombre à afficher
    {
      printf("Unable to render time texture!\n");
    }
    SDL_RenderClear(Rend);
    if ((*Motoj).moving() && (*Motoj).move((*EnMoto))) //si la moto du joueur bouge et que move renvoie true c'est que la moto est morte et donc on arrête la partie en affichant un écran de défaite
    {
      if (isMultiplayer) //si c'est en multijoueur il faut envoyer la position pour que l'autre joueur remarque qu'il a gagné et puisse terminer la partie
      {
        (*Motoj).sendPos(communication);
        if (isServer) //le générateur n'est utilisé que si le joueur est l'hôte
        {
          SDLNet_TCP_Close(gen);
        }
        SDLNet_TCP_Close(communication);
        isMultiplayer = false;
        isClient = false;
        isServer = false;
      }
      (*end) = true;
      FruitReset();
      RenderGameOver((*Motoj), (*EnMoto));
      SDL_Delay(3000);
    }
    else if (isMultiplayer)
    { //si la partie est en multijoueur on regarde si c'est le client ou l'ĥote: le premier recoit puis envoie les positions, l'autre fait dans l'autre ordre
      if (isClient)
      {
        SDL_Delay(10);
        (*enemyDead) = (*EnMoto).receivePos(communication, (*Motoj));
        (*Motoj).sendPos(communication);
      }
      else if (isServer)
      {
        generateFruit((*Motoj), (*EnMoto));
        (*Motoj).sendPos(communication);
        SDL_Delay(10);
        (*enemyDead) = (*EnMoto).receivePos(communication, (*Motoj));
      }
    }
    else // si la partie est en solo, on calcul le déplacement du bot et si il renvoie true et que le bot bouge alors c'est que le bot a perdu
    {
      generateFruit((*Motoj), (*EnMoto));
      (*enemyDead) = (*EnMoto).AImove((*Motoj)) && (*EnMoto).moving();
    }
    if ((*enemyDead))
    {
      Uint32 t = gameTimer.getTicks();
      (*end) = true;
      FruitReset();
      RenderWIN((*Motoj), (*EnMoto));
      SDL_Delay(3000);
      if (isMultiplayer) //si la partie est multijoueur on ferme les sockets et réinitialise les booléens relatifs au multijoueur
      {
        if (isServer)
        {
          SDLNet_TCP_Close(gen);
        }
        isMultiplayer = false;
        isClient = false;
        isServer = false;
        SDLNet_TCP_Close(communication);
      }
      else //sinon on ajoute la durée de la partie au tableau des scores car le joueur a gagné et on affiche ce tableau des scores
      {
        if((*EnMoto).getDifficulty() == Easy)
        {
          renderSb(addtoSb(t, ScoreboardE), ScoreboardE);
        }
        else if((*EnMoto).getDifficulty() == Medium)
        {
          renderSb(addtoSb(t, ScoreboardM), ScoreboardM);
        }
        else if((*EnMoto).getDifficulty() == Hard)
        {
          renderSb(addtoSb(t, ScoreboardH), ScoreboardH);
        }
        SDL_Delay(5000);
      }
    }
    else if (!(*end)) //si la partie n'est pas terminée, on affiche l'état actuel de la partie
    {
      GameBG.render(0,0);
      timerTexture.render((SCREEN_WIDTH - timerTexture.getWidth()) / 2, 0);
      (*Motoj).render();
      (*EnMoto).render();
      renderFruits();
      SDL_RenderSetScale(Rend, ScalingX, ScalingY);
      SDL_RenderPresent(Rend);
    }
  }
}

#endif
