#include <iostream>
#include <string>
#include <SDL.h>
#include "visualizer.hpp"
#include <iostream>
#include <thread>
#include <cstdlib>
#include <SDL_mixer.h>

Visualizer::~Visualizer()
{ 

    close();
}

bool Visualizer::init()
{
    std::cout << "init";
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_AUDIO ) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return -1;
        }

        gWindow = SDL_CreateWindow("Sorting visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
        }
        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        }

        font = TTF_OpenFont("font.ttf", 15); 
        if (!font) {
            std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        }
    }
   return success;
}

bool Visualizer::loadMedia()
{
	sort_button.texture = sort_button.load(gRenderer, "sort.png");
	if (sort_button.texture == nullptr)
	{
		std::cerr << "Failed to create texture from sort image!" << std::endl;
		return false;
	}

	back_button.texture = back_button.load(gRenderer, "back.png");
	if (back_button.texture == nullptr)
	{
		std::cerr << "Failed to create texture from back image!" << std::endl;
		return false;
	}
	new_array.texture = new_array.load(gRenderer, "new_array.png");
	if (new_array.texture == nullptr)
	{
		std::cerr << "Failed to create texture from new array image!" << std::endl;
		return false;
	}
    sortingImageTexture.texture = sortingImageTexture.load(gRenderer,"Sortinggg.png");
    if (sortingImageTexture.texture == nullptr)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", "Sortinggg.png", IMG_GetError());
    }
    music = Mix_LoadMUS("swish-sound-94707.mp3");
    if (!music) {
        printf("Failed to load MP3 file:");
    }
    Welcome_music = Mix_LoadMUS("Welcome_screen.mp3");
    if (!Welcome_music) {
        printf("Failed to load MP3 file:");
    }
    mouse_click = Mix_LoadMUS("pick-92276.mp3");
    if (!mouse_click) {
        printf("Failed to load MP3 file:");
    }
    return true;
}

void Visualizer::close()
{

    TTF_CloseFont(font);
    font = NULL;
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    
}
void Visualizer::Drawbars(std::vector<int> &array,int k)
{
    int width = ((SCREEN_WIDTH - 200) / array.size()) - 2;
    int count = 100;
    int i=0;
    for (const int &value : array)
    {
        int height = value;                                                    
        SDL_Rect bar = {count, SCREEN_HEIGHT - (height + 100), width, height}; 
        if (i>=k){SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);}
        else{SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);} 
        SDL_RenderFillRect(gRenderer, &bar);
        count += width + 2; 
        i++;
    }
}
void Visualizer::Drawswappedbars(std::vector<int> &array, int j, int k, int l, std::string s, std::string s_l,int start,int end)
{
    int width = ((SCREEN_WIDTH - 200) / array.size()) - 2;
    int count = 100;
    int i = 0;

    for (const int &value : array)
    {
        int height = value;             
        SDL_Rect bar = {count, SCREEN_HEIGHT - (height + 100), width, height};
        if (i>=start && i<=end)   {bar = {count, SCREEN_HEIGHT - (height + 70), width, height};}    
        if (i != j && i != k && i != l)
        {
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        }
        else
        {
            if (s == "GREEN" && (i == j || i == k))
            {
                SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
            }
            else if (s == "RED" && (i == j || i == k))
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
            }
            else if (s_l == "BLUE" && i == l)
            {
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
            }
            
        }
        SDL_RenderFillRect(gRenderer, &bar);
        count += width + 2; 
        i++;
    }
}

