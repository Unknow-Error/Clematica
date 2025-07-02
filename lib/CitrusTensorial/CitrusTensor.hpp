/*
Biblioteca que define clase Tensor : Objeto matemático que define un espacio vectorial o variedad diferenciable
y cuyas componentes se transforman de una mantera específica bajo un cambio de coordenadas.

Un Tensor de tipo (r, s) sobre un espacio vectorial V (de dimensión n) es una aplicación multilineal tal que:
T: V* x ... x V* x V x ... X -> R, siendo V* r veces y V s veces.
V es un espacio vectorial y V* es el espacio dual (conjunto de formas lineales sobre V).
r: número de covariantes (formas lineales)
s: número de contravariantes (vectores)
(r,s) define el tipo de tensor y r+s el rango.

Contravariantes => Vectores => Transforman inversamente al cambio de base : v^(i') = (dx^(i') dx^j) v^j (Superíndices)
Covariantes => Formas lineales o co-vectores => w_(i') = (dx^(j)/dx^(i')) w_j (subíndice) => transforman directamente al cambio de base.

Ejemplo:
Escalar (a) => Tipo (0,0) -> Temperatura en un punto
Vector (v^i)=> Tipo (1,0) -> Velocidad de una partícula (se transforma como vector contravariante) => Direccion en el espacio
Covector (w_i) => Tipo (0,1) => Gradiente de un campo escalar (Transforma colineal)
Matriz (transformación lineal) (T^i_j) => Tipo (1,1)
Métrica g_ij => Tipo (0,2)
Curvatura R^i_jkl => Tipo (1,3) => Tensor de Riemann.

Propiedades:
1-Multilinealidad : Es lineal en cada uno de sus Argumentos: T(av+bw, ...) = aT(v,...)+bT(w,...)
2-Regla transformación tensorial : Bajo un cambio de coordenadas, las componentes se transforman de la sguiente forma:
PAra un tensor T^(i_1, ...i_r)_(j_1,...j_r) :
T^(i'_1, ...i'_r)_(j'_1,...j'_r) = Productoria de k = 1 a r (dx^(i'k)/dx^(ik) ) *  Productorio k = 1 a s (dx^(jl)/dx^(j'l)) T^(i_1, ...i_r)_(j_1,...j_r)
3- Tensores del mismo tipo pueden sumarse o restarse. Sus componentes pueden multiplicarse/dividirse bis a bis.
4- Pueden multiplicarse tensorialmente (operador bilineal) generando un tensor cuyo tipo es la suma de los tipos de los tensores multiplicados.
*/

#include <stdexcept>
#include <type_traits>
#include "../ClematicaCore/ClemCore.hpp"

#ifndef CITRUSTENSOR_HPP
#define CITRUSTENSOR_HPP

template<typename numTipo> //Para sobrecargar el puntero y poder cambiar el tipo de datos.
struct Tensor {
    size_t indiceContravariante = 0, indiceCovariante = 0, rango = 0, componentes = 0; //valor s, valor r, valor r+s y cantidad de componentes del Array multidimensional
    size_t *forma = nullptr, *dimContravariante = nullptr, *dimCovariante = nullptr; //Puntero de array con tamaño de dimension. Es el arreglo que describe la cantidad de valores que puede tomar cada uno de los índices del tensor.
    numTipo *datos = nullptr; //Puntero al tipo de datos

    // Aplicar RAII (Resource Acquisition Is Initialization) : 
    // Para que se libere automáticamente la memoria cuando un Tensor<numTipo> deja de estar en uso — y evitar así fugas de memoria.

    // Constructor por defecto (Crear un tensor vacío)
    Tensor() noexcept = default;

    // Destructor
    ~Tensor() {
        delete[] forma;
        delete[] dimContravariante;
        delete[] dimCovariante;
        delete[] datos;
    }

    // Constructor por copia
    Tensor(const Tensor& otroTensor)
        : indiceContravariante(otroTensor.indiceContravariante),
          indiceCovariante(otroTensor.indiceCovariante),
          rango(otroTensor.rango),
          componentes(otroTensor.componentes)
     {
        forma = new size_t[rango];
        for (size_t i = 0; i < rango; ++i) forma[i] = otroTensor.forma[i];

        dimContravariante = new size_t[indiceContravariante];
        for (size_t i = 0; i < indiceContravariante; ++i)
            dimContravariante[i] = otroTensor.dimContravariante[i];

        dimCovariante = new size_t[indiceCovariante];
        for (size_t i = 0; i < indiceCovariante; ++i)
            dimCovariante[i] = otroTensor.dimCovariante[i];

        datos = new numTipo[componentes];
        for (size_t i = 0; i < componentes; ++i)
            datos[i] = otroTensor.datos[i];
    }

    // Constructor por movimiento
    Tensor(Tensor&& otroTensor) noexcept
        : indiceContravariante(otroTensor.indiceContravariante),
          indiceCovariante(otroTensor.indiceCovariante),
          rango(otroTensor.rango),
          componentes(otroTensor.componentes),
          forma(otroTensor.forma),
          dimContravariante(otroTensor.dimContravariante),
          dimCovariante(otroTensor.dimCovariante),
          datos(otroTensor.datos)
    {
        otroTensor.forma = nullptr;
        otroTensor.dimContravariante = nullptr;
        otroTensor.dimCovariante = nullptr;
        otroTensor.datos = nullptr;
        otroTensor.componentes = 0;
        otroTensor.rango = 0;
    }

