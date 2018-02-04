/*
Practica 02 : MasterMind
Grupo 1ºB FP
Alumnos: ALBERTO GARCIA DOMENECH y PABLO DAURELL MARINA
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>
using namespace std;

const int TAM_CODIGO = 4;
const int INTENTOS_INICIALES = 15;

typedef enum { ROJO, AZUL, VERDE, AMARILLO, MARRON, BLANCO, INCORRECTO, NUM_COLORES = INCORRECTO } tColor;
typedef tColor tCodigo[TAM_CODIGO];

char color2char(const tColor codigo);
tColor char2color(char codigo);
void codigoAleatorio(tCodigo codigo, bool admiteRepetidos);
void compararCodigos(const tCodigo codigo, const tCodigo hipotesis, int& colocados, int& descolocados);
int menu();
void juego(bool admiteRepetidos);
void mostrarCodigo(const tCodigo codigo);
void introducirCodigo(tCodigo hipotesis);
void inicializarArrayBool(bool array[], int n);


int main()
{
	srand(time(NULL));
	int opcion;
	bool admiteRepetidos;

	do
	{
		opcion = menu();
		switch (opcion)
		{
		case 1:
			if (TAM_CODIGO > NUM_COLORES) //Evita error por aumentar TAM_CODIGO pero no NUM_COLORES (Este error no ocurre si el usuario no toca el codigo fuente)
			{
				cout << "ERROR: No se puede generar un codigo de " << TAM_CODIGO << " elementos sin repeticiones, solo con "
					<< NUM_COLORES << " colores" << endl << endl;
			}
			else
			{
				cout << "Jugar sin repetidos" << endl << endl; admiteRepetidos = false; juego(admiteRepetidos);
			}
			break;
		case 2:
			cout << "Jugar con repetidos" << endl << endl; admiteRepetidos = true; juego(admiteRepetidos); break;
		}
	} while (opcion != 0);

	cout << endl << "FINALIZANDO JUEGO" << endl;

	system("pause");
	return 0;
}

int menu()
{
	int opcion;

	cout << "~ MASTERMIND ~" << endl << "Intenta adivinar el codigo de colores que he pensado." << endl
		<< "Tras cada intento sabras cuantos colores estan bien colocados y cuales no." << endl
		<< endl << "Averigua el codigo en el menor numero de intentos!!\n" << endl << "----------------------------------------"
		<< endl << "1.Jugar con codigo sin colores repetidos\n2.Jugar con codigo con colores repetidos\n\n0.Salir" << endl
		<< "----------------------------------------" << endl;

	cout << "Elige una opcion: ";
	cin >> opcion;

	while (opcion < 0 || opcion > 2)
	{
		if (opcion < 0 || opcion > 2)
		{
			cout << "Opcion no valida. Prueba otra vez: ";
		}
		cin >> opcion;
	}

	return opcion;
}

void juego(bool admiteRepetidos)
{
	tCodigo codigo, hipotesis;
	int colocados, descolocados, intentos = INTENTOS_INICIALES;

	codigoAleatorio(codigo, admiteRepetidos);

	cout << "[INFO DEPURACION] Codigo secreto: ";
	mostrarCodigo(codigo);
	cout << endl;

	do
	{
		colocados = 0;
		descolocados = 0;

		introducirCodigo(hipotesis);
		mostrarCodigo(hipotesis);

		compararCodigos(codigo, hipotesis, colocados, descolocados);

		cout << "Colocados: " << colocados << "  Descolocados: " << descolocados << endl;

		intentos--;

		cout << "Intentos: " << intentos << endl << endl;

	} while (intentos > 0 && colocados != TAM_CODIGO);

	if (colocados == TAM_CODIGO)
	{
		cout << "CORRECTO Has encontrado el codigo!" << endl;
		cout << "Has necesitado " << INTENTOS_INICIALES - intentos << " intentos" << endl << endl;
	}
	else
	{
		cout << "Has agotado los intentos" << endl << "El codigo era:  ";
		mostrarCodigo(codigo);
		cout << "FIN DEL JUEGO" << endl << endl;
	}
}

void codigoAleatorio(tCodigo codigo, bool admiteRepetidos)
{
	bool coloresUsados[NUM_COLORES];
	bool colocado;
	int color;

	inicializarArrayBool(coloresUsados, NUM_COLORES);

	if (admiteRepetidos)
	{
		for (int i = 0; i < TAM_CODIGO; i++)
		{
			codigo[i] = tColor(rand() % NUM_COLORES);
		}
	}
	else
	{
		for (int i = 0; i < TAM_CODIGO; i++)
		{
			do
			{
				color = rand() % NUM_COLORES; //Genera numero del color en tColor y posicion de ese color en coloresUsados[]
				codigo[i] = tColor(color);

			} while (coloresUsados[color] == true);

			coloresUsados[color] = true;
		}
	}
}

void introducirCodigo(tCodigo hipotesis)
{
	string hipotesisString;
	bool incorrecto;

	do
	{
		do
		{
			incorrecto = false;

			cout << "Introduce el codigo (" << TAM_CODIGO << " letras R, Z, V, A, M o B): ";
			cin >> hipotesisString;

			if (hipotesisString.length() != TAM_CODIGO)
			{
				cout << "ERROR: El codigo debe tener " << TAM_CODIGO << " caracteres" << endl << endl;
			}

		} while (hipotesisString.length() != TAM_CODIGO);

		int  i = 0;

		while (i < TAM_CODIGO && !incorrecto)
		{
			hipotesis[i] = char2color(toupper(hipotesisString[i])); //Convertir chars de string a MAYUS y a tColor y guardarlos en array tCodigo hipotesis

			if (hipotesis[i] == INCORRECTO)
			{
				cout << "ERROR: Has introducido algun caracter no valido" << endl << endl;
				incorrecto = true;
			}

			i++;
		}
	} while (incorrecto);

}

void compararCodigos(const tCodigo codigo, const tCodigo hipotesis, int& colocados, int& descolocados)
{
	bool posicionesHipotesis[TAM_CODIGO], posicionesCodigo[TAM_CODIGO]; //Marcan que posiciones han sido ya comprobadas

	inicializarArrayBool(posicionesHipotesis, TAM_CODIGO);
	inicializarArrayBool(posicionesCodigo, TAM_CODIGO);

	for (int i = 0; i < TAM_CODIGO; i++) //Colocados
	{
		if (hipotesis[i] == codigo[i])
		{
			colocados++;
			posicionesCodigo[i] = true;
			posicionesHipotesis[i] = true;
		}
		else
		{
			posicionesCodigo[i] = false;
			posicionesHipotesis[i] = false;
		}
	}

	for (int i = 0; i < TAM_CODIGO; i++) //Descolocados
	{
		if (posicionesHipotesis[i] == false)
		{
			int j = 0;
			bool encontrado = false;

			while ((j < TAM_CODIGO) && (encontrado == false))
			{
				if ((posicionesCodigo[j] == false) && (i != j) && (hipotesis[i] == codigo[j]))
				{
					descolocados++;
					encontrado = true;
					posicionesHipotesis[i] = true;
					posicionesCodigo[j] = true;
				}
				else
				{
					j++;
				}
			}
		}
	}
}

void mostrarCodigo(const tCodigo codigo)
{
	char color;

	for (int i = 0; i < TAM_CODIGO; i++)
	{
		color = color2char(codigo[i]);
		cout << color;
	}
	cout << endl;
}

char color2char(const tColor codigo)
{
	char color;

	switch (codigo)
	{
	case ROJO: color = 'R'; break;
	case AZUL: color = 'Z'; break;
	case VERDE: color = 'V'; break;
	case AMARILLO: color = 'A'; break;
	case MARRON: color = 'M'; break;
	case BLANCO: color = 'B'; break;
	}

	return color;
}

tColor char2color(char codigo)
{
	tColor color;

	switch (codigo)
	{
	case 'R': color = ROJO; break;
	case 'Z': color = AZUL; break;
	case 'V': color = VERDE; break;
	case 'A': color = AMARILLO; break;
	case 'M': color = MARRON; break;
	case 'B': color = BLANCO; break;
	default: color = INCORRECTO; break;
	}

	return color;
}

void inicializarArrayBool(bool array[], int n)
{
	for (int i = 0; i < n; i++)
	{
		array[i] = false;
	}
}