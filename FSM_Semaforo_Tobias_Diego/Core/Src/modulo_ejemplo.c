/*
 * modulo_ejemplo.c
 *
 *  Created on: [Fecha de creación]
 *      Author: [Tu nombre]
 */
#include "modulo_ejemplo.h"
#include "timer.h"
#include "main.h"
#include "blink_control.h"
#include "timer_period_manager.h"

char bandera=0;
//Blink control de parpadeo
BlinkControl blink_control_led1, blink_control_led2;

//Como no se ejecuta edge detec entonces no cambia el periodo del timer period manager y solo queda en 200 ms
extern __IO uint32_t uwTick;


/***********************************************
 *        PASO 4: FUNCIONES DE TRANSICIÓN       *
 * Definimos las funciones de transición que    *
 * dependen de las condiciones para cambiar de  *
 * estado.                                      *
 ***********************************************/

/**
 * @brief Condición para pasar del estado 1 al estado 2.
 */
static int condicion_estado1_a_estado2(void *context) {
	// Esta operacion de casteo se hace para poder acceder a los
	// miembros de la estructura que define a nuestro modulo.
	ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
	// Esta condición chequea que determinado pin se encuentre seteado
	return (HAL_GPIO_ReadPin(ejemplo->Port_sw2, ejemplo->sw2) == GPIO_PIN_SET || HAL_GPIO_ReadPin(ejemplo->Port_sw1, ejemplo->sw1) == GPIO_PIN_SET);
}

/**
 * @brief Condición para pasar del estado 2 al estado 3, 3 a 4.
 */
static int condicion_timer_expired(void *context) {
	ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
	return timer_has_expired(&ejemplo->Modulo_ejemplo_timer);
}



/***********************************************
 *          PASO 5: FUNCIONES DE ACCIÓN         *
 * Aquí se definen las acciones que se ejecutan *
 * al entrar en un nuevo estado.                *
 ***********************************************/

/**
 * @brief Acción al entrar en el estado 1.
 */
void on_state_estado1(void *context) {
	ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
	// Ejemplo de acción: Cambiar el estado del pin GPIO a RESET
	HAL_GPIO_WritePin(ejemplo->Port_led1, ejemplo->Pin1, 1); //Rojo off
	HAL_GPIO_WritePin(ejemplo->Port_led2, ejemplo->Pin2, 0); //Verde on
	bandera=0;
}
/**
 * @brief Acción al entrar en el estado 2.
 */
void on_state_estado2(void *context) {
	ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
	// Ejemplo de acción: Iniciar el timer asociado
	uwTick = 0;
	timer_start(&ejemplo->Modulo_ejemplo_timer, 1000); // Iniciar un timer de 1 segundos
	//Parpadeo
	blink_control_init(&blink_control_led2,ejemplo->Port_led2, ejemplo->Pin2, 200);
	bandera=1;
}

void on_state_estado3(void *context) {
	ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
	// Ejemplo de acción: Iniciar el timer asociado
	uwTick = 0;
	timer_start(&ejemplo->Modulo_ejemplo_timer, 3000); // Iniciar un timer de 1 segundos
	HAL_GPIO_WritePin(ejemplo->Port_led1, ejemplo->Pin1, 0); //Rojo on
	HAL_GPIO_WritePin(ejemplo->Port_led2, ejemplo->Pin2, 1); //Verde off
	bandera=0;
}
void on_state_estado4(void *context) {
	ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
	// Ejemplo de acción: Iniciar el timer asociado
	uwTick = 0;
	timer_start(&ejemplo->Modulo_ejemplo_timer, 1000);// Iniciar un timer de 1 segundos
	blink_control_init(&blink_control_led1, ejemplo->Port_led1, ejemplo->Pin1, 200);
	bandera=2;
}

// Donde esta la definición de on_state_estado3 ??
// Cuando lleguemos al PASO7 vamos a enterarnos!


/***********************************************
 *          PASO 6: DECLARAR TRANSICIONES       *
 * Aquí declaramos las transiciones de estado,  *
 * especificando las condiciones que hacen      *
 * posible cambiar entre estados.               *
 ***********************************************/

// Transiciones desde ESTADO_1
static Transition Estado1Transitions[] = {
		{condicion_estado1_a_estado2, ESTADO_2},  // Transición al estado 2
};