    // Sobrecarga al operador de asignación para hacer copia o movimiento con swap.
    Tensor& operator=(Tensor otroTensor) noexcept{
        swapTensor(*this, otroTensor);
        return *this;
    }

    friend Tensor<numTipo> copiarTensorForma(const Tensor<numTipo>& tensor);
};

//Función libre de Swap para luego usarla en la asignación por copia o movimiento 
template<typename numTipo>
void swapTensor(Tensor<numTipo>& tensor1, Tensor<numTipo>& tensor2) noexcept {
    std::swap(tensor1.indiceContravariante, tensor2.indiceContravariante);
    std::swap(tensor1.indiceCovariante,    tensor2.indiceCovariante);
    std::swap(tensor1.rango,               tensor2.rango);
    std::swap(tensor1.componentes,         tensor2.componentes);

    std::swap(tensor1.forma,               tensor2.forma);
    std::swap(tensor1.dimContravariante,   tensor2.dimContravariante);
    std::swap(tensor1.dimCovariante,       tensor2.dimCovariante);
    std::swap(tensor1.datos,               tensor2.datos);
}

template<typename numTipo> 
class CitrusTensor{
private:
    Tensor<numTipo> tensor;

public:
    // Constructor
    CitrusTensor();
    CitrusTensor(const size_t* forma_, size_t contravariante_, size_t covariante_); //const => para evitar modificar el estado interno del objeto
    // Por ejemplo, un tensor (1,2) en R^3 seria 3x3x3 por lo que forma[3] = {3,3,3}; <- Se le pasa este arreglo al constructor.
    //En el caso de que los índices contravariantes o covariantes tengan diferente dimensiones (multidimensionalidad). Se especifican con arrays similares a forma

    // Destructor
    ~CitrusTensor();

    //Regla : Si el método no modifica ningún atributo de la clase, debe ser const.
    // Prevenir errores o modificaciones indeseadas y Llamar al método desde otros métodos const.
    
    size_t getIndCovariante() const;
    size_t getIndContravariante() const;
    size_t getRango() const;
    const size_t* getDimContrav() const;
    const size_t* getDimCov() const;
    size_t getComponentes() const;
    const size_t* getForma() const; //Se le agregar const al puntero => Evitar que el llamador pueda modificar los datos apuntados por los punteros

    // Conversión de índice multidimensional a lineal
    void verificarIndices(const size_t* indices) const;
    size_t getIndiceLineal(const size_t* indices) const;

    // Modificar instancia actual de tensor y los datos.
    void setTensor(const size_t* forma, size_t nuevaContravariante, size_t nuevaCovariante);
    void setTensorCompleto(const Tensor<numTipo> tensorNuevo);
    void setDatos(const numTipo* arregloDatos);
    numTipo getDato(const size_t* indices) const;
    const numTipo* getDatos() const;
    Tensor<numTipo> getTensor() const;

    // Acceso y modificación por índices múltiples (estilo matemático) => Similar a MATLAB.
    template<typename... Indices>
    numTipo& operator()(Indices... indices);

    template<typename... Indices>
    const numTipo& operator()(Indices... indices) const;

    // Definir operaciones con Tensores : Producto tensorial, suma/resta tensores coherentes, producto/division interelementos de tensores coherentes. 
    void verificarOperabilidad(const Tensor<numTipo>& tensor1, const Tensor<numTipo>& tensor2) const;
    void verificarTensorNulo(const Tensor<numTipo>& tensor) const;
    Tensor<numTipo> sumaTensorial(const Tensor<numTipo>& tensor1, const Tensor<numTipo>& tensor2) const;
    Tensor<numTipo> restaTensorial(const Tensor<numTipo>& tensor1, const Tensor<numTipo>& tensor2) const;
    Tensor<numTipo> productoTensorial(const Tensor<numTipo>& tensor1,const Tensor<numTipo>& tensor2) const; //Operador ⊗
    Tensor<numTipo> prodComponetesTensorial(const Tensor<numTipo>& tensor1,const Tensor<numTipo>& tensor2) const;
    Tensor<numTipo> prodTensoEscalar(const numTipo& escalar,const Tensor<numTipo>& tensor) const;
    void verificarDivisionTensorial(const Tensor<numTipo>& tensor) const;
    Tensor<numTipo> divComponentesTensorial(const Tensor<numTipo>& tensor1,const Tensor<numTipo>& tensor2) const;

    // Otras operaciones: conjugado de los elementos, contracción de índices, Cambio de tipo de índice, contracción parcial, Simetrización y antisimetrización
    // Permutación de índices, Composición de tensores
    Tensor<numTipo> conjugadoTensor(const Tensor<numTipo>& tensor) const;
    //Tensor<numTipo> contraccionIndices(const Tensor<numTipo>& tensor) const;
    //Tensor<numTipo> cambiarIndice(const Tensor<numTipo>& tensor, const Tensor<numTipo>& metrica) const;
    //Tensor<numTipo> simetrizacion(const Tensor<numTipo>& tensor) const;
    //Tensor<numTipo> antisimetrizacion(const Tensor<numTipo>& tensor) const;  
    //Tensor<numTipo> permutarIndices(const Tensor<numTipo>& tensor, const size_t* dimContrvPermutar, const size_t* dimConvPermutar);
};

#endif
