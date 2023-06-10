#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

class Guardian 
{
	string name;
	string village;
	
	public: 
	int PL; //Nivel de Poder (Power Level en ingl�s, de ah� el nombre)
	vector<Guardian*> apprentices; //un registro que representa la jerarqu�a maestro-aprendiz dentro del programa (un �rbol general)
	
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
	
	void PrintGuardians() //Funcion para imprimir a todos los guardianes, menos al "Jefe" (EasterEgg)
	{
		for (Guardian* g : apprentices)
		{
			cout << "Nombre: " << g->name << "; Villa: " << g->village << endl;
			g->PrintGuardians();
		}
	}
	
	void PrintGuardian() //Funcion para imprimir al guardian que la llame
	{
		cout << endl << "Nombre: " << name << "; Nivel de Poder: " << PL << endl;
	}
};

class Village
{
	string name;
	
	public:
		
	Guardian* master;
	vector<Guardian*> apprentices;
	vector<Village*> adjVillages; //registro de las conexiones que existen entre villas (emulan un grafo)
	int pointsToGive;
	bool visited;
	
	Village (string name)
	{
		this->name = name;
		pointsToGive = 3;
		master = NULL;
	}
	
	string GetName()
	{
		return name;
	}
	
	void AddEdge(Village* u, Village* v) //Crea una conexi�n entre una ciudad y otra
	{
		u->adjVillages.push_back(v);
		v->adjVillages.push_back(u);
	}
};

class Record
{
	public:
		
	Guardian* opponent;
	bool result;
	int pointsGained;
	int pointsAccumulated;
	
	Record(Guardian* opponent, bool result, int pointsGained, int pointsAccumulated)
	{
		this->opponent = opponent;
		this->result = result;
		this->pointsGained = pointsGained;
		this->pointsAccumulated = pointsAccumulated;
	}
};

bool Exists(vector<string> readVillages, string name) //Funci�n que se utiliza especificamente para saber si ya se ha leido el nombre de una villa
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

void PrintGraph(vector<Village*> adjList) //////////TESTEO/////////
{
	for (Village* vertex : adjList)
	{
		if (vertex->GetName() != adjList.front()->GetName())
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
				cout << a->GetName() << ": " << a->PL;
			}
			cout << endl;	
		}
	}
}

void PrintMap(vector<Village*> map) ///////TESTEO/////////
{
	for (Village* vertex : map)
	{
		cout << "Vecinos de " << vertex->GetName() << ": ";
		for (Village* v : vertex->adjVillages)
		{
			cout << v->GetName() << "; ";
		}
		cout << endl;
	}
}

void PrintVillages(vector<string> villages) //Funci�n para mostrar en pantalla todas las villas que existen
{
	cout << endl;
	for (string v : villages)
	{
		cout << v << endl;
	}
	cout << endl;
}

bool IsNumber(string s) //Funcion para saber si un string entregado es un n�mero o no
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

void AssignGuardian(vector<Village*> map, Guardian* root, Guardian* g) //Funci�n para asignar a los guardianes a sus respectivas villas, teniendo en cuenta que quien tenga el mayor Nivel de Poder ser� el Maestro de la villa
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

bool CheckVillages(vector<Village*> map) //Funci�n para comprobar si todas las villas tienen un maestro y por lo menos un aprendiz, excepto la villa final, ya que esa ciudad no tiene aprendicez
{
	for (Village* v : map)
	{
		if (v->GetName() != map.front()->GetName())
		{
			if (v->master == NULL || v->apprentices.size() <= 0)
			{
				return false;
			}
		}
	}
	return true;
}

