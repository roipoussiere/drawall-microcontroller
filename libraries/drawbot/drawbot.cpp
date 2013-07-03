/*
This work is licensed under the Creative Commons Attribution
- Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions
3.0 France License. To view a copy of this license, visit
http://creativecommons.org/licenses/by-nc-sa/3.0/fr/.
*/

#include <drawbot.h>

Draw::Draw(int distanceEntreMoteurs, int largeurFeuille, int hauteurFeuille, int posFeuilleX, int posFeuilleY)
{
    // dimentions de la surface
    
    //***************************
    // À placer dans l'en-tête !!
    //***************************
        
    mSheetWidth = largeurFeuille;
    mSheetHeight = hauteurFeuille;
    mDistanceBetweenMotors = distanceEntreMoteurs;
    mSheetPositionX = posFeuilleX;
    mSheetPositionY = posFeuilleY;

    // pour que ecrireOk() fonctionne la 1ere fois
    mWriting = true;
}

void Draw::commencer(void)
{    
    // pins en entrée
    pinMode(PIN_LEFT_CAPTOR, INPUT);
    pinMode(PIN_RIGHT_CAPTOR, INPUT);
    pinMode(PIN_REMOTE, INPUT);
    
    // pins en sortie
    pinMode(PIN_OFF_MOTORS, OUTPUT);
    pinMode(PIN_LEFT_MOTOR_STEP, OUTPUT);
    pinMode(PIN_LEFT_MOTOR_DIR, OUTPUT);
    pinMode(PIN_RIGHT_MOTOR_STEP, OUTPUT);
    pinMode(PIN_RIGHT_MOTOR_DIR, OUTPUT);
    pinMode(PIN_SCREEN_SCE, OUTPUT);
    pinMode(PIN_SCREEN_RST, OUTPUT);
    pinMode(PIN_SCREEN_DC, OUTPUT);
    pinMode(PIN_SCREEN_SDIN, OUTPUT);
    pinMode(PIN_SCREEN_SCLK, OUTPUT);
    // pin CS au moment de la lecture de la carte
    
    // Lie le servo-moteur à son pin
    mServo.attach(PIN_SERVO);
    
    // Initialisation carte SD
    initSD();
    
    if (mDistanceBetweenMotors < mSheetWidth + mSheetPositionX) {
        Serial.print("Erreur : La distance entre les 2 moteurs est inférieure à la largeur de la feuille + position.");
        return;
    }

    setRatioDist(1.00);

    // Par défault initialise la position du stylo au centre de la surface
    /**********************
    * À remplacer par setPosition(CENTER);
    ***********************/
    mPositionX = mSheetWidth / 2;
    mPositionY = mSheetHeight / 2;

    // calcul de la longueur des fils au début
    mLeftLength = filG(mPositionX, mPositionY);
    mRightLength = filD(mPositionX, mPositionY);

    // le point de départ de la 1ere figure est le point courant
    mStartCurveX = mPositionX;
    mStartCurveY = mPositionY;

    setRatioVitesse(1.00);
    
    // une petite vitesse par défaut
    // pour que le moteur bouge même si on ne spécifie rien
    setVitesse(10);

    // *** Affichage des informations ***
    
    Serial.print("position : ");
    Serial.print(mPositionX);
    Serial.print(" , ");
    Serial.println(mPositionY);
    
    Serial.print("gauche : ");
    Serial.print(mLeftLength);
    Serial.print(" , droite : ");
    Serial.println(mRightLength);

    // *** envoi des données d'initialisation à Processing ***

    // caractère pour commencer l'init
    Serial.print('\t');

    // surfaceL, surfaceH, G, D    
    Serial.print(mDistanceBetweenMotors);
    Serial.print(',');
    Serial.print(mSheetPositionY + mSheetHeight + 100);
    
    Serial.print(',');
    Serial.print(mLeftLength);
    Serial.print(',');
    Serial.print(mRightLength);
    
    Serial.print(',');
    Serial.print(mScaleX);

    // limG, limD, limH, limB
    Serial.print(',');
    Serial.print(mSheetPositionX);
    Serial.print(',');
    Serial.print(mDistanceBetweenMotors - mSheetPositionX - mSheetWidth);
    Serial.print(',');
    Serial.print(mSheetPositionY);
    Serial.print(',');
    Serial.print(100);
    
    // caractère de fin d'init
    Serial.print('\n');
    
/*    echL = mSheetHeight / valNb("width");
    echH = mSheetWidth / valNb("height");

    if (echL > echH)
    {
        setRatioDist(echH);
    }
    else
    {
        setRatioDist(echL);
    }
*/

    // Pause jusqu'à l'appui sur le BP.
    // Serial.print(">Appuyez sur le bouton pour commencer<");
    // while(digitalRead(PIN_BP) == LOW) {}

    // alimente le moteur
    alimenter(true);
}

