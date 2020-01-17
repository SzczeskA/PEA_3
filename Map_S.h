#pragma once
#include <iostream>
#include <windows.h>
#include <exception>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
#include <random>
#include <ctime>
#include <chrono>

using namespace std;
#define M_VALID 0

class Map
{
	static vector<vector<int>> map;			//wejsciowa struktura zawieraj¹ca dane z pliku //map[begin][destination]
	static int size;						//wielkoœæ instancji
	bool map_valid;							//poprawnoœæ wczytania danych

public:
	const vector<int>& operator[](int index) const
	{
		return map[index];
	}
	int get_size() const 
	{
		return size;
	}
	void set_map(const char* file_path);
	int  file_fill_size(const char* file_path);			//pobranie wielkoœci instancji z pliku
	int  create_struct(void);			//stworzenie struktury danych
	int  file_fill_struct(const char* file_path);		//pobranie danych z pliku
	bool is_ok(void);
};





