#include "Map_S.h"

vector<vector<int>> Map::map;
int Map::size;	

void Map::set_map(const char* file_path)
{
	map_valid= false;
	//je¿eli poprawnie wykonano wszystkie funkcje wczytania danych mo¿na uznaæ ¿e wczytana instabncja problemu jest poprawna
	if(!file_fill_size(file_path) && !create_struct() && !file_fill_struct(file_path))  map_valid= true;
}

int Map::file_fill_size(const char* file_path)
{
	fstream plik;
	string s_row;
	int num;
	vector<int> row;
	stringstream iss;
	try
	{
		plik.open(file_path, ios::in);
		if(plik.is_open())
		{
			iss.clear();
			getline(plik, s_row);
			if(plik.fail())throw("::BAD SIZE FILE::");
			iss.write(s_row.c_str(), s_row.length());	//przekazanie przez string stream pierwszego wiersza do tablicy typu int
			while ( iss >> num )
				row.push_back( num );
			if(row.size()== 1) size= row[0];			//sprawdzenie iloœci wczytanycgh danych 
			else throw("::BAD SIZE::");
			plik.close();
		}
		else throw("::FILE FAIL::");
	}
	catch(const char * e)
	{
		cout<<e<<endl;
		plik.close();
		return 1;
	}
	catch(...)
	{
		plik.close();
		return 2;
	}
	return M_VALID;
}

int Map::create_struct(void)
{
	try
	{
		for(int i=0; i<size; i++)//0 
		{
			map.push_back(vector<int>());
			map[i].resize(size);
		}
	}
	catch(const std::bad_alloc& e)
	{
		cout<<e.what()<<endl;
		return 1;
	}
	catch(...)
	{
		cout<<"Struct error"<<endl;
		return 2;
	}
	return M_VALID;
}

int Map::file_fill_struct(const char* file_path)
{
	fstream plik;
	string s_row;
	int num;
	vector<int> row;
	stringstream iss;
	try
	{
		plik.open(file_path, ios::in);
		if(plik.is_open())
		{
			getline(plik, s_row);
			for(int i=0; i<size; i++)
			{
				iss.clear();
				row.clear();
				getline(plik, s_row);
				if(plik.fail())throw("::BAD ROW FILE::");
				iss.write(s_row.c_str(), s_row.length());

				while(iss >> num)
					row.push_back(num);

				if(row.size()== size)
					for(int j=0; j<size; j++)
					{
						map[j][i]= row[j];
					}

				else
					throw("::BAD ROW::");
			}
			plik.close();
		}
		else throw("::FILE FAIL::");
	}
	catch(const std::out_of_range& e)
	{
		cout<<e.what()<<endl;
		plik.close();
		return 3;
	}
	catch(const char * e)
	{
		cout<<e<<endl;
		plik.close();
		return 1;
	}
	catch(...)
	{
		plik.close();
		return 2;
	}
	return M_VALID;
}

bool Map::is_ok(void)
{
	return map_valid;
}
