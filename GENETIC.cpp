#include "GENETIC.h"
int GENETIC::generation;


GENETIC::GENETIC(const char* path, const char* Wynik): path(path), wynik(Wynik)
{
	map= new Map();
	gen= new mt19937(rand_());
	generation= 0;
	pmcross= false;
	invmut= true;
	stop_time=10;

	gen_size=140;
	mutation_ratio=30;
	cross_normal_dev=4;
}

void GENETIC::tsp(bool pmcross, bool invmut, int sec, int cross_deviation, int gen_size, int mutation_ratio)
{
	generation= 0;
	this->pmcross= pmcross;
	this->invmut= invmut;
	this->stop_time=sec;

	this->gen_size=gen_size;
	this->mutation_ratio=mutation_ratio;
	dist_2= new uniform_int_distribution<> (0, gen_size-1);
	tsp();
}

void GENETIC::tsp(void)
{
	//pobranie danych
	map->set_map(path);
	if(!map->is_ok()) return;

	dist_1= new uniform_int_distribution<> (1, map->get_size()-1);
	dist_2= new uniform_int_distribution<> (0, gen_size-1);
	
	Path::set_map(*map);
	fstream plik;
	plik.open(wynik,  ios::app | ios::out);
	if(!plik.is_open())
	{
		cout<<"::RESULT FILE ERROR::"<<endl;
		return;
	}
	//obliczanie rozk�adu normalnego dla wierzcho�k�w krzy�owanych 
	cross_normal_dist();
	//pocz�tkowa populacja tworzona w spos�b przypadkowy
	random();
	//najlepsze rozwi�zanie znal;ezione podczas pracy algorytmu
	Path best_path;
	best_path=A[0][0];
	
	//pomiar czasu
	std::chrono::time_point<std::chrono::system_clock> start, end, point; 
	start = std::chrono::system_clock::now();
	point = std::chrono::system_clock::now();
	while(1)
	{
		//krzy�owanie populacji rodzic�w w celu uzyskania populacji dzieci
		cross();
		//szukanie nowego najlepszego w populacji- potrzebne na rzecz wizualizacji wynik�w, w algorytmie zb�dne
		for(int i=0; i<A[(generation+1)%2].size(); i++)
			if(A[(generation+1)%2][i]<best_path)
				best_path=A[(generation+1)%2][i];

		//mutacja populacji dzieci 
		mutation();

		//sortowanie populacji dzieci w celu uwzgl�dnienia szansy rozmna�ania 
		sort(A[(generation+1)%2].begin(), A[(generation+1)%2].end());

		//sprawdzanie czy nowe najlepsze rozwi�zanie lepsze od dotychchczasowego
		if(A[(generation+1)%2][0]<best_path)
			best_path=A[(generation+1)%2][0];

		//czyszczenie populacji rodzic�w kt�ra w nast�pnym obiegu b�dzie populacj� dzieci
		A[(generation)%2].clear();

		//zamina populacji miejscami 
		generation++;

		end = std::chrono::system_clock::now();
		
		//czas dzia�ania algorytmu < czas maksymalny
		if(std::chrono::duration_cast<std::chrono::seconds>(end - start).count()>stop_time)break;
		
		//cosekundowy zapis do pliku
		if(std::chrono::duration_cast<std::chrono::seconds>(end - point).count()>0.5)
		{
			point = std::chrono::system_clock::now();
			plik<<generation<<"\t"<<int(pmcross)*2+int(invmut)<<"\t"<<gen_size<<"\t";
			plik<<best_path.get_lenght()<<"\t";
			plik<<unsigned(std::chrono::duration_cast<std::chrono::seconds>(end - start).count())<<endl;
		}
		
		//je�eli liczba generacji przekroczy INT_MAX zachowanie algorytmu niezdefiniowane.
		if(generation==INT_MAX)break;
	}
	//zapis wyniku koncowego do pliku
	plik<<generation<<"\t"<<int(pmcross)*2+int(invmut)<<"\t"<<gen_size<<"\t";
	plik<<best_path.get_lenght()<<"\t";
	plik<<unsigned(std::chrono::duration_cast<std::chrono::seconds>(end - start).count())<<endl;
	
	//zamykanie pliku wynik�w
	plik.close();
}