float Draw::getaX(void)
{
    return mPositionX;
}

float Draw::getaY(void)
{
    return mPositionY;
}

// renvoie la longueur du fil (en pas) en fonction de la position actuelle
long Draw::getA(void)
{
    return mLeftLength;
}

long Draw::getB(void)
{
    return mRightLength;
}

// vitesse du moteur (en tr/min)
void Draw::setVitesse(float vitesse)
{
    // delai entre chaque pas, en micro-secondes
    mDelay = (mSpeedScale)*60000000 / (vitesse*float(STEPS));
}

void Draw::setaXY(float aX, float aY)
{
    mPositionX = aX;
    mPositionY = aY;
}

// xx(mm)*ratio --> xx(pas)
// xx(pas)/ratio --> xx(mm)
void Draw::setRatioDist(float ratioDist)
{
    // ratio calculé en fonction du diametre moteur et du nb de pas
    mScaleX = ratioDist * float(STEPS) / (PI*DIAMETER);
}

void Draw::setRatioVitesse(float ratioVitesse)
{
    mSpeedScale = ratioVitesse;
}

// renvoie la longueur du fil (en pas) en fonction de la position donnée (en mm)
long Draw::filG(float x, float  y)
{
    return sqrt ( pow((mSheetPositionX + x) * mScaleX, 2)
    + pow((mSheetPositionY + y) * mScaleX, 2) );
}

long Draw::filD(float x, float  y)
{
    return sqrt ( pow((mDistanceBetweenMotors - mSheetPositionX - x) * mScaleX, 2)
    + pow((mSheetPositionY + y) * mScaleX, 2) );
}

void Draw::alimenter(bool alimenter)
{
    if (alimenter) {
        digitalWrite(PIN_OFF_MOTORS, LOW);
        // Processing: a=alimenter
        Serial.print('a');
    }
    else {
        digitalWrite(PIN_OFF_MOTORS, HIGH);
        // éloigne le stylo avant de couper tout
        ecrire(false);
        
        // Processing: b=désalimenter
        Serial.print('b');
    }
}

// approche ou eloigne le stylo pour écrire ou non
void Draw::ecrire(bool ecrireOk)
{
    // si on veut ecrire et que le stylo n'ecrit pas
    if (ecrireOk && !mWriting) {
        delay(DELAY_BEFORE_SERVO);
        mServo.write(MIN_SERVO);
        delay(DELAY_AFTER_SERVO);
        
        // Processing: w = ecrire
        Serial.print('w');

        mWriting = true;
    }
    
    // si on ne veut pas ecrire et que le stylo ecrit
    else if (!ecrireOk && mWriting) {
        delay(DELAY_BEFORE_SERVO);
        mServo.write(MAX_SERVO);
        delay(DELAY_AFTER_SERVO);

        // Processing: x = ne pas ecrire    
        Serial.print('x');
        mWriting = false;
    }
}

