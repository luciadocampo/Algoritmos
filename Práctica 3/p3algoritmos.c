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

#define UMBRAL 1

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

void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
/* con 'rand' se generan números pseudoaleatorio entre -n y +n */
}

void listar_vector(int v[], int n){
    int i;
    for(i = 0; i < n; i++){
        printf("%d", v[i]);
        if(i != n-1)
            printf(", ");
    }

}

void ascendente(int v [], int n) {
    int i;
    for (i = 0; i < n; i++)
        v[i] = i;
}

void descendente(int v [], int n) {
    int i;
    for (i = n-1; i >= 0; i--)
        v[i] = n - i;
}

int ordenado(const int v[], int n){
    int i, orden;

    for(i = 0; i < n-1; i++){
        if(v[i] > v[i+1]){
            orden = 0;
            break;
        } else{
            orden = 1;
        }
    }
    return orden;
}


void ord_ins (int v [], int n) {
    int i, j, x;
    for(i = 1; i<n; i++) {
        x = v[i];
        j = i - 1;
        while (j>=0 && v[j]>=x) {
            v[j+1] = v[j];
            j = j-1;
        }
        v[j+1] = x;
    }
}


void intercambiar(int *x, int *y){
    int aux;
    aux = *x;
    *x = *y;
    *y = aux;
}

void mediana3(int v[], int i, int j){
    int k;

    k = (i + j) / 2;
    if(v[k] > v[j])
        intercambiar(&v[k], &v[j]);
    if(v[k] > v[i])
        intercambiar(&v[k], &v[i]);
    if(v[i] > v[j])
        intercambiar(&v[i], &v[j]);
}

void ordenarAux (int v[], int izq, int der) {
    int i, j, pivote;
    if ((izq+UMBRAL) <= der) { /* UMBRAL >= 1 */
        mediana3(v, izq, der);
        pivote = v[izq];
        i = izq;
        j = der;

        do {
            do{
                i += 1;
            } while (v[i] < pivote);
            do{
                j -= 1;
            } while (v[j] > pivote);
            intercambiar(&v[i], &v[j]);
        } while (j > i);
        intercambiar(&v[i], &v[j]);
        intercambiar(&v[izq], &v[j]);
        ordenarAux(v, izq, j-1);
        ordenarAux(v, j+1, der);
    }
}


void ord_rapida(int v [],int n) {
    ordenarAux (v, 0, n-1);
    if (UMBRAL > 1)
        ord_ins(v, n);
}

void test(void algoritmo(int v[], int n), char *metodo) {
    int n = 17, a = 10, v[n];

    printf("Ordenacion por %s con inicializacion aleatoria\n", metodo);
    aleatorio(v, n);
    listar_vector(v, n);
    printf("\nordenado? %d\nordenando...\n", ordenado(v, n));
    algoritmo(v, n);
    listar_vector(v, n);
    printf("\nesta ordenado\n\n");

    printf("Ordenacion %s con inicializacion descendente\n", metodo);
    descendente(v, a);
    listar_vector(v, a);
    printf("\nordenado? %d\nordenando...\n", ordenado(v, a));
    algoritmo(v, a);
    listar_vector(v, a);
    printf("\nesta ordenado\n");

}

void tiempos(void (*algoritmo)(int v[], int n), void (*inicializar)
     (int v[], int n), float subestimada, float ajustada, float sobrestimada){
    int n, v[32000], i, k=1000;
    double t, ti, tf, t1, t2, a, b, c;

    for(n = 500; n <= 32000; n*=2){
        inicializar(v, n);
        ti = microsegundos();
        algoritmo(v, n);
        tf = microsegundos();
        t = tf-ti;
        if(t < 500){
            printf("(*)");
            ti = microsegundos();
            for(i = 0; i < k; i++){
                inicializar(v, n);
                algoritmo(v, n);
            }
            tf = microsegundos();
            t1 = tf - ti;
            ti = microsegundos();
            for(i = 0; i < k; i++){
                inicializar(v, n);
            }
            tf = microsegundos();
            t2 = tf-ti;
            t = (t1-t2)/k;
        }
        if (algoritmo == ord_ins){
            a = t/pow(n, subestimada);
            b = t/pow(n, ajustada);
            c = t/pow(n, sobrestimada);
            printf("\t%6d %15.3f %13.8f %13.8f %13.8f\n", n, t, a, b, c);
        } else if (algoritmo == ord_rapida){
            a = t/((pow(n, subestimada))*log(n));
            b = t/((pow(n, ajustada))*log(n));
            c = t/((pow(n, sobrestimada))*log(n));
            printf("\t%6d %15.3f %16.8f %16.8f %17.8f\n", n, t, a, b, c);
        }
    }
}

void tablas(void (*algoritmo)(int v[], int n), void (*inicializar)
     (int v[], int n), float subestimada, float ajustada, float sobrestimada){
    //int i;
    //for (i = 0; i<2; i++){
    if(algoritmo == ord_ins){
        printf("\n\t    n\t\tt(n)\t  t(n)/n^%.1f\tt(n)/n^%.1f    t(n)/n^%.1f\n"
               ,subestimada, ajustada, sobrestimada);
        tiempos(algoritmo, inicializar, subestimada, ajustada, sobrestimada);
        printf("\n%34s n^%.1f es      n^%.1f es      n^%.1f es\n","", 
        		  subestimada, ajustada, sobrestimada);
        printf("%33s subestimada    ajustada    sobreestimada\n\n\n", "");
    } else if(algoritmo == ord_rapida){
        printf("\n\t    n\t\tt(n)%11st(n)/%12st(n)/%13st(n)/\n","","","");
        printf("%35s(n^%.1f*log(n))   (n^%.2f*log(n))   (n^%.1f*log(n))\n"
               , "", subestimada, ajustada, sobrestimada);
        tiempos(algoritmo, inicializar, subestimada, ajustada, sobrestimada);
        printf("\n%35s(n^%.1f)*log(n)%3s(n^%.2f)*log(n)%4s(n^%.1f)*log(n)\n"
               , "", subestimada, "", ajustada, "", sobrestimada);
        printf("%35ses subestimada%5ses ajustada%5ses sobreestimada\n\n\n",
               "", "", "");
    }
    //}
}

int main() {
    inicializar_semilla();
    printf("\nTEST ORDENACION POR INSERCIÓN\n");
    test(ord_ins, "inserción");
    printf("\n\n");
    printf("TEST ORDENACION RÁPIDA\n");
    test(ord_rapida, "rápida");
    printf("\n\n");

    printf("ORDENACIÓN POR INSERCIÓN\n");
    printf("Ordenación por inserción con inicialización aleatoria\n");
    tablas(ord_ins, aleatorio, 1.8, 1.99, 2.2);
    printf("Ordenación por inserción con inicialización descendente\n");
    tablas(ord_ins, descendente, 1.8, 2, 2.2);
    printf("Ordenación por inserción con inicialización ascendente\n");
    tablas(ord_ins, ascendente, 0.8, 0.99, 1.2);

    printf("\n\nORDENACIÓN RÁPIDA\n");
    printf("Ordenación rápida con inicialización aleatoria\n");
    printf("UMBRAL: %d", UMBRAL);
    tablas(ord_rapida, aleatorio, 0.8, 0.98, 1.2);
    printf("Ordenación rápida con inicialización descendente\n");
    tablas(ord_rapida, descendente, 0.8, 0.96, 1.2);
    printf("Ordenación rápida con inicialización ascendente\n");
    tablas(ord_rapida, ascendente, 0.8, 0.97, 1.2);

    return 0;
}








