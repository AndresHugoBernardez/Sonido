
//estudio de FFT
// archivo para scilab
///FFT
/// extraído de https://www.researchgate.net/publication/336339137_Transformada_Rapida_de_Fourier
//// adhesión https://es.wikipedia.org/wiki/Transformada_r%C3%A1pida_de_Fourier
//Mariposas: "Algoritmo FFT para una secuencia de 8 muestras"(2021),Jaquelin Arzate Gordillo, Youtube, extraído de https://www.youtube.com/watch?v=DAkOXvybQio )


//x=[0 1  1 1 0 2 3 1 0 0 0 0 0 0 0 0]
x=[0; 1 ; 1;1;0;2;3;1;0;0; 0;0;0;0; 0; 0]
// 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16


//funcion logaritmo base 2 para números igual 2^x

function y=flog2(xx)
contador=0;
tt=xx;
while(tt>1) 
    tt=tt/2;
    contador=contador+1;
end    
y=contador;
endfunction



function y=andyfft(xxx)
N=size(xxx)(1);
resultado=2
while(resultado<N)
    resultado=resultado*2;
    
end

if(resultado<>N)y="error la cantidad de elementos debe ser potencia de 2" 
else


etapas=flog2(N);
//comienzo



WRe=zeros(N,1);
WIm=zeros(N,1);

WR=zeros(N,1);//memoria
WI=zeros(N,1);//memoria

//almacenar los valores de WRe y de WIm en algún lugar de la memoria.
for i=1:N/2
    WRe(i)=real(exp(-2*%i*(i-1)*%pi/N));
    WIm(i)=imag(exp(-2*%i*(i-1)*%pi/N));
end

//vector pow 2

pow2=[[1][2][4][8][16][32][64][128][256][512][1024]]

/// obtener cada vector W para mariposas.


// Creando vector XR (x real) y XI (x imaginario)
XR=zeros(N,1);
XI=zeros(N,1);

//Llenando X0 en el orden correcto. 

Nbits=etapas;
for i=0:N-1
    bits=dec2bin(i,Nbits);
    reversebits=strrev(bits);

    k=bin2dec(reversebits);
   
    
    XR(i+1)=x(k+1);
    
end;




/// FFT resolución por mariposas
///

PASO=1/2;
///

sqrt2=2^(etapas-1);
sqrt2=pow2(etapas);
exp2=1;
exp2index=1;
sqrt2index=etapas;
mitad=N/2;

while (PASO<mitad)
    

    //obtener cada vector
    sqrt2=pow2(sqrt2index);
   
   FILA=1;
    for grupo=1:(N/(exp2*2))
        
        for elemento=1:exp2
            
            aux=(FILA-1)*sqrt2;
            j=modulo(aux,mitad);
            ggg=FILA;           
            //parte negativa
            WR(ggg)=WRe(j+1);
            WI(ggg)=WIm(j+1);
            ggg2=ggg+exp2;
            WR(ggg2)=-WRe(j+1);
            WI(ggg2)=-WIm(j+1);
            FILA=FILA+1;
        end
        FILA=FILA+exp2;
    end    
 
    exp2index=exp2index+1;
    exp2=pow2(exp2index);
    sqrt2index=sqrt2index-1;
   
PASO=PASO*2


for j=0:PASO*2:N-1

    for i=1:PASO  
        kk=i+j
        kk2=kk+PASO

        aux=XR(kk)+WR(kk)*XR(kk2)-WI(kk)*XI(kk2);
        aux2=XR(kk)+WR(kk2)*XR(kk2)-WI(kk2)*XI(kk2);
        aux3=XI(kk)+WI(kk)*XR(kk2)+WR(kk)*XI(kk2); 
        aux4=XI(kk)+WI(kk2)*XR(kk2)+WR(kk2)*XI(kk2);
        XR(kk)=aux;
        XR(kk2)=aux2;
        XI(kk)=aux3;
        XI(kk2)=aux4;
        
         
    end    
end    

end


//RESULTADO


y=XR+%i*XI
end

endfunction



andyfft(x)
