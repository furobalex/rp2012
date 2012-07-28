#include "verre.h"
#include "constantes.h"

Objet rechercheVerrePlein(Objet verre[11])
{
  
  int i;
  Objet verreActu;
  bool get = false;
  int getI = 0;
  
    for (i = 0 ; verre[i].type != FULL && verre[i].isAlreadyHere != true ; i++)
    {
        if (verre[i].type == FULL && verre[i].isAlreadyHere == true)
        {
            verre[i].isAlreadyHere = false;
            return verre[i];
        }
        
        else if (verre[i].type == EMPTY && verre[i].isAlreadyHere != true && get == false)
        {
          verreActu = verre[i];
          get = true;
          getI = i;
        }
    }
    
    verre[getI].isAlreadyHere = false;
    return verreActu; 
    
}

Objet rechercheVerreVide(Objet verre[11])
{
  
  int i;
  Objet verreActu;
  bool get = false;
  int getI = 0;
  
  verreActu.x = 0;
  verreActu.y = 0;
  verreActu.type = EMPTY;
  verreActu.isAlreadyHere = true;
  
    for (i = 0 ; verre[i].type != EMPTY && verre[i].isAlreadyHere != true ; i++)
    {
        if (verre[i].type == EMPTY && verre[i].isAlreadyHere == true)
        {
            verre[i].isAlreadyHere = false;
            return verre[i];
        }
        
        else if (verre[i].type == FULL && verre[i].isAlreadyHere != true && get == false)
        {
          verreActu = verre[i];
          get = true;
          getI = i;
        }
    }
    
    verre[getI].isAlreadyHere = false;
    return verreActu; 
  
}

int checkVerre(bool tab[4])
{
  
  int i;
  
  for (i = 0 ; i < 4 ; i++)
  {
    
    if (tab[1] == false)
    {
       return VERRE_0_OK;
    }
    
    if (tab[2] == false)
    {
       return VERRE_1_OK;
    }
    
    if (tab[3] == false)
    {
       return VERRE_2_OK;
    }
    
  }

  return GO_BASE;
    
}


