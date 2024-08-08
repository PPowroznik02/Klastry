#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct dane_pliku {
    char typ[3];
    int wiersze;
    int kolumny;
    int max_wartosc;
} DANE_PLIKU;

typedef struct pixel {
    int barwa;
    bool stan;
} PIXEL;

typedef struct lista {
    int kolor;
    int rozmiar_klastra;
    struct lista *next;
} LISTA;

typedef struct kolejka {
    int wiersze;
    int kolumny;
    struct kolejka *next;
} KOLEJKA;

void wypisz_liste(LISTA *poczatkowy) {
    LISTA *n = poczatkowy;
    LISTA  *poprzedni = poczatkowy;
    if (n == NULL) {
        printf("Lista jest pusta");
    }else {
        printf("%3d : %d", n->kolor, n->rozmiar_klastra);
        n = n->next;
        while (n != NULL) {
            if(poprzedni->kolor != n->kolor){
                printf("\n%3d:", n->kolor);
            }

            printf(" %d", n->rozmiar_klastra);
            n = n->next;
            poprzedni = poprzedni->next;
        }
    }
    printf("\n");
}

void zalistuj(LISTA **adres_poczatkowy, int kolor, int rozmiar_klastra) {
    LISTA *nowy = malloc(sizeof(LISTA));
    LISTA *poprzedni = *adres_poczatkowy;
    nowy->rozmiar_klastra = rozmiar_klastra;
    nowy->kolor = kolor;
    nowy->next = NULL;
    if (*adres_poczatkowy == NULL) {
        *adres_poczatkowy = nowy;
    } else if (poprzedni->next == NULL && nowy->kolor < poprzedni->kolor) {
        *adres_poczatkowy = nowy;
        nowy->next = poprzedni;
    } else if (nowy->kolor < poprzedni->kolor && poprzedni->next->next == NULL) {
        *adres_poczatkowy = nowy;
        nowy->next = poprzedni;
    } else {
        LISTA *n = *adres_poczatkowy;
        LISTA *pierwszy = *adres_poczatkowy;
        while (n->next != NULL) {
            if (nowy->kolor <= pierwszy->kolor) {
                *adres_poczatkowy = nowy;
                nowy->next = pierwszy;
            } else if (nowy->kolor >= n->kolor && nowy->kolor < n->next->kolor) {
                LISTA *tmp;
                tmp = n->next;
                n->next = nowy;
                nowy->next = tmp;
            }
            n = n->next;
        }
        if (n->next == NULL && nowy->next == NULL) {
            n->next = nowy;
            nowy->next = NULL;
        }
    }
}

void usun_ostatni_z_listy(LISTA **adres_poczatkowy) {
    LISTA *n = *adres_poczatkowy;
    LISTA *poprzedni = *adres_poczatkowy;
    if ((*adres_poczatkowy)->next == NULL) {
        *adres_poczatkowy = NULL;
        free(n);
    } else {
        n = n->next;
        while (poprzedni->next->next != NULL) {
            poprzedni = poprzedni->next;
            n = n->next;
        }
        poprzedni->next = NULL;
        free(n);
    }
}

void zakolejkuj(LISTA **adres_poczatkowy, int wiersz, int kolumna) {
    KOLEJKA *nowy = malloc(sizeof(KOLEJKA));
    nowy->wiersze = wiersz;
    nowy->kolumny = kolumna;
    nowy->next = NULL;
    if (*adres_poczatkowy == NULL) {
        *adres_poczatkowy = nowy;
    } else {
        KOLEJKA *n = *adres_poczatkowy;
        while (n->next != NULL) {
            n = n->next;
        }
        n->next = nowy;
    }
}

void usun_pierwszy_kolejka(KOLEJKA **adres_poczatkowy) {
    KOLEJKA *n = *adres_poczatkowy;
    KOLEJKA *nast = *adres_poczatkowy;
    if ((*adres_poczatkowy)->next == NULL) {
        *adres_poczatkowy = NULL;
    } else {
        nast = nast->next;
        *adres_poczatkowy = nast;
        free(n);
    }
}

KOLEJKA zwroc_pierwszy_kolejka(KOLEJKA **adres_poczatkowy) {
    KOLEJKA *n = *adres_poczatkowy;
    return *n;
}

