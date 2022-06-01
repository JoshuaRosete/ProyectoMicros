#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "inc/tm4c1294ncpdt.h"
#include "elevador.h"

#define dos_segundos 1000000 //Calcular para dos segundos


int piso;
int pisoActual;
int espera = 0;

int main()
{
    //Se activan los reloj para cada puerto
    SYSCTL_RCGCGPIO_R = 0b111000010011;
    while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R0) == 0){};


    //Se configuran los pines para las salidas hacia el teclado.
    GPIO_PORTE_AHB_DIR_R = 0b1111;
    GPIO_PORTE_AHB_DEN_R = 0b1111;

    //Se configuran los pines para las entradas desde el teclado
    GPIO_PORTL_DIR_R = 0;
    GPIO_PORTL_DEN_R = 0b1111;

    //Se configura los pines para la salida hacia el motor
    GPIO_PORTK_DIR_R = 0b1111;
    GPIO_PORTK_DEN_R = 0b1111;

    //Se configuran los pines para la salida hacia los LEDS
    GPIO_PORTA_AHB_DIR_R = 0b1111;
    GPIO_PORTA_AHB_DEN_R = 0b1111;

    pisoActual = 0;

    while(1)
    {
        piso = 12;
        piso = atoi(Bluetooh_Read());
        //piso = leerTeclado();
        if(piso == 12)
        {
            piso = leerTeclado();
        }

        abierto();
        switch(piso)
        {
            case 0:
                   Delay_MicroSecond(dos_segundos);
                   servoPuerta(leerTeclado());
                   irPiso(0, pisoActual);
                   ledPiso(0);
                   servoPuerta(leerTeclado());
                   pisoActual = 0;
                   break;

            case 1:
                Delay_MicroSecond(dos_segundos); //Calcular para 2 segundos
                   servoPuerta(leerTeclado());
                   irPiso(1, pisoActual);
                   ledPiso(1);
                   servoPuerta(leerTeclado());
                   pisoActual = 1;
                   break;

            case 2:
                Delay_MicroSecond(dos_segundos); //Calcular para 2 segundos
                   servoPuerta(leerTeclado());
                   irPiso(2, pisoActual);
                   ledPiso(2);
                   servoPuerta(leerTeclado());
                   pisoActual = 2;
                   break;

            case 3:
                Delay_MicroSecond(dos_segundos); //Calcular para 2 segundos
                   servoPuerta(leerTeclado());
                   irPiso(3, pisoActual);
                   ledPiso(3);
                   servoPuerta(leerTeclado());
                   pisoActual = 3;
                   break;

            case 4:
                   espera = 1;
                   pisoActual = ultimoPiso(verificarContrasena(),pisoActual);
                   espera = 0;
                   break;

            default:
                    ledPiso(pisoActual);
                    break;
            }
    }

    return 0;
}
