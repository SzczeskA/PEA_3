#include <conio.h>
#include "GENETIC.h"
#include <iostream>
#include <string>
#include <fstream>

void fun_menu(void);//deklaracja funkcji menu 
void fun_pomiar(void);//deklaracja funkcji pomiaru czasu
void fun_testy(void);//deklaracja funkcji testu poprawności działania
void fun_sterowanie(void);

bool invmut=true;
bool pmcross=true;
int pop_size=150;
int seconds=10;
int normal_cros_deviation=3;
int mutation_ratio=30;

using namespace std;
int main(void)
{
	srand(unsigned int (time(NULL)));
	fun_menu();
}

void fun_menu(void)
{
	const char* menu[]=	{
		"Adam Szczesniak 241293  GENETIC 01\n",
		"1: Testy\n",
		"2: Pomiary\n",
		"3: Sterowanie\n",
		"4: Wyjscie\n"
	};
	char menu_ch;
	while(1)
	{
		system("cls");
		cout<<menu[0];
		cout<<menu[1];
		cout<<menu[2];
		cout<<menu[3];
		cout<<menu[4];
		menu_ch= _getch();
		switch(menu_ch)
		{
		case '1':
				fun_testy();
			break;
		case '2':
				fun_pomiar();
			break;
		case '3':
				fun_sterowanie();
			break;
		default:
				return;
			break;
		}
	}
}

void fun_pomiar(void)
{
	string wynik;
	fstream plik;
	const char* tests []={"tsp_443.txt", "tsp_171.txt", "tsp_48.txt"};
	for(int c=0; c<3; c++)//pętla po miastach
	{
		wynik=("W3_");
		wynik+=(to_string(c)+"_"+to_string(240)+".txt");
		plik.open(wynik.c_str(), ios::out| ios::trunc);
		plik.close();
		for(int k=0; k<4; k++)//pętla po bool cross/mutation
		{
			for(int p=100; p<=1100; p+=500)//pętla po trozmiarze populacji
			{
				GENETIC* g= new GENETIC(tests[c], wynik.c_str());
				g->tsp(k%2, (k>>1)%2, 300, 4, p, 30);
				delete g;

			}
		}
		wynik.clear();
	}
	system("cls");
}
void fun_testy(void)
{
	string plik;
	system("cls");
	cout<<"Nazwa Pliku: ";
	cin>>plik;
	cout<<"Wyniki pobierane z 1 sekundowym interwalem dostepne na koncum pliku Wyniki.txt";
	GENETIC* g= new GENETIC(plik.c_str());
	g->tsp(pmcross, invmut, seconds, normal_cros_deviation, pop_size, mutation_ratio);
	delete g;
}
void fun_sterowanie(void)
{
	int size;
	
	

	while(1)
	{
		cout<<"Krzyzowanie: \n1. PMX\n2. OX"<<endl;
		cin>>size;
		if(size==1 || size==2)
		{
			pmcross=bool(size%2);
			//system("cls");
			break;
		}
		//system("cls");
	}

	while(1)
	{
		cout<<"Mutacje: \n1. inversion mutation\n2. Scramble Mutation"<<endl;
		cin>>size;
		if(size==1 || size==2)
		{
			invmut=bool(size%2);
			//system("cls");
			break;
		}
		//system("cls");
	}
		
	while(1)
	{
		cout<<"Rozmiar populacji:"<<endl;
		cin>>size;
		if(size>1 && size<20000)
		{
			pop_size=size;
			//system("cls");
			break;
		}
		//system("cls");
	}
	while(1)
	{
		cout<<"Czas:"<<endl;
		cin>>size;
		if(size>1 && size<20000)
		{
			seconds=size;
			//system("cls");
			break;
		}
		//system("cls");
	}
	while(1)
	{
		cout<<"odchylenie rozkladu normalnego:"<<endl;
		cin>>size;
		if(size>1 && size<100)
		{
			normal_cros_deviation=size;
			//system("cls");
			break;
		}
		//system("cls");
	}
	while(1)
	{
		cout<<"szansa mutacji:"<<endl;
		cin>>size;
		if(size>1 && size<100)
		{
			mutation_ratio=size;
			//system("cls");
			break;
		}
		//system("cls");
	}
	system("cls");
}