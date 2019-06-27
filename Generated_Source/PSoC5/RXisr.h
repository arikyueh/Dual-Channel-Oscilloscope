/*******************************************************************************
* File Name: RXisr.h
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
#if !defined(CY_ISR_RXisr_H)
#define CY_ISR_RXisr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void RXisr_Start(void);
void RXisr_StartEx(cyisraddress address);
void RXisr_Stop(void);

CY_ISR_PROTO(RXisr_Interrupt);

void RXisr_SetVector(cyisraddress address);
cyisraddress RXisr_GetVector(void);

void RXisr_SetPriority(uint8 priority);
uint8 RXisr_GetPriority(void);

void RXisr_Enable(void);
uint8 RXisr_GetState(void);
void RXisr_Disable(void);

void RXisr_SetPending(void);
void RXisr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the RXisr ISR. */
#define RXisr_INTC_VECTOR            ((reg32 *) RXisr__INTC_VECT)

/* Address of the RXisr ISR priority. */
#define RXisr_INTC_PRIOR             ((reg8 *) RXisr__INTC_PRIOR_REG)

/* Priority of the RXisr interrupt. */
#define RXisr_INTC_PRIOR_NUMBER      RXisr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable RXisr interrupt. */
#define RXisr_INTC_SET_EN            ((reg32 *) RXisr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the RXisr interrupt. */
#define RXisr_INTC_CLR_EN            ((reg32 *) RXisr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the RXisr interrupt state to pending. */
#define RXisr_INTC_SET_PD            ((reg32 *) RXisr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the RXisr interrupt. */
#define RXisr_INTC_CLR_PD            ((reg32 *) RXisr__INTC_CLR_PD_REG)


#endif /* CY_ISR_RXisr_H */


/* [] END OF FILE */