void Visualizer::run()
{
    
    bool quit = false;
    SDL_Event e;
    std::vector<int> array;

    int size = 20; 
    for (int i = 0; i < size; i++)
    {

        array.push_back(rand() % 500);
    }

    SDL_Rect sortRect = { 280, 30, 250, 120 }; 
    SDL_Rect backRect = { 580, 30, 250, 120 }; 
    SDL_Rect newRect = { 0, 30, 250, 120 }; 

    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
    SDL_RenderPresent(gRenderer);
    bool sortStarted = false;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                    quit = true;
                
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) 
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= sortRect.x && x <= sortRect.x + sortRect.w &&
                    y >= sortRect.y && y <= sortRect.y + sortRect.h) {
                    Mix_PlayMusic(mouse_click, 1);
                    SDL_Delay(800);
                    sortStarted = true;
                } 
                if (x >= newRect.x && x <= newRect.x + newRect.w &&
                    y >= newRect.y && y <= newRect.y + newRect.h) {
                    Mix_PlayMusic(mouse_click, 1);
                    SDL_Delay(800);
                    fillarrayy(array);
                }
                if (x >= backRect.x && x <= backRect.x + backRect.w &&
                    y >= backRect.y && y <= backRect.y + backRect.h) {
                    Mix_PlayMusic(mouse_click, 1);
                    SDL_Delay(800);
                    quit=true;
                }  
           
            }
        }
       
        if(sortStarted!=true)
        {
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, back_button.texture, nullptr, &backRect);
            SDL_RenderCopy(gRenderer, sort_button.texture, nullptr, &sortRect);
            SDL_RenderCopy(gRenderer, new_array.texture, nullptr, &newRect);
            Drawbars(array, 1000);
            SDL_RenderPresent(gRenderer);
            
        }
        if (sortStarted) 
        {
            int x, y;
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderClear(gRenderer);
            SDL_Rect sortingImageRect = { 0, 0, 800, 800 }; // Full screen
            SDL_RenderCopy(gRenderer, sortingImageTexture.texture, nullptr, &sortingImageRect);
            SDL_RenderPresent(gRenderer);
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
            SDL_GetMouseState(&x, &y);
            if (x >= 115 && x <= 674 && y >= 215 && y <= 281) 
            {
                    Mix_PlayMusic(mouse_click, 1);
                    SDL_Delay(800);
                    Bubblesort(array);
                    Mix_PlayMusic(Welcome_music, 1);
                    SDL_Delay(800);
                    fillarrayy(array);
                    sortStarted=false;
            } 
            if (x >= 116 && x <= 675 && y >= 299 && y <= 365)
            {
                Mix_PlayMusic(mouse_click, 1);
                SDL_Delay(800);
                Selectionsort(array);
                Mix_PlayMusic(Welcome_music, 1);
                SDL_Delay(800);
                fillarrayy(array);
                sortStarted=false;
            }
            if (x >= 117 && x <= 686 && y >= 391 && y <= 453)
            {
                Mix_PlayMusic(mouse_click, 1);
                SDL_Delay(800);
                Insertionsort(array);
                Mix_PlayMusic(Welcome_music, 1);
                SDL_Delay(800);
                fillarrayy(array);
                sortStarted=false;
            }
            if (x >= 117 && x <= 686 && y >= 473 && y <= 535)
            {
                Mix_PlayMusic(mouse_click, 1);
                SDL_Delay(800);
                Mergesort(array);
                Mix_PlayMusic(Welcome_music, 1);
                SDL_Delay(800);
                fillarrayy(array);
                sortStarted=false;
            }
            if (x >= 124 && x <= 693 && y >= 553 && y <= 615)
            {
                Mix_PlayMusic(mouse_click, 1);
                SDL_Delay(800);
                QuickSort(array);
                Mix_PlayMusic(Welcome_music, 1);
                SDL_Delay(800);
                fillarrayy(array);
                sortStarted=false;
            }
            if (x >= 289 && x <= 499 && y >= 667 && y <= 707)
            {
                Mix_PlayMusic(mouse_click, 1);
                SDL_Delay(800);
                sortStarted=false;
            }

            }

        }
        
       
    
}}

