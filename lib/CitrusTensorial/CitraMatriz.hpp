
#include "../CitrusTensor.hpp"
#include "../CitraVector.hpp"

// Una matriz es un Tensor de tipo (1,1). Multidimensional, pero siempre rango 2.
template<typename numTipo> 
struct Matriz {
    Tensor<numTipo> matriz;
    matriz.indiceContravariante = 1;
    matriz.indiceCovariante = 1;
    matriz.rango = 2;
};

template<typename numTipo>
class CitraMatriz : CitrusTensor<numTipo> {
private:
    Matriz<numTipo> limon;
public:
    //Constructores
    CitraMatriz();
    CitraMatriz(const size_t numFilas_, const size_t numColumnas_);

    //Destructor
    ~CitraMatriz();

    size_t getNumFilas() const;
    size_t getNumColumnas() const;
    void setNumFilas(const size_t numFilas);
    void setNumColumnas(const size_t numColumnas);

    //Operaciones para matrices y de algebra lineal
    Matriz<numTipo> sumaMatricial(const Matriz<numTipo>& matriz1, const Matriz<numTipo>& matriz2) const;
    Matriz<numTipo> restaMatricial(const Matriz<numTipo>& matriz1, const Matriz<numTipo>& matriz2) const;
    Matriz<numTipo> productoMatricial(const Matriz<numTipo>& matriz1, const Matriz<numTipo>& matriz2) const;
    Matriz<numTipo> productoKronecker(const Matriz<numTipo>& matriz1, const Matriz<numTipo>& matriz2) const;
    numTipo traza(const Matriz<numTipo>& matriz) const;

    //Operaciones para la inversa
    Matriz<numTipo> matrizAdjunta(const Matriz<numTipo>& matriz) const;
    numTipo determinante(const Matriz<numTipo>& matriz) const;
    Matriz<numTipo> transpuesta(const Matriz<numTipo>& matriz) const;
    Matriz<numTipo> inversa(const Matriz<numTipo>& matriz) const;

    //Operaciones diagonalización
    const numTipo* eigenValores(const Matriz<numTipo>& matriz) const;
    const Vector<numTipo>* eigenVectores(const Matriz<numTipo>& matriz) const;
    Matriz<numTipo> matrizEigenVectores(const Matriz<numTipo>& matriz) const;
    Matriz<numTipo> matrizDiagonal(const Matriz<numTipo>& matriz) const;
};