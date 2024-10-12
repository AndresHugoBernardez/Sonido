#pragma once
//
//
//
//
//
//
//  ♦Andy's Softwares♦
//
// Estudio de archivos WAV: Generador de onda senoidal
// ***************************************************
//
//  Referencias: http://soundfile.sapp.org/doc/WaveFormat/
//
//
// *****************************
// Modo de uso:
//  Ejecute e ingrese datos
//
//  El resultado es un archivo con formato WAV con una frecuencia de onda senoidal que puede reproducir en un reproductor. 
//
//

#ifndef AndysWAV 
#define AndysWAV



#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"iostream"
#include<fstream>


using namespace std;
const double PI = 3.14159265;

///Convierte un número menor a 16 en char con valor hexadecimal
char numeroAHex(int numero)
{
	switch (numero) {
	case 0:return('0');
	case 1:return('1');
	case 2:return('2');
	case 3:return('3');
	case 4:return('4');
	case 5:return('5');
	case 6:return('6');
	case 7:return('7');
	case 8:return('8');
	case 9:return('9');
	case 10:return('A');
	case 11:return('B');
	case 12:return('C');
	case 13:return('D');
	case 14:return('E');
	case 15:return('F');
	default:return('0');
	}
}


///Imprime en pantalla un número menor a 256 en dos caracteres con su valor en hexadecimal
void ConvertirHex(int numero) {

	int hexenas, unidad;
	unidad = numero % 16;
	hexenas = numero / 16;




	printf("%c%c ", numeroAHex(hexenas), numeroAHex(unidad));
}

/// Retorna un valor de una onda senoidal según parámetros a base del eje discreto n.
long unsigned int OndaSenoidal(long int n, double A, long int f, long int fm) {

	long unsigned int salida;


	salida = (long unsigned int)(A * sin(2 * PI * (double)f * ((double)n / (double)fm)));

	return(salida);

}

/// Descompone un numero de 16 bits en dos numeros de 8 bits
void TransformLongIntToIntInt(unsigned char* alto, unsigned char* bajo, long unsigned  int entrada) {




	*bajo = (unsigned char)((entrada) % 256);
	*alto = (unsigned char)((entrada / 256));


}



/// descompone un numero de 24 bits en tres numeros de 8 bits
void TransformLongIntToCharCharChar(unsigned char* alto, unsigned char* medio, unsigned char* bajo, long int tiempo) {
	*bajo = (unsigned char)(tiempo % 256);
	*medio = (unsigned char)((tiempo / 256) % 256);
	*alto = (unsigned char)(tiempo / (256 * 256));


}

/// Encabezado típico de un archivo WAV en PCM audio format 1 con 2 canales con SampleRate de 44100hz, 176400 byteRate, blockalign de 4, 16 bits per sample,  
unsigned char EncabezadoWAV[44] = { 'R','I','F','F',0,0,0,0,'W','A','V','E','f','m','t',' ',16,0,0,0,1,0,2,0,68,172,0,0,16,177,2,0,4,0,16,0,'d','a','t','a',0,0,0,0 };