Guardian* CreateGuardian(vector<string> villages) //Funci�n para crear a un Guardian. Para ello se pide el nombre y la ciudad en la que quiere empezar. Sus puntos de poder comienzan en 50 (la mitad del m�ximo posible)
{
	string name;
	string village;
	
	cout << "Ingrese el nombre de su Guardian: ";
	getline(cin, name);
	while (name.length() == 0)
	{
		getline(cin, name);
	}
	
	PrintVillages(villages);
	
	while (true)
	{
		cout << "Ingrese el nombre de la villa de origen de su Guardian" << endl
		<< "(Asegurese de escribir exacactamente el mismo nombre, mayusculas incluidas): ";
		getline(cin, village);
		while (village.length() == 0)
		{
			getline(cin, village);
		}
		
		if (Exists(villages, village))
		{
			return new Guardian(name, 50, village);
		}
		
		cout << "Esa villa no existe, por favor ingrese otra" << endl;
	}
}

Guardian* SelectGuardian(Guardian* root) //Funci�n para seleccionar un guardian de los que ya existen. Para que no aparezca un clon del guardian seleccionado, este se reemplaza por Rex, quien conserva los mismos atributos que el personaje elegido, y este �ltimo pasa a ser el personaje del jugador, pero con los puntos de poder al 50
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

string RecommendedOpponent(vector<Guardian*> options) //Funci�n para recomendar al aprendiz con menor Nivel de Poder. Se devuelve el nombre de este guardian para mostrarselo al jugador
{
	Guardian* recommended = options.front();
	
	for (Guardian* o : options)
	{
		if (recommended->PL > o->PL)
		{
			recommended = o;
		}
	}
	
	return recommended->GetName();
}

void ShowApprentices(vector<Guardian*> apprentices) //Funci�n para mostrar todos los aprendices de la lista entregada
{
	cout << endl;
	for (Guardian* a : apprentices)
	{
		cout << "- " << a->GetName() << endl;
	}
	cout << endl;
}

Guardian* FindApprentice(vector<Guardian*> apprentices, string name) //Funci�n para devolver al guardian que tenga el nombre entregado
{
	for (Guardian* a : apprentices)
	{
		if (a->GetName() == name)
		{
			return a;
		}
	}
	return NULL;
}

Guardian* SelectOpponent(vector<Guardian*> options) //Funci�n para seleccionar al oponente con el cual enfrentarse, el cual es retornado
{
	string name;
	
	cout << RecommendedOpponent(options) << " es el aprendiz con menor nivel de poder," << endl
	<< "por ende, el mas facil de derrotar, de todas formas, estas son todas tus opciones: " << endl;
	
	ShowApprentices(options);
	
	while (true)
	{
		cout << "Ingresa el nombre del guardian al que te quieres enfrentar: ";
		getline(cin, name);
		while (name.length() == 0)
		{
			getline(cin, name);
		}
		
		Guardian* opponent = FindApprentice(options, name);
		
		if (opponent != NULL)
		{
			return opponent;
		}
		else
		{
			cout << "Ese aprendiz no existe, por favor ingrese otro nombre" << endl;
		}
	}
}

Record* Training(Guardian* opponent, Guardian* player, bool master, Village* current) //Funcion en la que se simula el combate entre el jugador y su oponente. Si este es el maestro, el nivel de dificultad aumenta, pero tambien se ganan m�s puntos
{
	int win;
	int pointsGained = 0;
	
	if (master)
	{
		win = 4;
	}
	else
	{
		win = 2;
	}
	
	cout << endl << "\tInicia el Combate!" << endl << "Tu oponente: ";
	opponent->PrintGuardian();
	cout << "Presiona una tecla para lanzar el dado" << endl
	<< "(Necesitas un numero mayor que " << win << " para ganar la pelea)" << endl;
	if (master)
	{
		getchar();	
	}
	getchar(); 
	fflush(stdin);
	int dado = rand() % 6 + 1;
	cout << dado << endl;
	
	if (dado > win)
	{
		cout << endl << "\tFelicidades, ganaste el combate!!" << endl;
		if (current->pointsToGive - win/2 >= 0)
		{
			current->pointsToGive -= win/2;
			if (player->PL + win/2 > 100)
			{
				player->PL = 100;
				cout << "Tu Nivel de Poder ha llegado al maximo, deberias considerar ir a enfrentar al jefe final..." << endl;
			}
			else
			{
				cout << "Por la experiencia vivida tu Nivel de Poder ha incrementado en " << win/2 << " puntos" << endl;
				player->PL += win/2;
				pointsGained = win/2;
			}
		}
		else
		{
			if (master && current->pointsToGive == 1)
			{
				current->pointsToGive--;
				player->PL++;
				pointsGained++;
			}
			cout << "Ya no puedes obtener mas Puntos de Poder de esta villa, has alcanzado el maximo posible (3)" << endl;
		}
		
		cout << "Nivel de Poder: " << player->PL << endl;
		getchar();
		fflush(stdin);
		return new Record(opponent, true, pointsGained, player->PL);
	}
	else
	{
		cout << endl << "\tPerdiste el combate" << endl
		<< "Mejor suerte para la proxima, tu Nivel de Poder se mantiene intacto" << endl;
		cout << "Nivel de Poder: " << player->PL << endl;
		getchar();
		fflush(stdin);
		return new Record(opponent, true, pointsGained, player->PL);
	}
}

