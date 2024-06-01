// game.hpp

#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Texture.hpp"
#include <SDL_mixer.h>
using namespace std;

class Visualizer
{

  const int SCREEN_WIDTH = 800;
  const int SCREEN_HEIGHT = 800;


  SDL_Window *gWindow = NULL;
  SDL_Renderer *gRenderer = NULL;
  TTF_Font* font = NULL;
  std::vector<std::string> sortOptions = { "Bubble Sort","Insertion Sort", "Selection Sort", "Merge Sort", "Quick Sort","select algo" };
  int selectedIndex = 5;
  std::string a="";
  bool menuOpen = false;
  Texture sort_button;
  Texture back_button;
  Texture new_array;
  Texture sortingImageTexture;
  Mix_Music* music;
  Mix_Music* Welcome_music;
  Mix_Music* mouse_click;



public:
  bool init();
  bool loadMedia();
  void close();
  void run();
  void Drawbars(std::vector<int> &array,int k);
  void Drawswappedbars(std::vector<int> &array, int j,int k,int l, std::string s,std::string s_l,int start,int end);
  bool Bubblesort(std::vector<int> & array);
  bool Insertionsort(std::vector<int> & array);
  bool Selectionsort(std::vector<int> &array);
  void Drawmergebars(std::vector<int> &array, std::vector<int> &elem_height,int l,int r,string s);
  bool Mergesort(std::vector<int> &array);
  void merge(std::vector<int> &array, int p, int q, int r,std::vector<int> elem_height);
  void sort(vector<int>& arr,int start,int end,std::vector<int> elem_height);
  void mergeSort(std::vector<int> &array, int l, int r,std::vector<int> elem_height,string s);
  void quicksort(vector<int>& arr,int low,int high);
  bool QuickSort(vector<int>& arr);
  int partition(vector<int>& arr,int low,int high);
  void fillarrayy(vector<int>& arr);
  void renderText(const std::string& text, int x, int y, SDL_Color color,int size,std::string text2="a",int x2=0);

 ~Visualizer();
};
