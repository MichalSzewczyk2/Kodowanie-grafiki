#include <exception>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
using namespace std;

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;
SDL_Surface* bmp = NULL;

#define tytul "Projekt"

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel (int x, int y);
SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface);

SDL_Color paleta[16];
int czarnapaleta[16] = {0};
char paletamode = ' ';

int wysokosc = -1;
int szerokosc = -1;


void pokazPalete() {

    for(int i =0; i<16;i++){
        cout<<i+1<<": ["<<(int)paleta[i].r<<","<<(int)paleta[i].g<<","<<(int)paleta[i].b<<"]"<<endl;
    }
    for(int y = 0; y < 32 ; y++){
        for(int x = 0 ; x < szerokosc/16 ; x++){
            for(int n =0;n<16;n++){
                setPixel(x+((szerokosc/16)*n),y,paleta[n].r,paleta[n].g,paleta[n].b);
            }
        }
    }
    SDL_UpdateWindowSurface(window);
}

void tworzPalete(){
    SDL_Color kolor;
    kolor.r = 0;
    kolor.g = 0;
    kolor.b = 0;
    paleta[0] = kolor;//0000
    kolor.r = 0;
    kolor.g = 0;
    kolor.b = 255;
    paleta[1] = kolor;//0001
    kolor.r = 0;
    kolor.g = 85;
    kolor.b = 0;
    paleta[2] = kolor;//0010
    kolor.r = 0;
    kolor.g = 85;
    kolor.b = 255;
    paleta[3] = kolor;//0011
    kolor.r = 0;
    kolor.g = 170;
    kolor.b = 0;
    paleta[4] = kolor;//0100
    kolor.r = 0;
    kolor.g = 170;
    kolor.b = 255;
    paleta[5] = kolor;//0101
    kolor.r = 0;
    kolor.g = 255;
    kolor.b = 0;
    paleta[6] = kolor;//0110
    kolor.r = 0;
    kolor.g = 255;
    kolor.b = 255;
    paleta[7] = kolor;//0111
    kolor.r = 255;
    kolor.g = 0;
    kolor.b = 0;
    paleta[8] = kolor;//1000
    kolor.r = 255;
    kolor.g = 0;
    kolor.b = 255;
    paleta[9] = kolor;//1001
    kolor.r = 255;
    kolor.g = 85;
    kolor.b = 0;
    paleta[10] = kolor;//1010
    kolor.r = 255;
    kolor.g = 85;
    kolor.b = 255;
    paleta[11] = kolor;//1011
    kolor.r = 255;
    kolor.g = 170;
    kolor.b = 0;
    paleta[12] = kolor;//1100
    kolor.r = 255;
    kolor.g = 170;
    kolor.b = 255;
    paleta[13] = kolor;//1101
    kolor.r = 255;
    kolor.g = 255;
    kolor.b = 0;
    paleta[14] = kolor;//1110
    kolor.r = 255;
    kolor.g = 255;
    kolor.b = 255;
    paleta[15] = kolor;//1111

    cout<<"Tworze palete"<<endl;
    //pokazPalete();

    paletamode = 'N';
}

void tworzCzarnaPalete(){
    float C = 16.0;
    SDL_Color BW;
    for(float i = 0.0; i < 16.0; i++){
        float bw = (0.299 * C*i - 1.0) + (0.587 * C*i - 1.0) + (0.114 * C*i - 1.0);
        BW.r = (Uint8) bw;
        BW.g = (Uint8) bw;
        BW.b = (Uint8) bw;

        paleta[(int)i] = BW;
    }
    paletamode = 'B';
}