void Visualizer::renderText(const std::string& text, int x, int y, SDL_Color color, int size,std::string text2,int x2) {
    TTF_Font *font = TTF_OpenFont("font.ttf", size);
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    
   
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
   
    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_Surface* surface2 ;
    SDL_Texture* texture2;
    SDL_Rect dstRect2;
   
    if (text2!="a" && x2!=0)
    {
        surface2 = TTF_RenderText_Solid(font, text2.c_str(), color);
        texture2 = SDL_CreateTextureFromSurface(gRenderer, surface2);
        dstRect2 = { x2, y, surface2->w, surface2->h };
  
    }
    SDL_RenderCopy(gRenderer, texture, nullptr, &dstRect);
    if (text2!="a" && x2!=0)
    {
        SDL_RenderCopy(gRenderer, texture2, nullptr, &dstRect2);
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    if (text2!="a" && x2!=0)
    {
        SDL_DestroyTexture(texture2);
        SDL_FreeSurface(surface2);
    }
    TTF_CloseFont(font);
    
}
bool Visualizer::Bubblesort(std::vector<int> &array)
{
    int i, j;
    bool swapped;
    int n = array.size();
    SDL_Color textColor = {0, 0, 0, 255}; 
    int k=n-1;
    SDL_Event event;
    std::string s = "RED";
    for (i = 0; i < n - 1; i++)
    {
        swapped = false;
        std::string passText = "Pass " + std::to_string(i + 1);

        for (j = 0; j < n - i - 1; j++)
        {
              
            while (SDL_PollEvent(&event)) 
            {
                if (event.type == SDL_QUIT) 
                {
                    return false; 
                }
            }

            if (array[j] > array[j + 1])
            {
                s = "RED";
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                renderText(passText, 350, 750, textColor,24);
                Drawswappedbars(array, j, j + 1, 1000, s, s,j,j+1);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(800);
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
                Mix_PlayMusic(music, 1);
                

                swapped = true;
                s = "GREEN";
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                renderText(passText, 350, 750, textColor,24);
                Drawswappedbars(array, j, j + 1, 1000, s, s,j,j+1);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(800);
            }
            else
            {
                s = "GREEN";
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                renderText(passText, 350, 750, textColor,24);
                Drawswappedbars(array, j, j + 1, 1000, s, s,j,j+1);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(800);
            }
        }
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);        
        SDL_RenderClear(gRenderer);
        Drawbars(array,k );
        renderText(passText, 350, 750, textColor,24);
        k--;
        SDL_RenderPresent(gRenderer);
        SDL_Delay(800);


        if (swapped == false)
            break;
    }
    std::string text="Sorted";
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    
    SDL_RenderClear(gRenderer);
    Drawbars(array,1000 );
    renderText(text, 350, 750, textColor,24);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(100);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    
    SDL_RenderClear(gRenderer);
    Drawbars(array,0 );
    renderText(text, 350, 750, textColor,24);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(800);
    return true;
}
bool Visualizer::Insertionsort(std::vector<int> &array)
{
    int width = ((SCREEN_WIDTH - 200) / array.size()) - 2;
    int n = array.size();
    int i, key, j;
    int size=50-(2*(n-4) );
    SDL_Event event;
    std::string text="KEY"; 
    SDL_Color textColor = {0, 0, 0, 255}; 
    for (i = 1; i < n; i++)
    {
        std::string t = "Pass " + std::to_string(i );
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                return false; 
            }
        }
        key = array[i];
        j = i - 1;
        std::string s = "RED";
        std::string s_l = "BLUE";
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        renderText(t, 350, 760, textColor,22);
        renderText(text, 101+(i*(width+2)), 720, textColor,size);
        Drawswappedbars(array, 1000, 1000, i, s, s_l,1000,1000);
     
        SDL_RenderPresent(gRenderer);
        SDL_Delay(1000);
        int temp = array[j];

        while (j >= 0 && temp > key)
        {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    return false; 
                }
            }
            s = "RED";

            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderClear(gRenderer);
            renderText(t, 350, 760, textColor,22); 
            if (array[i] == key)
            {
                renderText(text, 101+(i*(width+2)), 720, textColor,size);
                Drawswappedbars(array, j, 1000, i, s, s_l,1000,1000);
            }
            else if (array[j + 1] == key)
            {
                renderText(text, 101+((j+1)*(width+2)), 720, textColor,size);
                Drawswappedbars(array, j, 1000, j + 1, s, s_l,1000,1000);
            }
         
            SDL_RenderPresent(gRenderer);
            SDL_Delay(800);
            array[j + 1] = array[j];
            array[j] = key;
            Mix_PlayMusic(music, 1);

            s = "GREEN";
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderClear(gRenderer);
            renderText(t, 350, 760, textColor,22); //////
            renderText(text, 101+(j*(width+2)), 720, textColor,size);
            Drawswappedbars(array, j + 1, 1000, j, s, s_l,1000,1000);
       
            SDL_RenderPresent(gRenderer);
            SDL_Delay(1200);
            j = j - 1;
            temp = array[j];
        }
        array[j + 1] = key;
        SDL_Delay(400);
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        renderText(t, 350, 760, textColor,22); 
        renderText(text, 101+((j+1)*(width+2)), 720, textColor,size);
        Drawswappedbars(array, 1000, 1000, j + 1, s, s_l,1000,1000);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(1000);
    }
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    text="Sorted";
                SDL_RenderClear(gRenderer);
                Drawbars(array,0 );
                renderText(text, 350, 720, textColor,24);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(800);
   return true;
}

