#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include <menu.h>

/*******************************************************************************************************************

    DISCLAIMER: Pentru a afisa browserul, va rog sa aveti terminalul in FULLSCREEN si ecranul in 16:9 1920x1080 !!!!

********************************************************************************************************************/

/*
    Sumar functii si macro-uri ncurses folosite:
    -noecho(), echo() = ascunde/afiseaza textul tastat la tastatura
    -curs_set(0), curs_set(1) = ascunde/afiseaza cursorul pe ecran
    -wattron(window,atribut) = atribuie ecranului window un atribut
    -wattroff(window,atribut) = dezatribuie ecranului window un atribut deja setat
    -getmaxyx(window,y,x) = pune in y si x inaltimea/latimea maxima a ecranului window
    -wclear(window) = sterge tot scrisul de pe ecranul window
    -wrefresh(window) = reimprospateaza proprietatile ecranului window
    -init_pair(nr,culoare_text,culoare_fundal) = initializeaza o pereche de culori cu atributele date
    -wbkg(window,COLOR_PAIR(nr)) = schimba culoarea terminalul in perechea de culori cu nr. respectiv
    -mvwprintw(window,y,x,format) = afiseaza pe ecranul window la linia y si coloana x format-ul
    -wgetch(window) = citeste un caracter din ecranul window
    -wgetstr(window,sir)= citeste un sir de caractere fara '\n' in ecranul window 
*/

//Definam culorile ca enumeratie
typedef enum {black,white,red,green,blue,yellow}culori;

//Declararea tipului site ca struct
typedef struct 
{
    char url[201], titlu[201], culoare_fundal[10], culoare_text[10];
    char *continut;
    int nr_accs, checksum;
}site;

//Functie ce transforma un sir de caractere in numar
int numar(char *s)
{
    int nr=0, p=1, i;
    for(i=strlen(s)-1;i>=0;i--)
    {
        //Adunam cifra corespunzatoare caracterului dat, punand-o pe pozitie sa.
        nr+=((s[i]-'0')*p);
        p*=10;
    }
    return nr;
}

//Functie ce transforma culoarea in sir de caractere
char *format_culoare(int x)
{
    static char culori[8][15]={"black","white","red","green","blue","yellow"};
    return (culori[x]);
}

//Functie ce transforma sirul de caractere in culoare
int format_numar(char *s)
{
    int i;
    if(!strcmp(s,"black"))
    {
        i=black;
    }
    if(!strcmp(s,"white"))
    {
        i=white;
    }
    if(!strcmp(s,"red"))
    {
        i=red;
    }
    if(!strcmp(s,"green"))
    {
        i=green;
    }
    if(!strcmp(s,"blue"))
    {
        i=blue;
    }
    if(!strcmp(s,"yellow"))
    {
        i=yellow;
    }
    return i;
}

//Functie ce traduce culoarea din sir de caractere in culori pentru ncurses
int traducere_culori(char *s)
{
    int i;
    if(!strcmp(s,"black"))
    {
        i=COLOR_BLACK;
    }
    if(!strcmp(s,"white"))
    {
        i=COLOR_WHITE;
    }
    if(!strcmp(s,"red"))
    {
        i=COLOR_RED;
    }
    if(!strcmp(s,"green"))
    {
        i=COLOR_GREEN;
    }
    if(!strcmp(s,"blue"))
    {
        i=COLOR_BLUE;
    }
    if(!strcmp(s,"yellow"))
    {
        i=COLOR_YELLOW;
    }
    return i;
}

