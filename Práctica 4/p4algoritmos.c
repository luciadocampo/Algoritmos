/*
Nombres: Patricia Mato Miragaya (patricia.mato.miragaya@udc.es)
	     Sara Qing Liébana Rodríguez (s.qing.liebana@udc.es)
         Lucía Docampo Rodríguez (lucia.docampo@udc.es)
Grupo 3.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <limits.h>

#define TAM_MAX 1600

typedef int ** matriz;

typedef struct {
    int x, y, peso;
} arista;

typedef arista tipo_elemento;

typedef struct {
    int cabeza, final, tamano;
    tipo_elemento vector[TAM_MAX];
} cola;

void inicializar_semilla(){
    srand(time(NULL));
/* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}

double microsegundos() {
/* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void crear_cola(cola *);
int cola_vacia(cola);
void insertar(tipo_elemento, cola *);
tipo_elemento quitar_primero(cola *);
tipo_elemento primero(cola);
void mostrar_cola(cola);

void prim(matriz m, int nodos, cola *aristas) {
/* calcular el árbol de recubrimiento mínimo devolviendo
las aristas del arbol en la cola ’aristas’ */
    int min, i, j, k=0, x=0;
    arista a;
    int *masProximo = (int *) malloc(nodos*sizeof(int));
    int *distanciaMinima = (int *) malloc(nodos*sizeof(int));
    crear_cola(aristas);
    distanciaMinima[0] = -1;
    for(i = 1; i < nodos; i++) {
        masProximo[i] = 0;
        distanciaMinima[i] = m[i][0];
    }
    while (x < nodos-1){
        min = INT_MAX;
        for(j = 1; j < nodos; j++){
            if (0 <= distanciaMinima[j] && distanciaMinima[j] < min){
                min = distanciaMinima[j];
                k = j;
            }
        }
        a.peso = min; // T := T ∪ {(MasProximo[k], k)};
        a.x = masProximo[k];
        a.y = k;
        insertar(a, aristas);
        distanciaMinima[k] = -1;
        for(j = 1; j < nodos; j++){
            if (m[j][k] < distanciaMinima[j]){
                distanciaMinima[j] = m[j][k];
                masProximo[j] = k;
            }
        }
        x++;
    }
    free(masProximo);
    free(distanciaMinima);
}

matriz crear_matriz(int n) {
    int i;
    matriz aux;
    if ((aux = malloc(n*sizeof(int *))) == NULL)
        return NULL;
    for (i=0; i<n; i++)
        if ((aux[i] = malloc(n*sizeof(int))) == NULL)
            return NULL;
    return aux;
}

void inicializar_matriz(matriz m, int n) {
/* Crea un grafo completo no dirigido con valores aleatorios entre 1 y n */
    int i, j;
    for (i=0; i < n; i++)
        for (j=i+1; j<n; j++)
            m[i][j] = rand() % n + 1;
    for (i=0; i < n; i++)
        for (j=0; j<=i; j++)
            if (i==j)
                m[i][j] = 0;
            else
                m[i][j] = m[j][i];
}

void liberar_matriz(matriz m, int n) {
    int i;
    for (i=0; i < n; i++)
        free(m[i]);
    free(m);
}


void crear_cola(cola *c){
    c->tamano = 0;
    c->cabeza = 0;
    c->final = -1;
}

int cola_vacia(cola c){
    return (c.tamano == 0);
}

void incrementar(int *x){
    if (++(*x) == TAM_MAX)
        *x = 0;
}

void insertar(tipo_elemento x, cola *c){
    if (c->tamano == TAM_MAX){
        printf("error: cola llena: %d\n", c->tamano);
        exit(EXIT_FAILURE);
    } else{
        c->tamano++;
        incrementar(&(c->final));
        c->vector[c->final] = x;
    }
}

tipo_elemento quitar_primero(cola *c){
    tipo_elemento x;
    if (cola_vacia(*c)) {
        printf("error: cola vacia\n");
        exit(EXIT_FAILURE);
    }
    c->tamano--;
    x = c->vector[c->cabeza];
    incrementar(&(c->cabeza));
    return x;
}

tipo_elemento primero(cola c){
    if(cola_vacia(c)){
        printf("error: cola vacia\n");
        exit(EXIT_FAILURE);
    }
    return (c.vector[c.cabeza]);
}

