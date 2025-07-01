#include "CitrusTensor.hpp"

//Implementación de las sobrecargas de operadores de la struct Tensor:
template<typename tensorDatos>
Tensor<tensorDatos> copiarTensorForma(const Tensor<tensorDatos>& tensor){
    Tensor<tensorDatos> tensorResultado;
    tensorResultado.indiceContravariante = tensor.indiceContravariante;
    tensorResultado.indiceCovariante = tensor.indiceCovariante;
    tensorResultado.rango = tensor.rango;
    tensorResultado.componentes = tensor.componentes;

    tensorResultado.forma = new size_t[tensorResultado.rango];
    for(size_t i = 0; i < tensor.rango; i++){
        tensorResultado.forma[i] = tensor.forma[i];
    }

    if (tensor.indiceContravariante > 0) {
        tensorResultado.dimContravariante = new size_t[tensor.indiceContravariante];
        for(size_t i = 0; i < tensor.indiceContravariante; i++){
            tensorResultado.dimContravariante[i] = tensor.dimContravariante[i];
        }
    }

    if (tensor.indiceCovariante > 0) {
        tensorResultado.dimCovariante = new size_t[tensor.indiceCovariante];
        for(size_t i = 0; i < tensor.indiceCovariante; i++){
            tensorResultado.dimCovariante[i] = tensor.dimCovariante[i];
        }
    }

    tensorResultado.datos = nullptr;

    return tensorResultado;
}

template<typename tensorDatos>
Tensor<tensorDatos> copiarTensor(const Tensor<tensorDatos>& tensor){
    Tensor<tensorDatos> tensorResultado = copiarTensorForma<tensorDatos>(tensor);

    if (tensor.componentes > 0 && tensor.datos != nullptr) {
        tensorResultado.datos = new tensorDatos[tensor.componentes];
        for(size_t i = 0; i < tensor.componentes; i++){
            tensorResultado.datos[i] = tensor.datos[i];
        }
    }

    return tensorResultado;
}

template<typename tensorDatos>
void liberarTensor(Tensor<tensorDatos>& tensor) {
    if (tensor.datos != nullptr) {
        delete[] tensor.datos;
        tensor.datos = nullptr;
    }

    if (tensor.forma != nullptr) {
        delete[] tensor.forma;
        tensor.forma = nullptr;
    }

    if (tensor.dimContravariante != nullptr) {
        delete[] tensor.dimContravariante;
        tensor.dimContravariante = nullptr;
    }

    if (tensor.dimCovariante != nullptr) {
        delete[] tensor.dimCovariante;
        tensor.dimCovariante = nullptr;
    }

    tensor.componentes = 0;
    tensor.indiceContravariante = 0;
    tensor.indiceCovariante = 0;
    tensor.rango = 0;
}

template<typename tensorDatos>
CitrusTensor<tensorDatos>::CitrusTensor(){}

template<typename tensorDatos>
CitrusTensor<tensorDatos>::CitrusTensor(const size_t* forma_, size_t contravariante_, size_t covariante_) {

    this->tensor.indiceContravariante = contravariante_;
    this->tensor.indiceCovariante = covariante_;
    this->tensor.rango = this->tensor.indiceCovariante + this->tensor.indiceContravariante;

    if (forma_ == nullptr) {
        throw std::invalid_argument("[ERROR] El puntero 'forma_' no puede ser null.");
    }

    if (this->tensor.rango == 0) {
        throw std::invalid_argument("[ERROR] El tensor debe tener al menos un índice (contravariante o covariante).");
    }

    // Reserva de arrays
    this->tensor.forma = new size_t[this->tensor.rango];

    if (this->tensor.indiceContravariante > 0)
        this->tensor.dimContravariante = new size_t[this->tensor.indiceContravariante];
    else
        this->tensor.dimContravariante = nullptr;

    if (this->tensor.indiceCovariante > 0)
        this->tensor.dimCovariante = new size_t[this->tensor.indiceCovariante];
    else
        this->tensor.dimCovariante = nullptr;

    this->tensor.componentes = 1;

    for (size_t i = 0; i < this->tensor.rango; i++) {
        if (forma_[i] == 0) {
            throw std::invalid_argument("[ERROR] Cada dimensión en 'forma' debe ser mayor a cero.");
        }
        this->tensor.forma[i] = forma_[i];
        this->tensor.componentes *= this->tensor.forma[i];
    }

    for (size_t i = 0; i < this->tensor.indiceContravariante; i++) {
        this->tensor.dimContravariante[i] = forma_[i];
    }

    for (size_t i = 0; i < this->tensor.indiceCovariante; i++) {
        this->tensor.dimCovariante[i] = forma_[this->tensor.indiceContravariante + i];
    }

    this->tensor.datos = nullptr;
}