// Functie ce returneaza adresa de memorie a unui vector de site-uri parsate.
site *formatare(int *cap, int *size)
{
    //Initializam vectorul cu 3 elemente
    site *browser=(site *)malloc(3*sizeof(site));
    *cap=3, *size=0; //Retinem capacitatea si nr de elemente ale vectorului
    char fisier[31], linie[101], *p, *cat, *q;
    int nr_linie, k, marime_fisier, gasit, aux, ok1, ok2, i;
    FILE *master=fopen("master.txt","r"); //Deschidem fisierul master ce contine toate site-urile
    while(fgets(fisier,31,master))
    {
        ok1=0;
        ok2=1;
        if(fisier[strlen(fisier)-1]=='\n')
        {
            fisier[strlen(fisier)-1]='\0'; //Eliminam \n de la final
        }
        FILE *fp=fopen(fisier,"r");
        if((*size)==(*cap)-1)
        {
            (*cap)+=3;
            browser=realloc(browser,(*cap)*sizeof(site));
            //Realocam vectorul daca nu mai are spatiu
        }
        nr_linie=0;
        while(fgets(linie,101,fp))
        {
            k=0;
            if(nr_linie==0)
            {
                //Daca ne aflam pe prima linie formatam unele date
                linie[strlen(linie)-1]='\0';
                p=strtok(linie," ");
                //Impartim linia dupa " "
                while(p)
                {
                    switch (k)
                    {
                        case 0: strcpy(browser[(*size)].url,p); 
                            break; //Daca suntem la inceput copiem URL
                        case 1: marime_fisier=numar(p); 
                            break; //Primul nr este marimea fisierului    
                        case 2: browser[(*size)].nr_accs=numar(p); 
                            break; //Al doilea nr. reprezinta nr. de vizualizari
                        case 3: browser[(*size)].checksum=numar(p); 
                            break; //Al treilea nr. reprezinta checksum-ul        
                    }
                    k++;
                    p=strtok(NULL," ");
                }
                //Alocam memorie pentru continut si cod HTML cu marimea fisierului aflata anterior
                browser[*(size)].continut=(char *)malloc(marime_fisier*sizeof(char));
                cat=(char *)malloc(marime_fisier*sizeof(char));
            }
            if(nr_linie>0 && strstr(linie,"<p"))
            {
                //Vedem daca am gasit linia cu tag-ul de inceput de paragraf.
                gasit=1;
                i=1;
                aux=1;
            }
            if(gasit)
            {
                //Daca l-am gasit, pentru prima linie copiem
                if(i==1)
                {
                   strcpy(cat,linie); 
                }
                //Altfel, concatenam
                else
                {
                    strcat(cat,linie);
                }
                //Daca l-am gasit, pentru prima linie copiem ce se afla dupa >
                if(aux==1)
                {
                    strcpy(browser[*(size)].continut,strchr(linie,'>')+1);
                }
                //Altfel, concatenam
                else
                {
                    strcat(browser[*(size)].continut,linie);
                }
                i++;
                aux++;
            }
            if(strstr(linie,"/p>"))
            {
                gasit=0;
                browser[*(size)].continut[strlen(browser[*(size)].continut)-5]='\0';
                //Stergem \n si <\p> din sir
            }
            if(strstr(linie,"<title>"))
            {
                //In cazul in care nu mai exista spatii inainte de '<' titlul
                // se afla la a doua tokenizare
                if(linie[0]=='<')
                {
                    p=strtok(linie,"<>");
                    p=strtok(NULL,"<>");
                    strcpy(browser[(*size)].titlu,p); 
                }
                //In caz contrar se afla la a treia tokenizare
                else
                {
                    p=strtok(linie,"<>");
                    p=strtok(NULL,"<>");
                    p=strtok(NULL,"<>");
                    strcpy(browser[(*size)].titlu,p);
                }
            }
            nr_linie++;
        }
        //Formatam culorile, separand ce se aflta in CSS
        q=strtok(cat,";:=\n ");
        while(q)
        {
            //Daca avem culori in CSS le punem in ok1 si ok2
            if(ok1==-1)
            {
                ok1=format_numar(q);
            }
            if(ok2==-1)
            {
                ok2=format_numar(q);
            }
            //Daca a gasit culoarea font-ului punem -1
            if(strstr(q,"color") && !strchr(q,'-'))
            {
                ok1=-1;
            }
            //Daca a gasit culoarea fundalului punem -1
            if(strstr(q,"background-color"))
            {
                ok2=-1;
            }
            q=strtok(NULL,";:=\n ");
        }
        //Punem culoarea in format de sir de caractere in structura 
        strcpy(browser[(*size)].culoare_text,format_culoare(ok1));
        strcpy(browser[(*size)].culoare_fundal,format_culoare(ok2));
        (*size)++;
        fclose(fp); //inchidem fisierul din care am citit
    }
    fclose(master); //inchidem masterul
    return browser;
}

