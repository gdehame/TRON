#ifndef FRUITSHPP
#define FRUITSHPP

void generateFruit(Moto player, Enemy enemy) //Génère aléatoirement un fruit ou non
{
	if (rand() % 2000 == 0)
	{
		bool b = true;
		int x = 0;
		int y = 0;
		while (b)
		{
			x = rand() % 96;
			y = rand() % 54;
			b = ListMem(player, x * 20, y * 20) || ListMem(enemy, x * 20, y * 20) || (x * 20 == player.getX(player.Len - 1) && y * 20 == player.getY(player.Len - 1)) || (x * 20 == enemy.getX(player.Len - 1) && y * 20 == enemy.getY(player.Len - 1));
		}
		gameFruits[x][y] = 1;
	}
}

void FruitReset() //Vide le tableau des fruits pour débuter une nouvelle partie
{
	for (int i = 0; i < 96; i++)
	{
		for (int j = 0; j < 54; j++)
		{
			gameFruits[i][j] = 0;
		}
	}
}

void renderFruits()
{
  for (int i = 0; i < 96; i++)
  {
    for (int j = 0; j < 54; j++)
    {
      if (gameFruits[i][j] == 1)
      {
        Fruittexture.render(i * 20, j * 20);
      }
    }
  }
}

bool isFruit()
{
	for (int i = 0; i < 96; i++)
	{
		for (int j = 0; j < 54; j++)
		{
			if (gameFruits[i][j] == 1)
			{
				return true;
			}
		}
	}
	return false;
}

bool isOnFruit(int x, int y)
{
	return (gameFruits[x / 20][y / 20] == 1);
}

void destroyFruit(int x, int y)
{
	if (isClient)
	{
		gameFruits[x / 20][y / 20] = -1; //Mettre à -1 sert à éviter de recopier ce fruit lorsqu'on recoit la liste des fruits du serveur
	}
	else
	{
		gameFruits[x / 20][y / 20] = 0;
	}
}

#endif