//rozk�ad normalny krzy�owania o zadanym odchyleniu
void GENETIC::cross_normal_dist(void)
{
	const int nrolls=100000;  // liczba eksperyment�w rozk�adu normalnego
	const int nstars=gen_size;    // populacja

	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0, cross_normal_dev);
	cross_normal.resize(gen_size);

	for (int i=0; i<nrolls; ++i) 
	{
		double number = distribution(generator);
		if ((number>=0.0)&&(number<gen_size)) ++cross_normal[int(number)];
	}
	
	long double population=0;
	for (int i=0; i<gen_size; ++i) 
		population+= cross_normal[i];
	
	population/=gen_size;
	
	//rozk�ad normalny daje oko�o 90% wszystkich rozmna�a� reszta jest przypadkowa
	for (int i=0; i<gen_size; ++i)
		cross_normal[i]/=(population/0.9);


	/*for (int i=0; i<gen_size; ++i) {
		cout<<cross_normal[i]<<endl;;
		if(cross_normal[i]<=0)break;
	}*/
}


void GENETIC::cross(void)
{
	static Path A1_1, A1_2, A2_1, A2_2;
	int rand;
	bool f;

	while(A[(generation+1)%2].size()<gen_size)	//krzy�owanie a� do osi�gni�cia populacji na wypadek zbyt ma�ej populacji rodzic�w (cz�ste powtarzanie si� wierzcho�k�w)
		for(int i=0; i< A[(generation)%2].size(); i++)	//krzy�owanie ca�ej obecnej populacji
		{
			for(int j=0; j<cross_normal[i]; j++)		//krzy�owanie kolejnych wierzcho�k�w n razy (n zale�y od rozk�adu normalnego)
			{
				A1_1= A1_2= A[(generation)%2][i];
				rand=(*dist_2)(*gen) % A[(generation)%2].size();
				if(rand==i)continue;
				A2_1= A2_2= A[(generation)%2][rand];			//losowy wierzcho�ek krzy�owania
				
				if(pmcross)//krzy�owanie partial matched
				{
					A1_1.PM_cross(A2_1);//ka�de krzy�owanie tworzy 2 potomk�w
					A2_2.PM_cross(A1_2);
				}
				else
				{
					A1_1.O_cross(A2_1);
					A2_2.O_cross(A1_2);
				}

				f= true;
				for(int i=0; cross_normal[i]>0 && A[(generation+1)%2].size()>i; i++)//w populacji wyszczeg�lnionej no krzy�owania nie mog� powtrarza� si� wierzcho�ki W_A
				{

					if(A[(generation+1)%2][i]==A1_1)
					{
						f= false;
						break;
					}
				}
				if(f)
					A[(generation+1)%2].push_back(A1_1);//dodanie wierzcho�ka do populacji

				f= true;
				for(int i=0; cross_normal[i]>0 && A[(generation+1)%2].size()>i; i++)//w populacji wyszczeg�lnionej no krzy�owania nie mog� powtrarza� si� wierzcho�ki W_B
				{

					if(A[(generation+1)%2][i]==A2_2)
					{
						f= false;
						break;
					}
				}
				if(f)
					A[(generation+1)%2].push_back(A2_2);//dodanie wierzcho�ka do populacji
				
				if(A[(generation+1)%2].size()>=gen_size)//osi�gni�to pe�n� populacj�
					break;
			}
			if(A[(generation+1)%2].size()>=gen_size)//osi�gni�to pe�n� populacj�
				break;
		}
	//A[(generation+1)%2].resize(gen_size);
}
//
void GENETIC::mutation(void)
{
	if(invmut)
	{
		A[(generation+1)%2][0].INV_mut(generation);
		for(int i=0; i< (mutation_ratio*gen_size)/100; i++)
			A[(generation+1)%2][(*dist_2)(*gen)].INV_mut(generation);
	}
	else
	{
		A[(generation+1)%2][0].SCR_mut(generation);
		for(int i=0; i< (mutation_ratio*gen_size)/100; i++)
			A[(generation+1)%2][(*dist_2)(*gen)].SCR_mut(generation);
	}
	
}

//populacja losowa
void GENETIC::random(void)
{
	vector<int> rand_tab;
	int copy;
	int rand_2;
	int rand_3;

	for(int g=0; g<gen_size/2+4; g++)
	{
		rand_tab.clear();
		for(int i=0; i<map->get_size()+1; i++)
			rand_tab.push_back(i); //path<0-1-2-3-4-5>
		
								   //randomizacja drogi 
		for(int i=0; i< map->get_size(); i++)
		{
			rand_2= (*dist_1)(*gen);
			rand_3= (*dist_1)(*gen);
			rand_2+=rand_3;
			for(int j=rand_3; j<rand_2; j++)
			{
				copy=rand_tab[(j%(map->get_size()-1))+1];
				rand_tab[(j%(map->get_size()-1))+1]= rand_tab[((rand_2-j)%(map->get_size()-1))+1];
				rand_tab[((rand_2-j)%(map->get_size()-1))+1]=copy;
			}
		}
		A[0].push_back(*(new Path(rand_tab)));
	}

	sort(A[0].begin(), A[0].end());
}