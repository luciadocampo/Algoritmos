/*
Grupo 3.1

Patricia Mato Miragaya
Sara Qing Liébana Rodríguez
Lucía Docampo Rodríguez
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

int sumaSubMax1(int v[], int n) {
    int i, j;
    int estaSuma, sumaMax = 0;
    for (i = 0; i < n; i++) {
        estaSuma = 0;
        for (j = i; j < n; j++) {
            estaSuma += v[j];
            if (estaSuma > sumaMax) {
                sumaMax = estaSuma;
            }
        }
    }
    return sumaMax;
}

int sumaSubMax2(int v[], int n) {
    int j;
    int estaSuma = 0, sumaMax = 0;
    for(j = 0; j < n; j++){
        estaSuma += v[j];
        if(estaSuma > sumaMax)
            sumaMax = estaSuma;
        else if(estaSuma < 0)
            estaSuma = 0;

    } return sumaMax;
}

void listar_vector(int v[], int n){
    int i;
    for(i=0; i<n; i++)
        printf("%d\t ", v[i]);
}

void test1() {
    int a, b, i;
    int v[] [5]= {
            {-9, 2,  -5, -4, 6},
            {4,  0,  9,  2,  5},
            {-2, -1, -9, -7, -1},
            {9,  -2, 1,  -7, -8},
            {15, -2, -5, -4, 16},
            {7,  -5, 6,  7,  -7}
    };

    printf("\n\n\t test 1\n");
    printf("%46s%15s%15s\n", "", "maxSubSum1", "maxSubSum2");

    for (i=0; i<=5; i++) {
        listar_vector(v[i], 5);
        a = sumaSubMax1(v[i], 5);
        b = sumaSubMax2(v[i], 5);
        printf("%15d%15d\n", a, b);
    }
}

void inicializar_semilla(){
    srand(time(NULL));
/* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}

void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
/* con 'rand' se generan números pseudoaleatorio entre -n y +n */
}

void test2() {
    int i, a, b;
    int v[9];
    printf("\n\n\n            test2\n");
    printf("%78s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
    for (i=0; i<10; i++) {
        aleatorio(v, 9);
        listar_vector(v, 9);
        a = sumaSubMax1(v, 9);
        b = sumaSubMax2(v, 9);
        printf("%15d%15d\n", a, b);
    }
}

double microsegundos() {
/* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

double tiempoEjecucion1(int v[], int n){
    double ti, tf, t1;

    ti = microsegundos();
    sumaSubMax1(v, n);
    tf = microsegundos();
    t1 = tf - ti;
    return t1;
}

double tiempoEjecucion2(int v[], int n){
    double ti, tf, t2;

    ti = microsegundos();
    sumaSubMax2(v, n);
    tf = microsegundos();
    t2 = tf-ti;
    return t2;
}

void cotas1(int n){
    int v[n], i, k=10000;
    double t, y=0, a, b, c;

    inicializar_semilla();
    for (i=0; i<n; i++) {
        aleatorio(v, n);
    }
    t = tiempoEjecucion1(v, n);

    if(t < 500){        //umbral de confianza
        for(i=0;i<k;i++){
            aleatorio(v, n);
            y= tiempoEjecucion1(v, n);
            t += y;
        }
        printf("(*)");
        t=t/k;
    }

    a= t/pow(n, 1.8);
    b = t/pow(n,2);
    c = t/pow(n,2.2);
    printf("%6d %15.3f %13.8f %13.8f %13.8f\n", n, t, a, b, c);
}

void cotas2(int n){
    int v[n], i, k=10000;
    double y=0, t, a, b, c;

    inicializar_semilla();
    t = tiempoEjecucion2(v, n);

    if(t < 500){         //umbral de confianza
        for(i=0;i<k;i++){
            aleatorio(v, n);
            y= tiempoEjecucion2(v, n);
            t += y;
        }
        printf("(*)");
        t=t/k;
    }
        a= t/pow(n, 0.8);
        b = t/n;
        c = t/pow(n,1.2);
        printf("%6d %15.5f %13.8f %13.8f %13.8f    \n", n, t, a, b, c);
}

int main() {
    int n = 500;
    inicializar_semilla();
    test1();
    test2();
    printf("\n\n  Tiempos de ejecucion sumaSubMax1\n");
    printf("\n     n\t\tt(n)      t(n)/n^1.8     t(n)/n^2     t(n)/n^2.2\t\n");
    while(n <= 32000){
        printf("\n");
        cotas1(n);
        n = n*2;
    }
    printf("\n\t\t\t  n^1.8 es       n^2 es       n^2.2 es\n");
    printf("\t\t\t subestimada     ajustada    sobreestimada\n\n");
    printf("(*) corresponde a un tiempo promedio de 10000 ejecuciones"
           " del algoritmo\n\n");

    n = 500;
    printf("\n\n  Tiempos de ejecucion sumaSubMax2\n");
    printf("\n    n\t\t   t(n)       t(n)/n^0.8     t(n)/n\tt(n)/n^1.2\n");
    while(n <= 32000){
        printf("\n");
        cotas2(n);
        n = n*2;
    }
    printf("\n\t\t\t      n^1.8 es       n^2 es       n^2.2 es\n");
    printf("\t\t\t    subestimada     ajustada    sobreestimada\n\n");
    printf("(*) corresponde a un tiempo promedio de 10000 ejecuciones"
           " del algoritmo\n\n");
    return 0;
}