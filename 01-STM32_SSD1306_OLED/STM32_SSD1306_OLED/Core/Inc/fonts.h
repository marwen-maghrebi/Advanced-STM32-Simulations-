/*
 * fonts.h
 *
 *  Created on: Apr 9, 2024
 *      Author: PC-MAGHREBI
 */

#ifndef INC_FONTS_H_
#define INC_FONTS_H_

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

/**
 *
 * Default fonts library. It is used in all LCD based libraries.
 *
 * \par Supported fonts
 *
 * Currently, these fonts are supported:
 *  - 7 x 10 pixels
 *  - 11 x 18 pixels
 *  - 16 x 26 pixels
 */
//#include "stm32f7xx.h"                  // Device header

#include "stdint.h"
#include "string.h"

/**
 * @defgroup LIB_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Font structure used on my LCD libraries
 */
typedef struct {
  uint8_t FontWidth;    /*!< Font width in pixels */
  uint8_t FontHeight;   /*!< Font height in pixels */
  const uint16_t *data; /*!< Pointer to data font data array */
} FontDef_t;

/**
 * @brief  String length and height
 */
typedef struct {
  uint16_t Length;      /*!< String length in units of pixels */
  uint16_t Height;      /*!< String height in units of pixels */
} FONTS_SIZE_t;

/**
 * @}
 */

/**
 * @defgroup FONTS_FontVariables
 * @brief    Library font variables
 * @{
 */

/**
 * @brief  7 x 10 pixels font size structure
 */
extern FontDef_t Font_7x10;

/**
 * @brief  11 x 18 pixels font size structure
 */
extern FontDef_t Font_11x18;

/**
 * @brief  16 x 26 pixels font size structure
 */
extern FontDef_t Font_16x26;

/**
 * @}
 */

/**
 * @defgroup FONTS_Functions
 * @brief    Library functions
 * @{
 */

/**
 * @brief  Calculates string length and height in units of pixels depending on string and font used
 * @param  *str: String to be checked for length and height
 * @param  *SizeStruct: Pointer to empty @ref FONTS_SIZE_t structure where informations will be saved
 * @param  *Font: Pointer to @ref FontDef_t font used for calculations
 * @retval Pointer to string used for length and height
 */
char* FONTS_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_FONTS_H_ */
