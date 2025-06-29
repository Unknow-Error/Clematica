#include "CitrusTensor.hpp"

CitrusTensor::CitrusTensor(){}

CitrusTensor::CitrusTensor(const size_t* forma_, size_t contravariante_, size_t covariante_)
    : indiceCovariante(covariante_), indiceContravariante(contravariante_) {

    this->rango = this->indiceCovariante + this->indiceContravariante;

    if (forma_ == nullptr) {
        throw std::invalid_argument("[ERROR] El puntero 'forma_' no puede ser null.");
    }

    if (rango == 0) {
        throw std::invalid_argument("[ERROR] El tensor debe tener al menos un índice (contravariante o covariante).");
    }

    // Reserva de arrays
    this->forma             = new size_t[this->rango];

    // Sólo allocate si es necesario => Tensores puramente covariantes o puramente contravariantes.
    if (this->indiceContravariante > 0)
        this->dimContravariante = new size_t[this->indiceContravariante];
    else
        this->dimContravariante = nullptr;

    if (this->indiceCovariante > 0)
        this->dimCovariante = new size_t[this->indiceCovariante];
    else
        this->dimCovariante = nullptr;

    this->componentes = 1;

    for (size_t i = 0; i < this->rango; i++) {
        if (forma_[i] == 0) {
            throw std::invalid_argument("[ERROR] Cada dimensión en 'forma' debe ser mayor a cero.");
        }
        this->forma[i] = forma_[i];
        this->componentes *= this->forma[i];
    }

    for (size_t i = 0; i < this->indiceContravariante; i++) {
        this->dimContravariante[i] = forma_[i];
    }
    
    for (size_t i = 0; i < this->indiceCovariante; i++) {
        this->dimCovariante[i] = forma_[ this->indiceContravariante + i ];
    }

    this->datos = new long double[this->componentes]();
}

CitrusTensor::~CitrusTensor() {
    delete[] datos;
    delete[] forma;
}

void CitrusTensor::setCovariante(size_t indCov) { this->indiceCovariante = indCov; }
void CitrusTensor::setContravariante(size_t indiceContravariante) { this->indiceCovariante = indiceContravariante; }
size_t CitrusTensor::getIndCovariante() const { return this->indiceCovariante; }
size_t CitrusTensor::getIndContravariante() const { return this->indiceContravariante; }
size_t CitrusTensor::getRango() const { return this->rango; }
size_t CitrusTensor::getComponentes() const { return this->componentes; }
const size_t* CitrusTensor::getForma() const { return this->forma; }
const long double* CitrusTensor::getDatos() const { return this->datos; }
const size_t* CitrusTensor::getDimContrav() const { return this->dimContravariante; }
const size_t* CitrusTensor::getDimCov() const { return this->dimCovariante; }

// Cálculo del índice lineal
void CitrusTensor::verificarIndices(const size_t* indices) const{
    for (size_t i = 0; i < rango; ++i) {
        if ((indices[i]-1) >= this->forma[i]) {
            throw std::out_of_range("Índice fuera de rango.");
        }
    }
}

size_t CitrusTensor::getIndiceLineal(const size_t* indices) const {
    //Convierte los indices multilineales a un índice de un Array 1D de 0 a m.
    //Toma el equivalente de una forma que indique un indice dentro del tensor y devuelve un indice el array 1D.
    //Para un tensor (1,2) en R^3, el elemento de la componente 2 del vector, 3 del primer covector y 2 del segundo covector siendo T^2_32 se escribiria como indice[3] = {2, 3, 2} que se toma por puntero.
    //Y esta función devolvería la posición en el Array 1D.
    // indice = Sumatoria (k = 1 a r) de ( (i_k - 1)* Productorio (m = 1 a r-k) dim(i_m)* Productorio (n = 1 a s) dim(j_n) + Sumatoria (k = 1 a s) de ( (j_k - 1)* Productorio (m = 1+k a s) dim(j_m)
    // Se espera que indices este ordenador de primero los contravariantes y luego los covariantes en el orden en el que se cargo el tensor.

    size_t indiceLineal = 0;

    size_t r = this->indiceContravariante;
    size_t s = this->indiceCovariante;

    try{
        verificarIndices(indices);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }

    // Aporte de la parte contravariante
    size_t aporteContrav = 0;
    for (size_t i = 0; i < r; ++i) {
        // Producto de dimensiones contravariantes posteriores: dimContra[i+1..r-1]
        size_t prodDimContrav = 1;
        for (size_t m = i + 1; m < r; ++m) {
            prodDimContrav *= this->dimContravariante[m];
        }
        // Producto de todas las dimensiones covariantes dimCo[0..s-1]
        size_t prodDimCovAll = 1;
        for (size_t n = 0; n < s; ++n) {
            prodDimCovAll *= this->dimCovariante[n];
        }
        // (indices[i]-1) porque el usuario pasa 1..d, y aquí trabajamos 0..d-1
        aporteContrav += (indices[i] - 1) * prodDimContrav * prodDimCovAll;
    }

    // Aporte de la parte covariante
    size_t aporteCov = 0;
    for (size_t j = 0; j < s; ++j) {
        // Producto de dimensiones covariantes posteriores: dimCo[j+1..s-1]
        size_t prodDimCov = 1;
        for (size_t n = j + 1; n < s; ++n) {
            prodDimCov *= this->dimCovariante[n];
        }
        aporteCov += (indices[r + j] - 1) * prodDimCov;
    }

    indiceLineal = aporteContrav + aporteCov;

    return indiceLineal;
}
