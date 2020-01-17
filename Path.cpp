#include "Path.h"

Map Path::map;
random_device Path::rand_;
mt19937* Path::gen;
uniform_int_distribution<> *Path::dist_1;

Path::Path(vector<int> path_pattern, int lenght_pattern)
{
	lenght_v=lenght_pattern;
	path_v= path_pattern;
	get_path_lenght();
}
Path::Path()
{
	lenght_v=0;
	path_v.resize(map.get_size());
}
Path::Path(const Path& CP)
{
	this->lenght_v= CP.lenght_v;
	this->path_v= CP.path_v;
}
Path::~Path()
{
	path_v.clear();
}
bool Path::operator< (const Path& LT) const
{
	return this->lenght_v < LT.lenght_v;
}
bool Path::operator== (const Path& CP) const
{
	return this->path_v == CP.path_v;
}
Path& Path::operator= (const Path & CP)
{
	this->lenght_v= CP.lenght_v;
	this->path_v= CP.path_v;
	return *this;
}
int Path::get_path_lenght(bool resize)
{
	if(lenght_v != 0  && resize == false) return lenght_v;
	lenght_v=0;
	for(int i=0; i< map.get_size()-1; i++)
		lenght_v+=map[path_v[i]][path_v[i+1]];
	lenght_v+=map[path_v[int(map.get_size()-1)]][0];
	return lenght_v;
}		
void Path::set_path(vector<int> path_)
{
	path_v= path_;
}
int Path::get_lenght(void) const
{
	return lenght_v;
}
vector<int> Path::get_path(void) const
{
	return path_v;
}
void Path::printf(void)const 
{
	cout<<"("<<lenght_v<<") ";
	for(int i=0; i<map.get_size(); i++)
	{
		cout<<path_v[i];
		if(i<map.get_size()-1)cout<<"-";
	}
	cout<<endl;
}

void Path::set_map(Map& _map)
{
	map=_map;
	gen= new mt19937(rand_());
	dist_1= new uniform_int_distribution<> (1, map.get_size()-1);
}

void Path::PM_cross(const Path& SC)
{
	vector<int> AA= this->get_path();
	vector<int> BB= SC.get_path();
	vector<int> W(BB);
	int* used= new int[map.get_size()]();
	int rand_2;
	int rand_3;

	rand_2= (*dist_1)(*gen);
	do
		rand_3= (*dist_1)(*gen);
	while(rand_3 == rand_2);

	if(rand_2>rand_3)
		swap(rand_2, rand_3);

	for(int i=rand_2; i<rand_3; i++)
	{
		used[AA[i]]= i;
		W[i]= AA[i];
	}
	int liczba_do_zast¹pienia;
	int index=0;

	for(int i=rand_2; i<rand_3; i++)//pêtla po BB
	{
		if(used[BB[i]]==0)
		{
			liczba_do_zast¹pienia= W[i];
			do
			{
				for(int k=0; k<map.get_size(); k++)
				{
					if(BB[k]==liczba_do_zast¹pienia)
					{
						index= k;
						break;
					}
				}
				if(used[AA[index]]==0)break;
				liczba_do_zast¹pienia= W[index];
			} while(1);
			W[index]=BB[i];
		}
	}
	this->set_path(W);
	this->get_path_lenght(true);
	delete[] used;
}

void Path::O_cross(const Path& SC)
{
	vector<int> AA= this->get_path();
	vector<int> BB= SC.get_path();
	vector<int> W;
	int* used= new int[map.get_size()]();
	W.resize(map.get_size());
	int rand_2;
	int rand_3;


	rand_2= (*dist_1)(*gen);
	do
		rand_3= (*dist_1)(*gen);
	while(rand_3 == rand_2);

	if(rand_2>rand_3)
		swap(rand_2, rand_3);

	for(int i=rand_2; i<rand_3; i++)
	{
		used[AA[i]]= i;
		W[i]= AA[i];
	}
	int k=rand_3;
	for(int i=rand_3; i!= rand_2; i++)
	{
		i%= (map.get_size()-1)+1;
		k%= (map.get_size()-1)+1;
		if(used[BB[k]]==0)
		{
			W[i]=BB[k];
			used[BB[k]]=k;
			k++;
		}
		else
		{
			k++;
			i--;
		}
	}
	int cop;
	while(W[0]!= 0)
	{
		cop=W[map.get_size()-1];
		for(int i=map.get_size()-1; i>=1; i--)
		{
			W[i]= W[i-1];
		}
		W[0]=cop;
	}
	this->set_path(W);
	this->get_path_lenght(true);
	delete[] used;
}

void Path::INV_mut(int progress)
{
	int rand_2= (*dist_1)(*gen);
	int rand_3= (*dist_1)(*gen)+2;
	rand_3/=2; //minimum 1;
	//if(rand_2>rand_3)
		//swap(rand_2, rand_3);
	rand_3+=rand_2;
	for(int i=rand_2; i<rand_3; i++)
	{
		swap(path_v[(i%(map.get_size()-1))+1], path_v[((rand_3-i)%(map.get_size()-1))+1]);
	}
	this->get_path_lenght(true);//ponownie oblicz d³ugoæœ œcie¿ki
}


void Path::SCR_mut(int progress)
{
	int rand_2, rand_3;
	for(int i=0; i<3; i++)
	{
		rand_2= (*dist_1)(*gen);
		rand_3= (*dist_1)(*gen);
		swap(path_v[rand_2], path_v[rand_3]);
	}
	this->get_path_lenght(true);//ponownie oblicz d³ugoæœ œcie¿ki
}

