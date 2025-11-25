/**
 *
 * Nous proposons de modéliser *6 qui prend*, un jeu de cartes édité chez
 * Gigamic.
 *
 * Le plateau de jeu est constitué de n lignes (typiquement 4) de cartes. Ces
 * lignes contiennent initialement, une carte choisie au hasard parmi
 * MAX_CARD_VALUE cartes uniques (typiquement  MAX_CARD_VALUE vaut 104 et les
 * cartes sont numérotées 1 à 104). Les cartes seront simplement représentées
 * par leur valeur entière (`int`).
 *
 * A chaque tour, tous les joueurs, choisissent une carte dans leur main et
 * jouent simultanément. Les cartes sont placées (résolues) sur le plateau dans
 * l'ordre croissant. Chaque carte vient compléter la ligne où il y a la plus
 * petite différence positive. Par conséquence les valeurs des cartes sur une
 * ligne sont strictement croissantes. Si la carte venait à être la 6ème
 * (`MAX_LINE_LENGTH`) sur la ligne, le joueur remplace les cartes de la ligne
 * par sa carte.  Il accumule le nombre de points (vaches) correspondant aux
 * cartes qu'il remplace. S'il n'existe aucune ligne avec une plus petite
 * différence positive, le joueur choisit une ligne et la remplace par sa carte
 * (comme si c'était la 6ème).
 *
 * Le but du jeu est de faire le moins de points (vaches) possible. Le
 * premier joueur dépassant `max_cows` (typiquement 66) déclenche la
 * fin de partie.
 */

/**
 * Une ligne du plateau
 */
struct line {
    int count; // Le nombre de cartes dans cette ligne (> 0)
    int values[MAX_LINE_LENGTH - 1]; // La valeur des cartes posées sur cette ligne
    // Il n'y aura jamais MAX_LINE_LENGTH cartes dans une ligne
};

/**
 * Un joueur
 */
struct player {
    const char *name; // Le nom du joueur
    int cows; // Le score actuel
    int card_count; // Le nombre de cartes en main
    int cards[MAX_HAND_SIZE]; // Les cartes en main du joueur
    // L'ordre des cartes est indifférent, elles doivent êtres groupées au
    // début du tableau.
};

/**
 * Une partie en cours
 */
struct game {
    const int player_count; // Le nombre de joueurs
    struct player players[MAX_PLAYERS]; // Les joueurs

    const int max_cows; // Le nombre de vaches à partir duquel le jeu
                        // s'arrête

    const int line_count; // Le nombre de lignes du plateau
    struct line board[MAX_LINE_COUNT]; // Le plateau
};

/**
 *
 * Retourne le nombre de vaches d'une carte.
 *
 * Les cartes se terminant par un 5 valent 2 points (ex: *05*, *15*), les
 * cartes se terminant par un 0 valent 3 points (ex: *10*, *20*) et les doubles
 * valent 5 points (ex: *11*, *22*). Toutes les autres cartes valent 1 point.
 *
 * On peut remarquer que la carte 55 est à la fois un double et une carte se
 * terminant par 5, elle vaut donc 7 points.
 */

int cow_value(int card)
{
    int score = 0;

    if(card%11 ==0)
    {
        score += 5;
    }

    if(card%10 ==0)
    {
        score += 3;
    }
    else if(card%5 ==0)
    {
        score += 2;
    }

    if(score ==0)
    {
        return 1;
    }

    return score;
}

/**
 * Retourne le nombre de vaches contenues dans une ligne.
 */
int line_value(struct line l)
{
    int compte = 0;
    for( int i=0;i<l.count;i++)
    {
        compte += cow_value(l.values[i]);
    }
    return compte;
}

/**
 * Retourne la dernière carte d'une ligne.
 */
int top_card(struct line l)
{
    return l.values[l.count-1];
}

/**
 * Affiche joliment le plateau (`board`) de `lines` lignes.
 * La ligne est précédée du nombre de vaches qu'elle coute.
 *
 * Exemple:
 * ( 7)  12  17  22
 * ( 4) 100 101
 * ( 9)  32  44  52  57  63
 * ( 8)  45  51  55  74
 *
 * Note:
 *   Le format demandé pour `printf` est `%3d` pour les cartes et `%2d` pour
 *   le coût en vaches.
 *   (%Nd => minimum N caractères alignés à droite complété par des
 *   espaces, si nécessaire)
 */
