# 3D Moottorin kehitys oppimisprojektina
##  Johdanto
Minua on aina kiehtonut, miten 3D - grafiikka toimii ja miten kaksiulotteiselle näytölle voi luoda illuusion kolmannesta ulottuvuudesta. Shaderien ,tekstuurien ja 3D-tiedostojen toiminta periaate oli myös minulle mysteeri. Tässä projektissa perehdyn näihin asioihin ja rakennan 3D-moottorin tyhjästä hakemistosta, käyttäen vain openGL, CJson ja StbImage kirjastoja.

## Osa 1: Ikkunan luominen openGL kirjastolla ja testi kolmio
Aloitin projektin luomien ohjelmoimalla openGL kontekstin ja ikkunan main.c tiedostoon. Ja loin renderer.c tiedoston, jossa aloin tekemään logiikkaa. Ensimmäisen grafiikka piirrosta varten. Koodasin alustavan shaderi ohjelman, joka piirtää suuren keltaisen kolmion näytölle. Tämän testikolmion tarkoitus oli selvittää kuinka OpenGL shaderit piirtää näytölle. Kaikki 3D objectit yleensä koostuvat kolmioista tai neliöistä, joten tämä oli jo merkittävä askel projektissa. Seuraavaksi oli vuorossa luoda matriisi operatiot, jotka käsittelisivät 3D tiedostojen sijainti pisteitä.


<img width="358" height="333" alt="image" src="https://github.com/user-attachments/assets/b7b51086-1412-461b-a411-640990fbc068" />

## Osa 2: Matriisi operaatiot 

Loin matrix_utils.c tiedoston, jonne aloin luomaan matriisi operaatioita. Matriisien ja vectoreiden käsittelyyn. Loin perus kertolasku ja ristitulo funktiot seka funktiot jotka luo identiteettimatriisin, skaalausmatriisin, rotaatio matriisin ja scaalaus matriisit joita voi kertoa keskenään tehdäkseen eri operaatioita.
Saatat miettiä, miten näitä matriiseja käytetään. Ajatellaan, vaikka että meillä on neliö joka koostuu kahdeksasta pisteestä. Näillä kaikilla pisteillä on kolme float arvoa, jotka kuvaavat niiden paikkaa kolmeulotteisessa ympäristössä. Kun kerromme niitä lukuja näillä matriiseilla, voimme liikuttaa, scaalata ja kääntää näitä obejekteja näillä matriisi operaatioilla.


## Osa 3: Kamera ja orbitaalikontrollit

## Osa 4: Shaderien luominen (vertex ja fragment shader)

## Osa 5: 3D tiedostojen lataaminen grafiikka puskuriin

## Osa 6: Ensimmäinen 3D renderöinti

## Osa 7: Scene konfiguraatio tiedosto ja objektien transform matriisit

## Osa 7: Useamman tiedoston lataaminen

## Osa 8: Yksisuuntainen valaistus

## Osa 9: Lamber diffusion

## Osa 10: Tekstuurit

## Osa 11: Taivaslaatikko


<img width="1524" height="769" alt="image" src="https://github.com/user-attachments/assets/b525a123-0617-4bc8-8d3e-27cf10a463e1" />
<img width="1173" height="873" alt="image" src="https://github.com/user-attachments/assets/01f34af0-1827-46d9-abfa-95856a68bcb7" />
<img width="1900" height="962" alt="image" src="https://github.com/user-attachments/assets/e34ce901-4bbc-42ac-8d20-227e9c6906bb" />
<img width="1170" height="868" alt="image" src="https://github.com/user-attachments/assets/b9ca2dbf-96aa-426d-bebd-2295a3bb6e2c" />
<img width="1185" height="886" alt="image" src="https://github.com/user-attachments/assets/9171bcbe-0cc5-489b-b65e-c76163f1b96c" />
<img width="1907" height="981" alt="image" src="https://github.com/user-attachments/assets/7f8830ca-94b4-4424-9282-0bbfd3fdae43" />
