# 3D Moottorin kehitys oppimisprojektina
## Oppimispäiväkirja
<img width="500" height="250" alt="image" src="https://github.com/user-attachments/assets/ef4e2841-ec8a-4b56-8d1a-86a2cb0db14b" />

<img width="500" height="250" alt="image" src="https://github.com/user-attachments/assets/7f8830ca-94b4-4424-9282-0bbfd3fdae43" />

###   Johdanto
Minua on aina kiehtonut, miten 3D - grafiikka toimii ja miten kaksiulotteiselle näytölle voi luoda illuusion kolmannesta ulottuvuudesta. Shaderien, tekstuurien ja 3D-tiedostojen toimintaperiaate oli minulle pitkään mysteeri. Tässä projektissa perehdyn näihin aiheisiin ja rakennan 3D-moottorin C-kielellä tyhjästä käyttäen vain OpenGL-, CJSON- ja stb_image-kirjastoja. 

### Osa 1: Ikkunan luominen openGL kirjastolla ja testi kolmio
Aloitin projektin luomien ohjelmoimalla openGL kontekstin ja ikkunan main.c tiedostoon. Ja loin renderer.c tiedoston, jossa aloin tekemään logiikkaa. Ensimmäisen grafiikka piirrosta varten. Koodasin alustavan shader-ohjelman, joka piirtää suuren keltaisen kolmion näytölle. Tämän testikolmion tarkoitus oli selvittää kuinka OpenGL shaderit piirtää näytölle. Kaikki 3D objectit yleensä koostuvat kolmioista tai neliöistä, joten tämä oli jo merkittävä askel projektissa. Seuraavaksi oli vuorossa luoda matriisioperatiot, jotka käsittelisivät 3D tiedostojen sijainti pisteitä.

<img width="358" height="333" alt="image" src="https://github.com/user-attachments/assets/b7b51086-1412-461b-a411-640990fbc068" />

### Osa 2: Matriisi operaatiot 

Loin matrix_utils.c tiedoston, jonne aloin luomaan matriisi operaatioita. Matriisien ja vectoreiden käsittelyyn. Loin perus kertolasku ja ristitulo funktiot seka funktiot jotka luo identiteettimatriisin, skaalausmatriisin, rotaatio matriisin ja scaalaus matriisit joita voi kertoa keskenään tehdäkseen eri operaatioita.

Saatat miettiä, miten näitä matriiseja käytetään. Ajatellaan, vaikka että meillä on neliö joka koostuu kahdeksasta pisteestä. Näillä kaikilla pisteillä on kolme float arvoa, jotka kuvaavat niiden paikkaa kolmeulotteisessa ympäristössä. Kun kerromme niitä lukuja näillä matriiseilla, voimme liikuttaa, scaalata ja kääntää näitä obejekteja näillä matriisi operaatioilla.

4x4 identiteetti matriisi: 

$`
\begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1
\end{bmatrix}
`$

4x4 skaalaus matriisi: 


$`
\begin{bmatrix}
s_x & 0 & 0 & 0 \\
0 & s_y & 0 & 0 \\
0 & 0 & s_z & 0 \\
0 & 0 & 0 & 1
\end{bmatrix}
`$

4x4 transformaatio matriisi: 

$`
\begin{bmatrix}
1 & 0 & 0 & t_x \\
0 & 1 & 0 & t_y \\
0 & 0 & 1 & t_z \\
0 & 0 & 0 & 1
\end{bmatrix}
`$



### Osa 3: Kamera, projektiomatriisi ja näkymämatriisi

Voi pojat nyt tulee monimutkaista juttua. Seuraavana vaiheena oli luoda näkymä matriisi ja projektio matriisi, joita käytetään mallin sijainnin hallitsemiseen ja piirtämiseen suhteessa kameraan. Loin tiedostot projection.c ja cameracontrols.c. Näkymä matriisi luotiin cameracontrols.c tiedostossa, jonka tarkoitus on hallita kameran kulmaa ja paikkaa eli tämän matriisin tarkoitus on periaatteessa tarkoitus kertoa minne kamera katsoo. Loin myös projektio matriin, joka muuttaa objectien kolmeuloitteiset koordinaatit 2D näytölle sopiviksi sijainneiksi.
<img width="300" height="300" alt="image" src="https://github.com/user-attachments/assets/8d9c35eb-e9d2-4434-b463-76c2c3c36ac8" />


