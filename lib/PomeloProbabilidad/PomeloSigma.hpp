/*
Biblioteca que define una clase cuyos objetivos son las operaciones estadísticas y probabilísticas básicas : máx, min, promedio, esperanza, sumatoria, productorio, desviación estandar, varianza,
covarianza, entre otros.
Implementa vectores de CitraVector y matrices CitraMatriz
*/

#include "../CitraVector.hpp"
#include "../CitraMatriz.hpp"
#include "../CitraFunciones.hpp"

template<typename numTipo>
class PomeloSigma : CitraFunciones{
public:
    Vector<numTipo> muestreoCitrico;
public:
    //Constructores
    PomeloSigma();
    PomeloSigma(const Vector<numTipo> espacioMuestral_);

    //Destructor
    ~PomeloSigma();

    //Funciones estadísticas básicas:
    numTipo sumatoria(const Vector<numTipo>& espMuestral) const;
    numTipo productorio(const Vector<numTipo>& espMuestral) const;
    numTipo moda(const Vector<numTipo>& espMuestral) const;
    numTipo esperanza(const Vector<numTipo>& espMuestral, const Vector<numTipo>& probabilidades) const;
    numTipo promedio(const Vector<numTipo>& espMuestral) const;
    numTipo varianza(const Vector<numTipo>& espMuestral, const Vector<numTipo>& probabilidades) const;
    numTipo desviacionEstandar(const Vector<numTipo>& espMuestral, const Vector<numTipo>& probabilidades) const;
    numTipo media(const Vector<numTipo>& espMuestral) const;

    //Distribuciones estadísticas:
}