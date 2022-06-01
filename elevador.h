
#ifndef ELEVADOR_H_
#define ELEVADOR_H_

#include <math.h>
#include "inc/tm4c1294ncpdt.h"

int leerTeclado(void);
void irPiso(int piso, int pisoActual);
void ledPisos(int piso);
void servoPuerta(int seg);
int verificarContrasena(void);
int ultimoPiso(int bandera,int pisoActual);
void abierto(void);
void cerrado(void);
void Delay_MicroSecond(int time);


int leerTeclado()
{
    int lect = 0;

    for(;;)
    {
        GPIO_PORTE_AHB_DATA_R = 0b1000;
        lect = GPIO_PORTL_DATA_R;
            if (lect == 1)
            {
                return 1;
            }

            if (lect == 2)
            {
                return 2;
            }

            if (lect == 4)
            {
                return 3;
            }

            GPIO_PORTE_AHB_DATA_R = 0b0100;
            lect = GPIO_PORTL_DATA_R;
            if (lect == 1)
            {
                return 4;
            }

            if (lect == 2)
            {
                return 5;
            }

            if (lect == 4)
            {
                return 6;
            }

            GPIO_PORTE_AHB_DATA_R = 0b0010;
            lect = GPIO_PORTL_DATA_R;
            if (lect == 1)
            {
                return 7;
            }

            if (lect == 2)
            {
                return 8;
            }

            if (lect == 4)
            {
                return 9;
            }

            GPIO_PORTE_AHB_DATA_R = 0b0001;
            lect = GPIO_PORTL_DATA_R;
            if (lect == 1)
            {
                return 10;
            }

            if (lect == 2)
            {
                return 0;
            }

            if (lect == 4)
            {
                return 11;
            }
        }
    return 20;
}

void irPiso(int piso, int pisoActual)
{
    int i = 0;
    int N = 1; //Este valor cambiará de acuerdo con las vueltas que se tarde en llegar de un piso a otro.
    int sentido = piso - pisoActual;
    int vueltas = abs(512 * N * sentido);

    if (piso == 10)
    {
       sentido = 100;
    }


    if(sentido == 0)
    {
        //Encender LED VERDE
    }


    if(sentido > 0 && sentido < 50)
    {
        for(i = 0; i < vueltas; i++)
        {
            GPIO_PORTK_DATA_R = 0b1010;
            Delay_MicroSecond(2500);
            GPIO_PORTK_DATA_R = 0b0110;
            Delay_MicroSecond(2500);
            GPIO_PORTK_DATA_R = 0b0101;
            Delay_MicroSecond(2500);
            GPIO_PORTK_DATA_R = 0b1001;
            Delay_MicroSecond(2500);
        }
    }
    else
    {
        for(i = 0; i < vueltas; i++)
        {
            GPIO_PORTK_DATA_R = 0b1001;
            Delay_MicroSecond(2500);
            GPIO_PORTK_DATA_R = 0b0101;
            Delay_MicroSecond(2500);
            GPIO_PORTK_DATA_R = 0b0110;
            Delay_MicroSecond(2500);
            GPIO_PORTK_DATA_R = 0b1010;
            Delay_MicroSecond(2500);
        }
    }

}

void ledPiso(int piso)
{
    piso = pow(2,piso);
    GPIO_PORTA_AHB_DATA_R = piso;
}

void servoPuerta(int seg)
{
    if(seg == 20)
    {
        Delay_MicroSecond(1000000); //Calcular para 2 segundos
        cerrado();
    }

    if(seg == 10)
    {
        Delay_MicroSecond(500000); //Calcular para 1 segundos
        cerrado();
    }

    while(seg == 11)
    {
        Delay_MicroSecond(2000000); //Calcular para 4 segundos
        seg = leerTeclado();
    }

}

int verificarContrasena()
{
    int i = 0;
    int contra[5]= {0,0,0,0,0};

    for(i = 0; i<4; i++)
    {
        contra[i] = leerTeclado();
        Delay_MicroSecond(50000);
    }

    contra[2] = contra [2]*10;
    contra[1] = contra [1]*100;
    contra[0] = contra [0]*1000;

    for (i = 0; i<4; i++)
    {
        contra[4] = contra[4] + contra[i];
    }

    if (contra [4] == 3257)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }

    return i;
}

int ultimoPiso(int bandera, int pisoActual)
{
    int pisoAct = 0;
    if (bandera == 1)
    {
        Delay_MicroSecond(1000000); //Calcular para 2 segundos
        servoPuerta(leerTeclado());
        irPiso(4, pisoActual);
        ledPiso(4);
        servoPuerta(leerTeclado());
        pisoAct = 4;
    }

    else
    {
        pisoAct = pisoActual;
    }

    return pisoAct;
}


void abierto()
{
    int i=0;
    for(i=0; i<10; i++)
    {
        GPIO_PORTA_AHB_DATA_R |= (0b110000); /* make control  pin high */
        Delay_MicroSecond(400); /*0.4ms seconds delay Calcular*/
        GPIO_PORTA_AHB_DATA_R &= ~(0b110000); /* make control  pin low */
        Delay_MicroSecond(19600); /*1.96ms seconds delay Calcular */
    }
}

void cerrado()
{
    int i=0;
    for(i=0; i<10; i++)
    {
    /* Given 10us trigger pulse */
        GPIO_PORTA_AHB_DATA_R |= (0b110000); /* make control  pin high */
        Delay_MicroSecond(2100); /*2.1ms seconds delay */
        GPIO_PORTA_AHB_DATA_R &= ~(0b110000); /* make trigger  pin high */
        Delay_MicroSecond(17900); /*1.79ms seconds delay */
    }
}



void Delay_MicroSecond(int time)
{
    int i;
    SYSCTL_RCGCTIMER_R |= 4;     /* enable clock to Timer Block 2 */
    TIMER2_CTL_R = 0;
    TIMER2_CFG_R  = 0x04;
    TIMER2_TAMR_R=0x2;
    TIMER2_TAILR_R = 16 - 1;
    TIMER2_CTL_R |= 1;        /* enable Timer A after initialization */

    for(i = 0; i < time; i++)
    {
        while ((TIMER2_RIS_R & 0x1) == 0) ;      /* wait for TimerA timeout flag */
        TIMER2_ICR_R = 0x1;      /* clear the TimerA timeout flag */
    }
}
#endif /* ELEVADOR_H_ */
