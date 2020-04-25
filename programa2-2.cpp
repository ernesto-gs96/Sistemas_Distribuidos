#include "Fecha.h"
#include<iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    Fecha fecha (4,4,2014);
    fecha.muestraFecha();

    // Ejercicio 3
    cout<<fecha.convierte()<<endl;


    // Ejercicio 4
    cout<<fecha.leapyr()<<endl;


    /* code */
    return 0;
}