void Draw::ligne(float bX, float bY, bool ecrit)
{
    // aX, aY: Position du point courant
    // aG, aD: Longueur actuelle des fils.

    // bX, bY: position du point de destination.
    // bG, bD: Longueur des fils à la destination.

    // nbPasG, nbPasD: Nombre de pas à faire pour aller à destination.

    // delaiG, delaiD: Delai avant chaque pas.

    // 1) on calcule la longueur des fils à la destination
    // 2) on fait la différence par rapport à la longueur actuelle
    // 3) Calcule le délai en fonction du rapport de longeur des fils

    // stoque la position voulue
    // avant qu'elle soit modifiée par les limites
    mFictivePosX = bX;
    mFictivePosY = bY;

    // contrôle des limites, n'ecris pas si en dehors
    if (bX < 0 || bX > mSheetWidth || bY < 0 || bY > mSheetHeight) {
        if (bX < 0)
            bX = 0;
        else if (bX > mSheetWidth)
            bX = mSheetWidth;
        
        if (bY < 0)
            bY = 0;
        else if (bY > mSheetHeight)
            bY = mSheetHeight;

        ecrire(false);
    } else {
        // s'il est dedans et qu'il doit ecrire, écrit
        if (ecrit) {
            ecrire(true);
        } else {
            ecrire(false);
        }
    }

    // longueur fils à la destination (en pas)
    long bG = filG(bX, bY);
    long bD = filD(bX, bY);

    // nombre de pas à faire
    long nbPasG = bG-mLeftLength;
    long nbPasD = bD-mRightLength;

    bool sensGHaut = true;
    bool sensDHaut = true;

    float delaiG;
    float delaiD;

    long dernierTempsG;
    long dernierTempsD;

    // calcul de la direction
    if (nbPasG < 0) {
        sensGHaut = false;
    }

    if (nbPasD < 0) {
        sensDHaut = false;
    }

    // on a le sens
    // donc on peut retirer le signe pour simplifier les calculs
    nbPasG = fabs(nbPasG);
    nbPasD = fabs(nbPasD);

    if (nbPasG > nbPasD) {
        delaiG = mDelay;
        delaiD = mDelay * (float(nbPasG) / float(nbPasD));
    } else {
        delaiD = mDelay;
        delaiG = mDelay * (float(nbPasD) / float(nbPasG));
    }

    dernierTempsG = micros();
    dernierTempsD = micros();
    
    if (sensGHaut) {
        digitalWrite(PIN_LEFT_MOTOR_DIR, LEFT_DIRECTION);
    } else {
        digitalWrite(PIN_LEFT_MOTOR_DIR, !LEFT_DIRECTION);
    }
    
    if (sensDHaut) {
        digitalWrite(PIN_RIGHT_MOTOR_DIR, RIGHT_DIRECTION);
    } else {
        digitalWrite(PIN_RIGHT_MOTOR_DIR, !RIGHT_DIRECTION);
    }
    
    while(nbPasG > 0 || nbPasD > 0)
    {
        // si le delai est franchi et qu'il reste des pas à faire
        if ((nbPasG > 0) && (micros() - dernierTempsG >= delaiG)) {
            // stoque le temps actuel dans lastTimer
            dernierTempsG = micros();
            
            // incremente ou decremente (en fonction de la direction)
            if (sensGHaut) {
                mLeftLength++;
                Serial.print('L');
            } else {
                mLeftLength--;
                Serial.print('l');
            }
            
            // decremente le nb de pas restants
            nbPasG--;
            
            // Effectue le pas
            pasG();
        }

        if ((nbPasD > 0) && (micros() - dernierTempsD >= delaiD)) {
            // stoque le temps actuel dans lastTimer
            dernierTempsD = micros();
            
            // incremente ou decremente (en fonction de la direction)
            if (sensDHaut) {
                mRightLength++;
                Serial.print('R');
            } else {
                mRightLength--;
                Serial.print('r');
            }
            
            // decremente le nb de pas restants
            nbPasD--;
            
            // Effectue le pas
            pasD();
        }
    }

    mPositionX = bX;
    mPositionY = bY;
}

