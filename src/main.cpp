#include <iostream>
#include <iomanip> // necesario para setprecision y fixed
#include "../lib/ClematicaCore/ClemCore.cpp"
#include "../lib/ClematicaCore/CitraFunciones.cpp"
#include "../lib/CitrusTensorial/CitrusTensor.cpp" 


int main(int argc, char *argv[])
{
    CitraFunciones calculadora;

    calculadora.setError(1e-15);

    calculadora.setPi(5);
    long double pi = calculadora.pi();
    calculadora.setAureo();
    long double aureo = calculadora.aureo();
    calculadora.setE(8);
    long double numero_e = calculadora.calcularE(calculadora.getErrorTolerancia());

    //Operacion con complejos

    complejo c1 = {3.0, 4.0};
    complejo c2 = {1.0, -2.0};
    complejo c3 = {0, pi};
    complejo c4 = {calculadora.factorial(5),pi};

    std::cout << std::fixed << std::setprecision(3);

    size_t forma[2] = {3, 3};
    size_t indices[2] = {2, 3};
    size_t indice = 0;
    size_t formaVec[1] = {3};

    CitrusTensor<complejo> tc1(forma, 1, 1); 
    CitrusTensor<complejo> tc3(forma, 1, 1);
    CitrusTensor<complejo> tc4(formaVec, 1, 0), tc5(formaVec, 0, 1), tc6(forma, 1, 1);
    complejo buffer[9] = {c1, c2, c3, c4, c2, c1+c2, c3, c2, c3};
    tc1.setDatos(buffer);
    indice = tc1.getIndiceLineal(indices);
    complejo buffVec[3] = {c1, c2, c3};
    tc4.setDatos(buffVec);
    tc5.setDatos(buffVec);
    Tensor<complejo> resultadoProdTens = tc4.productoTensorial(tc4.getTensor(), tc5.getTensor());
    tc6.setTensorCompleto(resultadoProdTens);

    std::cout << "El índice para el elemento tensor(2,3) es " << indice << ".\n" << std::endl;
    std::cout << "El dato en tensor(2,3) es " << (tc1.getDato(indices)).real << " + " <<  (tc1.getDato(indices)).imaginario <<  "i.\n" << std::endl;
    tc1(1,1) = {5,6};
    std::cout << "El dato en tensor(1,1) es " << (tc1(1,1)).real <<  " + " <<  (tc1(1,1)).imaginario << "i.\n" << std::endl;
    Tensor<complejo> resultado = tc1.sumaTensorial(tc1.getTensor(), tc1.getTensor());
    tc3.setTensorCompleto(resultado);
    std::cout << "El dato en tc3(2,3) es " << (tc3(2,3)).real <<  " + " <<  (tc3(2,3)).imaginario << "i.\n" << std::endl;
    Tensor<complejo> conjugadoTc3 = tc1.conjugadoTensor(tc3.getTensor());
    tc3.setTensorCompleto(conjugadoTc3);
    std::cout << "El dato en tc3(2,3) al conjugar es " << (tc3(2,3)).real <<  " + " <<  (tc3(2,3)).imaginario << "i.\n" << std::endl;
    std::cout << "El resultado del producto tensorial en la componente tc6(1,1) es " << (tc6(1,1)).real <<  " + " <<  (tc6(1,1)).imaginario << "i.\n" << std::endl;
    std::cout << "El resultado del producto tensorial en la componente tc6(3,3) es " << (tc6(3,3)).real <<  " + " <<  (tc6(3,3)).imaginario << "i.\n" << std::endl;
    return 0;
}