//Functie ce returneaza adresa de memorie a unui vector de cuvinte rezultate din partitia unui sir de caractere
char **query(char *s, int *marime)
{
    int i, nr_cuv=0, sec;
    char **cuv, *p;
    for(i=0;i<strlen(s);i++)
    {
        if(s[i]==' ')
        {
            //Numaram nr. maxim de cuvinte.
            nr_cuv++;
        }
    }
    cuv=(char **)malloc((nr_cuv+2)*sizeof(char *)); 
    //Alocam memorie pentru vectorul de cuvinte
    for(i=0;i<(nr_cuv+2);i++)
    {
        cuv[i]=(char *)malloc(101*sizeof(char)); 
        //Alocam memorie pentru fiecare cuvant
    }
    //Partitionam sirul de caractere si copiem cuvinte in vector
    p=strtok(s," ");
    i=0;
    sec=0;
    while(p)
    {
        //Daca avem doar un singur cuvant intre " " il copiem si stergem ""
        if(p[0]=='"' && p[strlen(p)-1]=='"')
        {
            strcpy(cuv[i],p+1);
            cuv[i][strlen(cuv[i])-1]='\0';
            i++;
            sec=0;
        }
        //Altfel daca avem mai multe cuvinte intre " " il copiem pe primul
        else if(p[0]=='"')
        {
            sec=1;
            strcpy(cuv[i],p+1);
        }
        //Si pe restul le contatenam la primul, adaugand spatiu
        else if(sec)
        {
            strcat(cuv[i]," ");
            strcat(cuv[i],p);
            if(p[strlen(p)-1]=='"')
            {
                sec=0;
                cuv[i][strlen(cuv[i])-1]='\0';
                i++;
            }
        }
        //Daca nu contine "", il copiem direct
        else
        {
            strcpy(cuv[i],p);
            i++;
        }
        p=strtok(NULL," ");
    }
    *marime=i;
    return cuv;
}

//Functie ce filtreaza site-urile simplu si le pune in vector  
void filtrare_site_simple(site *browser_filtrat, int *k, site *browser, int size, char **cuvinte, int marime)
{
    char a, b;
    int  i, j;    
    (*k)=0;
    for(i=0;i<size;i++)
    {
        for(j=0;j<marime;j++)
        {
            //Verificam daca cuvantul se afla in continut de sine statator
            if(strstr(browser[i].continut,cuvinte[j]))
            {
                a=((strstr(browser[i].continut,cuvinte[j]))-1)[0];
                b=((strstr(browser[i].continut,cuvinte[j]))+strlen(cuvinte[j]))[0];
                if(!isalnum(a) && !isalnum(b))
                {
                    browser_filtrat[(*k)]=browser[i];
                    (*k)++;
                    break;
                }
            }
        }
    }
}

//Functie ce filtreaza site-urile avansat si le pune in vector  
void filtrare_site_advanced(site *browser_filtrat, int *k, site *browser, int size, char **cuvinte, int marime)
{
    char a, b;
    int  i, j, ok;    
    (*k)=0;
    for(i=0;i<size;i++)
    {
        ok=1;
        for(j=0;j<marime;j++)
        {
            //Verificam daca un cuvant care are - se afla in continut de sine statator
            if(strchr(cuvinte[j],'-') && strstr(browser[i].continut,(cuvinte[j]+1)))
            {
                a=((strstr(browser[i].continut,(cuvinte[j]+1)))-1)[0];
                b=((strstr(browser[i].continut,(cuvinte[j]+1)))+strlen(cuvinte[j])-1)[0];
                if(!isalnum(a) && !isalnum(b))
                {
                    ok=0;
                    break;
                }
            }
        }
        if(!ok)
        {
            continue;
        }
        else
        {
            for(j=0;j<marime;j++)
            {
                //Daca nu are un cuvant cu -, verificam daca are cuvinte 
                //sau o secventa de sine statatoare
                if(strstr(browser[i].continut,cuvinte[j]))
                {
                    a=((strstr(browser[i].continut,cuvinte[j]))-1)[0];
                    b=((strstr(browser[i].continut,cuvinte[j]))+strlen(cuvinte[j]))[0];
                    if(!isalnum(a) && !isalnum(b))
                    {
                        browser_filtrat[(*k)]=browser[i];
                        (*k)++;
                        break;
                    }
                }
            }
        }
    }
}

