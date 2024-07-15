//
//
//
//
//
//
//  Andy's Software
//
// Estudio de archivos WAV: Generador de onda senoidal
// ***************************************************
//
//  Referencias: http://soundfile.sapp.org/doc/WaveFormat/
//
//
//*****************************
// Modo de uso:
//  Ejecute e ingrese datos
//
//  El resultado es un archivo con formato WAV con una frecuencia de onda senoidal que puede reproducir en un reproductor. 
//
//



#include<stdio.h>
#include<stdlib.h>
#include<math.h>

const float PI=3.141592;

///Convierte un número menor a 16 en char con valor hexadecimal
char numeroAHex(int numero)
{
	switch(numero){
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
	}
}


///Imprime en pantalla un número menor a 256 en dos caracteres con su valor en hexadecimal
void ConvertirHex(int numero){
	
	int hexenas,unidad;
	unidad=numero%16;
	hexenas=numero/16;
	
	
	
	
	printf("%c%c ",numeroAHex(hexenas),numeroAHex(unidad));
}

/// Retorna un valor de una onda senoidal según parámetros a base del eje discreto n.
float OndaSenoidal(int n,int A,int f,int fm){
	
	float salida;	
	
	
	salida=A*sin(2*PI*f*n/fm);
	
	return(salida);
	
}

/// Descompone un numero de 16 bits en dos numeros de 8 bits
void TransformFloatToIntInt(unsigned char * alto,unsigned char * bajo, float entrada){
	
	
	*bajo=(unsigned char)(((int)entrada)%256);
	*alto=(unsigned char)((int)(entrada/256));
	
}



/// descompone un numero de 24 bits en tres numeros de 8 bits
void TransformLongIntToCharCharChar(unsigned char * alto, unsigned char * medio, unsigned char * bajo,long int tiempo){
	*bajo=(unsigned char)(tiempo%256);
	*medio=(unsigned char)((tiempo/256)%256);
	*alto=(unsigned char)(tiempo/(256*256));
	
	
}

/// Encabezado típico de un archivo WAV en PCM audio format 1 con 2 canales con SampleRate de 44100hz, 176400 byteRate, blockalign de 4, 16 bits per sample,  
unsigned char EncabezadoWAV[44]={'R','I','F','F',0,0,0,0,'W','A','V','E','f','m','t',' ',16,0,0,0,1,0,2,0,68,172,0,0,16,177,2,0,4,0,16,0,'d','a','t','a',0,0,0,0};




void main(){
	
	FILE * archivo;
	int salir;
	unsigned char alto,bajo,medio;
	long int i,tiempo,frec; 
	float salida;
	// datos del programa
	char direccion[200];
	char duracion[3]="00";
	char frecuencia[6]="00000";
	//-------------------
	
	
	//----------------Cargar datos adicionales
	printf("\nIngrese nombre del archivo:");
	scanf(" %s",&direccion);
	

	duracion[2]='\0';
	salir=0;
	while(salir!=1){
		printf("\nIngrese duracion en segundos(maximo 30)");
		scanf(" %s",&duracion);
		
		
		if(duracion[0]<='9'&&duracion[0]>='0'&&( (duracion[1]<='9'&&duracion[1]>='0') || duracion[1]=='\0'  ) ){
		
		
			if(duracion[1]!='\0')tiempo=((int)duracion[0]-48)*10+((int)duracion[1]-48);
			else tiempo=((int)duracion[0]-48);
			
				if(tiempo<31){
				printf("\n Tiempo elegido=%ld",tiempo);
				tiempo=tiempo*44100;
				printf(" muestras: %ld",tiempo);
				salir=1;
				}
				
	
		}
		
		if(salir==0) printf("\n Error de dato, debe ser un numero menor a 30");
	
	}
	
	
	salir=0;
	while(salir!=1){
		frec=0;
		printf("\nIngrese frecuencia en hz(maximo 20000)");
		scanf(" %s",&frecuencia);
		
		
		for(i=0;i<6;i++){
		
		if(frecuencia[i]!='\0'){
		
		
			if(frecuencia[i]<='9'&&frecuencia[i]>='0'){
			
			
				frec=((long int)(frecuencia[i]-48))+frec*10;
				
					
					
			}
			else{
				i=8;
				salir=2;
			}
		}
		else{
			salir=1;
			i=8;
		}
		}
		
		
		
		
		if(salir==2 || frec>=20000){
		 printf("\n Error de dato, debe ser un numero menor a 20000");
		 salir=0;
		
		 }
		else if(salir==1){
			printf("\n La frecuencia es %ld",frec);
		}
	
	}
	
	
	
	
	archivo=fopen(direccion,"wb+");
	
	if(archivo!=NULL){
		printf("\n\n Archivo abierto");
		fseek(archivo,0,SEEK_SET);

		printf("...Creando Encabezado");
		
		for(i=0;i<45;i++){
			
			
			
			
			fwrite(&(EncabezadoWAV[i]),sizeof(unsigned char),1,archivo);






		}
		
		//escribiendo memoria que va a ocupar:
		printf("...Escribiendo memoria");
		
		TransformLongIntToCharCharChar(&alto,&medio,&bajo,(tiempo*4+36));
		
		fseek(archivo,4,SEEK_SET);
		
		fwrite(&bajo,sizeof(unsigned char),1,archivo);
		fwrite(&medio,sizeof(unsigned char),1,archivo);
		fwrite(&alto,sizeof(unsigned char),1,archivo);

		
		
		TransformLongIntToCharCharChar(&alto,&medio,&bajo,(tiempo*4));
		
		fseek(archivo,40,SEEK_SET);
		
		fwrite(&bajo,sizeof(unsigned char),1,archivo);
		fwrite(&medio,sizeof(unsigned char),1,archivo);
		fwrite(&alto,sizeof(unsigned char),1,archivo);

		
		fseek(archivo,44,SEEK_SET);
	printf("...Escribiendo tono");

	for(i=0;i<tiempo;i++){
			
			salida=OndaSenoidal(i,20000,frec,44100);
			
			TransformFloatToIntInt(&alto,&bajo,salida);
			
			
			
			
			
		
			fwrite(&bajo,sizeof(unsigned char),1,archivo);
			fwrite(&alto,sizeof(unsigned char),1,archivo);
		
			fwrite(&bajo,sizeof(unsigned char),1,archivo);
			fwrite(&alto,sizeof(unsigned char),1,archivo);
			
			
		}
		
		printf("...listo!\n");
		fclose(archivo);
		printf("Archivo cerrado\n");
		system("pause");
		
		//<>Andy'sSoftwares<>
		
		
	}
	
	
	
	
}