bool Visualizer::Selectionsort(std::vector<int> &array)
{
    int width = ((SCREEN_WIDTH - 200) / array.size()) - 2;
    int i, j, min_idx;
    int n = array.size();
    int size=50-(2*(n-4) );
    SDL_Event event;
    std::string text="MIN"; 
    std::string text2="KEY"; 
    SDL_Color textColor = {0, 0, 0, 255};
    std::string s = "RED";
    std::string s_l = "BLUE";
    for (i = 0; i < n - 1; i++)
    {
        std::string t = "Pass " + std::to_string(i + 1);
        min_idx = i;
        for (j = i + 1; j < n; j++)
        {
            while (SDL_PollEvent(&event)) 
            {
                if (event.type == SDL_QUIT) 
                {
                    return false;
                }
            }
            if (array[j] < array[min_idx])
                min_idx = j;
        }

        s = "RED";
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        renderText(t, 350, 760, textColor,22); 
        std::string c="GREEN";
        if (i==min_idx){

            renderText(text, 101+(min_idx*(width+2)), 720, textColor,size);
            Drawswappedbars(array, i, min_idx, 1000, c, s_l,1000,1000);
        }
        else{
        renderText(text, 101+(min_idx*(width+2)), 720, textColor,size,text2,101+(i*(width+2)));
        Drawswappedbars(array, i, min_idx, 1000, s, s_l,1000,1000);}
        SDL_RenderPresent(gRenderer);
        SDL_Delay(1000);
        while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    return false; 
                }
            }
        if (min_idx != i)
        {
            int temp = array[min_idx];
            array[min_idx] = array[i];
            array[i] = temp;
            Mix_PlayMusic(music, 1);

            s = "GREEN";
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            // Clear the screen with the white color on each loop iteration
            SDL_RenderClear(gRenderer);
            renderText(t, 350, 760, textColor,22); //////
            renderText(text2, 101+(min_idx*(width+2)), 720, textColor,size,text,101+(i*(width+2)));
            // Draw bars
            Drawswappedbars(array, i, min_idx, 1000, s, s_l,1000,1000);
            // Update screen
            SDL_RenderPresent(gRenderer);
            SDL_Delay(1000);
        }
        
    }
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    text="Sorted";          
    SDL_RenderClear(gRenderer);
    Drawbars(array,0 );
    renderText(text, 350, 720, textColor,24);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(800);
    return true;
}
void Visualizer::Drawmergebars(std::vector<int> &array, std::vector<int> &elem_height, int l, int r,string s)
{
    int width = ((SCREEN_WIDTH - 200) / array.size()) - 2;
    int count = 100;

    for (int i=0; i < array.size(); i++)
    {
        int height = array[i];                                                     
        SDL_Rect bar = {count, SCREEN_HEIGHT - (height + elem_height[i]), width, height}; 
        if (i>=l && i<=r ){
            if (s=="RED"){
                 SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); 
            }
            else if (s=="GREEN"){
                 SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); 
            }
            else{
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
            }
        }
       
        else
       { SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); }
        SDL_RenderFillRect(gRenderer, &bar);
        count += width + 2;
    }
}

bool Visualizer::Mergesort(std::vector<int> &array)