//Functie ce afiseaza pagina web a site-ului
void draw_site_screen(WINDOW *fereastra, site sait, int *tasta)
{
    int latime, inaltime, ok;
    getmaxyx(fereastra,inaltime,latime);
    wclear(fereastra);
    noecho();
    init_pair(4,traducere_culori(sait.culoare_text), traducere_culori(sait.culoare_fundal));
    wbkgd(fereastra,COLOR_PAIR(4));
    wattron(fereastra,A_BOLD);
    mvwprintw(fereastra,0,latime/2-strlen(sait.titlu)/2,"%s",sait.titlu);
    wattroff(fereastra,A_BOLD);
    mvwprintw(fereastra,2,2,"%s",sait.continut);
    mvwprintw(fereastra,inaltime/2+7,latime/2+25,"LEGENDA:\n");
    mvwprintw(fereastra,inaltime/2+8,latime/2+20,"b: Inapoi la menu\n");
    ok=0;
    //Daca utilizatorul apasa oricare alta tasta decat cea din LEGENDA
    //browserul nu va face nimic, altfel iesim din ecran
    do
    {
        (*tasta)=wgetch(fereastra);
        if((*tasta)=='b')
        {
            return;
        }
    } while(!ok);
    
}

//Functie ce afiseaza fereastra de meniu cu site-urile filtrate
void draw_menu_screen(WINDOW *fereastra, site *browser_filtrat, int k, int *tasta, int* id)
{
    MENU *meniu;
    ITEM **iteme;
    WINDOW *subfereastra;
    int i, latime, inaltime, ok;
    iteme=(ITEM **)malloc((k+1)*sizeof(ITEM*));
    for(i=0;i<k;i++)
    {
        //Alocarea itemelor din menu (titlu si descriere)
        iteme[i]=new_item(browser_filtrat[i].url,browser_filtrat[i].titlu);
    }
    iteme[k]=NULL; //Daca nu puneam asta, imi dadea SEGFAULT
    wclear(fereastra);
    wrefresh(fereastra);
    meniu=new_menu(iteme); // initializare meniu
    getmaxyx(fereastra,inaltime,latime);
    subfereastra=derwin(fereastra,inaltime-9,latime-9,4,8);
    init_pair(3,COLOR_WHITE,COLOR_CYAN);
    wbkgd(fereastra,COLOR_PAIR(3));
    wbkgd(subfereastra,COLOR_PAIR(3));
    set_menu_win(meniu,fereastra);
    set_menu_sub(meniu,subfereastra);
    set_menu_back(meniu,COLOR_PAIR(3));  //atribuire de proprietati meniului
    set_menu_fore(meniu,COLOR_PAIR(3));
    set_menu_mark(meniu,"~");
    mvwprintw(fereastra,inaltime/2+7,latime/2+30,"LEGENDA:\n");
    mvwprintw(fereastra,inaltime/2+8,latime/2+15,"UP_ARROW si DOWN_ARROW: Scroll in meniu\n");
    mvwprintw(fereastra,inaltime/2+9,latime/2+15,"ENTER: Intrati in site\n");
    mvwprintw(fereastra,inaltime/2+10,latime/2+15,"b: Inapoi la cautare\n");
    post_menu(meniu); //afisarea meniului
    keypad(fereastra,1); //permite folosirea sagetilor si tastelor F1-F12
    ok=0;
    //Daca utilizatorul apasa oricare alta tasta decat cea din LEGENDA
    //browserul nu va face nimic, altfel iesim din ecran
    do
    {
        (*tasta)=wgetch(fereastra);
        switch ((*tasta))
        {
            case KEY_UP:
            {
                //muta in sus la urmatorul element din meniu
                menu_driver(meniu,REQ_UP_ITEM);
                break;
            }
            case KEY_DOWN:
            {
                //muta in jos la urmatorul element din meniu
                menu_driver(meniu,REQ_DOWN_ITEM);
                break;
            }
            case '\n':
            {
                //returneaza indexul elementului selectat din meniu
                (*id)=item_index(current_item(meniu));
                return;
                break;
            }
            case 'b':
            {
                return;
                break;
            }
        }
    }while(!ok);
}

