#include "Fecha.h"
#include <iostream>

using namespace std;

Fecha::Fecha(int dd, int mm, int aaaa)
{
    mes = mm;
    dia = dd;
    anio = aaaa;
}

void Fecha::inicializaFecha(int dd, int mm, int aaaa)
{
    anio = aaaa;
    mes = mm;
    dia = dd;
    return;
}

int Fecha::convierte(){
    return anio*10000+mes*100+dia;
}

bool Fecha::leapyr(){
    if (anio % 400 == 0)
    {
        return true;
    }
    else if (anio % 4 == 0 & anio % 100 != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Fecha::muestraFecha()
{
    cout << "La fecha es (dia-mes-anio): " << dia << "-" << mes << "-" << anio << endl;
    return;
}

