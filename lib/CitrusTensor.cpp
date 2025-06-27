#include "CitrusTensor.hpp"

CistrusTensor::CitrusTensor(const size_t* forma_, size_t dim_, size_t covariante_, size_t contravariante_)
    : indiceCovariante(covariante_), indiceContravariante(contravariante_), dimension(dim_) {

        this->rango = indiceCovariante + indiceContravariante;
        forma = new size_t[dimension];
        this->componentes = 1;

        for(size_t i = 0; i < dimensiones; i++){
            forma[i] = forma_[i];
            this->componentes *= forma[i];
        }

        datos = new long double[componentes];
        for(size_t i = 0; i < componentes; i++){
            datos[i] = 0.0; //Inicializar todos con valor nulo.
        }
}

CistrusTensor::~CitrusTensor() {
    delete[] datos;
    delete[] forma;
}

// Cálculo del índice lineal
size_t CistrusTensor::getIndiceLineal(const size_t* indices) const {
    size_t indiceLineal = 0;

}