void RLE(int wejscie[], int dlugosc){

    int datasize = wysokosc*szerokosc;

    int* dane = new int[datasize];

    int i =0;
    int n =0;
    int ile = 0;

    //dopóki wszystkie bajty nie są zdekompresowane
    while(i<dlugosc){

        //sekwencja powtarzających się bajtów
        if(wejscie[i]>0){
            for(int j = 0; j<wejscie[i] ; j++){
                cout<<wejscie[i+1]<<", ";
                dane[n]=wejscie[i+1];
                n++;
            }
            //przesuwamy wskaźnik o długość sekwencji
            i+=2;
        }
        //sekwencja różnych bajtów
        else{
            ile = wejscie[i+1];
            for(int j = 0; j<ile;j++){
                cout<<wejscie[i+1+j+1]<<", ";
                dane[n]=wejscie[i+1+j+1];
                n++;
            }
            //Przesuwamy wskaźnik o długość sekwencji
            i += ile+2;
            if(ile%2 != 0){
                i++;
            }
        }

    }

    n=0;

     for(int x = 0; x < szerokosc;x++){
            for(int y = 0; y < wysokosc;y++){
                setPixel(x,y,paleta[dane[n]].r,paleta[dane[n]].g,paleta[dane[n]].b);
                n++;
            }
                SDL_UpdateWindowSurface(window);
        }

}

int* dekompresjaRLE(int wejscie[], int dlugosc, int heigth, int width){


    int* dane = new int[heigth*width];
    cout<<"heigth*width: "<<heigth*width<<endl;
    int licznik = 0;


    int i =0;
    int ile = 0;

    //dopóki wszystkie bajty nie są zdekompresowane
    while(i<dlugosc and licznik <heigth*width){

        //sekwencja powtarzających się bajtów
        if(wejscie[i]>0){
            for(int j = 0; j<wejscie[i] ; j++){
               // cout<<wejscie[i+1]<<", ";
                dane[licznik]=wejscie[i+1];
                licznik++;
            }
            //przesuwamy wskaźnik o długość sekwencji
            i+=2;
        }
        //sekwencja różnych bajtów
        else{
            ile = wejscie[i+1];
            for(int j = 0; j<ile;j++){
                //cout<<wejscie[i+1+j+1]<<", ";
                dane[licznik]=wejscie[i+1+j+1];
                licznik++;
            }
            //Przesuwamy wskaźnik o długość sekwencji
            i += ile+2;
            if(ile%2 != 0){
                i++;
            }
        }
       //cout<<" : "<<licznik<<" : ";
    }

    return dane;
}

void odczytajPlik(char* nazwa){
    int kolor1 = 0;
    int kolor2 = 0;
    Uint8 wartosc = 0;
    Uint8 wartoscbis = 0;
    int dlugosc = 0;
    int width = 0;
    int heigth = 0;
    char mode[] = "  ";
    char tryb = ' ';


    ifstream wejscie(nazwa, ios::binary);
    wejscie.read((char*)&dlugosc,sizeof(int));
    wejscie.read((char*)&width,sizeof(int));
    wejscie.read((char*)&heigth,sizeof(int));
    wejscie.read((char*)&mode,sizeof(char)*2);
    wejscie.read((char*)&tryb,sizeof(char));

    szerokosc = width;
    wysokosc = heigth;

    cout<<"dlugosc: "<<dlugosc<<endl;
    cout<<"width: "<<width<<endl;
    cout<<"heigth: "<<heigth<<endl;
    cout<<"mode: "<<mode<<endl;
    cout<<"paleta: "<<tryb<<endl;

        window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width+1, heigth+1, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
            return ;
        }
        screen = SDL_GetWindowSurface(window);
        if (screen == NULL) {
            fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
            return ;
        }
        SDL_UpdateWindowSurface(window);

    if(mode[0]=='C'&&mode[1]=='P'){
        int data[dlugosc]={0};
        Uint8 tmp = 0;
        if(tryb == 'N'){
            tworzPalete();
            pokazPalete();
        }
        else if(tryb == 'B'){
            tworzCzarnaPalete();
        }
        else if(tryb == 'D'){
                SDL_Color tmp;
                for(int i = 0; i < 16; i++){
                    wejscie.read((char*)&tmp.r,sizeof(Uint8));
                    wejscie.read((char*)&tmp.g,sizeof(Uint8));
                    wejscie.read((char*)&tmp.b,sizeof(Uint8));
                    paleta[i]=tmp;
                }
        }
        else {
            cout<<"Error!"<<endl;
            return;
        }
        for(int i = 0; i<dlugosc;i++){
            wejscie.read((char*)&tmp,sizeof(Uint8));
            data[i]=tmp;
        }

        int* decompressed = new int;
        decompressed = dekompresjaRLE(data,dlugosc,heigth,width);
        int licznik = 0;
        cout<<"po petli"<<endl;

        for(int x = 0; x < szerokosc;x++){
            for(int y = 0; y < wysokosc;y++){
                setPixel(x,y,paleta[decompressed[licznik]].r,paleta[decompressed[licznik]].g,paleta[decompressed[licznik]].b);
                licznik++;
            }
                SDL_UpdateWindowSurface(window);
        }
        SDL_UpdateWindowSurface(window);
        char a =' ';
        cout<<"Zapisac obraz do postaci .bmp? [y/n]: ";
        cin>>a;
        if(a == 'y'){
            char name[] = "          ";
            cout<<"Podaj nazwe pliku: ";
            cin>>name;

            SDL_SaveBMP(screen, name);
        }
    }
    else if(mode[0]=='N' && mode[1]=='C'){

        if(tryb == 'N'){
            tworzPalete();
            pokazPalete();
        }
        else if(tryb == 'B'){
            tworzCzarnaPalete();
        }
        else if(tryb == 'D'){
                SDL_Color tmp;
                for(int i = 0; i < 16; i++){
                    wejscie.read((char*)&tmp.r,sizeof(Uint8));
                    wejscie.read((char*)&tmp.g,sizeof(Uint8));
                    wejscie.read((char*)&tmp.b,sizeof(Uint8));
                    paleta[i]=tmp;
                }
        }
        else {
            cout<<"Error!"<<endl;
            return;
        }

        for(int x = 0; x < szerokosc  ;x++){
            for(int y = 0; y < wysokosc  ;y+=2){
                kolor1 = 0;
                kolor2 = 0;
                wejscie.read((char*)&wartosc, sizeof(Uint8));
                wartoscbis = wartosc;
                kolor1 = wartosc /16;
                kolor2 = wartoscbis - kolor1*16;
                setPixel(x,y,paleta[kolor1].r,paleta[kolor1].g,paleta[kolor1].b);
                setPixel(x,y+1,paleta[kolor2].r,paleta[kolor2].g,paleta[kolor2].b);

            }
            SDL_UpdateWindowSurface(window);
        }
        SDL_UpdateWindowSurface(window);

        char a =' ';
        cout<<"Zapisac obraz do postaci .bmp? [y/n]: ";
        cin>>a;
        if(a == 'y'){
            char name[] = "          ";
            cout<<"Podaj nazwe pliku: ";
            cin>>name;

            SDL_SaveBMP(screen, name);

        }
    }
}