### Osa 4: Shaderien luominen (vertex ja fragment shader)
Seuraavaksi siirryttiin päivittämään shader koodia. Loin ensiksi simppelin vertex ja fragment shaderin, joiden tarkoituksena oli käyttää näitä matriiseja ja näyttää näytölle ensimmäinen 3D-renderöinti. Syötin grafiikkapuskuriin renderöitävä testiobjektin tietoja. Puskurin rakenne oli [sijaintiX,sijaintiY,sijaintiZ,normaaliX,nomaaliY,normaaliZ,väriR,väriG,väriB ....]. Eli, jokaiselle kolmiolle oli 8 indeksiä puskurissa.
Tein simppelin koodin, joka kertoo kolmioiden sijainnit näkymä, malli ja projektio matriiseilla, jotta objektit näkyisivät oikeilla paikoilla suhteessa kameraan ja sain luotua ensimmäisen renderöintini. Loin myös simppelin direktional light koodin, joka laskee varjostuksia kolmioiden kulmien avulla.

<img width="400" height="250" alt="image" src="https://github.com/user-attachments/assets/532dc6e2-0ab8-44fb-a67b-d84d4dd877c2" />

### Osa 5: 3D tiedostojen lataaminen grafiikka puskuriin ja Ensimmäinen 3D renderöinti
Tiedostojen hardcoodaaminen oli huono tapa, tehdä koodista uudelleen käytettävää ja halusin uuden tavan käsitellä useita objekteja 3D-rederissäni, joten loin scene configuraatio tiedoston json muodossa, joka on helposti
luettava ja muokattava. Loin myös OBJ_loader.c tiedoston obj tiedostojen lataamiseen, jota kutsun sceneloader.c tiedotossa, kaikille tiedostoille json scene config tiedostossa. Tämä helpotti tiedostojen ja tekstuurien hallinnasta huomattavasti helpompaa myöhemmsässä vaiheessa.

OBJ_loader.c tarkoitus on lukea .obj muotoine tiedosto, ja tallentaa sen tiedot MeshObj sructiin. Alla structin rakenne.
Object loader ottaa myös huomioon 3D-mallit, jotka koostuvat kolmioiden sijaan neliöistä.


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

Tätä dataa käyttäen pystymme luoda objectn, sisältää 3D-mallinmatriisin, tekstuurin tunnisteen ja vao-puskurin eli puskurin, joka sisältää tiedot kolmioiden kärkipisteistä, normaaleista ja tekstuurien koordinaateista. Lissin myös myöhemmin flagin, joka kertoo haluammeko objectin reagoivan valaistukseen.

    typedef struct {
        GLuint vao;
        int vertexCount;
        float modelMatrix[16];
        GLuint textureID;
        bool castsShadows;
    
    } RenderableObject;



sceneloader.c tiedosto hakee kaikki objectit Scene.json tiedostosta ja käyttää obj_loader.c logiikkaa muuttaakseen ne ohjelman vaatimaan muotoon. Alla esimerkki scene.json tiedostosta.

<img width="300" height="250" alt="image" src="https://github.com/user-attachments/assets/cc4397c8-5413-4385-98da-27572af6e1fd" />



### Osa 6: Tekstuurit
Tekstuurit oli haastava osa projektia. OpenGl onneksi tarjosi tähän hyviä funktioita ja StbImage kirjasto auttoi kuvien lataamisessa. 
Olin aiemmin jo luonut tesktuurien koordinaateille syöttämislogikaan videopuskuriin, joten minun vain tarvitsi ladata tekstuuri videomuistiin 3D tiedostojen lataamisen yhteydessä ja aina valita tekstuuri sen mukaan     mitä objektia renderöidään.

<img width="200" height="200" alt="image" src="https://github.com/user-attachments/assets/e34ce901-4bbc-42ac-8d20-227e9c6906bb" />