int main(int argc, char *argv[]) {
//otwarcie pliku
    FILE *f;
    f = fopen(argv[1], "rb");
    DANE_PLIKU meta_dane;
    LISTA *glowa_l = NULL;
    KOLEJKA *glowa_k = NULL;


//wczytanie pliku
    fscanf(f, "%s", meta_dane.typ);
    //printf("%s\n", meta_dane.typ);

    char ch, line[100];
    while ((ch = fgetc(f)) != EOF && isspace(ch));
    if (ch == '#') {
        fgets(line, sizeof(line), f);
    }

    char wiersze[20], kolumny[20], max_wartosc[20];
    fscanf(f, "%s%s", kolumny, wiersze);
    meta_dane.wiersze = atoi(wiersze);
    meta_dane.kolumny = atoi(kolumny);
    //printf("Wymiary : %d\t%d\n", meta_dane.wiersze, meta_dane.kolumny);

    int tmp2 = strcmp(meta_dane.typ, "P5");
    int tmp3 = strcmp(meta_dane.typ, "P2");
    if (tmp2 == 0 || tmp3 == 0) {
        fscanf(f, "%s", max_wartosc);
        meta_dane.max_wartosc = atoi(max_wartosc);
        //printf("Max wartosc : %d\n", meta_dane.max_wartosc);
    }

    PIXEL tab[meta_dane.wiersze][meta_dane.kolumny];

    for (int i = 0; i < meta_dane.wiersze; ++i) {
        for (int j = 0; j < meta_dane.kolumny; ++j) {
            tab[i][j].barwa = fgetc(f);
            tab[i][j].stan = 1;
            //printf("%d ", tab[i][j]);
        }
        //printf("\n");
    }
//koniec wczytywania pliku



//obliczanie wielkosci klastrow
    for (int i = 0; i < meta_dane.wiersze; ++i) {
        for (int j = 0; j < meta_dane.kolumny; ++j) {
            int rozmiar = 0;
            if (tab[i][j].stan == 1) {
                tab[i][j].stan = 0;
                rozmiar = 1;
                int x, y, r, k;
                zakolejkuj(&glowa_k, i, j);
                while (glowa_k != NULL) {
                    KOLEJKA tmp;
                    tmp = zwroc_pierwszy_kolejka(&glowa_k);
                    usun_pierwszy_kolejka(&glowa_k);
                    //w lewo
                    if (tmp.wiersze >= 0 && tmp.kolumny - 1 >= 0 && tmp.wiersze < meta_dane.wiersze &&
                        tmp.kolumny - 1 < meta_dane.kolumny) {
                        if (tab[tmp.wiersze][tmp.kolumny - 1].stan == 1 &&
                            tab[tmp.wiersze][tmp.kolumny - 1].barwa == tab[tmp.wiersze][tmp.kolumny].barwa) {

                            tab[tmp.wiersze][tmp.kolumny - 1].stan = 0;
                            rozmiar++;
                            zakolejkuj(&glowa_k, tmp.wiersze, tmp.kolumny - 1);
                        }
                    }
                    //w gore
                    if (tmp.wiersze - 1 >= 0 && tmp.kolumny >= 0 && tmp.wiersze + 1 < meta_dane.wiersze &&
                        tmp.kolumny - 1 < meta_dane.kolumny) {
                        if (tab[tmp.wiersze - 1][tmp.kolumny].stan == 1 &&
                            tab[tmp.wiersze - 1][tmp.kolumny].barwa == tab[tmp.wiersze][tmp.kolumny].barwa) {

                            tab[tmp.wiersze - 1][tmp.kolumny].stan = 0;
                            rozmiar++;
                            zakolejkuj(&glowa_k, tmp.wiersze - 1, tmp.kolumny);
                        }
                    }
                    //w prawo
                    if (tmp.wiersze >= 0 && tmp.kolumny + 1 >= 0 && tmp.wiersze < meta_dane.wiersze &&
                        tmp.kolumny + 1 < meta_dane.kolumny) {
                        if (tab[tmp.wiersze][tmp.kolumny + 1].stan == 1 &&
                            tab[tmp.wiersze][tmp.kolumny + 1].barwa == tab[tmp.wiersze][tmp.kolumny].barwa) {

                            tab[tmp.wiersze][tmp.kolumny + 1].stan = 0;
                            rozmiar++;
                            zakolejkuj(&glowa_k, tmp.wiersze, tmp.kolumny + 1);
                        }
                    }
                    //w dol
                    if (tmp.wiersze + 1 >= 0 && tmp.kolumny >= 0 && tmp.wiersze + 1 < meta_dane.wiersze &&
                        tmp.kolumny < meta_dane.kolumny) {
                        if (tab[tmp.wiersze + 1][tmp.kolumny].stan == 1 &&
                            tab[tmp.wiersze + 1][tmp.kolumny].barwa == tab[tmp.wiersze][tmp.kolumny].barwa) {

                            tab[tmp.wiersze + 1][tmp.kolumny].stan = 0;
                            rozmiar++;
                            zakolejkuj(&glowa_k, tmp.wiersze + 1, tmp.kolumny);
                        }
                    }
                    x = tmp.wiersze;
                    y = tmp.kolumny;
                }
                //printf("%d\t%d\n", tab[x][y].barwa, rozmiar);
                zalistuj(&glowa_l, tab[x][y].barwa, rozmiar);
            }
        }
    }
//koniec obliczania wielkosci klastrow



//wypisanie rozmiarow i kolorow klastrow
    printf("Kolor : Rozmiary\n");
    wypisz_liste(glowa_l);

    while (glowa_l != NULL) {
        usun_ostatni_z_listy(&glowa_l);
    }
    return 0;
}

