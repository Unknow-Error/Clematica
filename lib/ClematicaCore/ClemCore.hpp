//Biblioteca que contiene la clase base para las operaciones ariméticas básicas con numeros reales y complejos.
#ifndef CLEMCORE_HPP
#define CLEMCORE_HPP

#include <stdexcept>
#include <iostream>
#include <string>

//Sobrecargar operadores y que sea amiga de la estructura => mantener encapsulamiento (miembros privados) pero seguir usando operadores de forma natural.
struct complejo {
    long double real;
    long double imaginario;

    friend complejo operator+(const complejo& numC1, const complejo& numC2);
    friend complejo operator-(const complejo& numC1, const complejo& numC2);
    friend complejo operator*(const complejo& numC1, const complejo& numC2);
    friend complejo operator~(const complejo& numC);
    friend complejo operator/(const complejo& numC1, const complejo& numC2);
};

class ClemCore {
protected:
    long double resultado;
    complejo resultadoC;

public:
    ClemCore();

    void setResultado(long double resultado);
    void setResultadoC(complejo resultadoC);

    long double getResultado();
    complejo getResultadoC();

    long double sumaReal(long double num1, long double num2);
    long double productoReal(long double num1, long double num2);
    void verificarDivisionPorCero(long double num);
    long double divisionReal(long double num1, long double num2);
    int resto(long double num1, long double num2);
    void verificarPotencia(long double num, int exp);
    long double potenciaEntera(long double num, int exponente);
    long double tetracion(long double num, int exponente);
    long double valorAbsoluto(long double num);
    long double factorial(long int num);
    long double numCombinatorio(long int num1, long int num2);

    complejo conjugado(complejo numC);
    long double parteReal(complejo numC);
    long double parteImaginaria(complejo numC);
    complejo sumaComplejo(complejo numC1, complejo numC2);
    complejo productoComplejo(complejo numC1, complejo numC2);
    complejo potenciaComplejaEnetera(complejo numC, int exponente);
    long double denominadorC(complejo numC);
    void verificarDivisionPorCeroComplejo(complejo numC);
    complejo divisionCompleja(complejo numC1, complejo numC2);

    void resultadoReal(std::string operacion);
    void resultadoComplejo(std::string operacion);
};


#endif
