// directivas del preprocesador 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// macros para el preprocesador
#define OK 0
#define ERROR -1
#define FALSE 0
#define TRUE 1
#define FULL 1
#define MAX_PILA 1000
#define MAX_CADENA 100

// declaracion de variables 
int i;
int cant_operandos;
int superior = -1;
int indi_posfi = -1;
char pila[MAX_PILA];
char posfijo[MAX_PILA];
int cadena[MAX_CADENA];
int charcount;
int prioridad_operadores[3][8] = {{94, 42, 47, 43, 45, 40, 91, 123},        // codigo ascii de los operadores
                                  {4, 2, 2, 1, 1, 4, 4, 4},                 // prioridad de entrada
                                  {3, 2, 2, 1, 1, 0, 0, 0}};                // prioridad en pila

/* declaracion de funciones */
int vacio(int *posicion);                                   // declaro al funcion vacio
char extraer(char *temp, int *posicion);                    // declaro la funcion exteaer para el tratamiento de pilas
int insertar(char caracter, char *temp, int *posicion);     // declaro la funcion insertar para el tratamiento de pilas
int validar_caracteres(int cadena[], int temp);             // declaro la funcion para validar los caratereres de la formula
int vali_llave_corche_paren(int cadena[]);                  // declaro la funcion para validar que el orden de los caracteres agrupadores sea correto
void infijo_posfijo(int cadena[]);                          // declaro la fucnion para convertir de la expresion infijo a posfijo
int po(char caracter, int fil);                             // declaro al función para extraer la precedencia de operador tanto en la entrada como en la pila
float calcular_posfijo(int cant);                                   // declaro la función para calcular el posfijo
int posfijo_operando(int caracter);

// definicion de la funcion main
int main(int argc, char const *argv[]) {
    int caracter_invalido;                          // constante para almacenar el primer caracter invalido detectado en la formula                       
    int option, clean;                              // declaro la variable para almacenar el valor de la opcion indicada por el usuario
    int c;

    system("clear");                                // limpio la consola en linux
    printf("Bienvenidos, este programa calcula una ecuacion generando como resultado el valor de los operadores sobre los operandos ingresados.\n");
    printf("Puede ingresar valores y los simbolos {}, [] y () para modificar la presedencia de los operadores.\n");
    printf("Puede utilizar los siguientes operadores: +, -, / y ^ \n");
    printf("La ecucion debe tener una estructura similar a: (x + y) * j, donde y, j y x deben ser valores enteros y/o decimales.\n");
    printf("\n");

    do {
        printf("Menu de opciones: \n");
        printf("1) Presione 1 para calcular y mostrar el resultado de la ecuacion.\n");
        printf("2) Presione 2 para calcular y mostrar el posfijo y el resultado de la ecuacion.\n");
        printf("3) Precione 0 para salir del sistema.\n");
        printf ("Opcion: ");

        option = getchar();

        while ((clean = getchar()) !=  EOF && clean != '\n')            // limpiando el buffer
            ;

        switch (option) {
            case '1':
                printf("Ingrese la formula a calcular: ");
                
                for (i = 0; (c = getchar()) != EOF && c != '\n'; i++) {
                    cadena[i] = c;
                    charcount = i;
                }
                cadena[i+1] = '\0';

                if (!validar_caracteres(cadena, caracter_invalido)) {
                    if (!vali_llave_corche_paren(cadena)) {
                        infijo_posfijo(cadena);
                        printf("El resultado de la ecuacion es: %.2f\n\n", calcular_posfijo(cant_operandos));
                    }
                    else {
                        superior = -1;
                        printf("El orden de los {}, [] y () es invalido\n\n");
                    }
                }
                else
                    printf("La formula contiene el siguiente caracter invalido: %c\n\n", caracter_invalido);
                break;
            case '2':
                printf("Ingrese la formula a calcular: ");
                
                for (i = 0; (c = getchar()) != EOF && c != '\n'; i++) {
                    cadena[i] = c;
                    charcount = i;
                }
                cadena[i+1] = '\0';

                if (!validar_caracteres(cadena, caracter_invalido)) {
                    if (!vali_llave_corche_paren(cadena)) {
                        infijo_posfijo(cadena);
                        printf("La expresion en notacion posfijo es: %s\n", posfijo);
                        printf("El resultado de la ecuacion es: %.2f\n\n", calcular_posfijo(cant_operandos));
                    }
                    else {
                        superior = -1;
                        printf("El orden de los {}, [] y () es invalido\n\n");
                    }
                }
                else
                    printf("La formula contiene el siguiente caracter invalido: %c\n\n", caracter_invalido);
                break;
            case '0':
                break;
            default:
                printf("Opcion invalida, vuelva a intentarlo!\n\n");
                option = '3';
        }
    } while(option != '0');

    puts("Pulse enter para continuar");
    getchar();                              // pauso el flujo de la aplicacion 
return 0;                                   // finalizo la ejecucion
}

// definicion de funciones
int vacio(int *posicion) {              // funcion vacio
    if (*posicion < 0)
        return TRUE;

    return FALSE;
}

char extraer(char *temp, int *posicion) {               // funcion extraer caracter de la pila 
    return temp[(*posicion)--];                         // extaer extrae un valor de la pila y disminuye el contador en 1
}

