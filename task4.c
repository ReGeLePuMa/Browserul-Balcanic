#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Declararea tipului site ca struct
typedef struct 
{
    char url[201], titlu[201];
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

// Functie ce returneaza adresa de memorie a unui vector de site-uri parsate.
site *formatare(int *cap, int *size)
{
    //Initializam vectorul cu 3 elemente
    site *browser=(site *)malloc(3*sizeof(site));
    *cap=3, *size=0; //Retinem capacitatea si nr de elemente ale vectorului
    char fisier[31], linie[101], *p;
    int nr_linie, k, marime_fisier;
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
                browser[*(size)].continut=(char *)malloc((marime_fisier+1)*sizeof(char));
                //Alocam memorie pentru continut cu marimea fisierului aflata anterior
            }
            //Copiem sintaxa HTML in continut
            if(nr_linie==1)
            {
                strcpy(browser[*(size)].continut,linie);
            }
            //Concatenam restul codului HTML la continut
            if(nr_linie>1)
            {
                strcat(browser[*(size)].continut,linie);
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

//Functie ce transforma un numar din decimal in binar si il pune intr-un vector
void dec_to_bin(int n,int s[])
{
	int i;
	for(i=0;i<8;i++)
	{
		s[i]=0;
	}
	i=7;
	while(n)
	{
		s[i]=n%2;
		i--; 
		n/=2;
	}
}

//Functie ce transforma un numar stocat in format vectorial din binar in zecimal
int bin_to_dec(int s[])
{
	int nr=0,i;
	for(i=7; i>=0;i--)
	{
		nr+=(s[i]*(1<<(7-i))); 
    }
	return nr;
}

//Functie ce roteste bitii caracterului x cu k pozitii la stanga
int rotl(char x, int k)
{
    int i, j, n, nr[9], aux;
    n=(int)x;
    dec_to_bin(n,nr);
    for(i=1;i<=k;i++)
    {
        aux=nr[0]; //stocam prima valoare
        for(j=1;j<8;j++)
        {
            nr[j-1]=nr[j]; //mutam la stanga cu 1 pozitie fiecare casuta ce permite aceasta operatie
        }
        nr[7]=aux; //punem in ultima casuta prima valoare
    }
    return (bin_to_dec(nr));
}

//Functie ce roteste bitii caracterului x cu k pozitii la dreapta
int rotr(char x, int k)
{
    int i, j, n, nr[9], aux;
    n=(int)x;
    dec_to_bin(n,nr);
    for(i=1;i<=k;i++)
    {
        aux=nr[7]; //stocam ultima valoare
        for(j=7;j>=1;j--)
        {
            nr[j]=nr[j-1]; //mutam la dreapta cu 1 pozitie fiecare casuta ce permite aceasta operatie
        }
        nr[0]=aux; //punem in prima casuta ultima valoare
    }
    return (bin_to_dec(nr));
} 

//Functie ce calculeaza checksum-ul unui site dupa formula
int checksum(char *s)
{
    int sum, i;
    sum=rotl(s[0],0);
    for(i=1;i<strlen(s);i++)
    {
        //Daca pozitia este impara facem XOR cu bitii caracterul rotiti la dreapta cu pozitia lui in sir 
        if(i%2==1)
        {
            sum^=(rotr(s[i],i));
        }
        //Daca pozitia  este para facem XOR cu bitii caracterul rotiti la stanga cu pozitia lui in sir
        else sum^=(rotl(s[i],i));
    }
    return sum;
}

int main()
{
    site *browser;
    int i, size, cap, poz;
    char nume[101];
    browser=formatare(&cap, &size);
    //Introducem numele site-urilor pana la EOF
    while(scanf("%s",nume)!=EOF)
    {
        poz=-1;
        for(i=0;i<size;i++)
        {
            if(!strcmp(browser[i].url,nume))
            {
                poz=i;
                break;
            }
        }
        if(poz==-1)
        {
            printf("Website not found!\n");
        }
        //Verificam securitatea site-ului
        else
        {
            if(checksum(browser[poz].continut)==browser[poz].checksum)
            {
                printf("Website safe!\n");
            }
            else
            {
                printf("Malicious website! Official key: %d. Found key: %d\n",browser[poz].checksum,checksum(browser[poz].continut));
            }
        }
    } 
    for(i=0;i<size;i++)
    {
        free(browser[i].continut);
    }
    free(browser);
    return 0;
}