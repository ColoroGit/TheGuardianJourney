#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

class Guardian 
{
	string name;
	string village;
	
	public: 
	int PL; //Nivel de Poder (Power Level en inglés, de ahí el nombre)
	vector<Guardian*> apprentices; //un registro que representa la jerarquía maestro-aprendiz dentro del programa (un árbol general)
};

class Village
{
	string name;
	Guardian* master;
	
	public:
		
	vector<Village*> adjVillages; //registro de las conexiones que existen entre villas (emulan un grafo)
	
	Village (string name)
	{
		this->name = name;
	}
	
	string GetName()
	{
		return name;
	}
	
	void AddEdge(Village* u, Village* v) //Crea una conexión entre una ciudad y otra
	{
		u->adjVillages.push_back(v);
		v->adjVillages.push_back(u);
	}
};

bool Exists(vector<string> readVillages, string name) //Función que se utiliza especificamente para saber si ya se ha leido el nombre de una villa
{
	for (string v : readVillages)
	{
		if (v == name)
		{
			return true;
		}
	}
	
	return false;
}

Village* FindVillage(vector<Village*> adjList, string name) //retorna la referencia a la instancia de Village que tenga el nombre entregado
{
	for (Village* v : adjList)
	{
		if (v->GetName() == name)
		{
			return v;
		}
	}
	return NULL;
}

void PrintGraph(vector<Village*> adjList) //Funcion para imprimir las conexiones que existen entre villas
{
	for (Village* vertex : adjList)
	{
		cout << "Vecinos de " << vertex->GetName() << ": ";
		for (Village* v : vertex->adjVillages)
		{
			cout << v->GetName() << " ";
		}
		cout << endl;
	}
}

int main()
{
	vector<Village*> map;
	
	/*
		A continuación se realiza la carga del archivo de las Villas. Para ello, se mantiene un registro
		sobre los nombres de las villas que ya han sido ingresadas y creadas, para no repetrilas, y de esa
		forma se van agregando las distintas villas, y creando aristas entre ellas, representadas por la 
		lista de adyacencia declarada arriba (map), en base a lo entregado en el archivo Villages.txt.
	*/
	
	vector<string> readVillages;
	
	string filename = "Villages.txt";
	ifstream file(filename);
	
	if (file.is_open()) 
	{
		string line;
		
		while (getline(file, line)) 
		{
			stringstream ss(line);
			
			Village* new1;
			Village* new2;
			
			string name1;
			getline(ss, name1, ',');
			
			if (!Exists(readVillages, name1))
			{
				new1 = new Village(name1);
				map.push_back(new1);
				readVillages.push_back(name1);
			}
			else
			{
				new1 = FindVillage(map, name1);
			}
			
			string name2;
			getline(ss, name2, ',');
			
			if (!Exists(readVillages, name2))
			{
				new2 = new Village(name2);
				map.push_back(new2);
				readVillages.push_back(name2);
			}
			else
			{
				new2 = FindVillage(map, name2);
			}
			
			new1->AddEdge(new1, new2);
		}
		
		PrintGraph(map);
		
		file.close();
	}
	else 
	{	
		cout << "Failed to open file: " << filename << endl;
	}
	
	return 0;
}