//******************************************
// À modifier, il ne faut pas faire de pause
//******************************************

void Draw::pasG()
{
    digitalWrite(PIN_LEFT_MOTOR_STEP, true);
    delayMicroseconds(50);
    digitalWrite(PIN_LEFT_MOTOR_STEP, false);
    
}

void Draw::pasD()
{
    digitalWrite(PIN_RIGHT_MOTOR_STEP, true);
    delayMicroseconds(50);
    digitalWrite(PIN_RIGHT_MOTOR_STEP, false);
}

void Draw::ligneABS(float x, float y)
{
    mQuadraticCurveX = 0;
    mQuadraticCurveY = 0;
    mCubicCurveX = 0;
    mCubicCurveY = 0;
    
    int longmax = 20;
    
    float longX = abs(x-mPositionX);
    float longY = abs(y-mPositionY);

    float miniX;
    float miniY;
    int boucle;

    if (longX > longmax || longY > longmax) {
    
        if( longX > longY ) {
            boucle = ceil(longX/longmax);
        } else {
            boucle = ceil(longY/longmax);
        }
            
        miniX = ((x-mPositionX) / boucle);
        miniY = ((y-mPositionY) / boucle);
        
        for(int i=0; i<boucle; i++) {
            ligne(mPositionX + miniX, mPositionY + miniY, true);
        }
    }
    
    // si c'est une petite longueur, on y va directement
    // et puis pour finir au cas ou ça tombe pas juste avec les boucles
    ligne(x, y, true);
}

void Draw::ligneREL(float x, float y)
{
    ligneABS(mPositionX + x , mPositionY + y);
}

void Draw::finir(void)
{
    ligneABS(mStartCurveX, mStartCurveY);
}

void Draw::deplacerABS(float x, float y)
{
    ligne(x, y, false);
    
    mStartCurveX = x;
    mStartCurveY = y;
}

void Draw::deplacerREL(float x, float y)
{
    deplacerABS(mPositionX+x, mPositionY+y);
}

void Draw::centrer(void)
{
    deplacerABS(mSheetWidth/2 , mSheetHeight/2);
}

void Draw::horizABS(float y)
{
    deplacerABS(0, y);
    ligneABS(mSheetWidth, y);
}

void Draw::horizREL(float y)
{
    horizABS(mPositionY+y);
}

void Draw::vertiABS(float x)
{
    deplacerABS(x, 0);
    ligneABS(x, mSheetHeight);
}

void Draw::vertiREL(float x)
{
    vertiABS(mPositionX+x);
}

void Draw::bezierCubABS(float x1, float y1, float x2, float y2, float x, float y)
{
    // B(t) = P0(1-t)^3 + 3*P1*t(1-t)^2 + 3*P2*t^2*(1-t) + P3*t^3
    // t=[0,1]

    // P0 = pt source
    // P1-P2 = pts de controles
    // P3 = pt cible

    float x0 = mPositionX;
    float y0 = mPositionY;
    float ptx, pty;
    float t;

    for(t=0; t<=1; t+=0.01) {
        ptx = x0*pow((1-t),3) + 3*x1*t*pow(1-t, 2) + 3*x2*pow(t, 2)*(1-t) + x*pow(t,3);
        pty = y0*pow((1-t),3) + 3*y1*t*pow(1-t, 2) + 3*y2*pow(t, 2)*(1-t) + y*pow(t,3);
        ligne(ptx, pty, true);
    }
    
    // finit le dernier trajet au cas ou ça ne tombe pas juste
    ligne(x, y, true);

    mQuadraticCurveX = 0;
    mQuadraticCurveY = 0;
    mCubicCurveX = x2;
    mCubicCurveY = y2;
}