template<typename tensorDatos>
CitrusTensor<tensorDatos>::~CitrusTensor() {
    liberarTensor<tensorDatos>(this->tensor);
}

template<typename tensorDatos>
size_t CitrusTensor<tensorDatos>::getIndCovariante() const { return this->tensor.indiceCovariante; }
template<typename tensorDatos>
size_t CitrusTensor<tensorDatos>::getIndContravariante() const { return this->tensor.indiceContravariante; }
template<typename tensorDatos>
size_t CitrusTensor<tensorDatos>::getRango() const { return this->tensor.rango; }
template<typename tensorDatos>
size_t CitrusTensor<tensorDatos>::getComponentes() const { return this->tensor.componentes; }
template<typename tensorDatos>
const size_t* CitrusTensor<tensorDatos>::getForma() const { return this->tensor.forma; }
template<typename tensorDatos>
const size_t* CitrusTensor<tensorDatos>::getDimContrav() const { return this->tensor.dimContravariante; }
template<typename tensorDatos>
const size_t* CitrusTensor<tensorDatos>::getDimCov() const { return this->tensor.dimCovariante; }

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
        nuevaForma[i] = formaNueva[i];
        nuevosComponentes *= nuevaForma[i];
    }
    for (size_t i = 0; i < nuevaContravariante; ++i) {
        nuevoDimContra[i] = nuevaForma[i];
    }
    for (size_t i = 0; i < nuevaCovariante; ++i) {
        nuevoDimCo[i] = nuevaForma[nuevaContravariante + i];
    }

    // 2) Liberar valores viejos
    delete[] this->tensor.forma;
    delete[] this->tensor.dimContravariante;
    delete[] this->tensor.dimCovariante;
    delete[] this->tensor.datos;

    // 3) Reasignar
    this->tensor.indiceContravariante = nuevaContravariante;
    this->tensor.indiceCovariante    = nuevaCovariante;
    this->tensor.rango               = nuevoRango;
    this->tensor.componentes         = nuevosComponentes;
    this->tensor.forma               = nuevaForma;
    this->tensor.dimContravariante   = nuevoDimContra;
    this->tensor.dimCovariante       = nuevoDimCo;
    this->tensor.datos               = nullptr;
}

template<typename tensorDatos>
void CitrusTensor<tensorDatos>::setTensorCompleto(const Tensor<tensorDatos> tensorNuevo){
    liberarTensor<tensorDatos>(this->tensor); // Limpieza previa
    this->tensor = copiarTensor<tensorDatos>(tensorNuevo);
}

template<typename tensorDatos>
void CitrusTensor<tensorDatos>::setDatos(const tensorDatos* arregloDatos){
    if (!arregloDatos) {
        throw std::invalid_argument("setDatos: arregloDatos no puede ser null.");
    }
    // Libero búfer viejo
    delete[] this->tensor.datos;
    // Creo un nuevo búfer del tamaño “componentes”
    this->tensor.datos = new tensorDatos[this->tensor.componentes];
    // Copio elemento a elemento
    for (size_t i = 0; i < this->tensor.componentes; ++i) {
        this->tensor.datos[i] = arregloDatos[i];
    }
}