int insertar(char caracter, char *temp, int *posicion) {           // funcion insertar caracter en la pila
    if ((*posicion + 1) >= MAX_PILA)
        return FULL;

    temp[++(*posicion)] = caracter;
    return OK;
}

int validar_caracteres(int cadena[], int temp) {                    // funcion para validar que los caracteres tipeados por el usuario sean validos
    for (i = 0; cadena[i] != '\0'; i++) {
        switch (cadena[i]) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '.':
            case ' ':
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case '{':
            case '[':
            case '(':
            case ')':
            case ']':
            case '}':
                break;
            default:
                temp = cadena[i];
                return ERROR;
        }
    }
    return OK;
}

int vali_llave_corche_paren(int cadena[]) {          // fucnion para validar que las llaves, cohorchetes y parentesis esten en el orden correcto
    for (i = 0; cadena[i] != '\0'; i++) {
        switch (cadena[i]) {
            case '{':
            case '[':
            case '(':
                if (insertar(cadena[i], pila, &superior))
                    return ERROR;
                break;
            case ')':
                if (vacio(&superior) || extraer(pila, &superior) != '(')
                    return ERROR;
                break;
            case ']':
                if (vacio(&superior) || extraer(pila, &superior) != '[')
                    return ERROR;
                break;
            case '}':
                if (vacio(&superior) || extraer(pila, &superior) != '{')
                    return ERROR;
                break;
            default:
                break;
        }
    }

    if (vacio(&superior))
        return OK;

    return ERROR;
}

void infijo_posfijo(int cadena[]) {             // funcion para convertir una operacion en su forma infija a su forma sufija
    int flat_is_operan = 0;
    char temp;
    cant_operandos = 0;

    for (i = 0; cadena[i] != '\0'; i++) {
        switch (cadena[i]) {
            case '^':
            case '*':
            case '/':
            case '+':
            case '-':
            case '{':
            case '[':
            case '(':
                if (flat_is_operan)
                    insertar('|', posfijo, &indi_posfi);

                while (!vacio(&superior) && (po(pila[superior], 2) >= po(cadena[i], 1)))
                    insertar(extraer(pila, &superior), posfijo, &indi_posfi);

                insertar(cadena[i], pila, &superior);
                flat_is_operan = 0;
                break;
            case '}':
                if (flat_is_operan)
                    insertar('|', posfijo, &indi_posfi);
                while ((temp = extraer(pila, &superior)) != '{')
                    insertar(temp, posfijo, &indi_posfi);
                flat_is_operan = 0;
                break;
            case ']':
                if (flat_is_operan)
                    insertar('|', posfijo, &indi_posfi);
                while ((temp = extraer(pila, &superior)) != '[')
                    insertar(temp, posfijo, &indi_posfi);
                flat_is_operan = 0;
                break;
            case ')':
                if (flat_is_operan)
                    insertar('|', posfijo, &indi_posfi);
                while ((temp = extraer(pila, &superior)) != '(')
                    insertar(temp, posfijo, &indi_posfi);
                flat_is_operan = 0;
                break;
            default:
                if (cadena[i] == ' ')
                    break;

                insertar(cadena[i], posfijo, &indi_posfi);
                cant_operandos++;
                flat_is_operan = 1;
        }
    }
    while (!vacio(&superior))
        insertar(extraer(pila, &superior), posfijo, &indi_posfi);
    return;
}

int po(char caracter, int fil) {
    int j;
    int temp;

    temp = (int) caracter;

    for (j = 0; j < 8; j++) {
        if (temp == prioridad_operadores[0][j])
            return prioridad_operadores[fil][j];
    }
    return -1;
}

float calcular_posfijo(int cant) {
    double num1, num2;
    double operandos[cant];
    int indi_operan = -1;
    int indi_str_operando = -1;
    char str_operando[MAX_CADENA];
    float result;

    for (i = 0; posfijo[i] != '\0'; i++) {
        if (posfijo_operando(posfijo[i]) == TRUE) {
            str_operando[++indi_str_operando] = posfijo[i];
            str_operando[(indi_str_operando + 1)] = '\0';
        }
        else{
            if (indi_str_operando != -1) {
                operandos[++indi_operan] = atof(str_operando);
                indi_str_operando = -1;
            }
            if (posfijo[i] != '|') {
                num2 = operandos[indi_operan--];
                num1 = operandos[indi_operan];
                switch (posfijo[i]) {
                    case '^': 
                        operandos[indi_operan] = pow(num1, num2);
                        break;
                    case '*':
                        operandos[indi_operan] = num1 * num2;
                        break;
                    case '/':
                        if (num2 != 0)
                            operandos[indi_operan] = num1 / num2;
                        else {
                            printf("Los divisores de la función debe ser diferente de cero.\n");
                            exit(-1);
                        }
                        break;
                    case '+':
                        operandos[indi_operan] = num1 + num2;
                        break;
                    case '-':
                        operandos[indi_operan] = num1 - num2;
                        break;
                }
            }
        }
    }
    return operandos[indi_operan];
}

int posfijo_operando(int caracter) {
    switch (caracter) {
        case '^':
        case '*':
        case '/':
        case '+':
        case '-':
        case '|':
            return FALSE;
            break;
        default:
            return TRUE;
    }
}