void Draw::bezierCubABS(float x2, float y2, float x, float y)
{
    float x1, y1;
    
    if(mCubicCurveX == 0 && mCubicCurveY == 0) {
        x1 = x;
        y1 = y;
    } else {
        x1 = 2*mFictivePosX-mCubicCurveX;
        y1 = 2*mFictivePosY-mCubicCurveY;
    }
    
    bezierCubABS(x1, y1, x2, y2, x, y);
}

void Draw::bezierCubREL(float x1, float y1, float x2, float y2, float x, float y)
{
    bezierCubABS(mPositionX+x1, mPositionY+y1, mPositionX+x2, mPositionY+y2, mPositionX+x, mPositionY+y);
}

void Draw::bezierCubREL(float x2, float y2, float x, float y)
{
    bezierCubABS(mPositionX+x2, mPositionY+y2, mPositionX+x, mPositionY+y);
}

void Draw::bezierQuadABS(float x1, float y1, float x, float y)
{
    // B(t) = P0*(1-t)^2 + 2*P1*t(1-t) + P2*t^2
    // t=[0,1]

    // P0 = pt source
    // P1 = pt de controle
    // P2 = pt cible

    float x0 = mPositionX;
    float y0 = mPositionY;
    float ptx, pty;

    for(float t=0; t<=1; t+=0.01) {
        ptx = x0*pow(1-t, 2) + 2*x1*t*(1-t) + x*pow(t, 2);
        pty = y0*pow(1-t, 2) + 2*y1*t*(1-t) + y*pow(t, 2);
        ligne(ptx, pty, true);
    }
    ligne(x, y, true);

    mCubicCurveX = 0;
    mCubicCurveY = 0;
    mQuadraticCurveX = x1;
    mQuadraticCurveY = y1;
}

void Draw::bezierQuadABS(float x, float y)
{
    float x1, y1;
    if(mQuadraticCurveX == 0 && mQuadraticCurveY == 0) {
        x1 = x;
        y1 = y;
    } else {
        x1 = 2*mFictivePosX - mQuadraticCurveX;
        y1 = 2*mFictivePosY - mQuadraticCurveY;
    }

    bezierQuadABS(x1, y1, x, y);
}

void Draw::bezierQuadREL(float x1, float y1, float x, float y)
{
    bezierQuadABS(mPositionX+x1, mPositionY+y1, mPositionX+x, mPositionY+y);
}

void Draw::bezierQuadREL(float x, float y)
{
    bezierQuadABS(mPositionX+x, mPositionY+y);
}

void Draw::arcABS(float a1, float a2, float a3, float a4, float a5, float a6, float a7)
{
    // à compléter
}

void Draw::arcREL(float a1, float a2, float a3, float a4, float a5, float a6, float a7)
{
    // à compléter
}

void Draw::ellipse(float rx, float ry)
{
    // ratio pour déterminer
    // la distance du point de controle en fonction du rayon
    float k = 0.551915;

    // pour simplifier les formules
    float x = mPositionX;
    float y = mPositionY;

    // se déplace à gauche du cercle
    deplacerABS(x-rx, y);

    bezierCubABS(x-rx,y-ry*k , x-rx*k,y-ry , x,y-ry);
    bezierCubABS(x+rx,y-ry*k , x+rx,y);
    bezierCubABS(x+rx*k,y+ry , x,y+ry);
    bezierCubABS(x-rx,y+ry*k , x-rx,y);

    // revient au centre du cercle
    deplacerABS(x, y);
}

void Draw::cercle(float r)
{
    ellipse(r, r);
}

/******************************
* fonctions pour lire les svg *
******************************/

int Draw::initSD()
{
    // pin 10 en sortie pour etre sur qu'il ne sera pas utilisé
    pinMode(PIN_SD_CS, OUTPUT);

    if (!SD.begin(PIN_SD_CS)) {
        // Err. 01 : Carte absente ou non reconnue.
        return 01;
    }
    return 0;
}

