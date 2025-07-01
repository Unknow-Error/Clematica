#include "ClemCore.hpp"

// Nota sobre problemas de Overflow o precisión de coma flotante:
// En double (IEEE 754 de 64 bits): Precisión: 15-17 cifras decimales. Mantisa: 53 bits y Exponente: 11 bits
// En long double (De verdad) : (extensión x87, 80 bits):  Precisión: 18-21 cifras decimales. Mantisa: 64 bits y Exponente: 15 bits
// Pero en algunos sistemas un long double es double (entornos MSVC o ARM)
// Otro caso: Numeros muy grandes como factorial(100) desborda un long double o long int. Cuidado con Tetración.
// Hay definir limites menores y mayores para evitar estos problemas...

ClemCore::ClemCore() {}

void ClemCore::setResultado(long double resultado) { this->resultado = resultado; }
void ClemCore::setResultadoC(complejo resultadoC) { this->resultadoC = resultadoC; }

long double ClemCore::getResultado() { return this->resultado; }
complejo ClemCore::getResultadoC() { return this->resultadoC; }

long double ClemCore::sumaReal(long double num1, long double num2) {
    long double resultado = num1 + num2;
    setResultado(resultado);
    return resultado;
}

long double ClemCore::productoReal(long double num1, long double num2) {
    long double resultado = num1 * num2;
    setResultado(resultado);
    return resultado;
}

void ClemCore::verificarDivisionPorCero(long double num) {
    if (num == 0) throw std::runtime_error("Error: No se puede dividir por cero.");
}

long double ClemCore::divisionReal(long double num1, long double num2) {
    try {
        verificarDivisionPorCero(num2);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }
    long double resultado = num1 / num2;
    setResultado(resultado);
    return resultado;
}

int ClemCore::resto(long double num1, long double num2) {
    try {
        verificarDivisionPorCero(num2);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }
    int resultadoE = int(num1) % int(num2);
    setResultado(double(resultadoE));
    return resultadoE;
}

void ClemCore::verificarPotencia(long double num, int exp) {
    if (num == 0 && exp == 0)
        throw std::runtime_error("Error: La operación 0^0 no está definida");
}

long double ClemCore::potenciaEntera(long double num, int exponente) {
    try {
        verificarPotencia(num, exponente);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }
    long double resultado;

    if(exponente == 0){
        resultado = 1;
    } else {
        resultado = 1;
        for (int i = 0; i < exponente; i++)  resultado *= num;
    }

    setResultado(resultado);
    return resultado;
}

long double ClemCore::tetracion(long double num, int exponente){
    try {
        verificarPotencia(num, exponente);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }

    long double resultado = num;
    for (int i = 1; i < exponente; i++) resultado = potenciaEntera(resultado, num);

    setResultado(resultado);
    return resultado;
}

long double ClemCore::valorAbsoluto(long double num){
    long double resultado;
    if(num >= 0){
        resultado = num;
    } else {
        resultado = -num;
    }

    setResultado(resultado);
    return resultado;
}

long double ClemCore::factorial(long int num){
    long double resultado;
    if(num == 0){
        resultado = 1; //Por definición 0! = 1
    } else {
        resultado = 1;
        for(long int i = 1; i <= num; i++) resultado *= i;
    }

    setResultado(resultado);
    return resultado;
}

long double ClemCore::numCombinatorio(long int num1, long int num2){
    long double resultado;
    resultado = factorial(num1) / (factorial(num2) * factorial(num1 - num2)); //Definición por el coeficiente binomial.

    setResultado(resultado);
    return resultado;
}

complejo operator+(const complejo& numC1, const complejo& numC2) {
    return { numC1.real + numC2.real, numC1.imaginario + numC2.imaginario };
}

complejo operator-(const complejo& numC1, const complejo& numC2) {
    return { numC1.real - numC2.real, numC1.imaginario - numC2.imaginario };
}

complejo operator*(const complejo& numC1, const complejo& numC2) {
    return {numC1.real * numC2.real - numC1.imaginario * numC2.imaginario, numC1.real * numC2.imaginario + numC1.imaginario * numC2.real};
}

complejo operator~(const complejo& numC){
    return {numC.real, -numC.imaginario};
}

complejo operator/(const complejo& numC1, const complejo& numC2) {
    complejo conjugado = ~numC2;
    complejo numerador = numC1 * conjugado;
    long double denominador =  numC2.real * numC2.real + numC2.imaginario * numC2.imaginario;
    return {numerador.real / denominador, numerador.imaginario / denominador};
}

complejo ClemCore::sumaComplejo(complejo numC1, complejo numC2) {
    complejo resultado;
    resultado = numC1 + numC2;
    setResultadoC(resultado);
    return resultado;
}

complejo ClemCore::productoComplejo(complejo numC1, complejo numC2) {
    complejo resultado;
    resultado = numC1 * numC2;
    setResultadoC(resultado);
    return resultado;
}

complejo ClemCore::potenciaComplejaEnetera(complejo numC, int exponente) {
    try {
        if (numC.real == 0 && numC.imaginario == 0){
            verificarPotencia(numC.real, exponente);
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }

    complejo resultado;
    resultado = numC;
    if(exponente != 1){
        for (int i = 1; i < exponente; i++)  resultado = productoComplejo(resultado, numC);
    }

    setResultadoC(resultado);
    return resultado;
}

complejo ClemCore::conjugado(complejo numC1) {
    complejo resultado;
    resultado = ~ numC1;
    setResultadoC(resultado);
    return resultado;
}

long double ClemCore::parteReal(complejo numC){
    long double resultado = numC.real;
    setResultado(resultado);
    return resultado;
}

long double ClemCore::parteImaginaria(complejo numC){
    long double resultado = numC.imaginario;
    setResultado(resultado);
    return resultado;
}

long double ClemCore::denominadorC(complejo numC) {
    return numC.real * numC.real + numC.imaginario * numC.imaginario;
}

void ClemCore::verificarDivisionPorCeroComplejo(complejo numC) {
    // Se verifica si el cuadrado del módulo es menor que un umbral para evitar divisiones por cero
    if (denominadorC(numC) < 1e-16) // 1e-16 es un umbral pequeño para tolerar errores numéricos de computo por coma flotante.
        throw std::runtime_error("Error: división por cero complejo.");
}


complejo ClemCore::divisionCompleja(complejo numC1, complejo numC2) {
    try {
        verificarDivisionPorCeroComplejo(numC2);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }

    complejo resultado;
    resultado = numC1 / numC2;
    setResultadoC(resultado);
    return resultado;
}

void ClemCore::resultadoReal(std::string operacion) {
    std::cout << "\nEl resultado de la operación " << operacion << " es " << resultado << ".\n" << std::endl;
}

void ClemCore::resultadoComplejo(std::string operacion) {
    std::cout << "\nEl resultado de la operación " << operacion << " es " << resultadoC.real << "+" << resultadoC.imaginario << "i.\n" << std::endl;
}
