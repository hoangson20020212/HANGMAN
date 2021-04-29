#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <time.h>

#include <SDL.h>
#include "SDL_ulis.h"
using namespace std;

void Constructer(SDL_Window* &windows, SDL_Renderer* &renderer, string &word, string &guessedWord,int &badGuessCount, SDL_Texture* &wordImage);
string chooseWord(const string &title);
void rendergame(string guessedWord,int badGuessCount, SDL_Renderer* renderer, SDL_Texture* wordImage);
char readAGuess(SDL_Renderer* &renderer);
bool contains(string word,char guess);
string update(string guessedWord,string word,char guess);
bool GameOver(int badGuessCount,string word,string guessedWord);
void displayGameResult(int badGuessCount,string word,string guessedWord);
string getTitle(SDL_Renderer* &renderer);
void win(SDL_Renderer* &renderer);
void lose(SDL_Renderer* &renderer,string word, string guessedWord);
/*void rendererTexture(SDL_Texture *tex,SDL_Renderer *ren,int x,int y,int w,int h)*/

int main(int argc, char* argv[])
{
    SDL_Window* windows;
    SDL_Renderer* renderer;
    string word;
    string guessedWord;
    int badGuessCount;
    SDL_Texture* wordImage;

    Constructer(windows, renderer, word, guessedWord, badGuessCount, wordImage);
    do {
        char guess = readAGuess(renderer);
        if(contains(word,guess)) guessedWord= update(guessedWord,word,guess);
        else badGuessCount++;
        rendergame(guessedWord,badGuessCount,renderer,wordImage);
    } while(!GameOver(badGuessCount,word,guessedWord));

    SDL_Delay(1000);
    if(badGuessCount==7){
        cout<<"THUA MAT ROI"<< endl;
        cout << "DAP AN LA :"<< word<< endl;
        lose(renderer, word, guessedWord);
    }else{
        cout << "CHUC MUNG , BAN THANG"<< endl;
        cout << "DAP AN LA :"<< word<< endl;
        win(renderer);
    }
    SDL_DestroyTexture(wordImage);
    quitSDL(windows, renderer);
    return 0;
}

void Constructer(SDL_Window* &windows, SDL_Renderer* &renderer, string &word, string &guessedWord,int &badGuessCount, SDL_Texture* &wordImage) {
    srand(time(0));
    initSDL(windows, renderer);
    string title=getTitle(renderer);
    word = chooseWord(title);
    guessedWord = string(word.size(),'_');
    badGuessCount =0;

    SDL_Texture *background=loadTexture(renderer,"hangman.bmp");
    SDL_RenderCopy(renderer,background,NULL,NULL);
    SDL_DestroyTexture(background);

    wordImage=loadTexture(renderer,"hangman1.bmp");

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int leng = 46*word.length() + 3*(word.length()-1);
    int beginPaint = 322 - leng/2;
    for (int i=0;i<word.length();i++) {
        int x = beginPaint + i*46 + i*3;
        SDL_RenderDrawLine(renderer, x, 600, x+45, 600);
    }
    SDL_RenderPresent(renderer);
}