void zapiszPalete(ofstream& wyjscie){

    for(int i = 0; i < 16 ; i++){
        wyjscie.write((char*)&paleta[i].r,sizeof(Uint8));
        wyjscie.write((char*)&paleta[i].g,sizeof(Uint8));
        wyjscie.write((char*)&paleta[i].b,sizeof(Uint8));
    }
}

void zapisDoPliku(int* data, int dlugosc, bool compressed){

    Uint8 zapisz = 0;
    string name;
    cout<<"Podaj nazwe obrazka do zapisania: ";
    cin>>name;

    cout<<"name: "<<name<<endl;

    if(compressed){

        ofstream wyjscie(name, ios::binary);

        wyjscie.write((char*)&dlugosc,sizeof(int));
        wyjscie.write((char*)&szerokosc,sizeof(int));
        wyjscie.write((char*)&wysokosc,sizeof(int));

        char mode[] = "CP";
        wyjscie.write((char*)&mode,sizeof(char)*2);
        wyjscie.write((char*)&paletamode,sizeof(char));

        if(paletamode == 'D'){
            zapiszPalete(wyjscie);
        }
        for(int i = 0; i<dlugosc; i++){
           zapisz = data[i];
           wyjscie.write((char*)&zapisz,sizeof(Uint8));
        }
        wyjscie.close();
    }
     else{
        ofstream wyjscie(name, ofstream::out);

        wyjscie.write((char*)&dlugosc,sizeof(int));
        wyjscie.write((char*)&szerokosc,sizeof(int));
        wyjscie.write((char*)&wysokosc,sizeof(int));

        char mode[] = "NC";
        wyjscie.write((char*)&mode,sizeof(char)*2);
        wyjscie.write((char*)&paletamode,sizeof(char));

        if(paletamode == 'D'){
            zapiszPalete(wyjscie);
        }

        for(int i = 0; i < dlugosc; i+=2){
            zapisz = 0;
            zapisz = data[i];
            zapisz = zapisz*16;
            zapisz += data[i+1];
            wyjscie.write((char*)&zapisz,sizeof(Uint8));
        }
        wyjscie.close();
    }
}

