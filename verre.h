#ifndef VERRE

  #define VERRE
  
  typedef struct Objet Objet;
  
  struct Objet
  {
     int x;
     int y;
     int type;
     bool isAlreadyHere;
  };
  
  Objet rechercheVerrePlein(Objet verre[11]);
  Objet rechercheVerreVide(Objet verre[11]);
  int checkVerre(bool tab[4]);
  
#endif