Kun lisäsin yhden objektin lisää scene.json tiedostoon huomasin, että tekstuurien kordinaatit eivät toimi kunnolla ja tekstuurit ovat väärässä kohdassa. Ikkunassa oli tiiliä, joka ei ollut aivan suunnitelman mukainen lopputulos.

<img width="200" height="160" alt="image" src="https://github.com/user-attachments/assets/b9ca2dbf-96aa-426d-bebd-2295a3bb6e2c" />

Mutta kääntämällä vertikaalisen kordinaatin ympäri sain tekstuurit asettumaan oikein.

<img width="200" height="160" alt="image" src="https://github.com/user-attachments/assets/9171bcbe-0cc5-489b-b65e-c76163f1b96c" />


### Osa 7: Datan valmistelu Lambert diffusionia varten ja toteutus

Seuraava vaihe oli laskea pehmeät normaalit kaikille mallin kärkipisteille, jotta valaistus näyttäisi luonnollisemmalta. Tein tähän ComputeSmoothNormals funktion.
Aluksi funktio tarkistaa, onko normaaliarvot jo tallennettuna tiedostoon. Jos tiedosto löytyy ja sen koko vastaa mallin kärkipisteiden määrää, normaaleja ei tarvitse laskea uudestaan, vaan ne ladataan suoraan muistissa olevaan mesh-rakenteeseen. Tämä nopeuttaa ohjelman käynnistystä, kun samoja malleja renderöidään monta kertaa.

Jos tiedostoa ei löydy tai se ei vastaa mallia, normaaleja lasketaan “lennosta”. Jokaiselle kolmiolle lasketaan ensin kolmion normaali ristitulon avulla kolmion kärkipisteiden perusteella. Sen jälkeen kaikki samaan paikkaan sijoittuvat kärkipisteet ryhmitellään hash-taulukkoon. Tämä mahdollistaa smooth normalsien laskemisen niin, että vain lähes samansuuntaiset kolmiot otetaan huomioon, jolloin kovien reunojen kulmat säilyvät terävinä.

Tämän ansiosta 3D-mallien valaistus näyttää pehmeämmältä ja luonnollisemmalta, erityisesti pyöreissä tai monimutkaisissa pinnoissa.

**Ennen**

<img width="200" height="559" alt="200" src="https://github.com/user-attachments/assets/8f65e9c5-6d10-4f42-b9a8-7fe6bb628e4f" />

**Jälkeen**

<img width="200" height="200" alt="image" src="https://github.com/user-attachments/assets/0b287842-44dd-48a6-9145-be5068aa6904" />

### Osa 8: Taivaslaatikko ja ambientlight
Lopuksi loin taivaslaatikon, joka on periaatteessa pallo, jonka sisällä on taivaan tekstuuri. Koodasin logiikan, että taivaslaatikkoon ei tule varjoja ja lisäsin ympäristön valaistusta ja lopputulos oli upea.
<img width="1467" height="966" alt="image" src="https://github.com/user-attachments/assets/1741ee2d-6201-4429-9704-d6846e168b96" />

<img width="1907" height="981" alt="image" src="https://github.com/user-attachments/assets/7f8830ca-94b4-4424-9282-0bbfd3fdae43" />
<img width="1467" height="966" alt="image" src="https://github.com/user-attachments/assets/516bea8b-c07e-427b-9d13-ffa1bb30a562" />


### Lopputulos

Projektin myötä opin koko pipeline:n 3D-grafiikassa:  
- Miten matriisit liikuttavat ja kiertävät objekteja  
- Miten shaderit käyttävät matriiseja renderöintiin  
- Kuinka smooth normals ja Lambert-diffuusio saavat valaistuksen näyttämään luonnolliselta  
- Tekstuurien hallinnan ja scene-konfiguraation merkityksen  

Tämä oli oppimisprojekti, mutta lopputulos on toimiva 3D-moottori, joka osaa ladata malleja, renderöidä ne oikein, valaista ja teksturoida realistisesti. Kaikki tehtiin alusta asti itse, mikä auttoi ymmärtämään 3D-grafiikan perusteet käytännössä.




https://github.com/user-attachments/assets/2888b319-7a1b-4f50-87e8-def301fa494c

