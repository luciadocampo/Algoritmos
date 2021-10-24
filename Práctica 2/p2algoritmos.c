/*
Nombres: Patricia Mato Miragaya (patricia.mato.miragaya@udc.es)
	     Sara Qing Liébana Rodríguez (s.qing.liebana@udc.es)
         Lucía Docampo Rodríguez (lucia.docampo@udc.es)
Grupo 3.1
 */

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

void ord_sel (int v [], int n){
    int i, j, minj, minx;

    for(i = 0; i < n-1; i++){
        minj = i;
        minx = v[i];
        for(j = i+1; j < n; j++){ 
            if(v[j] < minx){
                minj = j;
                minx = v[j];
            }
        }
        v[minj] = v[i];
        v[i] = minx;
    }
}

void ord_shell (int v [], int n){
    int incremento, i, tmp, j;
    bool seguir;
    incremento = n;
    do{
        incremento = incremento/2;
        for(i = incremento; i < n; i++){
            tmp = v[i];
            j = i;
            seguir = true;

            while(j-incremento >= 0 && seguir){
                if(tmp < v[j-incremento]){
                    v[j] = v[j-incremento];
                    j = j-incremento;
                } else
                    seguir = false;
            }
            v[j] = tmp;
        }
    } while (incremento != 1);
}

void inicializar_semilla(){
    srand(time(NULL));
}

void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void listar_vector(int v[], int n){
    int i;
    for(i = 0; i < n; i++){
        printf("%d", v[i]);
        if(i != n-1)
            printf(", ");
    }

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

void test(void algoritmo(int v[], int n), char *metodo) {
    int n = 17, a = 10, v[n];

    printf("Inicialización aleatoria\n");
    aleatorio(v, n);
    listar_vector(v, n);
    printf("\nordenado? %d\n\n", ordenado(v, n));

    printf("Ordenación por %s\n", metodo);
    algoritmo(v, n);
    listar_vector(v, n);
    printf("\nordenado? %d\n\n", ordenado(v, n));

    printf("Inicialización descendente\n");
    descendente(v, a);
    listar_vector(v, a);
    printf("\nordenado? %d\n\n", ordenado(v, a));

    printf("Ordenación por %s\n", metodo);
    algoritmo(v, a);
    listar_vector(v, a);
    printf("\nordenado? %d\n\n", ordenado(v, a));

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
        a = t/pow(n, subestimada);
        b = t/pow(n, ajustada);
        c = t/pow(n, sobrestimada);
        printf("\t%6d %15.3f %13.8f %13.8f %13.8f\n", n, t, a, b, c);

    }

}

void tablas(void (*algoritmo)(int v[], int n), void (*inicializar)
      (int v[], int n), float subestimada, float ajustada, float sobrestimada){
    int i;
    
    for (i = 0; i<2; i++){
        printf("\n\t    n\t\tt(n)      t(n)/n^%.1f    t(n)/n^%.1f    t(n)/n^%.1f\n",
           subestimada, ajustada, sobrestimada);
        tiempos(algoritmo, inicializar, subestimada, ajustada, sobrestimada);
        printf("\n%34s n^%.1f es      n^%.1f es      n^%.1f es\n","", subestimada,
           ajustada, sobrestimada);
        printf("%33s subestimada    ajustada    sobreestimada\n\n\n", "");
    }
}



int main() {
    inicializar_semilla();
    printf("\nTEST ORDENACION POR SELECCION\n");
    test(ord_sel, "Seleccion");
    printf("\n\n");
    printf("TEST ORDENACION SHELL\n");
    test(ord_shell, "Shell");
    printf("\n\n");

    printf("Ordenación por selección con inicialización aleatoria\n");
    tablas(ord_sel, aleatorio, 1.8, 2, 2.2);
    printf("Ordenación por selección con inicialización descendente\n");
    tablas(ord_sel, descendente, 1.8, 2, 2.2);
    printf("Ordenación por selección con inicialización ascendente\n");
    tablas(ord_sel, ascendente, 1.8, 2, 2.2);
    printf("Ordenación por shell con inicialización aleatoria\n");
    tablas(ord_shell, aleatorio, 1.1, 1.2, 1.3);
    printf("Ordenación por shell con inicialización descendente\n");
    tablas(ord_shell, descendente, 0.9, 1.1, 1.3);
    printf("Ordenación por shell con inicialización ascendente\n");
    tablas(ord_shell, ascendente, 0.9, 1.1, 1.2);
    
    return 0;
}

