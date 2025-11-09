# 3D Moottorin kehitys oppimisprojektina
##  Johdanto
Minua on aina kiehtonut, miten 3D - grafiikka toimii ja miten kaksiulotteiselle näytölle voi luoda illuusion kolmannesta ulottuvuudesta. Shaderien ,tekstuurien ja 3D-tiedostojen toiminta periaate oli myös minulle mysteeri. Tässä projektissa perehdyn näihin asioihin ja rakennan 3D-moottorin tyhjästä hakemistosta, käyttäen vain openGL, CJson ja StbImage kirjastoja.

## Osa 1: Ikkunan luominen openGL kirjastolla ja testi kolmio
Aloitin projektin luomien ohjelmoimalla openGL kontekstin ja ikkunan main.c tiedostoon. Ja loin renderer.c tiedoston, jossa aloin tekemään logiikkaa. Ensimmäisen grafiikka piirrosta varten. Koodasin alustavan shaderi ohjelman, joka piirtää suuren keltaisen kolmion näytölle. Tämän testikolmion tarkoitus oli selvittää kuinka OpenGL shaderit piirtää näytölle. Kaikki 3D objectit yleensä koostuvat kolmioista tai neliöistä, joten tämä oli jo merkittävä askel projektissa. Seuraavaksi oli vuorossa luoda matriisi operatiot, jotka käsittelisivät 3D tiedostojen sijainti pisteitä.


<img width="358" height="333" alt="image" src="https://github.com/user-attachments/assets/b7b51086-1412-461b-a411-640990fbc068" />

## Osa 2: Matriisi operaatiot 

Loin matrix_utils.c tiedoston, jonne aloin luomaan matriisi operaatioita. Matriisien ja vectoreiden käsittelyyn. Loin perus kertolasku ja ristitulo funktiot seka funktiot jotka luo identiteettimatriisin, skaalausmatriisin, rotaatio matriisin ja scaalaus matriisit joita voi kertoa keskenään tehdäkseen eri operaatioita.

Saatat miettiä, miten näitä matriiseja käytetään. Ajatellaan, vaikka että meillä on neliö joka koostuu kahdeksasta pisteestä. Näillä kaikilla pisteillä on kolme float arvoa, jotka kuvaavat niiden paikkaa kolmeulotteisessa ympäristössä. Kun kerromme niitä lukuja näillä matriiseilla, voimme liikuttaa, scaalata ja kääntää näitä obejekteja näillä matriisi operaatioilla.


## Osa 3: Kamera, projektiomatriisi ja näkymämatriisi

Voi pojat nyt tulee monimutkaista juttua. Seuraavana vaiheena oli luoda näkymä matriisi ja projektio matriisi, joita käytetään mallin sijainnin hallitsemiseen ja piirtämiseen suhteessa kameraan. Loin tiedostot projection.c ja cameracontrols.c. Näkymä matriisi luotiin cameracontrols.c tiedostossa, jonka tarkoitus on hallita kameran kulmaa ja paikkaa eli tämän matriisin tarkoitus on periaatteessa tarkoitus kertoa minne kamera katsoo. Loin myös projektio matriin, joka muuttaa objectien kolmeuloitteiset koordinaatit 2D näytölle sopiviksi sijainneiksi.

## Osa 4: Shaderien luominen (vertex ja fragment shader)
Seuraavaksi siirryttiin päivittämään shader koodia. Loin ensiksi simppelin vertex ja fragment shaderin, joiden tarkoituksena oli käyttää näitä matriiseja ja näyttää näytölle ensimmäinen 3D-renderöinti. Syötin grafiikkapuskuriin renderöitävä testiobjektin tietoja. Puskurin rakenne oli [sijaintiX,sijaintiY,sijaintiZ,normaaliX,nomaaliY,normaaliZ,väriR,väriG,väriB ....]. Eli, jokaiselle kolmiolle oli 8 indeksiä puskurissa.
Tein simppelin koodin, joka kertoo kolmioiden sijainnit näkymä, malli ja projektio matriiseilla, jotta objektit näkyisivät oikeilla paikoilla suhteessa kameraan ja sain luotua ensimmäisen renderöintini. Loin myös simppelin direktional light koodin, joka laskee varjostuksia kolmioiden kulmien avulla.

<img width="400" height="250" alt="image" src="https://github.com/user-attachments/assets/532dc6e2-0ab8-44fb-a67b-d84d4dd877c2" />

## Osa 5: 3D tiedostojen lataaminen grafiikka puskuriin
Tiedostojen hardcoodaaminen oli huono tapa, tehdä koodista uudelleen käytettävää ja halusin uuden tavan käsitellä useita objekteja 3D-rederissäni, joten loin scene configuraatiotiedoston json muodossa, joka on helposti
luettava ja muokattava. Loin myös OBJ_loader.c tiedoston obj tiedostojen lataamiseen, jota kutsun sceneloader.c tiedotossa, kaikille tiedostoille json scene config tiedostossa. Tämä helpotti tiedostojen ja tekstuurien hallinnasta huomattavasti helpompaa myöhemmsässä vaiheessa.

OBJ_loader.c  tarkoitus on lukea .obj muotoine tiedosto, ja tallentaa sen tiedot ObjectVectoriin , joka koostuu MeshObj structeista, jotka sisältävät tiedot
verticistä, vertexien määrästä, normaleista, indiceistä, indexeistä, triangleista, vertex 


    typedef struct {
        float* vertices;     
        size_t vertex_count;  
    
        float* texcoords;     
        size_t texcoord_count; 
    
        float* normals;       
        size_t normal_count;  
    
        int* indices;         
        size_t index_count;   
    
        float* triangle_vertices; 
        size_t triangle_vertex_count;
    
        float transform[16];
    
        GLuint textureID;
    
        float color[3]; 
    } ObjMesh;
<img width="300" height="250" alt="image" src="https://github.com/user-attachments/assets/cc4397c8-5413-4385-98da-27572af6e1fd" />

## Osa 6: Ensimmäinen 3D renderöinti

## Osa 7: Scene konfiguraatio tiedosto ja objektien transform matriisit

## Osa 7: Useamman tiedoston lataaminen

## Osa 8: Yksisuuntainen valaistus

## Osa 9: Lamber diffusion

## Osa 10: Tekstuurit

## Osa 11: Taivaslaatikko


<img width="1173" height="873" alt="image" src="https://github.com/user-attachments/assets/01f34af0-1827-46d9-abfa-95856a68bcb7" />
<img width="1900" height="962" alt="image" src="https://github.com/user-attachments/assets/e34ce901-4bbc-42ac-8d20-227e9c6906bb" />
<img width="1170" height="868" alt="image" src="https://github.com/user-attachments/assets/b9ca2dbf-96aa-426d-bebd-2295a3bb6e2c" />
<img width="1185" height="886" alt="image" src="https://github.com/user-attachments/assets/9171bcbe-0cc5-489b-b65e-c76163f1b96c" />
<img width="1907" height="981" alt="image" src="https://github.com/user-attachments/assets/7f8830ca-94b4-4424-9282-0bbfd3fdae43" />