bool PrintAdjVillages(vector<Village*> adjVillages) //Funcion para mostrar las ciudades adyacentes, que se utiliza especificamente para el menu de Viaje
{
	if (adjVillages.size() == 0)
	{
		return false;
	}
	
	cout << endl;
	for (Village* adj : adjVillages)
	{
		cout << "- " << adj->GetName() << endl;
	}
	cout << endl;
	return true;
}

Village* Travel(Village* current, Guardian* player) //Funcion para cambiar de una villa a otra
{
	string name;
	
	if (PrintAdjVillages(current->adjVillages))
	{
		while (true)
		{
			cout << "A que villa deseas viajar? Ingresa el nombre de tu eleccion: ";
			getline(cin, name);
			while (name.length() == 0)
			{
				getline(cin, name);
			}
			
			Village* adj = FindVillage(current->adjVillages, name);
			
			if (adj != NULL)
			{
				player->PL++;
				if (player->PL > 100)
				{
					player->PL = 100;
					cout << "Tu Nivel de Poder ha llegado al maximo, deberias considerar ir a enfrentar al jefe final..." << endl;
				}
				else
				{
					cout << endl << "Por la experiencia ganada al superar los obstaculos del viaje, tu nivel de poder a aumentado en 1" << endl;
				}
				cout << "Nivel de Poder: " << player->PL << endl;
				getchar();
				fflush(stdin);
				return adj;
			}
			else
			{
				cout << "Esa villa no existe, o no esta dentro de las opciones entregadas" << endl
				<< "Por favor ingrese otro nombre" << endl;
			}
		}
	}
	else
	{
		cout << endl << "Esta villa no esta conectada a ninguna otra villa, para viajar deberas crear un portal usando Alquimia" << endl;
		getchar();
		getchar();
		fflush(stdin);
		return current;
	}
}

vector<Village*> PrintNotAdjVillages(Village* current, vector<Village*> map) //Funcion para mostrar en pantalla las villas que no estan conectadas a la villa actual
{
	vector<Village*> notAdjVillages;
	
	cout << endl;
	for (Village* v : map)
	{
		bool connected = false;
		
		if (v->GetName() == current->GetName())
		{
			continue;
		}
		
		for (Village* adj : current->adjVillages)
		{
			if (v->GetName() == adj->GetName() )
			{
				connected = true;
				break;
			}
		}
		
		if (!connected)
		{
			cout << "- " << v->GetName() << endl;
			notAdjVillages.push_back(v);
		}
	}
	cout << endl;
	
	return notAdjVillages;
}

