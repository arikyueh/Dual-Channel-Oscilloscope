/*******************************************************************************
* File Name: Onems.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_Onems_H)
#define CY_ISR_Onems_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Onems_Start(void);
void Onems_StartEx(cyisraddress address);
void Onems_Stop(void);

CY_ISR_PROTO(Onems_Interrupt);

void Onems_SetVector(cyisraddress address);
cyisraddress Onems_GetVector(void);

void Onems_SetPriority(uint8 priority);
uint8 Onems_GetPriority(void);

void Onems_Enable(void);
uint8 Onems_GetState(void);
void Onems_Disable(void);

void Onems_SetPending(void);
void Onems_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Onems ISR. */
#define Onems_INTC_VECTOR            ((reg32 *) Onems__INTC_VECT)

/* Address of the Onems ISR priority. */
#define Onems_INTC_PRIOR             ((reg8 *) Onems__INTC_PRIOR_REG)

/* Priority of the Onems interrupt. */
#define Onems_INTC_PRIOR_NUMBER      Onems__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Onems interrupt. */
#define Onems_INTC_SET_EN            ((reg32 *) Onems__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Onems interrupt. */
#define Onems_INTC_CLR_EN            ((reg32 *) Onems__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Onems interrupt state to pending. */
#define Onems_INTC_SET_PD            ((reg32 *) Onems__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Onems interrupt. */
#define Onems_INTC_CLR_PD            ((reg32 *) Onems__INTC_CLR_PD_REG)


#endif /* CY_ISR_Onems_H */


/* [] END OF FILE */
