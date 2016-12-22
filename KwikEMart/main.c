//
//  KwikEMart (Proyecto de ventas)
//
//  Creado por: Emiliano Carrillo Moncayo
//              Luis Fernando Ocejo Piedra
//              Carlos Manuel De Riquer Hernandez
//
//  28/11/16.
//  Copyright © 2016. All rights reserved.
//

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

////////////////////////////////////////////////////////VARIABLES GLOBALES/////////////////////////////////////////////////
int decisionUsuario;
FILE *InventarioFile;
FILE *GananciaDiaFile;

char cosa[50];
float precio;
int cantidad = 0;

struct Articulos{
    char  nombre[50];
    float precio;
    int cantidad;
};

float IngresosTotales;

/////////////////////////////////////////////////////DECLARACIÓN DE LAS FUNCIONES/////////////////////////////////////////
void MostrarItems();
void ComprarItems();
void VenderItems();
float SumarIngresos(float Ganancia);
float RestarIngresos(float Perdida);


int main() {
    
    //ARCHIVOS
                            /* Cambiar la ruta y el modo (segundo argumento de fopen) */
    InventarioFile = fopen("/Users/emilianocarrillo/Desktop/KwikEMart/Inventario.txt", "ab+");
    
    //PRESENTACIÓN
    printf("              B  I  E  N  V  E  N  I  D  O     A  :                     \n");
    printf("  _  __        _ _           ______      __  __            _            \n");
    printf(" | |/ /       (_) |         |  ____|    |  \\/  |          | |          \n");
    printf(" | ' /_      ___| | ________| |__ ______| \\  / | __ _ _ __| |_         \n");
    printf(" |  <\\ \\ /\\ / / | |/ /______|  __|______| |\\/| |/ _` | '__| __|     \n");
    printf(" | . \\\\ V  V /| |   <       | |____     | |  | | (_| | |  | |_        \n");
    printf(" |_|\\_\\\\_/\\_/ |_|_|\\_\\      |______|    |_|  |_|\\__,_|_|   \\__| \n");
    

    //CICLO REPETITIVO HASTA QUE EL USUARIO DECIDA SALIR
    do {
        //MENÚ DESPLEGABLE DE OPCIONES
        printf("\n\nPOR FAVOR, SELECCIONE LA ACCIÓN QUE DESEE REALIZAR.\n");
        printf("\t1. Mostrar lo que está a la venta.\n");
        printf("\t2. Comprar artículos.\n");
        printf("\t3. Vender artículos.\n");
        printf("\t0. SALIR.\n");
        printf("DECISIÓN: ");
        scanf("%d", &decisionUsuario);
        
        //MANEJO DE ACCIONES
        switch (decisionUsuario) {
            case 1:
                MostrarItems();
                break;
            case 2:
                ComprarItems();
                break;
            case 3:
                VenderItems();
                break;
            case 0:
                return 0;
                break;
            default:
                printf("ERROR 42: No existe una acción con el número %d asignado.\n\n", decisionUsuario);
                break;
        }
    } while (decisionUsuario!=0);
    
    return 0;
}


///////////////////////////////////////////////////FUNCIÓNES DE CADA ACCIÓN///////////////////////////////////////////////////

void MostrarItems(){
    
    rewind(InventarioFile);
    
    //Mostrar en pantalla cada item con sus precios y disponibilidad.
    printf("\n\n███████████████████████████████████████████████████████████████\n");
    while(fscanf(InventarioFile,"%s %f %d", cosa, &precio, &cantidad)>0) {
        printf("\t\t\t%d $%.2f %s\n",cantidad, precio, cosa);
    }
     printf("███████████████████████████████████████████████████████████████\n");
    
}

