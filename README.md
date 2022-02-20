#                                                                           Browserul balcanic

## Prezentare generala

Browserul balcanic reprezinta solutia proprie pentru rezolvarea temei 2 (https://pastebin.com/ghX0st69) din cadrul cursului de *PCLP* al *Facultatii de Automatica si Calculatoare* din *Universitatea Politehnica Bucuresti*. Numele de *balcanic* provine de la culorile folosite pentru diferitele ecrane care se aseamana cu culorile steagurilor unor tari din peninsula.

Implementarea este realizata cu ajutorul bibliotecii **NCurses** (https://invisible-island.net/ncurses/man/ncurses.3x.html) si limbajului de programare **C**, iar functionalitatea acestuia se bazeaza pe cele 4 task-uri precedente (task1.c task2.c task3.c task4.c).

Pentru a instala **NCurses** pe un sistem linux tip *Debian* rulati comanda:
```
sudo apt-get install libncurses5-dev
```

Pentru a compila browseruk rulati comanda:
```
make browser
```

Pentru a compila doar task 1-4 rulati comanda:
```
make build
```

Pentru a sterge toate fisierele obiect si executabile rulati comanda:
```
make clean
```

## Prezentarea browserului

I. Pagina de pornire

![ecran1](https://user-images.githubusercontent.com/93268175/154868653-518c3010-be2d-4a6f-a572-06cecace34b0.JPG)

Pagina de pornire este bazata pe steagul statului cvasi-independent Kosovo (nu este recunoscut de Romania). 
Sunt afisate numele aplicatiei, autorul precum si o lista cu comenzile valabile pe acea pagina.
Prin apasarea tastei **Q** se iese din aplicatie iar prin apasarea taste **C** se intra in modul de cautare.

II. Pagina de cautare

![ecran2](https://user-images.githubusercontent.com/93268175/154868882-94b82992-3de5-485e-b33e-f4d872bd5dc6.JPG)

Pagina de cautare este bazata pe steagul Albaniei.
In cadrul acestei paginii, utilizatorul poate introduce un sir de caractere in bara de cautare pentru a gasi site-uri ce contin acel sir.
La fel ca si in pagina precedenta avem o lista de comenzi.
Dupa introducerea sirului, acesta are posibilitatea de a alege o cautare simpla prin apasarea tastei **S** sau o cautare avansata prin apasarea tastei **A**.
Cautarea simpla presupune gasirea a cel putin un cuvant din intreg sirul in continutul site-urilor, iar cautarea avansata presupune folosirea unei sintaxe similara cu cea folosita de *Google*: 
1. "*text*" pentru cautarea exacta a unei secventa
2. -*text*  pentru excluderea unui cuvant din sir
La apasarea tastei **Q** se va inchide browserul.

III. Pagina de rezultate

![meniu](https://user-images.githubusercontent.com/93268175/154869199-c97d8bb5-a0e8-42a9-833f-0d576dec8284.JPG)

Pagina de rezultate este bazata pe steagul Greciei.
Se vor afisa **URL**-urile site-urilor rezultate cautarii de pe pagina anteroara.
Prin folosirea sagetilor **↑** si **↓** se poate naviga prin meniu.
Folosind tasta **ENTER** se intra in pagina selectata.
La apasarea tastei **B** se intoarce la pagina de cautare.

IV. Pagina site-ului

![ecran4](https://user-images.githubusercontent.com/93268175/154869376-a112bb81-a374-4b31-9b96-f82f53faf30c.JPG)

Spre deosebire de celelalte paginii, formatarea acestei ferestre se bazeaza pe **HTML** sursa a fiecarui site. 
In susul paginii, centrat si boldat se afla titulul, apoi continutul site-ului.
Culorile se bazeaza pe un element **CSS** din cadrul fisierului .html (in cazul in care acesta lipseste se foloseste schema clasica negru pe alb).
La apasarea tastei **B** se revine la pagina de rezultate.



