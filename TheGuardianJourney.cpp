#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <unistd.h>

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
	
	void SetName(string name)
	{
		this->name = name;
	}
	
	string GetVillage()
	{
		return village;
	}
	
	void PrintTree() ////////////TESTEO//////////////
	{
		cout << "Nombre: " << name << "; Nivel de Poder: " << PL << "; Villa: " << village << "; Aprendices: " << endl;
		for (Guardian* g : apprentices)
		{
			g->PrintTree();
		} 
	}
	
	void PrintGuardians()
	{
		for (Guardian* g : apprentices)
		{
			cout << "Nombre: " << g->name << "; Villa: " << g->village << endl;
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

void PrintVillages(vector<string> villages) //Función para mostrar en pantalla todas las villas que existen
{
	cout << endl;
	for (string v : villages)
	{
		cout << v << endl;
	}
	cout << endl;
}

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

bool CheckVillages(vector<Village*> map) //Función para comprobar si todas las villas tienen un maestro y por lo menos un aprendiz, excepto Tesla, ya que esa ciudad no tiene aprendicez
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

Guardian* CreateGuardian(vector<string> villages)
{
	string name;
	string village;
	
	cout << "Ingrese el nombre de su Guardian: ";
	cin >> name;
	
	PrintVillages(villages);
	
	while (true)
	{
		cout << "Ingrese el nombre de la villa de origen de su Guardian" << endl
		<< "(Asegurese de escribir exacactamente el mismo nombre, mayusculas incluidas): ";
		cin >> village;
		
		if (Exists(villages, village))
		{
			return new Guardian(name, 50, village);
		}
		
		cout << "Esa villa no existe, por favor ingrese otra" << endl;
	}
}

Guardian* SelectGuardian(Guardian* root)
{
	cout << endl;
	root->PrintGuardians();
	string name;
	
	while (true)
	{
		cout << endl << "Ingrese el nombre del Guardian que desea utilizar" << endl
		<< "(Asegurese de escribir exacactamente el mismo nombre, mayusculas incluidas): ";
		cin >> name;
		
		Guardian* selected = FindGuardian(root, name);
		
		if (selected != NULL)
		{
			Guardian* replace = new Guardian (selected->GetName(), 50, selected->GetVillage());
			selected->SetName("Rex"); 
			return replace;
		}
		
		cout << "Ese Guardian no existe, por favor ingrese otro" << endl;
	}
}

int main()
{
	vector<Village*> map;
	vector<string> readVillages;
	
	cout << "Cargando archivos . . .   " << endl;
	sleep(1);
	
	/*
		A continuación se realiza la carga del archivo de las Villas. Para ello, se mantiene un registro
		sobre los nombres de las villas que ya han sido ingresadas y creadas, para no repetrilas, y de esa
		forma se van agregando las distintas villas, y creando aristas entre ellas, representadas por la 
		lista de adyacencia declarada arriba (map), en base a lo entregado en el archivo Villages.txt.
	*/
	
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
		
		root = new Guardian(name, 100, "Tesla");
		
		while (getline(file2, line))
		{
			stringstream ss(line);
			
			getline(ss, name, ',');
			
			string powerLevel;
			getline(ss, powerLevel, ',');
			if (!IsNumber(powerLevel))
			{
				cout << "Hubo un problema al leer los puntos de poder de uno de los guardianes, ya que, o se ingreso algo que no es un numero," << endl
				<< "(Se recomienda que sea un numero entre 100 y 50)" << endl;
				return 0;
			}
			int pl = stoi(powerLevel);
			
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
							cout << "Hubo un problema al leer los puntos de poder de uno de los guardianes, ya que ese numero es muy pequeño" << endl
							<< "(Se recomienda que sea un numero entre 100 y 50)" << endl;
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
	}
	else
	{
		cout << "Failed to open file: " << filename << endl;
		return 0;
	}
	
	if (CheckVillages(map))
	{
		cout << "Todo Listo" << endl;
		sleep(1);
	}
	else
	{
		cout << "Hubo un problema al finalizar la carga de archivos, ya que hay una o mas villas que no tienen un maestro y por lo menos un aprendiz" << endl
		<< "(Asegurese de que, en el archivo Guardians.txt, para todas las villas haya por lo menos dos Guardianes asignados a la misma villa)" << endl;
		return 0;
	}
	
	Guardian* player;
	int op;
	bool done = false;
	//Bienvenida y explicación del juego (dar opción si desea ver el tutorial antes de empezar [Eres nuevo o ya sabes jugar?])
	
	while (!done)
	{
		cout << endl << "Desea crear un personaje o seleccionar uno preexistente?" << endl
		<< "Ingrese <1> si dese crear, <2> si desea seleccionar: ";
		cin >> op;
		
		if (op == 1)
		{
			player = CreateGuardian(readVillages);
			cout << endl << "Guardian Creado Exitosamente" << endl;
			done = true;
		}
		else if (op == 2)
		{
			player = SelectGuardian(root);
			cout << endl << "Guardian Seleccionado Exitosamente" << endl;
			done = true;
		}
		else
		{
			cout << "Ese valor no es valido, por favor ingrese otro" << endl;
			cin.clear();
			cin.ignore();
		}
	}
	
	return 0;
}