void kompresjaRLE(int* wejscie){

    int i = 0;
    int length = 0;
    int dlugosc = szerokosc*wysokosc;
    int* compressed = new int[dlugosc];
    while(i < dlugosc){

            if((i<dlugosc-1) && (wejscie[i] == wejscie[i+1])){
            int j = 0;
            while((i+j<dlugosc-1)&&(wejscie[i+j]==wejscie[i+j+1])and(j<254)){
                j++;
            }
            //cout<<j+1<<", "<<wejscie[i+j]<<", ";
            compressed[length]=j+1;
            length++;
            compressed[length]=wejscie[i+j];
            length++;
            i += (j+1);
        }
        else{
            int j = 0;
            while((i+j < dlugosc-1)&&(wejscie[i+j]!=wejscie[i+j+1])and(j<254)){
                j++;
            }
            if((i+j == dlugosc -1)and(j<254)){
                j++;
            }
            //cout<<(int)0<<", "<< j <<"j, ";
            compressed[length]=0;
            length++;
            compressed[length]=j;
            length++;
            for(int k = 0; k<j ;k++){
               // cout<<wejscie[i+k]<<", ";
                compressed[length]=wejscie[i+k];
                length++;
            }
            if ( j%2  != 0){
              // cout<<(int)0<<", ";
                compressed[length]=0;
                length++;
            }
            i += j;
        }

    }
    if(length<dlugosc){
        cout<<"Zapis skompresowany"<<endl;
        zapisDoPliku(wejscie, dlugosc, true);
    }
    else{
        cout<<"Zapis nieskompresowany"<<endl;
        zapisDoPliku(compressed, length, true);
    }
}


int findClosest(SDL_Color a){
    int blad = 100000;
    int color = -1;
    int tmp = 0;
    for(int i = 0; i < 16; i++){
        tmp  = abs(paleta[i].r - a.r);
        tmp += abs(paleta[i].g - a.g);
        tmp += abs(paleta[i].b - a.b);
        if(tmp < blad){
            blad = tmp;
            color = i;
        }
    }
    return color;
}

void dithering(){

    SDL_Color oldpixel;
    SDL_Color newpixel;

    int* pixelTab = new int [szerokosc*wysokosc];
    int o = 0;

    float rerror = 0.0;
    float gerror = 0.0;
    float berror = 0.0;

    int d = 1;
    float** rpixel = new float* [szerokosc+2];
    for(int i = 0; i<szerokosc+2; i++){
        rpixel[i] = new float[wysokosc+2];
        for(int n = 0; n < wysokosc+2; n++){
            rpixel[i][n] = 0.0;
        }
    }
    float** gpixel = new float* [szerokosc+2];
    for(int i = 0; i<szerokosc+2; i++){
        gpixel[i] = new float[wysokosc+2];
        for(int n = 0; n < wysokosc+2; n++){
            gpixel[i][n] = 0.0;
        }
    }
    float** bpixel = new float* [szerokosc+2];
    for(int i = 0; i<szerokosc+2; i++){
        bpixel[i] = new float[wysokosc+2];
        for(int n = 0; n < wysokosc+2; n++){
            bpixel[i][n] = 0.0;
        }
    }


    for(int x=0; x<szerokosc;x++){
        for(int y=0;y<wysokosc;y++){

            oldpixel = getPixelSurface(x, y, bmp);

            oldpixel.r = oldpixel.r + (Uint8)rpixel[x+d][y];
            oldpixel.g = oldpixel.g + (Uint8)gpixel[x+d][y];
            oldpixel.b = oldpixel.b + (Uint8)bpixel[x+d][y];

            int color = findClosest(oldpixel);
            newpixel = paleta[color];

            rerror = (float)oldpixel.r - (float)newpixel.r;
            gerror = (float)oldpixel.g - (float)newpixel.g;
            berror = (float)oldpixel.b - (float)newpixel.b;

            rpixel[x+d+1][y] += (7.0/16.0*rerror);
            gpixel[x+d+1][y] += (7.0/16.0*gerror);
            bpixel[x+d+1][y] += (7.0/16.0*berror);

            rpixel[x+d-1][y+1] += (3.0/16.0*rerror);
            gpixel[x+d-1][y+1] += (3.0/16.0*gerror);
            bpixel[x+d-1][y+1] += (3.0/16.0*berror);

            rpixel[x+d][y+1] =rpixel[x+d][y+1] + (5.0/16.0*rerror);
            gpixel[x+d][y+1] =gpixel[x+d][y+1] + (5.0/16.0*gerror);
            bpixel[x+d][y+1] =bpixel[x+d][y+1] + (5.0/16.0*berror);

            rpixel[x+d+1][y+1] += (1.0/16.0*rerror);
            gpixel[x+d+1][y+1] += (1.0/16.0*gerror);
            bpixel[x+d+1][y+1] += (1.0/16.0*berror);

            pixelTab[o] = color;
            setPixel(x,y,newpixel.r,newpixel.g,newpixel.b);
            o++;

        }
            SDL_UpdateWindowSurface(window);
    }
    char a =' ';
    cout<<"Zapisac obrazek?  [y/n]: ";
    cin>>a;
    if(a=='y'){
        int* data = new int [szerokosc*wysokosc];
        SDL_Color kolor;
        int i = 0;
        for(int x = 0; x < szerokosc; x ++){
            for(int y = 0; y < wysokosc; y++){
                kolor = getPixel(x, y);
                int color = findClosest(kolor);
                data[i] = color;
                i++;
        }
    }
        zapisDoPliku(data,szerokosc*wysokosc,false);
    }
}