void CreatePortal(Guardian* player, Village* current, vector<Village*> map) //Funcion para crear un camino entre una villa y otra
{
	string name;
	vector<Village*> notAdjVillages = PrintNotAdjVillages(current, map);
	
	while (true)
	{
		cout << "Ingresa el nombre de la villa con la que quieres conectar tu villa actual: ";
		getline(cin, name);
		while (name.length() == 0)
		{
			getline(cin, name);
		}
		
		Village* village = FindVillage(notAdjVillages, name);
		
		if (village != NULL)
		{
			int cost = rand() % 3 + 2;
			if (player->PL - cost >= 0)
			{
				player->PL -= cost;
				current->AddEdge(current, village);
				cout << endl << "Has creado un portal desde " << current->GetName() << " hasta " << village->GetName() << endl
				<< "Pero has gastado " << cost << " Puntos de Poder para lograrlo" << endl;
				cout << "Nivel de Poder: " << player->PL << endl;
			}
			else
			{
				cout << "No tienes los Puntos de Poder suficientes para crear un portal" << endl
				<< "Viaja o entrena maspara acumular los necesarios" << endl;
			}
			getchar();
			fflush(stdin);
			return;
		}
		else
		{
			cout << "Esa villa no existe, o ya esta conectada a tu villa actual, por favor ingrese otro nombre" << endl;
		}
	}
}

void PrintJourney(list<string> journey, Guardian* player) //Funcion para mostrar el recorrido de villas realizado por el jugador
{
	for (string v : journey)
	{
		if (v == journey.front())
		{
			cout << endl << "El viaje de " << player->GetName() << " inicio en " << v << ", y luego su aventura siguio por: " << endl;
			continue;
		}
		cout << v << endl;
	}
	cout << "En ese orden" << endl;
	getchar();
	getchar();
	fflush(stdin);
}

void PrintHistory(list<Record*> history, Guardian* player) //Funcion para mostrar el historial de combates del jugador
{
	cout << endl << player->GetName() << " ha peleado incontables batallas" << endl 
	<< "y como buen candidato a convertirse en Guardian, las recuerda todas: " << endl;
	for (Record* r : history)
	{
		cout << endl << "Se enfrento a " << r->opponent->GetName() << " una vez en " << r->opponent->GetVillage();
		if (r->result)
		{
			cout << " y gano ese combate..." << endl;
		}
		else
		{
			cout << " y perdio ese combate..." << endl;
		}
		cout << "...Con lo cual obtuvo " << r->pointsGained << " Puntos de Poder"
		<< " terminando con un total de " << r->pointsAccumulated << " Puntos" << endl;
	}
	getchar();
	getchar();
	fflush(stdin);
}

bool AllVillagesVisited(vector<Village*> map) //Funcion para comprobar si todas las villas han sido visitadas
{
	for (Village* v : map)
	{
		if (!v->visited)
		{
			return false;
		}
	}
	return true;
}

void PrintFinalFight(Guardian* boss, Guardian* player) //Funcion que muestra el combate final entre el jugador y el jefe
{
	boss->PrintGuardian();
	cout << endl << "\t\t V/S " << endl;
	player->PrintGuardian();
}

void FinalFight(Guardian* boss, Guardian* player, list<string> journey, list<Record*> history) //Ultima funcion que hace las de conclusion para la aventura vivida por el Guardi�n del jugador
{
	string op;
	
	cout << endl << "Finalmente, despues de todos los entrenamientos y viajes que " << player->GetName() << " ha vivido durante su aventura," << endl
	<< "llego la hora de enfrentarse a " << boss->GetName() << " para convertirse en Guardian..." << endl;
	PrintFinalFight(boss, player);
	getchar();
	getchar();
	fflush(stdin);
	cout << endl << "\t...Felicidades, has completado el juego" << endl
	<< endl << "Te gustaria ver tu recorrido como aprendiz? Ingresa <1> para verlo: ";
	cin >> op;
	
	if (op == "1")
	{
		PrintJourney(journey, player);
	}
	
	cout << endl << "Te gustaria ver tu historial de combate? Ingresa <1> para verlo: ";
	cin >> op;
	
	if (op == "1")
	{
		PrintHistory(history, player);
	}
}

