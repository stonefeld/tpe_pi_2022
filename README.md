# Trabajo Práctico Especial - PI 2022

## Tabla de contenidos

* [Objetivo](#objetivo)
* [Descripción](#descripción)
* [Compilación](#compilación)
* [Funcionamiento](#funcionamiento)
* [Extendiendo las queries](#extendiendo-las-queries)
* [Un extra](#un-extra)

## Objetivo

El objetivo del proyecto es el diseño e implementación de un **programa de procesamiento de peatones**, realizando un programa completo que incluya _front-end_ y _back-end_, utilizando las herramientas aprendidas a lo largo de la cursada.

## Descripción

El programa consiste en el "parseo" o procesamiento de una serie de archivos de texto de tipo `.csv`, con información con respecto a los sensores disponibles y las lecturas de cada sensor a lo largo del tiempo.

Ambos archivos deben ser procesados y su información debe ser utilizada para realizar una serie de **queries** preestablecidas.

El proyecto fue diseñado con una estructura tal que facilite la implementación de nuevos **queries** o la utilización del mismo "motor" de **queries** otro tipo de _front-ends_.

## Compilación

Para automatizar los procesos de compilación del proyecto se implementó un `Makefile` con los siguientes targets de ejecución:

* `build`: compila el proyecto en un ejecutable llamado `pedestrians` ubicado dentro de la carpeta `build`.
* `run`: ejecuta el programa compilado, sin embargo, este target de ejecución no es de gran utilidad pues el programa fue diseñado para recibir parámetros por línea de comandos.
* `clean`: elimina los object files y el ejecutable generado.

A su vez se dispone de una variable pasada como parametro llamada `DBG`, la cuál al establecer su valor en `1`, añade información de debugueo al momento de compilar.

Por lo tanto para poder compilar el programa se puede correr el siguiente comando:

```sh
make clean build
# O si se quiere informacion de debugueo
make clean build DBG=1
```

Luego de completada la compilación el programa puede ejecutarse de la siguiente manera:

```sh
build/pedestrians readings.csv sensors.csv
```

Donde de forma obligatoria, el primer archivo, debe contener la información de las lecturas realizadas y el segundo la información de cada uno de los sensores. Caso contrario el programa cerrará.

## Funcionamiento

Al ejecutar el programa con los parametros correspondientes, el mismo realizará un parseo de todos los datos de los archivos correspondientes y realizará una serie de **queries** cuyos resultados serán almacenados en archivos con nombres acordes.

El motor de queries puede ser utilizado de dos manera diferentes:

1. A través de la librería encapsuladora `readings`.
2. Llamando **query** por **query**, a cada una de sus funciones respectivas.

La librería encapsuladora permite definir que **queries** se desea procesar desde su declaración y luego al llamar a la función `readings_add` para agregar un dato, la misma agrega la información y la procesa para cada **query** que fue definido previamente.

```c
Readings r = readings_new((unsigned int[]){ 1, 2, 3 }, 3);
```

Donde `readings_new` recibe dos parámetros, un vector con los **queries** que desea procesar (cada elemento del vector indica el número correspondiente al query) y la cantidad de **queries** que se solicita procesar.

Sin embargo, el motor fue diseñado para que, en caso de requerirlo o desearlo, se pueda acceder a las queries de forma directa.

```c
Query1 q1 = query1_new();
Query2 q2 = query2_new();
Query3 q3 = query3_new();
```

Además el motor fue diseñado para que desde el _front_end_, no se pueda acceder ni modificar los registros de los **queries** por lo que si se quiere obtener los resultados de los **queries** solicitados, existe la función `readings_get_matrix` que devuelve una matriz de strings, además de sus filas y columnas para poder trabajar a partir de ahí con los datos de manera genérica.

En el _front_end_ de este proyecto particular, utilizando la matriz devuelta por el _back-end_ se diseño la función que permite "dumpear" o depositar los datos obtenidos de la misma matriz en archivos de tipo `.csv` correspondientes.

## Extendiendo las queries

El motor de **queries** fue diseñado de manera tal que extender el _back-end_ a una mayor cantidad de **queries** sea muy simple.

Como primer instancia cada **query** nueva que desee agregarse al motor, deberá basarse en la estructura de los ya existentes a modo de template. Es decir, cada **query** nueva deberá contar con las siguientes funciones:

```c
static int _compare(struct queryn *e1, struct queryn *e2);
static void _ifequal(struct queryn *e1, struct queryn *e2);
static ErrorCodes _tostring(char ***ret, struct queryn *q);
Queryn queryn_new(void);
ErrorCodes queryn_add(...);
ErrorCodes queryn_tomatrix(Queryn self, Matrix *mat, unsigned int *rows, unsigned int *cols);
void queryn_free(Queryn self);

// En caso de requerir un orden utilizando la funcion `list_order`, se necesita de una funcion
// que determine el orden que se espera luego de su ejecucion
static int _orderby(struct queryn *e1, struct queryn *e2);
```

Además de eso, se requiere de un tipo de dato que sea enviado a `list_add` como `void*` para su almacenamiento. Esta estructura dependerá de las columnas que se espera que la **query** contenga una vez finalizado el procesamiento.

Finalmente, al utilizar la librería encapsuladora de `readings`, es necesario agregar a las respectivas funciones, el case apropiado en cada caso, como ejemplo en `readings_new` es necesario agregar el siguiente case si se agregase `Query100`.

```c
switch (queries[i]) {
  case 1: self->query1 = query1_new(); break;
  case 2: self->query2 = query2_new(); break;
  case 3: self->query3 = query3_new(); break;
  case 100: self->query100 = query100_new(); break;
}
```

Al igual que su respectivo campo dentro del `TAD`.

```c
struct readings_adt {
  Query1 query1;
  Query2 query2;
  Query3 query3;
  Query100 query100;
  unsigned int *queries;
  unsigned int count;
};
```

## Un extra

Dentro del archivo `include/config.h` existe una constante simbólica comentada con el siguiente define:

```c
#define TRACE
```

Al descomentar esta línea y compilar el proyecto con información de debugueo, al correr el programa se imprímirán mensajes adicionales con respecto al estado del programa a medida que trabaja.