{
    int l = 0;
    int r = array.size() - 1;
    std::vector<int> elem_height;
    for (int i=0;i<array.size();i++)
    {
        
        elem_height.push_back(100);
    }
    
    string s="BLACK";
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
    Drawmergebars(array, elem_height, l, r,s);
    SDL_RenderPresent(gRenderer);
    mergeSort(array, l, r,elem_height,s);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    std::string text="Sorted";
    SDL_Color textColor = {0, 0, 0, 255}; 
    SDL_RenderClear(gRenderer);
    Drawbars(array,0 );
    renderText(text, 350, 720, textColor,24);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(800);
    return true;
}
void Visualizer::sort(vector<int>& arr,int start,int end,std::vector<int> elem_height){
   
    int temp;
    bool swapped;
    string s="BLACK";
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
    Drawmergebars(arr, elem_height, start, end,s);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(600);
     
    for (int i=start; i<end;i++){
        swapped=false;
        for (int j=start; j<end;j++){
            if (arr[j] > arr[j+1]){
                s = "RED";
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                Drawswappedbars(arr, j, j + 1, 1000, s, s,start,end);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(600);
                temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
                Mix_PlayMusic(music, 1);
                swapped=true;
                s = "GREEN";
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                Drawswappedbars(arr, j, j + 1, 1000, s, s,start,end);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(600);
            }
             else
            {
                s = "GREEN";
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                Drawswappedbars(arr, j, j + 1, 1000, s, s,start,end);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(600);
            }
        }
        if (swapped==false){
            break;
        }
    }
   
}
void Visualizer::merge(std::vector<int> &array, int p, int q, int r,std::vector<int> elem_height)
{
     
   
    sort(array,p,r,elem_height);
    string s="BLACK";
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
    Drawmergebars(array, elem_height, p, r,s);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(600);    
}


void Visualizer::mergeSort(std::vector<int> &array, int l, int r,std::vector<int> elem_height,string s) 
{
    if (l < r)
    {
        
        int m = l + (r - l) / 2;
      
        
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        Drawmergebars(array, elem_height, l, r,s);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(300);
      
        s="BLACK";
        mergeSort(array, l, m, elem_height,s);
        mergeSort(array, m + 1, r, elem_height,s);
        merge(array, l, m, r,elem_height);

    }
}
int Visualizer::partition(vector<int>& arr,int low,int high){

     int pivot=arr[high];
     int i=low;
     int temp;
     std::string s = "RED";
        std::string s_l = "BLUE";
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        Drawswappedbars(arr, 1000, 1000, high, s, s_l,low,high);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(500);
     for (int j=low;j<=high;j++)
     {
        if (arr[j]<pivot)
        {
            if (i!=j){
            s="RED";
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderClear(gRenderer);
            Drawswappedbars(arr, j, 1000, high, s, s_l,low,high);
            SDL_RenderPresent(gRenderer);
            SDL_Delay(500);
            }
            temp=arr[i];
            arr[i]=arr[j];
            arr[j]=temp;
            if (i!=j){
             Mix_PlayMusic(music, 1);}
            i++;
            s="GREEN";
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderClear(gRenderer);
            Drawswappedbars(arr, i, 1000, high, s, s_l,low,high);
            SDL_RenderPresent(gRenderer);
            SDL_Delay(500);
        }
     }
    temp=arr[i];
    arr[i]=pivot;
    arr[high]=temp;
    Mix_PlayMusic(music, 1);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
    Drawswappedbars(arr, 1000, 1000, i, s, s_l,low,high);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(500);
     return i;

}
bool Visualizer::QuickSort(vector<int>& arr){
    quicksort(arr,0,arr.size()-1);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    std::string text="Sorted";
    SDL_Color textColor = {0, 0, 0, 255}; 
    SDL_RenderClear(gRenderer);
    Drawbars(arr,0 );
    renderText(text, 350, 720, textColor,24);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(800);
    return true;
}
void Visualizer::quicksort(vector<int>& arr,int low,int high){
    int n=arr.size();
    int width = ((SCREEN_WIDTH - 200) / arr.size()) - 2;
    int size=50-(2*(n-4) );
    string s="BLACK";
  
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
    Drawbars(arr,1000 );
    SDL_RenderPresent(gRenderer);
    SDL_Delay(400);
    if (low<high)
    {
        int pivot=partition(arr,low,high);
        std::string s_l = "BLUE";
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        std::string text="PIVOT";
        SDL_Color textColor = {0, 0, 0, 255};       
        renderText(text, 101+(pivot*(width+2)), 720, textColor,size);
        Drawswappedbars(arr, 1000, 1000, pivot, s, s_l,1000,1000);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(800);
        quicksort(arr,low,pivot-1);
        quicksort(arr,pivot+1,high);

    }
    

}
void Visualizer::fillarrayy(vector<int>& arr){
    arr.clear();
    int size = 20;
    for (int i = 0; i < size; i++)
    {
    arr.push_back(rand() % 500);
    }
}