// recuperation des valeurs sous forme de chaine
void Draw::val(const char * requette, char * valeur)
{
    // requette: ce que l'on veut obtenir, ex: (svg)
    // requ: requette + (=") , ex: (svg=")
    // valeur: valeur de la requette
    // ex: (21cm) ou (fill:none;stroke:#000000)

    // crée une chaine de la meme taille que valeur (+2 pour (=") )
    char requ[strlen(requette)+2];
    
    char car = ' ';
    int i = 0;
    
    // copie la chaine cherchée et ajoute le (=")    
    strcpy(requ, requette);
    strcat(requ, "=\"");

    // se positionne à la fin de la requette cherchée + (=")
    // pour récupérer la valeur de la requette
    trouveSD(requ);
    
    // tant qu'il y a qqch à lire
    // et qu'on n'est pas rendu au (") correspondant à la fin de la valeur
    while( mFile.available() && car != '"') {
        // lis
        car = mFile.read();

        // stocke le car.
        valeur[i] = car;

        i++;
    }

    // quand on a fini, remplace le (") par le car. de fin de chaine.
    valeur[i-1] = '\0';
}

// Renvoie la valeur du nombre.
// Pre-condition : Le curseur doit être positionné au début d'un nombre
// Parcours le nombre jusqu'au prochain caractère qui n'est pas un chiffre.
// Converti le nombre en float

bool Draw::estChiffre(char car)
{
    int i;

    // caractères considérés comme étant des chiffres
    const char * chiffres = "-.0123456789";

    // parcours le tableau de chiffres
    for(i=0; chiffres[i] != '\0'; i++) {
        // si c'est un chiffre, renvoie vrai
        if (car == chiffres[i]) {
            return true;
        }
    }
    
    // si on est là c'est qu'aucun chiffre n'a été trouvé, renvoie faux
    return false;
}

float Draw::valNb(const char * requette)
{
    // nombre de max 20 caractères
    char chaine[20+1];

    // se positionne en début de fichier
    mFile.seek(0);

    // recuperation des valeurs sous forme de chaine
    val(requette, chaine);

    // unité de mesure de la valeur : (cm) ou (mm)
    char unite[3];

    // chaine contenant le nombre : (-3456.345)
    char nbCh[strlen(chaine)];

    // chaine convertie en float
    float nbFl;

    int i;

    // taille des 2 chaines
    int tNbCh = 0;
    int tUnite = 0;
    
    // parcourt toute la chaine.
    for(i=0; chaine[i] != '\0'; i++) {
        if ( estChiffre(chaine[i]) ) {
            // remplit nbCh avec les chiffres
            nbCh[tNbCh] = chaine[i];

            tNbCh++;
        }

        else {
            unite[tUnite] = chaine[i];
            tUnite++;
        }
    }

    // met la fin de chaine
    nbCh[tNbCh] = '\0';
    unite[tUnite] = '\0';

    // converti la chaine en float
    nbFl = atof(nbCh);

    // On converti les unités en px (unités utilisateur) :

    // px (aucun changement)
    if (!strcmp(unite, "px")) {
        nbFl *= 1;
    }

    // pt (*1.25)
    else if (!strcmp(unite, "pt")) {
        nbFl *= 1.25;
    }
    
    // pc (*15)
    else if (!strcmp(unite, "pc")) {
        nbFl *= 15;
    }

    // mm (*3.543307)
    else if (!strcmp(unite, "mm")) {
        nbFl *= 3.543307;
    }

    // cm (*35.43307)
    else if (!strcmp(unite, "cm")) {
        nbFl *= 35.43307;
    }

    // in (*90)
    else if (!strcmp(unite, "in")) {
        nbFl *= 90;
    }

    // si l'unité n'est pas spécifiée, c'est l'unité utilisateur par défaut
    else {
        nbFl *= 1;
    }

    return nbFl;
}

