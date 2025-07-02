
#include "../CitrusTensor.hpp"

// Una vector es un Tensor de tipo (0,1) fila o tipo (1,0) columna. Multidimensional, pero siempre rango 1.
template<typename numTipo>
struct Vector{
    Tensor<numTipo> vector;
    vector.rango = 1;
};

template<typename numTipo>
class CitraVector : CitrusTensor<numTipo> {
private:
    Vector<numTipo> naranja;
public:
    //Constructores
    CitraVector();
    CitraVector(const size_t tamanio_, bool tipo_);

    //Destructor
    ~CitraVector();

    //Operaciones vectoriales.
    Vector<numTipo> sumaVectorial(const Vector<numTipo>& vector1, const Vector<numTipo>& vector2) const;
    Vector<numTipo> restaVectorial(const Vector<numTipo>& vector1, const Vector<numTipo>& vector2) const;
    Vector<numTipo> productoEscalar(const Vector<numTipo>& vector1, const Vector<numTipo>& vector2) const;
    Vector<numTipo> productoVectorial(const Vector<numTipo>& vector1, const Vector<numTipo>& vector2) const;
    Vector<numTipo> proyeccionEscalar(const Vector<numTipo>& vector1, const Vector<numTipo>& vector2) const;
    Vector<numTipo> proyeccionVectorial(const Vector<numTipo>& vector1, const Vector<numTipo>& vector2) const;

    //Geometria lineal (Euclidiana)
    numTipo distEuclidianaInterPuntos(const Vector<numTipo>& punto1, const Vector<numTipo>& punto2) const; //Distancia Euclideana
    Vector<numTipo> vectorDirector(const Vector<numTipo>& punto1, const Vector<numTipo>& punto2) const;
    const Vector<numTipo>* recta(const Vector<numTipo>& vectorDirector, const Vector<numTipo>& punto) const;
    numTipo distEuclidianaInterRectas(const Vector<numTipo>* recta1, const Vector<numTipo>* recta2) const;
    numTipo distEuclidianaInterRectaPunto(const Vector<numTipo>* recta, const Vector<numTipo> punto) const;
    Vector<numTipo> vectorNormal(const Vector<numTipo>& vectorDirector1, const Vector<numTipo>& vectorDirector2) const;
    const Vector<numTipo>* plano(const Vector<numTipo>& normal, const Vector<numTipo>& punto1) const;
};