void ComprarItems(){
    
    rewind(InventarioFile);
    
    //Local variables
    struct Articulos articuloDeseado;
    int i = 0;
    int compraExitosa = 0; //0: false; 1:True
    
    printf("\n##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=\n");
    printf("NOMBRE DEL ARTÍCULO DESEADO: ");
    scanf("%s", articuloDeseado.nombre);
    
    //Hacer lo siguiente mientras no haya errores
    do{
        /*Checar si existe e item que quiere en el inventario*/
        while(fscanf(InventarioFile,"%s %f %d", cosa, &precio, &cantidad)>0) {
            
            //Si el item desedo coincide con algun item en el inventario, continuar con la compra
            if (strcmp(cosa, articuloDeseado.nombre) == 0) {
                
                printf("CANTIDAD DE ITEMS QUE DESEA COMPRAR: ");
                scanf("%d", &articuloDeseado.cantidad);
                
                if (articuloDeseado.cantidad > cantidad) {
                    //En caso de que el usuario quiera más items de los que hay disponibles.
                    printf("No hay items suficientes. Tenemos %d %s disponibles.\n", cantidad, cosa);
                    printf("##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=\n");
                    compraExitosa = 2;
                    break;

                } else {
                    //SIN ERRORES
                    printf("¡COMPRA EXITOSA!\n");
                    printf("##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=\n");
                    /*Sumar a ingresos del día*/
                    SumarIngresos(precio*articuloDeseado.cantidad);
                    
                     /*⚠️Restar la cantidad de productos en el inventario⚠️*/
                    char almacenarNombres[50][50];
                    int almacenarCantidades[50];
                    float almacenarPrecios[50];
                    int j = 0;
                    int k = 0;
                    
                    rewind(InventarioFile);
                    while(fscanf(InventarioFile,"%s %f %d", cosa, &precio, &cantidad)>0) {
                        strcpy(almacenarNombres[j], cosa);
                        almacenarCantidades[j] = cantidad;
                        almacenarPrecios[j] = precio;
                        j++;
                    }
                    
                    InventarioFile = fopen("/Users/emilianocarrillo/Desktop/KwikEMart/Inventario.txt", "w");
                    for (k=0; k<j; k++) {
                        if (strcmp(almacenarNombres[k], articuloDeseado.nombre) == 0) {
                            fprintf(InventarioFile,"%s %f %d\n",almacenarNombres[k],almacenarPrecios[k],almacenarCantidades[k]-articuloDeseado.cantidad);
                        } else{
                            fprintf(InventarioFile,"%s %f %d\n",almacenarNombres[k],almacenarPrecios[k],almacenarCantidades[k]);
                        }
                    }
                    fclose(InventarioFile);
                    InventarioFile = fopen("/Users/emilianocarrillo/Desktop/KwikEMart/Inventario.txt", "ab+");
                    
                    compraExitosa = 2;
                    break;
                }
                
            }else{
                compraExitosa = 1;
            }
            i++;
        }
    }while(compraExitosa == 0);
    
    //Si la compra no se realza quiere decir que el item no está a la venta (No está en el inventario)
    if (compraExitosa == 1) {
        printf("ERROR 404: ITEM NO ENCONTRADO.\n");
        printf("##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=##=\n");
    }
}


void VenderItems(){
    
    //Crea una estructura para los artículos nuevos por añadir
    struct Articulos articuloNuevo;
    
    //El usuario ingresa los datos de su venta
    rewind(InventarioFile);
    printf("\n:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("NOMBRE DEL ARTÍCULO QUE NOS DESEA VENDER: ");
    scanf("%s", articuloNuevo.nombre);
    printf("CANTIDAD DE ITEMS QUE NOS VENDERÁ: ");
    scanf("%d", &articuloNuevo.cantidad);
    printf("ESCRIBA EL PRECIO QUE DESEA DARLE A UN SOLO ITEM ($): ");
    scanf("%f", &articuloNuevo.precio);
    
    /*Restar a ingresos del día*/
    if (SumarIngresos(0)< articuloNuevo.cantidad*articuloNuevo.precio) {
        //No se puede realizar
        printf("ERROR 101: No nos alcanza para comprarle el objeto que nos desea vender.\n");
        printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    } else{
        printf("\t\t¡GRACIAS! LO HEMOS AÑADIDO A NUESTRO INVENTARIO\n");
        printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
        //Se escribe en el archivo Inventario.txt el nuevo item
        fprintf(InventarioFile, "\n%s %.2f %d", articuloNuevo.nombre, articuloNuevo.precio, articuloNuevo.cantidad);
        /*Restar a ingresos del día*/
        RestarIngresos(articuloNuevo.precio*articuloNuevo.cantidad);
    
    }
    
}



//////////////////////////////////////////////////FUNCIÓNES VARIAS///////////////////////////////////////////////////
float SumarIngresos(float Ganado){

    //Abre el archivo para ser leído
    GananciaDiaFile = fopen("/Users/emilianocarrillo/Desktop/KwikEMart/GananciaDia.txt", "ab+");
    //Sumarle a las ganancias del día (Lo del archivo) lo ganado (INPUT DE LA FUNCION)
    rewind(GananciaDiaFile);
    fscanf(GananciaDiaFile, "%f", &IngresosTotales);
    IngresosTotales += Ganado;
    
    //Abrir el archivo únicamente para escribir (Borra todo contenido antiguo)
    GananciaDiaFile = fopen("/Users/emilianocarrillo/Desktop/KwikEMart/GananciaDia.txt", "w");
    fprintf(GananciaDiaFile, "%.2f", IngresosTotales);
    fclose(GananciaDiaFile);

    return IngresosTotales;
}


float RestarIngresos(float Perdido){
    
    //Abre el archivo para ser leído
    GananciaDiaFile = fopen("/Users/emilianocarrillo/Desktop/KwikEMart/GananciaDia.txt", "ab+");
    //Sumarle a las ganancias del día (Lo del archivo) lo ganado (INPUT DE LA FUNCION)
    rewind(GananciaDiaFile);
    fscanf(GananciaDiaFile, "%f", &IngresosTotales);
    IngresosTotales -= Perdido;
    
    //Abrir el archivo únicamente para escribir (Borra todo contenido antiguo)
    GananciaDiaFile = fopen("/Users/emilianocarrillo/Desktop/KwikEMart/GananciaDia.txt", "w");
    fprintf(GananciaDiaFile, "%.2f", IngresosTotales);
    fclose(GananciaDiaFile);
    
    return IngresosTotales;
}



































