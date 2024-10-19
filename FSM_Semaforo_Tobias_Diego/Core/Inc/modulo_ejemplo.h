/*
 * modulo_ejemplo.h
 *
 *  Created on: [Fecha de creación]
 *      Author: [Tu nombre]
 */

#ifndef INC_MODULO_EJEMPLO_H_
#define INC_MODULO_EJEMPLO_H_

#include "main.h"
#include "fsm.h"
#include "timer.h"

/***********************************************
*        PASO 1: DEFINICIÓN DE ESTADOS         *
* Aquí definimos los estados del módulo        *
* para que luego podamos usar en la FSM.       *
***********************************************/
typedef enum {
    ESTADO_1,
    ESTADO_2,
    ESTADO_3
} ModuloEjemploEstado;

/***********************************************
*       PASO 2: ESTRUCTURA DEL MÓDULO          *
* Esta estructura contiene la FSM y todas las  *
* variables necesarias del módulo.             *
***********************************************/
typedef struct {
    FSM fsm;                // Instancia de la FSM (detalle de implementación)
    GPIO_TypeDef *Port;     // Puerto GPIO (si fuese necesario)
    uint16_t Pin;           // Pin GPIO (si fuese necesario)
    Timer *timer;           // Timer asociado al módulo (si fuese necesario)
} ModuloEjemplo;

/***********************************************
*       PASO 3: DECLARACIÓN DE FUNCIONES       *
* Declaramos las funciones públicas que        *
* inicializan y actualizan el módulo.          *
***********************************************/

/**
 * @brief Inicializa el módulo.
 *
 * @param ejemplo Puntero a la estructura del módulo.
 * @param Port Puerto GPIO.
 * @param Pin Pin GPIO.
 */
void modulo_ejemplo_init(ModuloEjemplo *ejemplo, GPIO_TypeDef *Port, uint16_t Pin);

/**
 * @brief Actualiza el módulo.
 *
 * @param ejemplo Puntero a la estructura del módulo.
 */
void modulo_ejemplo_update(ModuloEjemplo *ejemplo);

#endif /* INC_MODULO_EJEMPLO_H_ */