//Functie ce afiseaza pagina de cautare a browserului
void draw_search_screen(WINDOW *fereastra, int *tasta, char *lista)
{
    int inaltime, latime, i, ok;
    getmaxyx(fereastra,inaltime,latime);
    wclear(fereastra);
    curs_set(1);
    init_pair(2,COLOR_BLACK,COLOR_RED);
    wbkgd(fereastra,COLOR_PAIR(2));
    for(i=15;i>=-15;i--)
    {
        mvwprintw(fereastra,inaltime/2-1,latime/2+i,"-");
    }
    mvwprintw(fereastra,inaltime/2,latime/2+15,"|");
    mvwprintw(fereastra,inaltime/2,latime/2-15,"|");
    for(i=15;i>=-15;i--)
    {
        mvwprintw(fereastra,inaltime/2+1,latime/2+i,"-");
    }
    mvwprintw(fereastra,inaltime/2+7,latime/2+25,"LEGENDA:\n");
    mvwprintw(fereastra,inaltime/2+8,latime/2+20,"a: Cautare avansata\n");
    mvwprintw(fereastra,inaltime/2+9,latime/2+20,"s: Cautare simpla\n");
    mvwprintw(fereastra,inaltime/2+10,latime/2+20,"q: Iesiti din browser\n");
    wmove(fereastra,inaltime/2,latime/2-14);
    echo();
    wgetstr(fereastra,lista);
    noecho();
    curs_set(0);
    //Daca utilizatorul apasa oricare alta tasta decat cea din LEGENDA
    //browserul nu va face nimic, altfel iesim din ecran
    ok=1;
    do
    {
        (*tasta)=wgetch(fereastra);
        if((*tasta) =='q')
        {
            return;
        }
        else if((*tasta) =='a')
        {
            return;
        }
        else if((*tasta)=='s')
        {
            return;
        }
        else
        {
            ok=0;
        }
    }while(!ok);  
}

//Functie ce afiseaza pagina de pornire a browserului
void draw_startup_screen(WINDOW *fereastra, int *tasta)
{
    int inaltime, latime, ok;
    char nume[]="BROWSER BALCANIC";
    init_pair(1,COLOR_YELLOW,COLOR_BLUE);
    wattron(fereastra,COLOR_PAIR(1));
    wbkgd(fereastra,COLOR_PAIR(1));
    wattron(fereastra,A_BOLD);
    getmaxyx(fereastra,inaltime,latime);
    curs_set(0);
    mvwprintw(fereastra,inaltime/2,latime/2-8,"%s",nume);
    wattroff(fereastra,A_BOLD);
    wrefresh(fereastra);
    wattron(fereastra,A_STANDOUT);
    wrefresh(fereastra);
    mvwprintw(fereastra,inaltime/2+1,latime/2+3,"by ReGeLePuMa");
    wattroff(fereastra,A_STANDOUT);
    wrefresh(fereastra);
    mvwprintw(fereastra,inaltime/2+7,latime/2+25,"LEGENDA:\n");
    mvwprintw(fereastra,inaltime/2+8,latime/2+15,"c: Intrati in modul de cautare\n");
    mvwprintw(fereastra,inaltime/2+9,latime/2+15,"q: Iesiti din browser\n");
    ok=1;
    //Daca utilizatorul apasa oricare alta tasta decat cea din LEGENDA
    //browserul nu va face nimic, altfel iesim din ecran
    do
    {
        (*tasta)=wgetch(fereastra);
        if((*tasta) =='q')
        {
            return;
        }
        else if((*tasta) =='c')
        {
            return;
        }
        else
        {
            ok=0;
        }
    }while(!ok);  
}

