//Biblioteca para funciones matemáticas como sen(x), cos(x), raizN(x), logN(x), entre otras, aproximadas por algoritmos y también la incorporacion de métodos numéricos para aproximar.
#ifndef CITRAFUNCIONES_HPP
#define CITRAFUNCIONES_HPP

#include "ClemCore.hpp"

class CitraFunciones : public ClemCore{
protected:
    long double num_pi, numero_e, proporcion_aurea, error_aceptable;
public:
    CitraFunciones();

    void setPi(int iteracion_deseable);
    void setE(int iteracion_deseable);
    void setAureo();
    void setError(long double error_aceptable);
    long double pi();
    long double num_e();
    long double aureo();
    long double getErrorTolerancia();

    void verificarIndice(int indice);
    complejo raizN(long double x, int N, long double error_aceptable, int max_iteracion); //Calculo de Raiz N de un numero X por metodo de Newton-Raphson
    long double modulo(complejo z); //Calcula el modulo de un número complejo usando Raiz N = 2.
    long double calcularPi(int max_iteracion, long double error_aceptable); //Calculo de Pi usando el algoritmo de Gauss-Legendre de convergencia rapida.
    long double calcularE(long double error_aceptable); //Calculo de e usando su serie de MacLaurin.
    long double exponencial(long double x, int max_iteracion, long double error_aceptable); //Calculo de exp(x) por su serie de Taylor.
    complejo exponencialComplejo(complejo z, int max_iteracion, long double error_aceptable); //Calculo de exp(z) por su serie de Taylor, con Z siendo un número complejo.
    long double sen(long double x, int max_iteracion, double error_aceptable); //Calculo de sen(x) mediante su serie de MacLaurin.
    long double cos(long double x, int max_iteracion, double error_aceptable); //Calculo de cos(x) mediante su serie de MacLaurin.
};

#endif
