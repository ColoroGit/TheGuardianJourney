#include <iostream>
#include <string>
#include <vector>
#include <queue>
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
	
	Guardian(string name, int pl, string village)
	{
		this->name = name;
		this->PL = pl;
		this->village = village;
	}
	
	string GetName()
	{
		return name;
	}
	
	string GetVillage()
	{
		return village;
	}
	
	void PrintGuardians() ////////////TESTEO//////////////
	{
		cout << "Nombre: " << name << "; Nivel de Poder: " << PL << "; Villa: " << village << "; Aprendices: " << endl;
		for (Guardian* g : apprentices)
		{
			g->PrintGuardians();
		} 
	}
};

class Village
{
	string name;
	
	public:
		
	Guardian* master;
	vector<Guardian*> apprentices;
	vector<Village*> adjVillages; //registro de las conexiones que existen entre villas (emulan un grafo)
	
	Village (string name)
	{
		this->name = name;
		master = NULL;
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

void PrintGraph(vector<Village*> adjList) //Funcion para imprimir las conexiones que existen entre villas //////////TESTEO/////////
{
	for (Village* vertex : adjList)
	{
		if (vertex->GetName() != "Tesla")
		{
			cout << "Vecinos de " << vertex->GetName() << ": ";
			for (Village* v : vertex->adjVillages)
			{
				cout << v->GetName() << " ";
			}
			cout << "Maestro de la villa: " << vertex->master->GetName() << endl;
			cout << "Aprendices de la villa: ";
			for (Guardian* a : vertex->apprentices)
			{
				cout << a->GetName() << " ";
			}
			cout << endl;	
		}
	}
}

/*
bool IsNumber(string s) //Funcion para saber si un string entregado es un número o no
{
	for (int i = 0; i < s.length(); i++ ) 
	{
		if (!isdigit(s[i])) 
		{
			return false;
		}
	}
	return true;
}
*/

Guardian* FindGuardian(Guardian* root, string name) //retorna la referencia a la instancia del Guardian que tenga el nombre entregado
{
	queue<Guardian*> queue;
	queue.push(root);
	
	if (root == NULL)
	{
		return NULL;
	}
	
	while (!queue.empty())
	{
		Guardian* current = queue.front();
		queue.pop();
		
		if (current->GetName() == name)
		{
			return current;
		}
		
		for (Guardian* g : current->apprentices)
		{
			queue.push(g);
		}
	}
	
	return NULL;
}

void AssignGuardian(vector<Village*> map, Guardian* root, Guardian* g)
{
	Village* current = FindVillage(map, g->GetVillage());
	
	if (current->master == NULL)
	{
		current->master = g;
		return;
	}
	
	if (current->master->PL < g->PL)
	{
		current->apprentices.push_back(current->master);
		current->master = g;
	}
	else
	{
		current->apprentices.push_back(g);
	}
}

bool CheckVillages(vector<Village*> map)
{
	for (Village* v : map)
	{
		if (v->GetName() != "Tesla")
		{
			if (v->master == NULL || v->apprentices.size() <= 0)
			{
				return false;
			}
		}
	}
	return true;
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
		bool tesla = false;
		
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
			
			if (name1 == "Tesla" || name2 == "Tesla")
			{
				tesla = true;
			}
		}
		
		file.close();
		
		if (!tesla)
		{
			cout << "Falta la ciudad <Tesla> en la lista de ciudades, no se puede iniciar el juego sin ella" << endl
			<< "(Asegurese de que se encuentre escrita tal cual como se indica entre <> en el archivo Villages.txt)";
			return 0;
		}
	}
	else 
	{	
		cout << "Failed to open file: " << filename << endl;
		return 0;
	}
	
	/*
		Carga del archivo de Guardianes: Se parte por hacer la carga del "Jefe Final", que en el caso específico del proyecto es
		Stormheart, según sus puntos de poder se define el máximo posible. Posteriormente se le asignan sus aprendices según
		se vayan entregando en el archivo Guardians.txt, y por último, se asignan a sus respectivas villas.
	*/
	
	Guardian* root; //Arbol para representar la jerarquía maestro/aprendiz de los Guardianes
	
	filename = "Guardians.txt";
	ifstream file2(filename);
	
	if (file2.is_open())
	{
		string line;
		
		getline(file2, line);
		stringstream ss(line);
		
		string name;
		getline(ss, name, ',');
		
		string powerLevel;
		getline(ss, powerLevel, ',');
		int pl = stoi(powerLevel);
		const int MaxPL = stoi(powerLevel);
		
		if (MaxPL <= 1)
		{
			cout << "Hubo un problema al leer los puntos de poder maximos, ya que, o se ingreso algo que no es un numero," << endl
			<< "o ese numero es menor o igual a 1 (Se recomienda que sea un numero mayor o igual a 50)" << endl;
			return 0;
		}
		
		root = new Guardian(name, pl, "Tesla");
		
		while (getline(file2, line))
		{
			stringstream ss(line);
			
			getline(ss, name, ',');
			
			getline(ss, powerLevel, ',');
			pl = stoi(powerLevel);
			
			if (pl <= 1)
			{
				cout << "Hubo un problema al leer los puntos de poder de uno de los guardianes, ya que, o se ingreso algo que no es un numero," << endl
				<< "o ese numero es menor o igual a 1 (Se recomienda que sea un numero mayor o igual a 50)" << endl;
				return 0;
			}
			
			string master;
			getline(ss, master, ',');
			Guardian* Master = FindGuardian(root, master);
			
			string village;
			getline(ss, village, ',');
			
			if (Exists(readVillages, village))
			{
				if (Master != NULL)
				{
					if (Master->PL < pl)
					{
						if (Master->PL - 1 <= 0)
						{
							cout << "Hubo un problema al leer los puntos de poder de uno de los guardianes, ya que ese numero es menor o igual a 0 (Se recomienda que sea un numero mayor o igual a 50)" << endl;
							return 0;
						}
						else
						{
							Guardian* newGuardian = new Guardian(name, Master->PL - 1, village);
							Master->apprentices.push_back(newGuardian);
							AssignGuardian(map, root, newGuardian);
						}
					}
					else
					{
						Guardian* newGuardian = new Guardian(name, pl, village);
						Master->apprentices.push_back(newGuardian);
						AssignGuardian(map, root, newGuardian);
					}
				}
				else
				{
					cout << "Hubo un problema al tratar de asignar a un aprendiz a su maestro, ya que el segundo no existia" << endl
					<< "(Asegurese de que, en el archivo de texto Guardians.txt, exista un giardian antes de que se le puedan asignar aprendices" << endl
					<< "En otras palabras, que siempre se escriba un mastro antes que su aprendiz)";
					return 0;
				}
			}
			else
			{
				cout << "Hubo un problema al tratar de asignar un guardian a su villa respectiva," << endl
				<< "ya que la segunda no se encontraba dentro de las villas cargadas en el archivo Villages.txt" << endl;
				return 0;
			}
		}
		
		root->PrintGuardians();
	}
	else
	{
		cout << "Failed to open file: " << filename << endl;
		return 0;
	}
	
	PrintGraph(map);
	
	if (CheckVillages(map))
	{
		cout << "Todo Listo" << endl;
	}
	else
	{
		cout << "Hubo un problema al finalizar la carga de archivos, ya que hay una o mas villas que no tienen un maestro y por lo menos un aprendiz" << endl
		<< "(Asegurese de que, en el archivo Guardians.txt, para todas las villas haya por lo menos dos Guardianes asignados a la misma villa)" << endl;
		return 0;
	}
	
	return 0;
}
