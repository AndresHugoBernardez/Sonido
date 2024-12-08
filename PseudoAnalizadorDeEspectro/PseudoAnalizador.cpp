//
// Configuración de opencv: https://www.youtube.com/watch?v=ijlshyzsNbc
//
//  AS: Andy's Softwares
//
//
//   Principio para crear un analizador de espectro de sonido. 
//
//
//

#include<opencv2\opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;



/// <summary>
/// Dibuja bastones de 20px de ancho separadas 10 px entre ellos.
/// </summary>
/// <param name="Imagen">Matriz donde se dibujará</param>
/// <param name="Posicion">Posicion que va desde 1, 2,3 hasta n</param>
/// <param name="Base">Punto base de referencia de los bastones.</param>
/// <param name="Altura">Altura del bastón.</param>
/// <param name="Color">Color del bastón.</param>
void Baston(Mat Imagen, int Posicion, Point Base,int Altura,Scalar Color ) {

	int posicionFinal = 0, i = 0;

	posicionFinal = Base.x;
	for (i = 1; i < Posicion; i++) {
		posicionFinal += 30;
	}


	rectangle(Imagen,
		Point(posicionFinal, Base.y),
		Point(posicionFinal+20, Base.y - Altura),
		Color,
		1,
		LINE_8);

}



int bastones[3] = {10, 10, 10};




void SelectBaston(Mat Imagen,char tecla) {
	Point base(40, 270);
	switch (tecla) {
	case '1':  Baston(Imagen, 1, base, bastones[0], Scalar(200, 200, 200)); bastones[0] = 10; Baston(Imagen, 1, base, 10, Scalar(255, 0, 0)); break;
	case '4':  Baston(Imagen, 1, base, bastones[0], Scalar(200, 200, 200)); bastones[0] = 50; Baston(Imagen, 1, base, 50, Scalar(255, 0, 0)); break;
	case '7':  Baston(Imagen, 1, base, bastones[0], Scalar(200, 200, 200)); bastones[0] = 100; Baston(Imagen, 1, base, 100, Scalar(255, 0, 0)); break;
	case '2':  Baston(Imagen, 3, base, bastones[1], Scalar(200, 200, 200)); bastones[1] = 10; Baston(Imagen, 3, base, 10, Scalar(255, 0, 0)); break;
	case '5':  Baston(Imagen, 3, base, bastones[1], Scalar(200, 200, 200)); bastones[1] = 50; Baston(Imagen, 3, base, 50, Scalar(255, 0, 0)); break;
	case '8':  Baston(Imagen, 3, base, bastones[1], Scalar(200, 200, 200)); bastones[1] = 100; Baston(Imagen, 3, base, 100, Scalar(255, 0, 0)); break;
	case '3':  Baston(Imagen, 5, base, bastones[2], Scalar(200, 200, 200)); bastones[2] = 10; Baston(Imagen, 5, base, 10, Scalar(255, 0, 0)); break;
	case '6':  Baston(Imagen, 5, base, bastones[2], Scalar(200, 200, 200)); bastones[2] = 50; Baston(Imagen, 5, base, 50, Scalar(255, 0, 0)); break;
	case '9':  Baston(Imagen, 5, base, bastones[2], Scalar(200, 200, 200)); bastones[2] = 100; Baston(Imagen, 5, base, 100, Scalar(255, 0, 0)); break;
	case '0':  Baston(Imagen, 1, base, bastones[0], Scalar(200, 200, 200)); bastones[0] = 10; Baston(Imagen, 1, base, 10, Scalar(255, 0, 0)); 
			   Baston(Imagen, 3, base, bastones[1], Scalar(200, 200, 200)); bastones[1] = 10; Baston(Imagen, 3, base, 10, Scalar(255, 0, 0)); 
			   Baston(Imagen, 5, base, bastones[2], Scalar(200, 200, 200)); bastones[2] = 10; Baston(Imagen, 5, base, 10, Scalar(255, 0, 0)); break;
	}


}




int main() {

	Mat img;
	char caracter;
	img= Mat(300, 600, CV_8UC3, Scalar(200, 200, 200));
	
	
	cout << "Utilice los numeros desde 0 hasta 9, 'c' para cerrar"<<endl;
	system("pause");
	
	caracter = '0';
	SelectBaston(img, caracter);

	namedWindow("imagen", WINDOW_NORMAL);
	imshow("imagen", img);
	while (caracter != 'c' && caracter != 'C')
	{
		
		caracter = waitKey(1000);

		if (caracter == -1) caracter = '0';
		SelectBaston(img, caracter);
		imshow("imagen", img);

	}
	
	destroyWindow("imagen");

	cout << "\nGracias por usar este comienzo de analizador de espectro" << endl;

	system("pause");


	return 0;
}