// Transiciones desde ESTADO_2 (2 transiciones)
// El Estado 2 tiene "dos flechas salientes", es decir, puede transicionar
// a mas de un estado. Por ese motivo este array tiene dos entradas y no una.
static Transition Estado2Transitions[] = {
		{condicion_timer_expired, ESTADO_3},  // Transición al estado 3
};

// Transiciones desde ESTADO_3
static Transition Estado3Transitions[] = {
		{condicion_timer_expired, ESTADO_4}   // Transición al estado 4
};

// Transiciones desde ESTADO_4
static Transition Estado4Transitions[] = {
		{condicion_timer_expired, ESTADO_1}   // Transición al estado 1
};


/***********************************************
 *           PASO 7: ENSAMBLAR EL MÓDULO        *
 * Aquí se ensamblan los estados, las           *
 * transiciones y las acciones correspondientes.*
 ***********************************************/

static FSMState ModuloEjemploEstados[] = {
		{Estado1Transitions, 1, on_state_estado1},  // Estado 1: tiene 1 transición y ejecuta una acción
		{Estado2Transitions, 1, on_state_estado2},  // Estado 2: tiene 2 transiciones y ejecuta una acción
		{Estado3Transitions, 1, on_state_estado3},               // Estado 3: tiene 1 transición y no ejecuta ninguna acción, por lo que podremos
		{Estado4Transitions, 1, on_state_estado4}
};                                              // evitar la definición de una función vacía y simplemente pasarle aquí NULL.

/***********************************************
 *         PASO 8: INICIALIZAR EL MÓDULO        *
 * Aquí inicializamos el módulo con el estado   *
 * inicial y el contexto adecuado.              *
 ***********************************************/

/**
 * @brief Inicializa el módulo.
 */
void modulo_ejemplo_init(ModuloEjemplo *ejemplo, GPIO_TypeDef *Port_led1, uint16_t Pin1, GPIO_TypeDef *Port_led2, uint16_t Pin2,GPIO_TypeDef *Port_sw1, uint16_t sw1, GPIO_TypeDef *Port_sw2, uint16_t sw2)
{
	// Iniciar la FSM en el estado 1
	fsm_init(&ejemplo->fsm, ModuloEjemploEstados, ESTADO_1, ejemplo);
	// Inicializar el puerto y pin GPIO
	ejemplo->Port_led1 = Port_led1;     // Puerto GPIO (si fuese necesario)
	ejemplo->Pin1 = Pin1;           // Pin GPIO (si fuese necesario)
	ejemplo->Port_led2 = Port_led2 ;     // Puerto GPIO (si fuese necesario)
	ejemplo->Pin2 = Pin2;           // Pin GPIO (si fuese necesario)
	ejemplo->Port_sw1 = Port_sw1;     // Puerto GPIO (si fuese necesario)
	ejemplo->sw1 = sw1;           // Pin GPIO (si fuese necesario)
	ejemplo->Port_sw2=Port_sw2;     // Puerto GPIO (si fuese necesario)
	ejemplo->sw2 = sw2;           // Pin GPIO (si fuese necesario)

	// Inicializar otros recursos (si es necesario)
	//Inicializo los puertos, porque para el inicio como se ejecuta primero el init y luego se
	//definen los puertos no ejecuta el estado inicial porque no sabe los puertos
	HAL_GPIO_WritePin(ejemplo->Port_led1, ejemplo->Pin1, 1); //Rojo off
	HAL_GPIO_WritePin(ejemplo->Port_led2, ejemplo->Pin2, 0); //Verde on

	timer_start(&ejemplo->Modulo_ejemplo_timer, 200);  // Start modulo ejemplo timer

}

/***********************************************
 *         PASO 9: ACTUALIZAR EL MÓDULO         *
 * Esta función se llama en cada ciclo para     *
 * verificar las transiciones y actualizar el   *
 * estado actual si corresponde.                *
 ***********************************************/

/**
 * @brief Actualiza el módulo.
 */
void modulo_ejemplo_update(ModuloEjemplo *ejemplo) {
	fsm_update(&ejemplo->fsm);  // Llamar a fsm_update para gestionar las transiciones

	//Condiciones de actualizacion del blink control si estan en el estado de parpadeo
	if (bandera==1){
		//Blink control led verde
		blink_control_update(&blink_control_led2);
	}
	if(bandera==2){
		//Blink control led Rojo
		blink_control_update(&blink_control_led1);
	}
}
