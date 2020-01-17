#pragma once
#include <iostream>
#include <windows.h>
#include <exception>
#include <memory>
#include <math.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
#include <random>
#include <ctime>
#include <chrono>

#include"Map_S.h"
#include"Path.h"
using namespace std;

class GENETIC
{
	random_device rand_;
	mt19937* gen;
	uniform_int_distribution<> *dist_1;
	uniform_int_distribution<>* dist_2;
	
	//numer generacji kolejki ¿ród³owe /nieparzysty =>A[0], B[0] /parzysty =>A[1], B[1]
	static int generation;
	//plik Ÿród³owy, obs³uga wczytania danych
	Map* map;
	
	//generacja rodziców[0], i dzieci[1]
	vector<Path> A[2];
	
	//rozk³ady normalne dla krzyrzowania i mutacji
	void cross_normal_dist(void);
	vector<int> cross_normal;
	
	//Krzy¿owanie
	void cross(void);

	//Mutacje
	void mutation(void);

	//Start z losowymi scie¿kami
	void random(void);

	//Sterowanie
	bool pmcross;
	bool invmut;
	int stop_time;

	int mutation_ratio;
	int gen_size;
	int cross_normal_dev;

	const char* wynik;
	const char* path;
public:
	GENETIC(const char* path, const char* wynik="Wyniki.txt");
	void tsp(bool pmcross, bool invmut, int sec, int cross_deviation, int gen_size, int mutation_ratio);
	void tsp(void);
};