int main()
{
    site *browser, *browser_filtrat;
    WINDOW *ecran;
    int size, cap, tasta, marime, k, id, tasta2;
    char cautare[101], **cuvinte;
    browser=formatare(&cap, &size);
    browser_filtrat=(site *)malloc((size+1)*sizeof(site)); 
    //Alocam memorie pentru un vector de site-uri
    initscr(); //Initializare ecran
    if(!has_colors())
    {
        //Verificam daca ecranul suporta culori
        endwin();
        fprintf(stderr,"Terminalul nu suporta culori.\n");
        exit(1);
    }
    ecran=newwin(0,0,0,0);
    noecho();
    start_color(); //Declaram ca ecranul sa foloseasca culori
    draw_startup_screen(ecran, &tasta);
    //Pentru a trece de la un ecran in altul, transmitem tasta 
    //si cand se apasa o tasta din LEGENDA, iesim din ecran 
    //si verificam ce tasta a fost apasata
    if(tasta=='c')
    {
        draw_search_screen(ecran, &tasta, cautare);
    }
    if(tasta=='a')
    {
        cuvinte=query(cautare, &marime);
        filtrare_site_advanced(browser_filtrat, &k, browser, size, cuvinte, marime);
        draw_menu_screen(ecran,browser_filtrat, k, &tasta, &id);
    }
    else if(tasta =='s')
    {
        cuvinte=query(cautare, &marime);
        filtrare_site_simple(browser_filtrat, &k, browser, size, cuvinte, marime);
        draw_menu_screen(ecran,browser_filtrat, k, &tasta, &id);
    }
    /*
      Pentru a creea ciclul infinit CAUTA <-> MENIU <-> SITE
      vom retine ultimele 2 taste apasate in tasta si tasta2 
      Astfel, secventa din while se traduce astfel:
        - Daca avem 'ENTER' in tasta2 si tasta 'b' inseamna ca suntem in pagina
        de site si vrem sa ne intoarcem la pagina de meniu
        - Daca nu avem 'ENTER' in tasta2 si avem tasta 'b' inseamna ca  
        suntem in pagina de meniu si vrem sa ne intoarcem la pagina de cautare
        - Din pagina de cautare, mergem secvential in ferestrele urmatoare, unde vom 
        restarta ciclul
    */
    tasta2=0;
    if(tasta=='\n')
    {
        tasta2='\n';
        draw_site_screen(ecran, browser_filtrat[id], &tasta);
    }
    while(tasta=='b')
    {
        if(tasta2!='\n')
        {
            draw_search_screen(ecran,&tasta,cautare);
            tasta2='\n';
        } 
        else 
        {
            draw_menu_screen(ecran,browser_filtrat, k, &tasta, &id);
            tasta2=0;
        }
        if(tasta=='a')
        {
            cuvinte=query(cautare, &marime);
            filtrare_site_advanced(browser_filtrat, &k, browser, size, cuvinte, marime);
            draw_menu_screen(ecran,browser_filtrat, k, &tasta, &id);
            tasta2=0;
        }
        else if(tasta =='s')
        {
            cuvinte=query(cautare, &marime);
            filtrare_site_simple(browser_filtrat, &k, browser, size, cuvinte, marime);
            draw_menu_screen(ecran,browser_filtrat, k, &tasta, &id);
            tasta2=0;
        }
        if(tasta=='\n')
        {
            draw_site_screen(ecran, browser_filtrat[id], &tasta);
            tasta2='\n';
        }
    }
    endwin(); //inchidem ecranul
    return 0;
}