void rysujObrazek(){

    SDL_Color kolor;
    int* pixelTab = new int [szerokosc*wysokosc];
    int i = 0;

    for(int x = 0; x < szerokosc; x ++){
        for(int y = 0; y < wysokosc;y++){


            kolor = getPixelSurface(x, y, bmp);
            int color = findClosest(kolor);

            pixelTab[i] = color;
            i++;
            setPixel(x,y,paleta[color].r,paleta[color].g,paleta[color].b);
        }

        SDL_UpdateWindowSurface(window);
    }
    char a =' ';
    cout<<"Zapisac obrazek?  [y/n]: ";
    cin>>a;
    if(a=='y'){
        kompresjaRLE(pixelTab);
    }
}


void swapElements(SDL_Color* a, SDL_Color* b){
    SDL_Color tmp = *a;
    *a = *b;
    *b = tmp;
}
int quickpartition (SDL_Color arr[], int low, int high, int condition){
    if(condition == 0){
        SDL_Color pivotElement = arr[high];
        int i = (low - 1);
        for (int j = low; j <= high- 1; j++) {
            if (arr[j].r <= pivotElement.r){
            i++;
            swapElements(&arr[i], &arr[j]);
            }
        }
        swapElements(&arr[i + 1], &arr[high]);
        return (i + 1);
    }
    else if(condition == 1){
        SDL_Color pivotElement = arr[high];
        int i = (low - 1);
        for (int j = low; j <= high- 1; j++) {
            if (arr[j].g <= pivotElement.g){
            i++;
            swapElements(&arr[i], &arr[j]);
            }
        }
        swapElements(&arr[i + 1], &arr[high]);
        return (i + 1);
    }
    else{
        SDL_Color pivotElement = arr[high];
        int i = (low - 1);
        for (int j = low; j <= high- 1; j++) {
            if (arr[j].b <= pivotElement.b){
            i++;
            swapElements(&arr[i], &arr[j]);
            }
        }
        swapElements(&arr[i + 1], &arr[high]);
        return (i + 1);
    }
}
void quickSort(SDL_Color arr[], int lowIndex, int highIndex, int condition){
    if (lowIndex < highIndex){
        int pivot = quickpartition(arr, lowIndex, highIndex, condition);
        quickSort(arr, lowIndex, pivot - 1, condition);
        quickSort(arr, pivot + 1, highIndex, condition);
    }
}
int mniejszy(int a, int b){
    if(a<b){
        return a;
    }
    else{
        return b;
    }
}
int wiekszy(int a, int b){
     if(a>b){
        return a;
    }
    else{
        return b;
    }
}
void sortForMedian(int pixelend,SDL_Color piksele[], int maximum, int red, int green, int blue){

    if(maximum == red){
        quickSort(piksele, 0,pixelend,0);
    }
    else if(maximum  == green){
        quickSort(piksele,0,pixelend,1);
    }
    else{
        quickSort(piksele,0,pixelend,2);
    }

}
SDL_Color znajdzSrednie(SDL_Color* kolory, int dlugosc){

    int r = 0;
    int g = 0;
    int b = 0;

    for(int i = 0; i < dlugosc; i++){
        r = r + kolory[i].r;
        g = g + kolory[i].g;
        b = b + kolory[i].b;
    }

    r = r/dlugosc;
    g = g/dlugosc;
    b = b/dlugosc;

    SDL_Color tmp;

    tmp.r = (Uint8 )r;
    tmp.g = (Uint8 )g;
    tmp.b = (Uint8 )b;

    return tmp;
}
void medianCut(){

    int piksele = szerokosc*wysokosc;

    SDL_Color* arrpiksele = new SDL_Color[piksele];
    int counter = 0;

    SDL_Color tmp = getPixelSurface(0, 0, bmp);

    int lower_red = tmp.r;
    int lower_green = tmp.g;
    int lower_blue = tmp.b;
    int upper_red = 0;
    int upper_green = 0;
    int upper_blue = 0;

    for(int y = 0; y < wysokosc/2; y++){
        for(int x = 0; x < szerokosc/2; x++){
            tmp = getPixelSurface(x, y, bmp);
            lower_red = mniejszy(lower_red, tmp.r);
            lower_green = mniejszy(lower_green, tmp.g);
            lower_blue = mniejszy(lower_blue, tmp.b);

            upper_red = wiekszy(upper_red, tmp.r);
            upper_green = wiekszy(upper_green, tmp.g);
            upper_blue = wiekszy(upper_blue, tmp.b);

            arrpiksele[counter] = tmp;
            counter++;

        }
    }
    int red = upper_red - lower_red;
    int green = upper_green - lower_green;
    int blue = upper_blue - lower_blue;

    int maximum = wiekszy(wiekszy(red,green),blue);

    sortForMedian(counter, arrpiksele, maximum, red, green, blue);


    SDL_Color lists[16];
    SDL_Color means[counter/16];
    int c = 0;

    for(int i = 0; i <16; i++){
        for(int n = 0; n < counter/16 ; n++){
            means[n] = arrpiksele[c];
            c++;
        }
        lists[i] = znajdzSrednie(means,counter/16);
    }

    for(int i = 0; i < 16; i++){
        paleta[i] = lists[i];
    }
    SDL_UpdateWindowSurface(window);

    paletamode = 'D';
}