template<typename tensorDatos>
tensorDatos CitrusTensor<tensorDatos>::getDato(const size_t* indices) const {
    return this->tensor.datos[getIndiceLineal(indices)];
}

template<typename tensorDatos>
const tensorDatos* CitrusTensor<tensorDatos>::getDatos() const {
    return this->tensor.datos;
}

template<typename tensorDatos>
Tensor<tensorDatos> CitrusTensor<tensorDatos>::getTensor() const { return this->tensor;}

// Cálculo del índice lineal
template<typename tensorDatos>
void CitrusTensor<tensorDatos>::verificarIndices(const size_t* indices) const{
    for (size_t i = 0; i < this->tensor.rango; ++i) {
        if ((indices[i]-1) >= this->tensor.forma[i]) {
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

    try{
        verificarIndices(indices);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }

    size_t indiceLineal = 0;

    size_t r = this->tensor.indiceContravariante;
    size_t s = this->tensor.indiceCovariante;

    // Aporte de la parte contravariante
    size_t aporteContrav = 0;
    for (size_t i = 0; i < r; ++i) {
        // Producto de dimensiones contravariantes posteriores: dimContra[i+1..r-1]
        size_t prodDimContrav = 1;
        for (size_t m = i + 1; m < r; ++m) {
            prodDimContrav *= this->tensor.dimContravariante[m];
        }
        // Producto de todas las dimensiones covariantes dimCo[0..s-1]
        size_t prodDimCovAll = 1;
        for (size_t n = 0; n < s; ++n) {
            prodDimCovAll *= this->tensor.dimCovariante[n];
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
            prodDimCov *= this->tensor.dimCovariante[n];
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

    if (sizeof...(indices) != this->tensor.rango) throw std::invalid_argument("Número de índices incorrecto.");
    
    size_t indice_array[] = { static_cast<size_t>(indices)... };
    size_t indiceLineal = this->getIndiceLineal(indice_array);
    return this->tensor.datos[indiceLineal];    
}

template<typename tensorDatos>
template<typename... Indices>
const tensorDatos& CitrusTensor<tensorDatos>::operator()(Indices... indices) const {
    static_assert(sizeof...(indices) > 0, "Debe haber al menos un índice.");
    static_assert((std::is_convertible_v<Indices, size_t> && ...), "Todos los índices deben ser size_t o convertibles.");
    if (sizeof...(indices) != this->tensor.rango) throw std::invalid_argument("Número de índices incorrecto.");

    size_t indice_array[] = { static_cast<size_t>(indices)... };
    size_t indiceLineal = this->getIndiceLineal(indice_array);

    return this->tensor.datos[indiceLineal];
}

template<typename tensorDatos>
void CitrusTensor<tensorDatos>::verificarOperabilidad(
    const Tensor<tensorDatos>& tensor1, 
    const Tensor<tensorDatos>& tensor2
    ) const{
    if (tensor1.rango != tensor2.rango) {
        throw std::out_of_range("Operación inválida. Rango distinto.");
    }
    for(size_t i = 0; i < tensor1.rango ; i++){
        if(tensor1.forma[i] != tensor2.forma[i]){
            throw std::out_of_range("Operación inválida. Los tensores no son del mismo tipo.");
        }
    }
}

template<typename tensorDatos>
Tensor<tensorDatos> CitrusTensor<tensorDatos>::sumaTensorial(
    const Tensor<tensorDatos>& tensor1,
    const Tensor<tensorDatos>& tensor2
    ) const{
    try{
        verificarOperabilidad(tensor1, tensor2);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }

    Tensor<tensorDatos> tensorResultado = copiarTensorForma<tensorDatos>(tensor1);
    tensorResultado.datos = new tensorDatos[tensorResultado.componentes];
    for (size_t i = 0; i < tensor1.componentes; i++){
        tensorResultado.datos[i] = tensor1.datos[i] + tensor2.datos[i];
    }

    return tensorResultado;
}

template<typename tensorDatos>
Tensor<tensorDatos> CitrusTensor<tensorDatos>::restaTensorial(
    const Tensor<tensorDatos>& tensor1,
    const Tensor<tensorDatos>& tensor2
    ) const{
    try{
        verificarOperabilidad(tensor1, tensor2);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }

    Tensor<tensorDatos> tensorResultado = copiarTensorForma<tensorDatos>(tensor1);
    tensorResultado.datos = new tensorDatos[tensorResultado.componentes];
    for (size_t i = 0; i < tensor1.componentes; i++){
        tensorResultado.datos[i] = tensor1.datos[i] - tensor2.datos[i];
    }

    return tensorResultado;
}

template<typename tensorDatos>
Tensor<tensorDatos> CitrusTensor<tensorDatos>::prodComponetesTensorial(
    const Tensor<tensorDatos>& tensor1,
    const Tensor<tensorDatos>& tensor2
    ) const{
    try{
        verificarOperabilidad(tensor1, tensor2);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }

    Tensor<tensorDatos> tensorResultado = copiarTensorForma<tensorDatos>(tensor1);
    tensorResultado.datos = new tensorDatos[tensorResultado.componentes];
    for (size_t i = 0; i < tensor1.componentes; i++){
        tensorResultado.datos[i] = tensor1.datos[i] * tensor2.datos[i];
    }

    return tensorResultado;
}

template<typename tensorDatos>
void CitrusTensor<tensorDatos>::verificarDivisionTensorial (const Tensor<tensorDatos>& tensor) const{
    for(size_t i = 0; i < tensor.componentes; i++){
        if constexpr (std::is_same_v<tensorDatos, complejo>) {
            long double den = tensor.datos[i].real * tensor.datos[i].real 
                            + tensor.datos[i].imaginario + tensor.datos[i].imaginario ;
            if (den < 1e-20L) throw std::runtime_error("Error: División por cero complejo en componente " + std::to_string(i));
        } else {
            // asumimos tipo escalar (long double)
            if (tensor.datos[i] == 0.0L) throw std::runtime_error("Error: División por cero real en componente " + std::to_string(i));
        }
    }
}

template<typename tensorDatos>
Tensor<tensorDatos> CitrusTensor<tensorDatos>::divComponentesTensorial(
    const Tensor<tensorDatos>& tensor1,
    const Tensor<tensorDatos>& tensor2
    ) const{
    try{
        verificarOperabilidad(tensor1, tensor2);
        //Verificar divisibilidad si es complejo o si real
        verificarDivisionTensorial(tensor2);
    } catch (const std::runtime_error& e) {
        std::cerr << "[verificación] " << e.what() << std::endl;
        throw;
    }

    Tensor<tensorDatos> tensorResultado = copiarTensorForma<tensorDatos>(tensor1);
    tensorResultado.datos = new tensorDatos[tensorResultado.componentes];
    for (size_t i = 0; i < tensor1.componentes; i++){
        tensorResultado.datos[i] = tensor1.datos[i] / tensor2.datos[i];
    }

    return tensorResultado;
}

template<typename tensorDatos>
Tensor<tensorDatos> CitrusTensor<tensorDatos>::conjugadoTensor(const Tensor<tensorDatos>& tensor) const{
    Tensor<tensorDatos> tensorResultado = copiarTensorForma<tensorDatos>(tensor);
    tensorResultado.datos = new tensorDatos[tensorResultado.componentes];
    complejo datoResultado;
    if constexpr (std::is_same_v<tensorDatos, complejo>) {
        for (size_t i = 0; i < tensor.componentes; i++){
            tensorResultado.datos[i] = ~(tensor.datos[i]);
        }
    } else {
        // asumimos tipo escalar (long double)
        for (size_t i = 0; i < tensor.componentes; i++){
            tensorResultado.datos[i] = tensor.datos[i];
        }
    } 
    
    return tensorResultado;
}

template class CitrusTensor<long double>;
template class CitrusTensor<complejo>;