string chooseWord(const string &title)
{
    vector<string> wordList; //Khai báo vector chứa các từ sẽ đọc
    ifstream file(title + ".txt"); //Mở tệp có đường dẫn như trong tham số
    if (file.is_open()) { // Kiểm tra tệp mở thành công
        string word;
        while (file >> word) { //Đọc từng từ (giống cin) đến khi không đọc được nữa
        wordList.push_back(word); //đưa từ vừa đọc vào vector
        }
        file.close();
        }
    if (wordList.size() > 0) { // nếu có dữ liệu đọc thành công
        int randomIndex = rand() % wordList.size();
        return wordList[randomIndex]; // trả về một từ ngẫu nhiên trong vector
    } else return "";
}
 void rendergame(string guessedWord,int badGuessCount, SDL_Renderer* renderer, SDL_Texture* wordImage){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if(badGuessCount==1) SDL_RenderDrawLine(renderer, 397, 221, 397, 271);
    else if(badGuessCount==2){
        for (int angle = 0; angle < 360; angle++) {
            int x = 397 + 20 * cos(angle);
            int y = 291 + 20 * sin(angle);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    else if(badGuessCount==3){SDL_RenderDrawLine(renderer, 397, 311, 397, 361);}
    else if(badGuessCount==4){ SDL_RenderDrawLine(renderer, 397, 361, 355, 403);}
    else if(badGuessCount==5){ SDL_RenderDrawLine(renderer, 397, 361, 440, 404);}
    else if(badGuessCount==6){ SDL_RenderDrawLine(renderer, 397, 327, 358, 347);}
    else if(badGuessCount==7){ SDL_RenderDrawLine(renderer, 397, 327, 438, 347);}
    int leng = 46*guessedWord.length() + 3*(guessedWord.length()-1);
    int beginPaint = 322 - leng/2;
    for (int i=0;i<guessedWord.length();i++) {
        int x = beginPaint + i*46 + i*3;
        char image = guessedWord[i];
        if (image == '_') continue;
        SDL_Rect srcrect;
        srcrect.x = 0;
        srcrect.y = 0 + 46*((int)image-97);
        srcrect.w = 46;
        srcrect.h = 46;
        SDL_Rect dstrect;
        dstrect.x = x;
        dstrect.y = 600-46+1;
        dstrect.w = 46;
        dstrect.h = 46;
        SDL_RenderCopy(renderer, wordImage, &srcrect, &dstrect);
    }
    SDL_RenderPresent(renderer);
}

 bool contains(string word,char guess){
    for( char c:word) if(c==guess) return true;
    return false;
}
string update(string guessedWord,string word,char guess){
    for(int i=0;i<word.size();i++) if (word[i]==guess) guessedWord[i]=guess;
    return guessedWord;
}
 char readAGuess(SDL_Renderer* &renderer){
    int x,y;
    static SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        if (e.type == SDL_QUIT) exit(0);
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&x, &y);
            if(y>=749 && y<=795){
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_Rect rect;
                rect.x=(x-5)/49*49+5;
                rect.y=749;
                rect.w=46; rect.h=46;
                SDL_RenderFillRect(renderer,&rect);
                return (x-5)/49+'n';
            }
            else if(y>=700 && y<=746){
                    SDL_SetRenderDrawColor(renderer,255,255,255,255);
                    SDL_Rect rect;
                    rect.x=(x-5)/49*49+5;
                    rect.y=700;
                    rect.w=46;
                    rect.h=46;
                    SDL_RenderFillRect(renderer,&rect);
                        return (x-5)/49+'a';
            }
        }
    }
 }

 bool GameOver(int badGuessCount,string word,string guessedWord){
    if(badGuessCount==7) return true;
    if(word==guessedWord) return true;
    return false;
}
string getTitle(SDL_Renderer* &renderer){
    string title ="";
    SDL_Texture *bginitial=loadTexture(renderer,"hangmaninitial.bmp");
    SDL_RenderCopy(renderer,bginitial,NULL,NULL);
    SDL_RenderPresent(renderer);
    int x,y;
    static SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        if (e.type == SDL_QUIT) exit(0);
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&x, &y);
            if(x>=15 && x<=301 &&y>=645&&y<=709) title="music" ;
            if(x>=15 && x<=301 &&y>=530&&y<=634) title="sport" ;
            if(x>=15 && x<=301 &&y>=416&&y<=520) title="object" ;
            if(x>=15 && x<=301 &&y>=302&&y<=406) title="animal" ;
            if(x>=335 && x<=621 &&y>=645  &&y<=709) title="nature" ;
            if(x>=335 && x<=621&&y>=530  &&y<=634) title="color" ;
            if(x>=335 && x<=621 &&y>=416  &&y<=520) title="subject" ;
            if(x>=335 && x<=621 &&y>=302  &&y<=406) title="country" ;
            if (title != "") return title;
        }
    }
}
void win(SDL_Renderer* &renderer){
    SDL_Texture* winImage[3];
    winImage[0] = loadTexture(renderer,"WON2.bmp");
    winImage[1] = loadTexture(renderer,"WON3.bmp");
    winImage[2] = loadTexture(renderer,"WON4.bmp");
    int Count = 0;
    while (true) {
            SDL_RenderCopy(renderer,winImage[(Count++)%3],NULL,NULL);
            SDL_RenderPresent(renderer);
            static SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) exit(0);
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int x,y;
                    SDL_GetMouseState(&x, &y);
                    if (x>368 && y>593 && x<561 && y<646) {
                        for (int i=0;i<3;i++)SDL_DestroyTexture(winImage[i]);
                        return ;
                    }
                }
            }
            SDL_Delay(200);
    }
}
void rendererTexture(SDL_Texture* tex,SDL_Renderer* &renderer,int x,int y,int w,int h){
            SDL_Rect dst;
            dst.x=x;
            dst.y=y;
            dst.w=w;
            dst.h=h;

    SDL_RenderCopy(renderer,tex,NULL,&dst);
}

void lose(SDL_Renderer* &renderer,string word, string guessedWord) {
        SDL_Texture* blackWord = loadTexture(renderer, "hangman1.bmp");
        SDL_Texture* redWord = loadTexture(renderer, "hangman2.bmp");
        int leng = 46*guessedWord.length() + 3*(guessedWord.length()-1);
        int beginPaint = 322 - leng/2;
        for (int i=0;i<guessedWord.length();i++) {
            int x = beginPaint + i*46 + i*3;
            char image = word[i];
            SDL_Rect srcrect = {0, 46*((int)image-97), 46, 46};
            SDL_Rect dstrect = {x, 600-46+1, 46, 46};
            if (guessedWord[i] != '_') SDL_RenderCopy(renderer, blackWord, &srcrect, &dstrect);
            else SDL_RenderCopy(renderer, redWord, &srcrect, &dstrect);
    }

        SDL_Texture* image=loadTexture(renderer,"youlose.bmp");
        rendererTexture(image,renderer,500,300,90,90);
        SDL_Texture* image1=loadTexture(renderer,"sad.bmp");
        rendererTexture(image1,renderer,387,280,20,20);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(blackWord);
        SDL_DestroyTexture(redWord);
        SDL_Delay(4000);
}