SDL_Color getPixel(int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        //określamy pozycję
        char* pPosition=(char*)screen->pixels ;

        //przesunięcie względem y
        pPosition+=(screen->pitch*y) ;

        //przesunięcie względem x
        pPosition+=(screen->format->BytesPerPixel*x);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, screen->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}
void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B){
  if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p1 = (Uint8 *)screen->pixels + (y) * screen->pitch + (x) * bpp;
    Uint8 *p2 = (Uint8 *)screen->pixels + (y+1) * screen->pitch + (x) * bpp;
    Uint8 *p3 = (Uint8 *)screen->pixels + (y) * screen->pitch + (x+1) * bpp;
    Uint8 *p4 = (Uint8 *)screen->pixels + (y+1) * screen->pitch + (x+1) * bpp;

    /* Ustawiamy wartość piksela, w zależnoœci od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p1 = pixel;
            *p2 = pixel;
            *p3 = pixel;
            *p4 = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p1 = pixel;
            *(Uint16 *)p2 = pixel;
            *(Uint16 *)p3 = pixel;
            *(Uint16 *)p4 = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p1[0] = (pixel >> 16) & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = pixel & 0xff;
                p2[0] = (pixel >> 16) & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = pixel & 0xff;
                p3[0] = (pixel >> 16) & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = pixel & 0xff;
                p4[0] = (pixel >> 16) & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = pixel & 0xff;
            } else {
                p1[0] = pixel & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = (pixel >> 16) & 0xff;
                p2[0] = pixel & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = (pixel >> 16) & 0xff;
                p3[0] = pixel & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = (pixel >> 16) & 0xff;
                p4[0] = pixel & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p1 = pixel;
            *(Uint32 *)p2 = pixel;
            *(Uint32 *)p3 = pixel;
            *(Uint32 *)p4 = pixel;
            break;

        }
    }
}
void setPixelSurface(int x, int y, Uint8 R, Uint8 G, Uint8 B){
  if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    /* Ustawiamy wartość piksela, w zależności od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p = pixel;
            break;
        }
    }
}
SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        //określamy pozycję
        char* pPosition=(char*)surface->pixels ;

        //przesunięcie względem y
        pPosition+=(surface->pitch*y) ;

        //przesunięcie względem x
        pPosition+=(surface->format->BytesPerPixel*x);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, surface->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}
bool laduj_bmp(char* nazwa){

    bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Nie mozna zaladowac pliku bmp %s\n", SDL_GetError());
        return false;
    }
    else{

        wysokosc = bmp->h;
        szerokosc = bmp->w;

        window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc+1, wysokosc+1, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
            return EXIT_FAILURE;
        }
        screen = SDL_GetWindowSurface(window);
        if (screen == NULL) {
            fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
            return false;
        }
        SDL_UpdateWindowSurface(window);

        SDL_Color kolor;
        for (int y=0; y<wysokosc; y++) {
			for (int x=0; x<szerokosc; x++) {
				kolor = getPixelSurface(x, y, bmp);
				setPixel(x, y, kolor.r, kolor.g, kolor.b);
			}
        }
        SDL_UpdateWindowSurface(window);
    }

    return true;
}

void menus(int var){
    if(var == 0){
        cout<<"1. Laduj bmp"<<endl;
        cout<<"2. Dekoduj obrazek"<<endl;
        cout<<"3. Zakoncz"<<endl;

    }
    else if(var == 1){
        cout<<"1. Paleta narzucona"<<endl;
        cout<<"2. Paleta dedykowana"<<endl;
        cout<<"3. Paleta czarno biala"<<endl;
        cout<<"4. Powrot"<<endl;
    }
    else if(var == 2){
        cout<<"1. Z ditheringiem"<<endl;
        cout<<"2. Bez ditheringu"<<endl;
        cout<<"3. Powrot"<<endl;
    }
    cout<<"Opcja: ";
}

bool first_case(char* nazwa){
    menus(1);
    char a=' ';
    int c = -1;
    cin>>c;
    switch(c){
        case 1:
            tworzPalete();
            pokazPalete();
            cout<<"Czy zastosowac dithering?: [y/n] ";
            cin>>a;
            if(a=='n'){
                rysujObrazek();
            }
            else if(a=='y'){
                dithering();
            }
            break;
        case 2:
            medianCut();
            pokazPalete();
            cout<<"Czy zastosowac dithering?: [y/n] ";
            cin>>a;
            if(a=='n'){
                rysujObrazek();
            }
            else if(a=='y'){
                dithering();
            }
            break;
        case 3:
            tworzCzarnaPalete();
            pokazPalete();
            cout<<"Czy zastosowac dithering?: [y/n] ";
            cin>>a;
            if(a=='n'){
                rysujObrazek();
            }
            else if(a=='y'){
                dithering();
            }
            break;
        case 4:

            return false;
            break;
        default:
            break;
    }
    return true;
}

int main( int argc, char * argv[] ){

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
    }

    int c = -1;
    bool endflag = true;
    bool flag1 = true;
    char* nazwa;

    while(endflag){
        menus(0);
        cin>>c;
        switch(c){

        case 1:

            cout<<"Podaj nazwe pliku ktory chcesz wczytac: ";
            cin>>nazwa;
            if(laduj_bmp(nazwa)){
                flag1 = true;
                while(flag1){
                    flag1=first_case(nazwa);
                }
            }
            break;

        case 2:

            cout<<"Podaj nazwe pliku ktory chcesz odczytac: ";
            cin>>nazwa;
            odczytajPlik(nazwa);


            break;

        case 3:
            endflag = false;
            break;
        default:
            cout<<"Nie ma takiej opcji!"<<endl;
            cout<<"Sprobuj ponownie"<<endl;
            break;
        }
    }


    if (screen) {
        SDL_FreeSurface(screen);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    return 0;
}
