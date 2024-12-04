
//estudio de FFT
// archivo para scilab


//x=[0 1  1 1 0 2 3 1 0 0 0 0 0 0 0 0]
x=[0; 1 ; 1;1;0;2;3;1;0;0; 0;0;0;0; 0; 0]
// 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
n=0:15
k=n'
N=16
Wo=exp(-%i*2*%pi*k*n/16);

out=Wo*x


///FFT
/// extraído de https://www.researchgate.net/publication/336339137_Transformada_Rapida_de_Fourier
//// adhesión https://es.wikipedia.org/wiki/Transformada_r%C3%A1pida_de_Fourier

W=exp(-2*%i*%pi/4)
WW0=[
1  1   1   1;
1 W^1 W^2 W^3; 
1 W^2 W^0 W^2; 
1 W^3 W^2 W^1]
WW1=[
1 W^0 0 0; 
1 W^2 0 0; 
0   0 1 W^1; 
0   0 1 W^3;]
WW2=[
1 0 W^0 0;
0 1 0   W^0;
1 0 W^2 0;
0 1 0   W^2;]

// se comprueba que WW1*WW2= WW0' (DONDE FILA 1 SE CAMBIA POR LA 2 O EN BINARIO 01<-->10 esto puede ahorrar tiempo de procesamiento al no tener que leer memoria RAM ni cache fuera del procesador)

//Mariposas: "Algoritmo FFT para una secuencia de 8 muestras"(2021),Jaquelin Arzate Gordillo, Youtube, extraído de https://www.youtube.com/watch?v=DAkOXvybQio )


// FFT en Scilab

//  Si N=2 Xout[0]=X[0]+W0*X[1] Xout[0]=X[0]-(-W0*X[1])
//  W0=W^0=1.... to be continued. 

// To do: usar assembler para la FFT. 

// nota: El hecho de que los datos pertenezcan a los números reales facilita la tarea de trabajar con números imaginarios en la primera mariposa

// se separan los números imaginarios de W en parte real e imaginario. 

// multiplicar numeros imaginarios:
// (a+jb)*(c+jd)=a*c-b*d  + j (a*d+b*c)
// cantidad de operaciones: 4 multiplicaciones, 2 sumas (la suma de reales con imaginarios no se hace)

// multiplicar numeros imaginarios con módulo y argumento
// (a+jb)*(c+jd)=sqrt(a*a+b*b)*sqrt(c*c+d*d)exp(arctg(b/a)+arctg(d/c))
// cantidad de operaciones: 4 multiplicaciones, 2 divisiones, 2 arctg, 2 raíces. (la exponencial no se hace)

// evidentemente no conviene multiplicar números imginarios con módulo y argumento. 


// nota: investigar si en procesadores avanzados hay función para operar con números complejos y cuántos ciclos reloj consumen. 


//asumiré aquí que los procesadores no operan facilmente con números complejos. Y la ALU no posee función para hacerlo. Esto último parece ser una realidad de muchos procesadores. 


//FFT en scilab
//preparando el vector W. este paso solo debe hacerse una vez para cada elección de N, si N no varía puede guardarse el vector con número real y el vector con número irreal 



//Prueba de generacion de Matriz W

mitad=N/2;

k=1


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







etapas=log2(N)
etapass=flog2(N)
//comienzo


Wk=zeros(N,etapas)
Wkk=zeros(N,etapas)
Wk2=zeros(N,etapas)
WRe=zeros(N,1);
WIm=zeros(N,1);

WR=zeros(N,etapas);//memoria
WI=zeros(N,etapas);//memoria

//almacenar los valores de WRe y de WIm en algún lugar de la memoria.
for i=1:mitad
    WRe(i)=real(exp(-2*%i*(i-1)*%pi/N));
    WIm(i)=imag(exp(-2*%i*(i-1)*%pi/N));
end
WRe
WIm






//vector pow 2

pow2=[[1][2][4][8][16][32][64][128][256][512][1024]]