void mostrar_cola(cola c){
    int i;
    if (cola_vacia(c)){
        printf("error: cola vacia\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < c.tamano; i++){
        printf("(%d,%d)", c.vector[c.cabeza+i].x, c.vector[c.cabeza+i].y);
        if(i != c.tamano-1)
            printf(", ");
    }
    printf("\n");
}

int calculo_peso(cola c){
    int i, pesototal = 0;
    if(cola_vacia(c)){
        pesototal = 0;
    } else {
        for(i = 0; i < c.tamano; i++)
            pesototal += c.vector[c.cabeza+i].peso;
    }
    return pesototal;
}

void aristas_pesos(matriz m, int nodos){
    cola c;

    crear_cola(&c);
    prim(m, nodos, &c);
    printf("\n\n Aristas: ");
    mostrar_cola(c);
    printf(" Peso: %d\n\n", calculo_peso(c));
}

void imprimir_matriz(matriz m, int n){
    int i, j;
    for(i = 0; i < n; i++){
        printf("\n");
        for(j = 0; j < n; j++){
            printf("%4d", m[i][j]);
        }
    }
}

matriz obtener_matriz (matriz mat, int n, const int grafo[]){
    int i, j, k=0;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            mat[i][j] = grafo[k++];
        }
    }
    return mat;
}

void free_matriz(matriz mat, int n){
    int i;

    for(i = 0; i < n; i++){
        free(mat[i]);
    }
    free(mat);
}

void test(){
    int grafoA [25] = {
            0, 1, 8, 4, 7, 1, 0, 2, 6, 5,
            8, 2, 0, 9, 5,4, 6, 9, 0,
            3, 7, 5, 5, 3, 0
    };
    int grafoB [16] = {
            0, 1, 4, 7, 1, 0, 2, 8, 4, 2,
            0, 3, 7, 8, 3, 0
    };
    int grafoC [49] = {
            0, 7, 99, 5, 99, 99, 99, 7, 0,
            8, 9, 7, 99, 99,99, 8, 0,
            99, 5, 99, 99, 5, 9, 99, 0,
            15, 6, 99,99, 7, 5, 15, 0,
            8, 9, 99, 99, 99, 6, 8, 0,
            11,99, 99, 99, 99, 9, 11, 0
    };

    matriz matA = crear_matriz(5);
    matriz matB = crear_matriz(4);
    matriz matC = crear_matriz(7);

    printf("\n Grafo A");
    obtener_matriz(matA, 5, grafoA);
    imprimir_matriz(matA, 5);
    aristas_pesos(matA, 5);

    printf("\n Grafo B");
    obtener_matriz(matB, 4, grafoB);
    imprimir_matriz(matB, 4);
    aristas_pesos(matB, 4);

    printf("\n Grafo C");
    obtener_matriz(matC, 7, grafoC);
    imprimir_matriz(matC, 7);
    aristas_pesos(matC, 7);

    free_matriz(matA, 5);
    free_matriz(matB, 4);
    free_matriz(matC, 7);
}


void tiempos(){
    int n, i, k=1000;
    double t, ti, tf, t1, t2, a, b, c;
    matriz m;
    cola co;
    for(n = 10; n <= 800; n*=2){
        m = crear_matriz(n);
        inicializar_matriz(m, n);
        crear_cola(&co);
        ti = microsegundos();
        prim(m, n, &co);
        tf = microsegundos();
        t = tf-ti;
        if(t < 500){
            printf("(*)");
            ti = microsegundos();
            for(i = 0; i < k; i++){
                crear_cola(&co);
                prim(m, n, &co);
            }
            tf = microsegundos();
            t1 = tf - ti;
            ti = microsegundos();
            for(i = 0; i < k; i++)
                crear_cola(&co);
            tf = microsegundos();
            t2 = tf-ti;
            t = (t1-t2)/k;
        }
        a = t/pow(n, 1.5);
        b = t/pow(n, 2.06);
        c = t/pow(n, 2.2);
        printf("\t%6d %14.3f %13.8f %14.8f %14.8f\n", n, t, a, b, c);
        free_matriz(m, n);
    }
}

int main() {
    int i;

    inicializar_semilla();
    test();
    printf("\n\nALGORITMO DE PRIM\n");
    for(i = 0; i < 4; i++){
        printf("\n\t%5sn\t\t t(n)%3s t(n)/n^1.5%4s t(n)/n^2.06%3s t(n)/n^2.2\n"
                , "", "", "", "");
        tiempos();
        printf("\n%33s n^1.5 es       n^2.06 es      n^2.2 es\n","");
        printf("%31s subestimada      ajustada     sobreestimada\n\n\n", "");
    }

    return 0;
}