boolean Draw::trouveSD(const char * mot)
{
    int i = 0;
    char car;
        
    // tant qu'il y a qqch à lire
    while( mFile.available() ) {
        car = mFile.read();
        
        // si le caractère correspond, passe au suivant
        if (car == mot[i]) {
            i++;

            // si fin du mot (mot trouvé)
            if(mot[i] == '\0') {
                // on s'est bien positionné, opération réeussie! :)
                return true;
            }
        }
        // si un des car. n'est pas bon on reprends depuis le debut
        else {
            i=0;
        }
    }

    // si on est là c'est qu'on a pas trouvé le mot, fail!
    return false;
}

void Draw::dessiner(void)
{
    char car;

    // tableau contenant les valeurs à lançer dans la requette
    // (max 7 valeurs avec la fonction arc() )
    float tNb[7];

    // tant qu'il y a qqch à lire
    while( mFile.available() ) {
        car = mFile.read();

        // Appelle les fonctions de dessin svg en fonction du caractère détecté
        switch (car) {
            // deplacerABS (2 args)
            case 'M':
                // lis les paramètres tant qu'il y a des nombres
                do {
                    // la fonction deplacerABS() attends 2 paramètres
                    // on les récupère sous forme de tableau (tNb)
                    params(tNb, 2);

                    // appel de la fonction
                    deplacerABS (tNb[0], tNb[1]);
                }
                // teste le prochain car (le curseur ne bouge pas)
                while ( estChiffre(mFile.peek()) );

            break;

            // deplacerREL (2 args)
            case 'm':
                do {
                    params(tNb, 2);
                    deplacerREL (tNb[0], tNb[1]);
                }
                while ( estChiffre(mFile.peek()) );
            break;

            // finir (aucun arg)
            case 'Z':
                finir ();
            break;

            // finir (aucun arg)
            case 'z':
                finir ();
            break;

            // ligneABS (2 args)
            case 'L':
                do {
                    params(tNb, 2);
                    ligneABS (tNb[0], tNb[1]);
                }
                while ( estChiffre(mFile.peek()) );

            break;

            // ligneREL (2 args)
            case 'l':
                do {
                    params(tNb, 2);
                    ligneREL (tNb[0], tNb[1]);
                }
                while ( estChiffre(mFile.peek()) );
            break;

            // horizABS (1 arg)
            case 'H':
                do {
                    params(tNb, 1);
                    horizABS (tNb[0]);
                }
                while ( estChiffre(mFile.peek()) );
            break;

            // horizREL (1 arg)
            case 'h':
                do {
                    params(tNb, 1);
                    horizREL (tNb[0]);
                }
                while ( estChiffre(mFile.peek()) );
            break;

            // vertiABS (1 arg)
            case 'V':
                do {
                    params(tNb, 1);
                    vertiABS (tNb[0]);
                }
                while ( estChiffre(mFile.peek()) );
            break;

            // vertiABS (1 arg)
            case 'v':
                do {
                    params(tNb, 1);
                    vertiREL (tNb[0]);
                }
                while ( estChiffre(mFile.peek()) );
            break;
            
            // bezierCubABS (6 args)
            case 'C':
                do {
                    params (tNb, 6);
                    bezierCubABS (tNb[0], tNb[1], tNb[2], tNb[3], tNb[4], tNb[5]);
                }
                while ( estChiffre(mFile.peek()) );
            break;

            // bezierCubREL (6 args)
            case 'c':
                do {
                    params (tNb, 6);
                    bezierCubREL (tNb[0], tNb[1], tNb[2], tNb[3], tNb[4], tNb[5]);
                }
                while ( estChiffre(mFile.peek()) );
            break;
            
            // bezierCubABS (4 args)
            case 'S':
                do {
                    params (tNb, 4);
                    bezierCubABS (tNb[0], tNb[1], tNb[2], tNb[3]);
                }
                while ( estChiffre(mFile.peek()) );
            break;

            // bezierCubREL (4 args)
            case 's':
                do {
                    params (tNb, 4);
                    bezierCubREL (tNb[0], tNb[1], tNb[2], tNb[3]);
                }
                while ( estChiffre(mFile.peek()) );
            break;
            
            // bezierQuadABS (4 args)
            case 'Q':
                do {
                    params (tNb, 4);
                    bezierQuadABS (tNb[0], tNb[1], tNb[2], tNb[3]);
                }
                while ( estChiffre(mFile.peek()) );
            break;

            // bezierQuadREL (4 args)
            case 'q':
                do {
                    params (tNb, 4);
                    bezierQuadREL (tNb[0], tNb[1], tNb[2], tNb[3]);
                }
                while ( estChiffre(mFile.peek()) );
            break;
            
            // bezierQuadABS (2 args)
            case 'T':
                do {
                    params (tNb, 2);
                    bezierQuadABS (tNb[0], tNb[1]);
                }
                while ( estChiffre(mFile.peek()) );
            break;

            // bezierQuadREL (2 args)
            case 't':
                do {
                    params (tNb, 2);
                    bezierQuadREL (tNb[0], tNb[1]);
                }
                while ( estChiffre(mFile.peek()) );
            break;
            
            // arcABS (7 args)
            case 'A':
                do {
                    params (tNb, 7);
                    arcABS (tNb[0], tNb[1], tNb[2], tNb[3], tNb[4], tNb[5], tNb[6]);
                }
                while ( estChiffre(mFile.peek()) );
            break;

            // arcREL (7 args)
            case 'a':
                do {
                    params (tNb, 7);
                    arcREL (tNb[0], tNb[1], tNb[2], tNb[3], tNb[4], tNb[5], tNb[6]);
                }
                while ( estChiffre(mFile.peek()) );
            break;
            
            // si on détecte la fin du contenu de "d"
            // on a parcouru toute les données, ça a reeussi !!
            case '"':
                return;
            break;
            
            // si ce n'est pas une lettre de déplacement
            default:
                // Serial.print(car);
            break;
        }
    
    }

    // si on est là c'est qu'on a parcouru tout le fichier
    // sans trouver le (") de fin de la balise (d) : fail

    // Err. 11 : Le fichier svg est incomplet.
    Serial.print("E11");
    return;
}

