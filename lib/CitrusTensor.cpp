#include "CitrusTensor.hpp"

template<typename tensorDatos>
CitrusTensor<tensorDatos>::CitrusTensor(){}

template<typename tensorDatos>
CitrusTensor<tensorDatos>::CitrusTensor(const size_t* forma_, size_t contravariante_, size_t covariante_)
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

    this->datos = nullptr;
}

template<typename tensorDatos>
CitrusTensor<tensorDatos>::~CitrusTensor() {
    delete[] this->datos;
    delete[] this->forma;
    delete[] this->dimContravariante;
    delete[] this->dimCovariante;
    this->componentes = 0;
    this->indiceContravariante = 0;
    this->indiceCovariante = 0;
}

template<typename tensorDatos>
size_t CitrusTensor<tensorDatos>::getIndCovariante() const { return this->indiceCovariante; }
template<typename tensorDatos>
size_t CitrusTensor<tensorDatos>::getIndContravariante() const { return this->indiceContravariante; }
template<typename tensorDatos>
size_t CitrusTensor<tensorDatos>::getRango() const { return this->rango; }
template<typename tensorDatos>
size_t CitrusTensor<tensorDatos>::getComponentes() const { return this->componentes; }
template<typename tensorDatos>
const size_t* CitrusTensor<tensorDatos>::getForma() const { return this->forma; }
template<typename tensorDatos>
const size_t* CitrusTensor<tensorDatos>::getDimContrav() const { return this->dimContravariante; }
template<typename tensorDatos>
const size_t* CitrusTensor<tensorDatos>::getDimCov() const { return this->dimCovariante; }

template<typename tensorDatos>
void CitrusTensor<tensorDatos>::setTensor(const size_t* formaNueva, size_t nuevaContravariante, size_t nuevaCovariante) {
    // Permite cambiar la instacia del tensor actual.
    // 1) Asignar valores:
    if (formaNueva == nullptr) {
        throw std::invalid_argument("[ERROR] El puntero 'form' no puede ser null.");
    }

    size_t nuevoRango = nuevaContravariante + nuevaCovariante;
    if (nuevoRango == 0) {
        throw std::invalid_argument("[ERROR] El tensor debe tener al menos un índice.");
    }

    // Reservas temporales
    size_t*  nuevaForma   = new size_t[nuevoRango];
    size_t*  nuevoDimContra = (nuevaContravariante > 0 ? new size_t[nuevaContravariante] : nullptr);
    size_t*  nuevoDimCo     = (nuevaCovariante    > 0 ? new size_t[nuevaCovariante]    : nullptr);

    // CCopiar la nueva forma
    size_t nuevosComponentes = 1;
    for (size_t i = 0; i < nuevoRango; ++i) {
        nuevaForma[i] = forma[i];
        nuevosComponentes *= nuevaForma[i];
    }
    for (size_t i = 0; i < nuevaContravariante; ++i) {
        nuevoDimContra[i] = forma[i];
    }
    for (size_t i = 0; i < nuevaCovariante; ++i) {
        nuevoDimCo[i] = forma[nuevaContravariante + i];
    }

    // 2) Liberar valores viejos
    delete[] this->forma;
    delete[] this->dimContravariante;
    delete[] this->dimCovariante;
    delete[] this->datos;

    // 3) Reasignar
    this->indiceContravariante = nuevaContravariante;
    this->indiceCovariante    = nuevaCovariante;
    this->rango               = nuevoRango;
    this->componentes         = nuevosComponentes;
    this->forma               = nuevaForma;
    this->dimContravariante   = nuevoDimContra;
    this->dimCovariante       = nuevoDimCo;
    this->datos               = nullptr;
}

template<typename tensorDatos>
void CitrusTensor<tensorDatos>::setDatos(const tensorDatos* arregloDatos){
    if (!arregloDatos) {
        throw std::invalid_argument("setDatos: arregloDatos no puede ser null.");
    }
    // Libero búfer viejo
    delete[] this->datos;
    // Creo un nuevo búfer del tamaño “componentes”
    this->datos = new tensorDatos[this->componentes];
    // Copio elemento a elemento
    for (size_t i = 0; i < this->componentes; ++i) {
        this->datos[i] = arregloDatos[i];
    }
}

template<typename tensorDatos>
tensorDatos CitrusTensor<tensorDatos>::getDato(const size_t* indices) const {
    return datos[getIndiceLineal(indices)];
}

template<typename tensorDatos>
const tensorDatos* CitrusTensor<tensorDatos>::getDatos() const {
    return this->datos;
}

// Cálculo del índice lineal
template<typename tensorDatos>
void CitrusTensor<tensorDatos>::verificarIndices(const size_t* indices) const{
    for (size_t i = 0; i < rango; ++i) {
        if ((indices[i]-1) >= this->forma[i]) {
            throw std::out_of_range("Índice fuera de rango.");
        }
    }
}

template<typename tensorDatos>
size_t CitrusTensor<tensorDatos>::getIndiceLineal(const size_t* indices) const {
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

// Sobrecarga a los operadores () -> Usarlo para notacion, modificación y recuerar elementos con indices directo (encapsular lo de getIndiceLineal o getDato(indices))
template<typename tensorDatos>
template<typename... Indices>
tensorDatos& CitrusTensor<tensorDatos>::operator()(Indices... indices) {
    static_assert(sizeof...(indices) > 0, "Debe haber al menos un índice.");
    static_assert((std::is_convertible_v<Indices, size_t> && ...), "Todos los índices deben ser size_t o convertibles.");

    if (sizeof...(indices) != this->rango) throw std::invalid_argument("Número de índices incorrecto.");
    
    size_t indice_array[] = { static_cast<size_t>(indices)... };
    size_t indiceLineal = this->getIndiceLineal(indice_array);
    return this->datos[indiceLineal];    
}

template<typename tensorDatos>
template<typename... Indices>
const tensorDatos& CitrusTensor<tensorDatos>::operator()(Indices... indices) const {
    static_assert(sizeof...(indices) > 0, "Debe haber al menos un índice.");
    static_assert((std::is_convertible_v<Indices, size_t> && ...), "Todos los índices deben ser size_t o convertibles.");
    if (sizeof...(indices) != this->rango) throw std::invalid_argument("Número de índices incorrecto.");

    ssize_t indice_array[] = { static_cast<size_t>(indices)... };
    size_t indiceLineal = this->getIndiceLineal(indice_array);

    return this->datos[indiceLineal];
}

template class CitrusTensor<long double>;
template class CitrusTensor<complejo>;