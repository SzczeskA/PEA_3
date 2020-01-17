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

#include"Map_S.h"
class Path
{
	static Map map;
	int lenght_v;
	vector<int> path_v;

	static random_device rand_;
	static mt19937* gen;
	static uniform_int_distribution<> *dist_1;

public:
	Path(vector<int> path_pattern, int lenght_pattern= 0);
	Path();
	Path(const Path& CP);
	~Path();
	bool operator< (const Path& LT) const;
	bool operator== (const Path& CP) const;
	Path& operator= (const Path& CP);
	int get_path_lenght(bool resize= false);
	void set_path(vector<int> path_);
	int get_lenght(void)const;
	vector<int> get_path(void) const;
	void printf(void)const;
	static void set_map(Map& _map);

	void PM_cross(const Path& SC);
	void O_cross(const Path& SC);

	void INV_mut(int progress);
	void SCR_mut(int progress);
};

