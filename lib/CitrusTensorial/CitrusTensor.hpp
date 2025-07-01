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
3-Tensores del mismo tipo pueden sumarse o restarse. Pueden multiplicarse tensorialmente.
*/

#include <stdexcept>
#include <type_traits>
#include "../ClematicaCore/ClemCore.hpp"

#ifndef CITRUSTENSOR_HPP
#define CITRUSTENSOR_HPP

template<typename tensorDatos> //Para sobrecargar el puntero y poder cambiar el tipo de datos.
struct Tensor {
    size_t indiceContravariante = 0, indiceCovariante = 0, rango = 0, componentes = 0; //valor s, valor r, valor r+s y cantidad de componentes del Array multidimensional
    size_t *forma = nullptr, *dimContravariante = nullptr, *dimCovariante = nullptr; //Puntero de array con tamaño de dimension. Es el arreglo que describe la cantidad de valores que puede tomar cada uno de los índices del tensor.
    tensorDatos *datos = nullptr; //Puntero al tipo de datos

    friend Tensor<tensorDatos> copiarTensorForma(const Tensor<tensorDatos>& tensor);
    friend Tensor<tensorDatos> copiarTensor(const Tensor<tensorDatos>& tensor);
    //friend Tensor operator*<tensorDatos>(const Tensor& tensor1, const Tensor& tensor2); //Esto sería un producto tensorial : Un operador bilineal dando lugar a un nuevo tensor de diferente dimension y covariantes/contravariantes.
    friend void liberarTensor(Tensor<tensorDatos>& tensor);
};

template<typename tensorDatos> 
class CitrusTensor{
private:
    Tensor<tensorDatos> tensor;

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
    void setTensorCompleto(const Tensor<tensorDatos> tensorNuevo);
    void setDatos(const tensorDatos* arregloDatos);
    tensorDatos getDato(const size_t* indices) const;
    const tensorDatos* getDatos() const;
    Tensor<tensorDatos> getTensor() const;

    // Acceso y modificación por índices múltiples (estilo matemático) => Similar a MATLAB.
    template<typename... Indices>
    tensorDatos& operator()(Indices... indices);

    template<typename... Indices>
    const tensorDatos& operator()(Indices... indices) const;

    // Definir operaciones con Tensores : Producto tensorial, suma/resta tensores coherentes, producto/division interelementos de tensores coherentes. 
    void verificarOperabilidad(const Tensor<tensorDatos>& tensor1, const Tensor<tensorDatos>& tensor2) const;
    Tensor<tensorDatos> sumaTensorial(const Tensor<tensorDatos>& tensor1, const Tensor<tensorDatos>& tensor2) const;
    Tensor<tensorDatos> restaTensorial(const Tensor<tensorDatos>& tensor1, const Tensor<tensorDatos>& tensor2) const;
    //Tensor<tensorDatos> productoTensorial(const Tensor<tensorDatos> tensor1,const Tensor<tensorDatos> tensor2) const;
    Tensor<tensorDatos> prodComponetesTensorial(const Tensor<tensorDatos>& tensor1,const Tensor<tensorDatos>& tensor2) const;
    void verificarDivisionTensorial (const Tensor<tensorDatos>& tensor) const;
    Tensor<tensorDatos> divComponentesTensorial(const Tensor<tensorDatos>& tensor1,const Tensor<tensorDatos>& tensor2) const;
    Tensor<tensorDatos> conjugadoTensor(const Tensor<tensorDatos>& tensor) const;
};

#endif