void Draw::svg(char* nomFichier)
{
    mFile = SD.open(nomFichier);

    if (!mFile) {
        // Err. 02 : Erreur d'ouverture de fichier.
        Serial.print("E02");
        return;
    }

    // se positionne en début de fichier
    mFile.seek(0);

    // Se positionne jusqu'à la balise SVG
    // Si on ne la trouve pas, on renvoie une erreur
    if (! trouveSD("<svg") ) {
        // Err. 12 : Le fichier n'est pas un fichier svg.
        Serial.print("E12");
        return;
    }

    // Se positionne jusqu'à la balise PATH
    // Si on ne la trouve pas, on renvoie une erreur
    if (! trouveSD("<path") ) {
        // Err. 13 : Le fichier svg n'inclut aucune donnée de dessin.
        Serial.print("E13");
        return;
    }

    // tant que l'on trouve le début des données d'un traçé, on dessine
    while ( trouveSD("d=\"") ) {
        dessiner();
    }
    
    mFile.close();
    centrer();
    alimenter(false);
    Serial.print("n");
}

void Draw::params(float * tNb, int nbParams)
{
    // chaine qui va contenir le nombre à convertir en float
    // ex: (23456.43532)
    char valeur[20];

    char car;

    int i, j;

    for (i=0; i < nbParams; i++) {
        // passe les espaces et virgules
        do {
            car = mFile.read();
        }
        while (car == ' ' || car == ',');

        // tant que les car. sont des chiffres, lis et stoque dans une chaine
        for (j=0; estChiffre(car); j++) {
            valeur[j] = car;
            car = mFile.read();
        }
        valeur[j] = '\0';

        tNb[i] = atof(valeur);
    }
}
