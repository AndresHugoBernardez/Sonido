

#include <iostream>

using namespace std;



namespace AndyFFTSimple {

    float WRe[16] = { 1,
       0.9238795,
       0.7071068,
       0.3826834,
       0,
      -0.3826834,
      -0.7071068,
      -0.9238795,
      -1,
      -0.9238795,
      -0.7071068,
      -0.3826834,
      0,
       0.3826834,
       0.7071068,
       0.9238795 };
    float WIm[16] = {
    0,
      -0.3826834,
      -0.7071068,
      -0.9238795,
      -1,
      -0.9238795,
      -0.7071068,
      -0.3826834,
      0,
       0.3826834,
       0.7071068,
       0.9238795,
       1,
       0.9238795,
       0.7071068,
       0.3826834 };
    float WR[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
    float WI[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };


    int pow2[15] = { 1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384 };


    float Xprueba[16] = { 0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15 };
    float Xprueba2[16] = { 0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15 };
    float XR[16] = { 0,1,1,1, 0,2,3,1, 0,0,0,0, 0,0,0,0 };
    float XI[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };

    int etapas = 4;
    int N = 16;
    int mitad = 8;
    int auxiliar = 1;
    float aux = 0, aux2 = 0, aux3 = 0, aux4 = 0;

    int sqrt2 = 1;
    int E2 = 1;
    int sqrt2index = 1;
    int E2index = 1;

    int paso = 1;
    int fila = 1;

    int i = 1;
    int j = 1;
    int k = 1;
    int k2 = 1;



    /// <summary>
    /// La forma más fácil de resolver el problema de bits inversos es ordenarlos directamente, menos tiempo de procesamiento.
    /// </summary>
    /// <param name="X"> Vector X a ordenar de tamaño16</param>
    void BitsEspejadosN16(float* X) {
        float intercambiador;

        //0= 0000 = 0000 No necesita cambiar

        //1= 0001 = 1000 =8
        intercambiador = X[1];
        X[1] = X[8];
        X[8] = intercambiador;

        //2=0010 =0100 =4
        intercambiador = X[2];
        X[2] = X[4];
        X[4] = intercambiador;

        //3=0011 =1100 =12
        intercambiador = X[3];
        X[3] = X[12];
        X[12] = intercambiador;

        //4 ya está hecho en 2

        //5=0101 =1010 =10
        intercambiador = X[5];
        X[5] = X[10];
        X[10] = intercambiador;

        //6=0110 =0110 =6 No necesita cambiar


        //7=0111 =1110 =14
        intercambiador = X[7];
        X[7] = X[14];
        X[14] = intercambiador;

        //8 esta hecho en 1


        //10 esta hecho en 5


        //11=1011 =1101 =13
        intercambiador = X[11];
        X[11] = X[13];
        X[13] = intercambiador;

        //12 esta hecho en 3


        //13 esta hecho en 11
        //14 esta hecho en 7
        // 15= 1111 no cambia.



    }


    void BitsEspejadosGeneral(float* X, int NN) {

        int i, j, k, comparacion, lenght;

        float auxiliar, auxiliar2;
        i = 0;

        while (pow2[i] < NN) {
            i++;
        }
        lenght = i;


        for (i = 0; i < NN; i++) {

            comparacion = i;
            j = 0;

            for (k = lenght - 1; k > -1; k--)
            {
                if (pow2[k] <= comparacion) {
                    comparacion = comparacion - pow2[k];
                    j = j + pow2[lenght - 1 - k];
                }
            }


            if (j != i && i < j) {
                auxiliar = X[i];
                X[i] = X[j];
                X[j] = auxiliar;
            }
        }


    }




    void printvector(float* X, int NN) {
        int i;
        cout << "Vector: ";
        for (i = 0; i < NN; i++) {

            cout << i << ":" << X[i] << " ";

        }
        cout << endl;

    }

    void printComplexVector(float* RR, float* II, int NN) {

        int i;
        cout << endl;
        for (i = 0; i < NN; i++) {

            cout << RR[i] << " + i " << II[i] << endl;

        }

    }

    /*
    */
    void fft() {



        //sqrt2 = pow2[etapas];
        sqrt2index = etapas - 1;
        E2 = 1;
        E2index = 0;

        paso = 1;


        while (paso <= mitad) {

            sqrt2 = pow2[sqrt2index];
            fila = 0;


            for (k = 0; k < (N / (E2 * 2)); k++) {

                for (k2 = 0; k2 < E2; k2++) {

                    j = (fila * sqrt2) % mitad;
                    i = fila;



                    WR[i] = WRe[j];
                    WI[i] = WIm[j];
                    i = i + E2;
                    WR[i] = -WRe[j];
                    WI[i] = -WIm[j];

                    fila = fila + 1;



                }

                fila = fila + E2;

            }










            E2index = E2index + 1;
            E2 = pow2[E2index];
            sqrt2index = sqrt2index - 1;


            //-----
            auxiliar = paso * 2;
            for (j = 0; j < N; j = j + auxiliar) {
                for (i = 0; i < paso; i++) {

                    k = i + j;
                    k2 = k + paso;

                    aux = XR[k] + WR[k] * XR[k2] - WI[k] * XI[k2];
                    aux2 = XR[k] + WR[k2] * XR[k2] - WI[k2] * XI[k2];
                    aux3 = XI[k] + WI[k] * XR[k2] + WR[k] * XI[k2];
                    aux4 = XI[k] + WI[k2] * XR[k2] + WR[k2] * XI[k2];
                    XR[k] = aux;
                    XR[k2] = aux2;
                    XI[k] = aux3;
                    XI[k2] = aux4;


                }
            }







            paso = paso * 2;


        }




    }






};

using namespace AndyFFTSimple;

int main()
{
    //printvector(Xprueba, N);
    //BitsEspejadosGeneral(Xprueba, N);
    //printvector(Xprueba, N);
    //printvector(WRe, N);
   // printvector(WIm, N);
    
    cout  << "FFT:" << endl;
    
    

    BitsEspejadosN16(XR);
    fft();

    printComplexVector(XR, XI, N);
    system("pause");
    
}
