#include <iostream>
#include "../lib/ClematicaCore/ClemCore.cpp"
#include "../lib/ClematicaCore/CitraFunciones.cpp"
#include "../lib/CitrusTensorial/CitrusTensor.cpp"
#include <iomanip> // necesario para setprecision y fixed

int main(int argc, char *argv[])
{
    CitraFunciones calculadora;

    //Operaciones con numeros reales

    calculadora.tetracion(3,3);
    calculadora.resultadoReal("tetracion");

    calculadora.valorAbsoluto(-3);
    calculadora.resultadoReal("valor absoluto");

    calculadora.factorial(4);
    calculadora.resultadoReal("factorial");



    calculadora.setError(1e-15);

    calculadora.setPi(5);
    long double pi = calculadora.pi();
    calculadora.setAureo();
    long double aureo = calculadora.aureo();
    calculadora.setE(8);
    long double numero_e = calculadora.calcularE(calculadora.getErrorTolerancia());
    long double exp_3 = calculadora.exponencial(3, 1000, calculadora.getErrorTolerancia());
    long double sin_pi = calculadora.sen(pi, 1000, calculadora.getErrorTolerancia());
    long double cos_pi = calculadora.cos(pi, 1000, calculadora.getErrorTolerancia());
    long double cos_2pi = calculadora.cos(2*pi, 1000, calculadora.getErrorTolerancia());
    long double sin_pi_4 = calculadora.sen(pi/4, 1000, calculadora.getErrorTolerancia());

    std::cout << std::fixed << std::setprecision(15);
    std::cout << "\nEl valor de pi es " << pi << ".\n" << std::endl;
    std::cout << "\nEl valor de el numero de oro es es " << aureo << ".\n" << std::endl;
    std::cout << "\nEl valor de el numero e es " << numero_e << ".\n" << std::endl;
    std::cout << "\nEl valor de el numero e^3 es " << exp_3 << ".\n" << std::endl;
    std::cout << "\nEl valor de el numero sen(pi) es " << sin_pi << ".\n" << std::endl;
    std::cout << "\nEl valor de el numero sen(pi/4) es " << sin_pi_4 << ".\n" << std::endl;
    std::cout << "\nEl valor de el numero cos(pi) es " << cos_pi << ".\n" << std::endl;
    std::cout << "\nEl valor de el numero cos(2*pi) es " << cos_2pi << ".\n" << std::endl;

    //Operacion con complejos

    complejo c1 = {3.0, 4.0};
    complejo c2 = {1.0, -2.0};
    complejo c3 = {0, pi};
    complejo c4 = {calculadora.factorial(5),pi};

    std::cout << std::fixed << std::setprecision(3);

    calculadora.sumaComplejo(c1, c2);
    calculadora.resultadoComplejo("suma");
    complejo division = calculadora.divisionCompleja(c2, c1);
    calculadora.resultadoComplejo("division");
    calculadora.potenciaComplejaEnetera(c1, 2);
    calculadora.resultadoComplejo("potencia");
    calculadora.modulo(c4);
    calculadora.resultadoReal("modulo");
    calculadora.modulo(c3);
    calculadora.resultadoReal("modulo");
    calculadora.potenciaComplejaEnetera(c4, 2);
    calculadora.resultadoComplejo("potencia de 2");
    calculadora.exponencialComplejo(c3, 1000, 1e-14);
    calculadora.resultadoComplejo("exponencial de base e");
    calculadora.exponencialComplejo(c2, 1000, 1e-10);
    calculadora.resultadoComplejo("exponencial de base e");

    size_t forma[2] = {3, 3};
    size_t indices[2] = {2, 3};
    size_t indice = 0;

    CitrusTensor<complejo> tr(forma, 1, 1); 
    complejo buffer[9] = {c1, c2, c3, c4, c2, c1+c2, c3, c2, c3};
    tr.setDatos(buffer);
    indice = tr.getIndiceLineal(indices);

    std::cout << "El índice para el elemento tensor(2,3) es " << indice << ".\n" << std::endl;
    std::cout << "El dato en tensor(2,3) es " << (tr.getDato(indices)).real << " + " <<  (tr.getDato(indices)).imaginario <<  "i.\n" << std::endl;
    tr(1,1) = {5,6};
    std::cout << "El dato en tensor(1,1) es " << (tr(1,1)).real <<  " + " <<  (tr(1,1)).imaginario << "i.\n" << std::endl;
    return 0;
}
