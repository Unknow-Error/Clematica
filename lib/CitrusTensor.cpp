#include "CitrusTensor.hpp"

CitrusTensor::CitrusTensor(){}

CitrusTensor::CitrusTensor(const size_t* forma_, size_t dim_, size_t contravariante_, size_t covariante_)
    : indiceCovariante(covariante_), indiceContravariante(contravariante_), dimension(dim_) {

    this->rango = this->indiceCovariante + this->indiceContravariante;

    if (forma_ == nullptr) {
        throw std::invalid_argument("[ERROR] El puntero 'forma_' no puede ser null.");
    }

    if (dim_ == 0) {
        throw std::invalid_argument("[ERROR] La dimensión del espacio no puede ser cero.");
    }

    if (rango == 0) {
        throw std::invalid_argument("[ERROR] El tensor debe tener al menos un índice (contravariante o covariante).");
    }

    this->forma = new size_t[this->rango];
    this->componentes = 1;

    for (size_t i = 0; i < this->rango; i++) {
        if (forma_[i] == 0) {
            throw std::invalid_argument("[ERROR] Cada dimensión en 'forma' debe ser mayor a cero.");
        }
        this->forma[i] = forma_[i];
        this->componentes *= this->forma[i];
    }

    this->datos = new long double[this->componentes];
    for (size_t i = 0; i < this->componentes; i++) {
        this->datos[i] = 0.0L;
    }
}

CitrusTensor::~CitrusTensor() {
    delete[] datos;
    delete[] forma;
}

void CitrusTensor::setCovariante(size_t indCov) { this->indiceCovariante = indCov; }
void CitrusTensor::setContravariante(size_t indiceContravariante) { this->indiceCovariante = indiceContravariante; }
void CitrusTensor::setDimension(size_t dim) { this->dimension = dim; }
size_t CitrusTensor::getIndCovariante() const { return this->indiceCovariante; }
size_t CitrusTensor::getIndContravariante() const { return this->indiceContravariante; }
size_t CitrusTensor::getRango() const { return this->rango; }
size_t CitrusTensor::getDimension() const { return this->dimension; }
size_t CitrusTensor::getComponentes() const { return this->componentes; }
const size_t* CitrusTensor::getForma() const { return this->forma; }
const long double* CitrusTensor::getDatos() const { return this->datos; }

// Cálculo del índice lineal
void CitrusTensor::verificarIndices(const size_t* indices) const{
    for (size_t i = 0; i < rango; ++i) {
        if ((indices[i]-1) >= this->forma[i]) {
            throw std::out_of_range("Índice fuera de rango.");
        }
    }
}

size_t CitrusTensor::potenciaBaseEntera(size_t base, size_t exponente) const {
    size_t resultado = 1;

    if(exponente == 0){
        resultado = 1;
    } else {
        resultado = 1;
        for (size_t i = 0; i < exponente; i++)  resultado *= base;
    }

    return resultado;
}

size_t CitrusTensor::getIndiceLineal(const size_t* indices) const {
    //Convierte los indices multilineales a un índice de un Array 1D de 0 a m.
    //Toma el equivalente de una forma que indique un indice dentro del tensor y devuelve un indice el array 1D.
    //Para un tensor (1,2) en R^3, el elemento de la componente 2 del vector, 3 del primer covector y 2 del segundo covector siendo T^2_32 se escribiria como indice[3] = {2, 3, 2} que se toma por puntero.
    //Y esta función devolvería la posición en el Array 1D.
    // El indice = Sumatoria (k = 1 a rango) de ( (i_k - 1)*dim^(rango-k)) con i_k siendo un indice de indices.

    size_t indiceLineal = 0;

    try{
        verificarIndices(indices);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }

    size_t j = 1, exponente, potencia;
    for(int i = 0; i < static_cast<int>(getRango()); i++){
        exponente = getRango() - j;
        potencia = potenciaBaseEntera(getDimension(), exponente);
        indiceLineal += (indices[i]-1) * potencia;
        j++;
    }

    return indiceLineal;
}
