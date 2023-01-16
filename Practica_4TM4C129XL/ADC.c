/*
 * ADC.c
 *
 *  Created on: 04/11/2019
 *      Author: ravenelco
 */
#include "lib/include.h"

extern void Configura_Reg_ADC0(void)
{
    /*
    Habilitar el modulo 0 del ADC con dos canales analogicos 
    en el puerto E a una velocidad de conversion de 250ksps
    dandole la mayor prioridad al secuenciador 2 con evento
    de procesador 
    */
     //Pag 396 para inicializar el modulo de reloj del adc RCGCADC
     //Escoger y habilitar que adc vamos a ocupar
    SYSCTL->RCGCADC = (1<<0); 
    //(RGCGPIO) Puertos base habilitación del reloj
    //                     F     E      D       C      B     A
    SYSCTL->RCGCGPIO |= (0<<5)|(1<<4)|(1<<3)|(0<<2)|(1<<1)|(0<<0)|(1<<12)|(1<<8);// Activo los puertos que ocupo E D B
   
    //(GPIODIR) Habilta los pines como I/O un cero para entrada y un uno para salida en este caso se pone un 0 pa que sea entrada
    GPIOB_AHB->DIR = (0<<4);
    GPIOD_AHB->DIR = (0<<7) | (0<<5)|(0<<4);
    GPIOE_AHB->DIR = (0<<1) | (0<<2);
    
    //(GPIOAFSEL) Enable alternate función para que el modulo analógico tenga control de esos pines (Habilitar analógico)
    
    GPIOB_AHB->AFSEL = (1<<4);
    GPIOD_AHB->AFSEL = (1<<7) | (1<<5)|(1<<4);
    GPIOE_AHB->AFSEL = (1<<1) | (1<<2);
    //GPIOE_AHB->AFSEL =  (1<<4) | (1<<5);
  
    //(GPIODEN) desabilita el modo digital

    
    GPIOB_AHB->DEN = (0<<4);//puertos B
    GPIOD_AHB->DEN = (0<<7) | (0<<5)|(0<<4);//puertos D
    GPIOE_AHB->DEN = (0<<1) | (0<<2);//puertos E
   
    // GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1808
    //GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFF00FFFF);
    //----------------------------------(0X76543210)
    GPIOB_AHB->PCTL = GPIOB_AHB->PCTL & (0xFFF0FFFF);
    GPIOD_AHB->PCTL = GPIOD_AHB->PCTL & (0x0F00FFFF);
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFFFFF00F);
   
   
    //(GPIOAMSEL)habilitar analogico SOLO lo USA EL ADC
    //GPIOE_AHB->AMSEL = (1<<5) | (1<<4);
   

    GPIOB_AHB->AMSEL = (1<<4);
    GPIOD_AHB->AMSEL = (1<<7) | (1<<5)|(1<<4);
    GPIOE_AHB->AMSEL = (1<<1) | (1<<2);
    
    // El registro (ADCPC) establece la velocidad de conversión por segundo
    ADC0->PC = 0x5;//1mSPS
    // Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    //PRIORIDAD MENOR<----MAYOR
    ADC0->SSPRI = 0x0213;
    //(ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);
    // Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger)
    ADC0->EMUX  = (0x0<<12)|(0x0<<4);//Es el que determina como comenzar a medir se selecciono por default del secuenciador
    
    
    
    
    ADC0->SSMUX3 = (2<<0); 
                           //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción
    ADC0->SSMUX1 = (4<<0)|(6<<4)|(7<<8)|(10<<12); 


    ADC0->SSCTL3 = (1<<1) | (1<<2);
    ADC0->SSCTL1 = (1<<1) | (1<<2)|(1<<5) | (1<<6)|(1<<9) | (1<<10)|(1<<13) | (1<<14);
    
    
    /* Enable ADC Interrupt */
    ADC0->IM = (0<<2)|(0<<1); /* Unmask ADC0 sequence 2 interrupt pag 1082*/
    //NVIC->PRI4 = (NVICPRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC->EN0 = 0x00010000;
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS = (1<<3) | (0<<2) | (1<<1) | (0<<0);
    
}