/// obtener cada vector W para mariposas.
sqrt2=2^(etapas-1)
sqrt2=pow2(etapas)
exp2=1
exp2index=1
sqrt2index=etapas
for k=1:etapas 
    //obtener cada vector
    sqrt2=pow2(sqrt2index);
    COLUMNA=(N)*(k-1);
   FILA=1;
    for grupo=1:(N/(exp2*2))
        
        for elemento=1:exp2
            
            aux=(FILA-1)*sqrt2;
            j=modulo(aux,mitad);
            ggg=COLUMNA+FILA;
            Wk(ggg)=WRe(j+1)+%i*WIm(j+1);
            Wkk(ggg)=j;
            Wk2(ggg)=aux;
            //parte negativa
            
            WR(ggg)=WRe(j+1);
            WI(ggg)=WIm(j+1);
            
            ggg2=ggg+exp2;
            Wkk(ggg2)=j;
            Wk2(ggg2)=(FILA-1+exp2)*sqrt2;
            Wk(ggg2)=-WRe(j+1)-%i*WIm(j+1);     
            
            WR(ggg2)=-WRe(j+1);
            WI(ggg2)=-WIm(j+1);
            
            FILA=FILA+1;
        end
        FILA=FILA+exp2;
    end    
        
        
    exp2index=exp2index+1;
    exp2=pow2(exp2index);
    sqrt2index=sqrt2index-1;
    
end

Wk
//Wkk
//Wk2
WR
WI




















// Creando vector XR (x real) y XI (x imaginario)
XR=zeros(N,1)
XI=zeros(N,1)

//Llenando X0 en el orden correcto. 

Nbits=etapas
for i=0:N-1
    bits=dec2bin(i,Nbits)
    reversebits=strrev(bits)

    k=bin2dec(reversebits)
    N-1-i
    
    XR(i+1)=x(k+1);
    
end;
x
XR




//primera etapa de mariposas WR[0]=1 WR[N]=-1 XI es cero
if N>1 then
for i=0:2:N-1
    aux=XR(i+1)+XR(i+2);
    XR(i+2)=XR(i+1)-XR(i+2);
    XR(i+1)=aux;
end

end


///Llenando 

///// con parte real e imaginaria




//segunda mariposas

if(etapas>1)then
    
///
PASO=2
COLUMNA=N

for j=0:PASO*2:N-1

    for i=1:PASO  
        kk=i+j
        kk2=kk+PASO
        k=kk+COLUMNA
        k2=kk2+COLUMNA
        
        //XI(k) y XI(k2) son cero aún
        aux=XR(kk)+WR(k)*XR(kk2);
        aux2=XR(kk)+WR(k2)*XR(kk2);
        aux3=WI(k)*XR(kk2);
        aux4=WI(k2)*XR(kk2);
        
        XR(kk)=aux;
        XR(kk2)=aux2;
        XI(kk)=aux3;
        XI(kk2)=aux4;
        
         
    end    
end    
   
end

//resto de mariposas


// multiplicar numeros imaginarios:
// (a+jb)*(c+jd)=a*c-b*d  + j (a*d+b*c)
// cantidad de operaciones: 4 multiplicaciones, 2 sumas (la suma de reales con imaginarios no se hace)
///
///
///Xknuevo=Xk+Wk*Xk2    = (XRk+jXIk)+(WRk+jWIk)*(XRk2+jXIk2)=(XRk+WRk*XRk2-WIk*XIk2)+j(XIk+WIk*XRk2+WRk*XIk2)
///Xk2nuevo=Xk+Wk2*Xk2  =  (XRk+jXIk)+(WRk2+jWIk2)*(XRk2+jXIk2)=(XRk+WRk*XRk2-WIk2*XIk2)+j(XIk+WIk2*XRk2+WRk*XIk2)
///
///XRknuevo=XRk+WRk*XRk2-WIk*XIk2
///XIknuevo=XIk+WIk*XRk2+WRk*XIk2
///
///XRk2nuevo=XRk+WRk2*XRk2-WIk2*XIk2
///XIk2nuevo=XIk+WIk2*XRk2+WRk2*XIk2
///
///

if(etapas>2)then
contador=2
PASO=2
///
while (PASO<N/2)
contador=contador+1    
PASO=PASO*2
COLUMNA=N*(contador-1)

for j=0:PASO*2:N-1

    for i=1:PASO  
        kk=i+j
        kk2=kk+PASO
        k=kk+COLUMNA
        k2=kk2+COLUMNA
        
        
        //XI(k) y XI(k2) son cero aún
        aux=XR(kk)+WR(k)*XR(kk2)-WI(k)*XI(kk2);
        aux2=XR(kk)+WR(k2)*XR(kk2)-WI(k2)*XI(kk2);
        aux3=XI(kk)+WI(k)*XR(kk2)+WR(k)*XI(kk2); 
        aux4=XI(kk)+WI(k2)*XR(kk2)+WR(k2)*XI(kk2);
        
        
        
        XR(kk)=aux;
        XR(kk2)=aux2;
        XI(kk)=aux3;
        XI(kk2)=aux4;
        
         
    end    
end    

end


end


//resto de las mariposas


XR+%i*XI

