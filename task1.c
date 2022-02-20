#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Declararea tipului site ca struct
typedef struct 
{
    char url[201], titlu[201];
    char *continut;
    long long nr_accs, checksum;
}site;

//Functie ce transforma un sir de caractere in numar
long long numar(char *s)
{
    long long nr=0, p=1, i;
    for(i=strlen(s)-1;i>=0;i--)
    { 
        //Adunam cifra corespunzatoare caracterului dat, punand-o pe pozitie sa.
        nr+=((s[i]-'0')*p);
        p*=10;
    }
    return nr;
}

// Functie ce returneaza adresa de memorie a unui vector de site-uri parsate.
site *formatare(int *cap, int *size)
{
    site *browser=(site *)malloc(3*sizeof(site)); //Initializam vectorul cu 3 elemente
    *cap=3, *size=0; //Retinem capacitatea si nr de elemente ale vectorului
    char fisier[31], linie[101], *p;
    int nr_linie=0, k, marime_fisier=0, gasit=0, aux=0;
    FILE *master=fopen("master.txt","r"); //Deschidem fisierul master ce contine toate site-urile
    while(fgets(fisier,31,master))
    {
        if(fisier[strlen(fisier)-1]=='\n')
        {
            fisier[strlen(fisier)-1]='\0'; //Eliminam \n de la final
        }
        FILE *fp=fopen(fisier,"r"); // Deschidem site-ul citit
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
            //Daca ne aflam pe prima linie formatam unele date
            if(nr_linie==0)
            {
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
                //Alocam memorie pentru continut cu marimea fisierului aflata anterior
                browser[*(size)].continut=(char *)malloc(marime_fisier*sizeof(char));
            }
            if(nr_linie>0 && strstr(linie,"<p"))
            {
                //Vedem daca am gasit linia cu tag-ul de inceput de paragraf.
                gasit=1;
                aux=1;
            }
            if(gasit)
            {
                //Daca l-am gasit, pentru prima linie copiem ce se afla dupa >
                if(aux==1)
                {
                    strcpy(browser[*(size)].continut,strchr(linie,'>')+1);
                }
                //Pentru restul, le concatenam la prima
                else
                {
                    strcat(browser[*(size)].continut,linie);
                }
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
        (*size)++;
        fclose(fp); //inchidem fisierul din care am citit
    }
    fclose(master); //inchidem masterul
    return browser;
}

int main()
{
    site *browser;
    int i, size, cap;
    browser=formatare(&cap, &size);
    for(i=0;i<size;i++)
    {
        printf("%s %lld %s\n",browser[i].url,browser[i].nr_accs,browser[i].titlu);
    }
    for(i=0;i<size;i++)
    {
        free(browser[i].continut); //eliberam memoria alocata pentru continutul site-ului
    }
    free(browser); //eliberam memoria din browser
    return 0;
}