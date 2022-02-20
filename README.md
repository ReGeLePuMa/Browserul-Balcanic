Formatare : 2 ore
    Pasi de rezolvare:
        *Deschiderea master-ului
        *Citirea fiecarui site din master
        *Citirea fiecarei linii din site
        *Parsarea primei linii pentru a afla URL-ul, nr. de caractere, nr. de vizionari si checksum-ul
        *Parsarea titlului prin eliminarea tag-urilor <title> si </title>
        *Parsarea continutului prrin eliminarea tag-urilor <p> si </p>
        *Parsarea culorilor cu ajutorul unei enumeratii
        *Inchiderea fisierelor folosite
Task1 - 1 ora
    Pasi de rezolvare:
        *Aplicarea formatarii de mai sus 
        *Afisarea URL-ului, titlului si nr. de vizionari
        *Eliberarea memoriei alocate
Task2 - 2 ore
    Pasi de rezolvare:
        *Aplicarea formatarii de mai sus
        *Implementarea functiei comparator care interschimba in functie de cerinta datele
        *Implementarea BubbleSort pentru a sorta conform cerintei
        *Citirea de la tastatura a cuvintelor intr-o linie
        *Spargerea liniei in cuvinte, puse apoi intr-un vector de siruri
        *Filtrarea site-urilor dupa apararitia in continut a cuvintelor
        *Afisarea sortata a URL-urilor
        *Eliberarea memoriei alocate
Task3 - 2 ore
    Pasi de rezolvare:
        *Aplicarea formatarii de mai sus
        *Implementarea functiei comparator care interschimba in functie de cerinta datele, 
diferita de cea precedenta
        *Implementarea BubbleSort pentru a sorta conform cerintei
        *Citirea de la tastatura a cuvintelor intr-o linie
        *Spargerea liniei in cuvinte, inclusiv a secventei si a cuvantului de exclus,
puse apoi intr-un vector de siruri  
        *Filtrarea site-urilor dupa apararitia in continut a cuvintelor sau a secventei 
si care nu contin cuvantul exclus
        *Afisarea sortata a URL-urilor  
        *Eliberarea memoriei alocate            
Task4 - 2 ore
    Pasi de rezolvare:
        *Aplicarea formatarii de mai sus
        *Crearea unei functii ce transforma un nr. zecimal in binar pe 8 biti intr-un vector
        *Crearea unei functii ce transforma un nr. binar pe 8 biti in format vectorial intr-un nr. zecimal
        *Crearea functiei de rotire cu k pozitii la stanga
        *Crearea functiei de rotire cu k pozitii la dreapta
        *Crearea unei functii ce calculeaza checksum-ul conform formulei
        *Citirea pana la sfarsitul input-ului a URL-lor
        *Verificarea existentei site-ului cu pricina
        *Calcularea checksum-ului pentru site-ul gasit
        *Verifcarea checksum-ului calculat cu cel gasit la formatare
        *Eliberarea memoriei alocate
Browser - 15 ore (Rulati "make browser" pentru a-l compila)
    Pasi premergatori:
        *Citire documentatie ncurses
        *Vizualizare tutorial ncurses facut de catre indieni
    Pasi de rezolvare:
        *Aplicarea formatarii de mai sus
        *Initializarea ecranului si verificarea functionalitatii acestuia
        *Afisarea paginii de pornire
        *Afisarea paginii de cautare
        *Citrea cuvintelor pentru cautare
        *Filtrarea site-urilor dupa modul de cautare (avansat sau simplu) intr-un
alt vector 
        *Crearea paginii de meniu
            -creearea unei subferestre in care sa se afiseze meniul
            -creearea unui meniu si punerea in lista a vectorului filtrat
            -afisarea meniului pe ecran 
        *Afisarea paginii web a site-ului conform cu html sau        
