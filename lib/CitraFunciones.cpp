#include "CitraFunciones.hpp"
#define INTERACION_MAXIMA 1000

CitraFunciones::CitraFunciones(){}

void CitraFunciones::setPi(int iteracion_deseable) { this->num_pi = calcularPi(iteracion_deseable, getErrorTolerancia()); }
void CitraFunciones::setE(int iteracion_deseable) { this->numero_e = calcularE(getErrorTolerancia()); }
void CitraFunciones::setAureo() { this->proporcion_aurea = (1 + raizN(5, 2, getErrorTolerancia(),INTERACION_MAXIMA).real)/2; }
void CitraFunciones::setError(long double error_aceptable) { this->error_aceptable = error_aceptable; }

long double CitraFunciones::pi() { return this->num_pi; }
long double CitraFunciones::num_e() { return this->numero_e; }
long double CitraFunciones::aureo() { return this->proporcion_aurea; }
long double CitraFunciones::getErrorTolerancia() { return this->error_aceptable; }

void CitraFunciones::verificarIndice(int indice){
    if(indice <= 0) throw std::runtime_error("Error: El indice no puede ser 0 o negativo.");
}

complejo CitraFunciones::raizN(long double x, int N, long double error_aceptable, int max_iteracion){
    /*
        Estima la raiz N-esima de un numero por el método de Newton-Raphson
        De f(y) = y^(1/N), se quiere calcular f(a)=a^(1/N) que es solución de la ecuación : x^N - a = 0.

        Lo que se hace es hacer df(a)/dx * dx = -f(a) + error => N*X^(N-1) * (X(k+1) - Xk) = -(x^N - a) siendo k las interaciones.

        Se estima por despeje x(k+1) = -(x^N - a)/N*X^(N-1)) + Xk
    */

    try{
        verificarIndice(N);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }
    complejo resultado;
    long double x_valor = valorAbsoluto(x);
    long double y_actual = (x_valor > 1.0) ? x_valor : 1.0;
    long double y_siguiente;

    for( int i = 0; i < max_iteracion; i++){
        long double y_n = potenciaEntera(y_actual, N); // Se caclula x^N
        long double y_n_1 = N * potenciaEntera(y_actual, (N-1));

        try{
            verificarDivisionPorCero(y_n_1);
        } catch (const std::runtime_error& e){
            std::cerr << "[verificación] " << e.what() << std::endl;
            throw;
        }

        y_siguiente = y_actual - (y_n - x_valor) / y_n_1;
        if(y_siguiente == y_actual || valorAbsoluto(y_siguiente - y_actual) < error_aceptable) break;
        y_actual = y_siguiente;
    }

    //Definir el resultado según si la base era positiva o negativa y el indice par o impar.

    if (x < 0 && N % 2 == 0) {
        // Raíz de número negativo con índice par: imaginaria pura
        resultado.real = 0;
        resultado.imaginario = y_actual;
    } else if (x < 0 && N % 2 != 0) {
        // Raíz de negativo con índice impar: número real negativo
        resultado.real = -y_actual;
        resultado.imaginario = 0;
    } else {
        resultado.real = y_actual;
        resultado.imaginario = 0;
    }
    setResultadoC(resultado);

    return resultado;
}

long double CitraFunciones::modulo(complejo z){
    /*
        Determina el modulo de un número complejo utilizando la raiz cuadrada de la suma de las potencias de sus componentes.
    */
    long double resultado, moduloCuadrado;
    moduloCuadrado = parteReal(z)*parteReal(z) + parteImaginaria(z)*parteImaginaria(z);
    resultado = parteReal(raizN(moduloCuadrado, 2, getErrorTolerancia(), INTERACION_MAXIMA));
    setResultado(resultado);

    return resultado;
}