void print_board(int lines, const struct line board[])
{
    for( int i=0;i<lines;i++)
    {
        printf("(%2d) ",line_value(board[i]));
        for( int j=0;j<board[i].count;j++)
        {
            printf("%3d",board[i].values[j]);
            if(j!=board[i].count-1)
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}

/**
 * Retourne le nombre de personnes ayant atteint le seuil de vaches
 * (`max_cows`) et stocke l'indice de ceux-ci dans le tableau `players`.
 */
int losers(const struct game *g, int players[])
{
    int cpt = 0;
    for( int i=0;i<g->player_count;i++)
    {
        if(g->players[i].cows >= g->max_cows)
        {
            players[cpt] = i;
            cpt++;
        }
    }
    return cpt;
}

/**
 * Retourne l'indice de la première valeur supérieure (ou égale) à `bound` dans
 * le tableau `values` (de longueur `len`), ou -1 s'il n'existe pas une telle
 * valeur.
 */
int index_first_above(int bound, int len, const int values[])
{
    for( int i=0;i<len;i++)
    {
        if(values[i] >= bound)
        {
            return i;
        }
    }
    return -1;
}

/**
 * Retourne l'indice de la plus petite valeur supérieure (ou égale) à `bound`
 * dans le tableau `values` (de longueur `len`), ou -1 s'il n'existe pas une
 * telle valeur.
 */
int index_min_over(int bound, int len, const int values[])
{
    int value = -1, precedent = 65535 ;
    for( int i=0;i<len;i++)
    {
        if((values[i] >= bound)&&(values[i] < precedent))
        {
            value = i;
            precedent = values[i];
        }
    }
    return value;
}

/**
 * Retourne l'indice de la ligne de coût minimal (en nombre de vaches).
 * En cas d'égalité, l'indice le plus petit sera choisi (pour simplifier).
 *
 * Remarque: Les lignes contiennent au moins une carte par construction
 */
int less_expensive_line(int lines, const struct line game[])
{
  int precedent = line_value(game[0]);
  int value = 0;
  for (int i = 1; i <lines; i++)
  {
    if(line_value(game[i]) <= precedent)
    {
      value = i;
    }
  }
  return value;
}

/**
 * Retourne l'indice de la ligne où la carte `card` est censée aller (en
 * respectant les règles du jeu), ou -1 si elle ne peut pas être posée.
 *
 * Rappel de la règle: la carte doit aller sur la ligne avec la plus petite
 * différence positive.
 */
int card_should_go(int card, int lines, const struct line game[]);
/*{
  int value = cow_value(card);
  int val = -1, precedent = 65535 ;
  for( int i=0;i<len;i++)
  {
      if((game[i] >= bound)&&(game[i] < precedent))
      {
          val = i;
          precedent = game[i];
      }
  }
  return val;
}*/

/**
 * Remplit le tableau `player` (de longueur `player_count`) avec les
 * indices des éléments de `cards` (de longueur `player_count`) dans l'ordre
 * strictement croissant.
 *
 * Exemple:
 *     sort_player(4, {10, 1, 63, 32}, ...) => {1, 0, 3, 2}
 *
 * Remarque: Il n'est clairement pas demandé un algorithme de tri compliqué,
 * le nombre de joueur est petit (une dizaine max).
 */
void sort_player(int player_count, const int cards[], int players[])
{
  int cpt = 0;
  for (int i = 0; i < player_count; i++)
  {
    cpt = 0;
    for (int j = 0; j < player_count; j++)
    {
      if(cards[i]>cards[j])
      {
        cpt++;
      }
    }
    players[cpt] = i;
  }
}

/**
 * Pose la carte `card` sur la ligne `l`, en respectant la règle, et retourne
 * le coût des vaches prises.
 *
 * Rappel:
 * - On ne peut rajouter à une ligne que si elle conserve un ordre croissant
 * - On ne peut poser la MAX_LINE_LENGTH ième carte sur une ligne
 * - On n'enlève jamais une carte mais on remplace toute la ligne
 */
int place_card(struct line *l, int card)
{
  if ((card > l->values[l->count-1])&&(l->count < MAX_LINE_LENGTH-1))
  {
    l->values[l->count] = card;
    l->count++;
    return 0;
  }
  else
  {
    int vache = line_value(*l);
    struct line ligne;
    ligne.count = 1;
    ligne.values[0] = card;
    *l = ligne;
    return vache;
  }
}

/**
 * Retire de la main du joueur `p` la carte `card`.
 *
 * Remarque: Peu importe l'ordre des cartes en main à la fin, pourvu
 * qu'elles soient contigües.
 */
void pick_card(struct player *p, int card)
{
    int i=0;
    for( i=0;i<p->card_count;i++)
    {
        if(p->cards[i] == card)
        {
            for( int j=i;j<p->card_count;j++)
            {
                p->cards[j] = p->cards[j+1];
            }
            p->card_count--;
            break;
        }
    }
}

/**
 *
 * Joue effectivement la carte `card` de la main du joueur `p` sur la "bonne"
 * ligne (la plus petite différence positive) ou, à défaut, sur la première
 * ligne qui vaut le moins de vaches.
 *
 * Remarque: Le score ainsi que la main du jplay_cardoueur `p` est mis à jour.
 */

void play_card(struct player *p, int card, int lines, struct line board[]);
/*{
  pick_card(p,card);
  int indice_line = card_should_go(card, lines, board);
  if(indice_line!=-1)
  {
    board[indice_line].values[board[indice_line].count] = card;
    board[indice_line].count++;
  }
  else
  {
    int minimal = less_expensive_line(lines, board);
    board[minimal].values[board[minimal].count] = card;
    board[minimal].count++;
  }
  p->cows = cow_value(card);
}*/

/**
 * Joue un tour de jeu, où les joueurs jouent dans l'ordre.
 * Le joueur i joue la carte cards[i].
 */
void play_round(struct game *g, int cards[])
;