int InicializarArchivo(ofstream *OutFile,long int * tiempo) {

	int salir;
	//ofstream OutFile;
	unsigned char alto, bajo, medio;
	long int i;
	//long int tiempo, frec;
	long unsigned int salida;;

	//double Amplitud;
	// datos del programa
	char direccion[200] = "";
	char duracion[3] = "00";
	//char frecuencia[6] = "00000";
	//char volumen[4] = "000";
	//-------------------


	//----------------Cargar datos adicionales
	//cout << "\n\n Andy's Softwares:\n\n Generador de onda senoidal\n\n **recuerde bajar el volumen al escuchar los archivos...\n\n";
	cout << "\n\n Andy's Softwares:\n\n Grabador de microfono\n\n **recuerde bajar el volumen al escuchar los archivos...\n\n";
	cout << "\nIngrese nombre del archivo(agregue extension '.wav'):";

	cin >> direccion;


	duracion[2] = '\0';
	salir = 0;
	while (salir != 1) {
		cout << "\nIngrese duracion en segundos(maximo 30):";
		cin >> duracion;


		if (duracion[0] <= '9' && duracion[0] >= '0' && ((duracion[1] <= '9' && duracion[1] >= '0') || duracion[1] == '\0')) {

		
			if (duracion[1] != '\0')*tiempo = ((int)duracion[0] - 48) * 10 + ((int)duracion[1] - 48);
			else *tiempo = (int)(duracion[0] - '0');

			if (*tiempo < 31) {
				cout << "\n Tiempo elegido=" << *tiempo;
				*tiempo = *tiempo * 44100;
				cout << " muestras:" << *tiempo << endl;
				salir = 1;
			}
			

		}

		if (salir == 0) cout << "\n Error de dato, debe ser un numero menor a 30";

	}

	/*


	salir = 0;
	while (salir != 1) {
		frec = 0;
		cout << "\nIngrese frecuencia en hz(maximo 20000):";
		cin >> frecuencia;


		for (i = 0; i < 6; i++) {

			if (frecuencia[i] != '\0') {


				if (frecuencia[i] <= '9' && frecuencia[i] >= '0') {


					frec = ((long int)(frecuencia[i] - '0')) + frec * 10;



				}
				else {
					i = 8;
					salir = 2;
				}
			}
			else {
				salir = 1;
				i = 8;
			}
		}




		if (salir == 2 || frec >= 20000) {
			cout << "\n Error de dato, debe ser un numero menor a 20000";
			salir = 0;

		}
		else if (salir == 1) {
			cout << "\n La frecuencia es " << frec << "Hz" << endl;
		}

	}


	salir = 0;
	while (salir != 1) {
		Amplitud = 0;
		cout << "\nIngrese Amplitud de 0 a 100%(maximo 100):";
		cin >> volumen;


		for (i = 0; i < 4; i++) {

			if (volumen[i] != '\0') {


				if (volumen[i] <= '9' && volumen[i] >= '0') {


					Amplitud = ((long int)(volumen[i] - '0')) + Amplitud * 10;



				}
				else {
					i = 8;
					salir = 2;
				}
			}
			else {
				salir = 1;
				i = 8;
			}
		}




		if (salir == 2 || Amplitud > 100) {
			cout << "\n Error de dato, debe ser un numero menor a 100";
			salir = 0;

		}
		else if (salir == 1) {
			cout << "\n La Amplitud es " << Amplitud << "%";
			Amplitud = (Amplitud / 100);
			Amplitud = 32767 * Amplitud;//32767
			cout << "    En 16 bits es el numero=" << Amplitud << endl;
		}

	}



	*/


	/////*********************************Generando archivo.....

	//cout << "\n\n Creando archivo";

	(*OutFile).open(direccion, ios::out | ios::binary);

	if ((*OutFile)) {
		//cout << "... Archivo abierto";
		(*OutFile).seekp(0, ios::beg);

		//cout << "...Creando Encabezado";

		for (i = 0; i < 45; i++) {



			(*OutFile).write((const char*)&EncabezadoWAV[i], sizeof(unsigned char));



		}

		//escribiendo memoria que va a ocupar:
		//cout << "...Escribiendo memoria";

		
		
		TransformLongIntToCharCharChar(&alto, &medio, &bajo, (*tiempo * 4 + 36));
		

		(*OutFile).seekp(4, ios::beg);


		(*OutFile).write((const char*)&bajo, sizeof(unsigned char));
		(*OutFile).write((const char*)&medio, sizeof(unsigned char));
		(*OutFile).write((const char*)&alto, sizeof(unsigned char));

		TransformLongIntToCharCharChar(&alto, &medio, &bajo, (*tiempo * 4));


		(*OutFile).seekp(40, ios::beg);



		(*OutFile).write((const char*)&bajo, sizeof(unsigned char));
		(*OutFile).write((const char*)&medio, sizeof(unsigned char));
		(*OutFile).write((const char*)&alto, sizeof(unsigned char));


		(*OutFile).seekp(44, ios::beg);

		//cout << "...Escribiendo tono";
		/*

		for (i = 0; i < tiempo; i++) {

			salida = OndaSenoidal(i, Amplitud, frec, 44100);

			TransformLongIntToIntInt(&alto, &bajo, salida);




			OutFile.write((const char*)&bajo, sizeof(unsigned char));
			OutFile.write((const char*)&alto, sizeof(unsigned char));

			OutFile.write((const char*)&bajo, sizeof(unsigned char));
			OutFile.write((const char*)&alto, sizeof(unsigned char));


		}

		cout << "...Listo!";
		OutFile.close();
		cout << "... Archivo cerrado" << endl << endl;
		system("pause");

		//<>Andy'sSoftwares<>
		*/

	}
	else {
		cout << "... Error al abrir archivo";
		return(1);
	}
	//*/




	if (salir == 1)return (0);
	else return(1);

}


#endif // !AndysWAV		