int main()
{
	vector<Village*> map;
	vector<string> readVillages;
	
	cout << "Cargando archivos . . .   " << endl;
	sleep(1);
	
	/*
		A continuacion se realiza la carga del archivo de las Villas. Para ello, se mantiene un registro
		sobre los nombres de las villas que ya han sido ingresadas y creadas, para no repetrilas, y de esa
		forma se van agregando las distintas villas, y creando aristas entre ellas, representadas por la 
		lista de adyacencia declarada arriba (map), en base a lo entregado en el archivo Villages.txt.
	*/
	
	string filename = "Villages.txt";
	ifstream file(filename);
	
	if (file.is_open()) 
	{
		string line;
		
		getline(file, line);
		stringstream ss(line);
		
		Village* end;
		
		string name;
		getline(ss, name, ',');
		end = new Village(name);
		map.push_back(end);
		readVillages.push_back(name);
		
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
		
		file.close();
	}
	else 
	{	
		cout << "Failed to open file: " << filename << endl;
		return 0;
	}
	
	if (map.size() < 3)
	{
		cout << "Hubo un problema al leer las villas, ya que en total son menos de 3" << endl
		<< "(Se recomienda que sean por lo menos 3)" << endl;
		return 0;
	}
	
	/*
		Carga del archivo de Guardianes: Se parte por hacer la carga del "Jefe Final", que en el caso especifico del proyecto es
		Stormheart, segun sus puntos de poder se define el maximo posible. Posteriormente se le asignan sus aprendices segun
		se vayan entregando en el archivo Guardians.txt, y por ultimo, se asignan a sus respectivas villas.
	*/
	
	Guardian* root; //Arbol para representar la jerarqu�a maestro/aprendiz de los Guardianes
	
	filename = "Guardians.txt";
	ifstream file2(filename);
	
	if (file2.is_open())
	{
		string line;
		
		getline(file2, line);
		stringstream ss(line);
		
		string name;
		getline(ss, name, ',');
		
		root = new Guardian(name, 100, map.front()->GetName());
		
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
							cout << "Hubo un problema al leer los puntos de poder de uno de los guardianes, ya que ese numero es muy peque�o" << endl
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
		
		file.close();
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
	
	/*
		Inicio del juego: Bienvenida y explicacion del juego
	*/
	
	Guardian* player;
	Village* current;
	list<string> journey;
	list<Record*> history;
	int op;
	bool done = false;
	
	cout << "\tBienvenido a The Guardian Journey" << endl
	<< "Eres nuev@ o ya sabes jugar? Ingresa <1> si te gustaria ver una pequena explicacion del juego: ";
	cin >> op;
	
	if (op == 1)
	{
		cout << endl << "The Guardian Journey es un pequeno juego que busca solucionar una problematica entregada como proyecto" << endl
		<< "de unidad para la asignatura Algoritmo y Estructuras de Datos, el cual usa los mismos para crear" << endl
		<< "el programa que utilizara a continuacion" << endl << endl
		<< "En el juego tendras la opcion de crear o seleccionar un Guardian en base a los personajes cargados del archivo" << endl
		<< "Guardian.txt, y posteriormente podras viajar, enfrentarte a aprendices o maestros para acumular Puntos o Nivel de Poder," << endl
		<< "con el objetivo de llegar a la ciudad del jefe final y vencerlo" << endl 
		<< "Tambien podras crear caminos entre ciudades desconectadas usando alquimia," << endl
		<< "lo cual gasta entre 2 y 4 Puntos de Poder" << endl << endl
		<< "Ademas, podras ver las ciudades por las que has viajado a lo largo del juego y tu historial de combate" << endl << endl
		<< "Para jugar solo debera seguir las instrucciones que aparecen en pantalla e ingresar por teclado sus opciones" << endl
		<< "Para mas informacion, revise el archivo README adjunto al mismo programa que esta utilizando" << endl;
	}
	
	cin.clear();
	cin.ignore();
	fflush(stdin);
	
	/*
		Selección o creación de personaje: En esta sección se pregunta al usuario qué desea hacer y segun eso ingresa el
		nombre del nuevo guardia que quiere crear, y si no se reemplaza al jugador seleccionado por un guardian interno
	*/
	while (!done)
	{
		
		cout << endl << endl << "Desea crear un personaje o seleccionar uno preexistente?" << endl
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
			cout << "Esa opcion no es valida, por favor ingrese otra" << endl;
			cin.clear();
			cin.ignore();
			fflush(stdin);
		}
	}
	
	cout << endl << "Hoy es el gran dia que los videntes profetizaban, el nacimiento de una leyenda, y es que" << endl
	<< player->GetName() << " inicia su viaje para convertirse en el mejor Guardian del nuevo mundo" << endl << endl
	<< "Este es el comienzo de su aventura, en la que debera acumular Puntos de Poder para poder enfrentarse a " << endl
	<< root->GetName() << ", que se encuentra en " << map.front()->GetName() << ", y obtener el titulo de Guardian" << endl << endl
	<< "\tSera capaz nuestro valiente aprendiz de lograr su cometido..?" << endl << endl
	<< "Presiona <Enter> para continuar";
	getchar();
	getchar(); //esto está pensado únicamente para hacer una pausa y que haya interacción por parte del usuario
	fflush(stdin);
	
	current = FindVillage(map, player->GetVillage());
	journey.push_back(current->GetName());
	done = false;
	
	/*
		En la siguiente sección se comienzan a mostrar todas las acciones que el jugador puede realizar durante el juego, 
		haciendo la distincion sobre si se encuentra en la villa del jefe final o no.
	*/
	
	while (!done)
	{
		if (current->GetName() != journey.back())
		{
			journey.push_back(current->GetName());
		}
		
		current->visited = true;
		
		cout << endl << "\t\tTe encuentras actualmente en " << current->GetName() << endl << endl
		<< "\tQue deseas hacer?" << endl;
		if (current->GetName() == map.front()->GetName())
		{
			cout << "(1) Enfrentarte a " << root->GetName() << endl;
		}
		else
		{
			cout << "(1) Entrenar con un aprendiz" << endl
			<< "(2) Entrenar con el maestro de la villa" << endl;
		}
		cout << "(3) Viajar" << endl
		<< "(4) Usar alquimia (crear un portal para conectar dos ciudades desconectadas)" << endl
		<< "(5) Ver tu recorrido" << endl
		<< "(6) Ver tu historial de entrenamiento" << endl
		<< "(7) Salir del programa" << endl;
		cin >> op;
		
		switch (op)
		{
			case 1:
				if (current->GetName() == map.front()->GetName())
				{
					if (player->PL >= 90 || AllVillagesVisited(map))
					{
						FinalFight(root, player, journey, history);
						done = true;
					}
					else
					{
						cout << endl << "Todavia no tienes la experiencia necesaria para enfrentarte a " << root->GetName() << endl
						<< "(Necesitas haber visitado todas las villas o tener un Nivel de Poder mayor o igual a 90)" << endl
						<< "Vuelve mas tarde cuando estes preparado" << endl;
						getchar();
						getchar();
						fflush(stdin);
					}
				}
				else
				{
					history.push_back(Training(SelectOpponent(current->apprentices), player, false, current));
				}
				break;
			
			case 2:
				if (current->GetName() == map.front()->GetName())
				{
					cout << "Esa Opcion no es valida, por favor ingrese otra" << endl
					<< "(Actualmente te encuentras en " << map.front()->GetName() << ", y aqui no hay aprendices)" << endl;
					getchar();
					getchar();
					fflush(stdin);
				}
				else
				{
					history.push_back(Training(current->master, player, true, current));
				}
				break;
			
			case 3:
				current = Travel(current, player);
				break;
			
			case 4:
				CreatePortal(player, current, map);
				break;
			
			case 5:
				PrintJourney(journey, player);
				break;
			
			case 6:
				PrintHistory(history, player);
				break;
				
			case 7:
				done = true;
				break;
			
			default:
				cout << "Esa opcion no es valida, por favor ingrese otra" << endl;
				cin.clear();
				cin.ignore();
				break;
		}
	}
	
	cout << endl << "\tGracias por jugar The Guardian Journey" << endl;
	getchar();
	getchar();
	fflush(stdin);
	
	return 0;
}