long double CitraFunciones::calcularPi(int max_iteracion, long double error_aceptable){
    /*
        El Algoritmo de Gauss-Legendre permite computar los digitos de pi con una convergencia rápida y eficiente empleando multiplicación y raíz cuadrada.
        Sustituye dos numeros por una especie de media aritmética-geometrica.
        Se requieren al menos 4 parametros con valores iniciales semilla : a_0, b_0, p_0 y t_0.
        Se iteran dichos parametros de la siguiente manera:
        a_n_1 = (a_n + b_n) / 2 <- a(n+1) por media aritmetica.
        b_n_1 = raizCuadrada(a_n*b_n) <- b(n+1) por media geométrica.
        p_n_1 = 2*p_n
        t_n_1 = t_n -p_n*(a_n_1-a_n)^2

        Luego Pi es aproximado a [(a_n_1 + b_n_1)^2]/(4*tn_1)

        Este algoritmo tiene convergencia cuadrática y su derivación es a partir de la integral elíptica completa de primera especie K(k).
    */

    long double a_actual, a_siguiente, b_actual, b_siguiente, p_actual, p_siguiente, t_actual, t_siguiente, pi_aproximado_actual, pi_aproximado_siguiente;
    a_actual = 1.0;
    b_actual = 1 / raizN(2, 2, getErrorTolerancia(), INTERACION_MAXIMA).real;
    p_actual = 1;
    t_actual = 0.25;
    pi_aproximado_actual = potenciaEntera((a_actual+b_actual),2) / (4*t_actual);


    for (int i = 0; i < max_iteracion; i++){
        a_siguiente = (a_actual + b_actual) / 2;
        b_siguiente = raizN(a_actual*b_actual, 2, getErrorTolerancia(), INTERACION_MAXIMA).real;
        t_siguiente = t_actual - p_actual*potenciaEntera((a_actual-a_siguiente), 2);
        p_siguiente = p_actual * 2;

        pi_aproximado_siguiente = potenciaEntera((a_siguiente+b_siguiente), 2)/ (4*t_siguiente);
        if(valorAbsoluto(pi_aproximado_actual-pi_aproximado_siguiente) < error_aceptable) {
            pi_aproximado_actual = pi_aproximado_siguiente;
            break;
        }

        a_actual = a_siguiente;
        b_actual = b_siguiente;
        t_actual = t_siguiente;
        p_actual = p_siguiente;
        pi_aproximado_actual = pi_aproximado_siguiente;
    }
    setResultado(pi_aproximado_actual);

    return pi_aproximado_actual;
}

long double CitraFunciones::exponencial(long double x, int max_iteracion, long double error_aceptable){
    /*
        Se calcula la función Exp(x) empleando su serie de Taylor.

        Exp(x) = x^0/0! + x^1/1! + x^2/2! + x^3/3! + x^4/4! + ...

    */
    long double resultado = 1, termino = 1;
    for (int i = 1; i < max_iteracion; i++){
        termino *= x / i;
        resultado += termino;
        if (valorAbsoluto(termino)< error_aceptable) break;
    }

    setResultado(resultado);

    return resultado;
}

complejo CitraFunciones::exponencialComplejo(complejo z, int max_iteracion, long double error_aceptable){
    /*
        Se calcula la función Exp(z) empleando su serie de Taylor.

        Exp(z) = z^0/0! + z^1/1! + z^2/2! + z^3/3! + z^4/4! + ...
    */

    complejo termino = {1,0}, resultado = {1,0}, den;
    for (long int i = 1; i < max_iteracion; i++){
        den = {factorial(i),0.0};
        termino = divisionCompleja(potenciaComplejaEnetera(z, i),den);
        resultado = sumaComplejo(termino, resultado);
        if (modulo(termino) < error_aceptable) {
            break;
        }
    }
    setResultadoC(resultado);

    return resultado;
}

long double CitraFunciones::calcularE(long double error_aceptable){
    /*
        Se calcula el numero neperiano o e mediante la serie de MacLaurin.

        e = 1/0! + 1/1! + 1/2! + 1/3! + 1/4! + ...

    */

    return exponencial(1, INTERACION_MAXIMA, error_aceptable);
}

long double CitraFunciones::sen(long double x, int max_iteracion, double error_aceptable){
    /*
        Se calcula el seno a partir de su Serie de MacLaurin:
        sen(x) = x^1/1! - x^3/3! + x^5/5! - x^7/7! -...
    */
    long double resultado = 0, termino = 0;
    for(int i = 0; i <  max_iteracion; i++){
        termino = potenciaEntera((-1), i) * potenciaEntera(x, (2*i+1)) / factorial(2*i+1);
        resultado += termino;
        if (valorAbsoluto(termino) < error_aceptable) break;
    }
    setResultado(resultado);

    return resultado;
}

long double CitraFunciones::cos(long double x, int max_iteracion, double error_aceptable){
    /*
        Se calcula el coseno a partir de su Serie de MacLaurin:
        cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! -...
    */
    long double resultado = 0, termino = 0;
    for(int i = 0; i <  max_iteracion; i++){
        termino = potenciaEntera((-1), i) * potenciaEntera(x, (2*i)) / factorial(2*i);
        resultado += termino;
        if (valorAbsoluto(termino) < error_aceptable) break;
    }
    setResultado(resultado);

    return resultado;
}
