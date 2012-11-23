/**************************************************************************************************
  Filename:       helloBluetoothprofile.c
  Revised:        $Date: 2010-08-06 08:56:11 -0700 (Fri, 06 Aug 2010) $
  Revision:       $Revision: 23333 $

  Description:    This file contains the HelloBluetooth GATT profile sample GATT service 
                  profile for use with the BLE sample application.

  Copyright 2010 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "helloBluetoothprofile.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED        4

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// 128-Bit HelloBluetooth Service UUID: 5ab20001-b355-4d8a-96ef-2963812dd0b8
CONST uint8 hbpServUUID[ATT_UUID_SIZE] = 
{
  0xB8, 0xD0, 0x2D, 0x81, 0x63, 0x29, 0xEF, 0x96, 
  0x8A, 0x4D, 0x55, 0xB3, 0x01, 0x00, 0xB2, 0x5A
};

// 128-Bit Characteristic Name UUID: 5ab20002-b355-4d8a-96ef-2963812dd0b8
CONST uint8 hbpNameUUID[ATT_UUID_SIZE] = 
{
  0xB8, 0xD0, 0x2D, 0x81, 0x63, 0x29, 0xEF, 0x96, 
  0x8A, 0x4D, 0x55, 0xB3, 0x02, 0x00, 0xB2, 0x5A
};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static hbpCBs_t *hbp_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// HelloBluetooth Profile Service attribute
static CONST gattAttrType_t hbpService = { ATT_UUID_SIZE, hbpServUUID };


// HelloBluetooth Profile Characteristic Name Properties
static uint8 hbpNameProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic Name Value
static uint8 hbpName[HBP_NAME_LEN] = "Muhammad\0";


// HelloBluetooth Profile Characteristic Name User Description
static uint8 hbpNameUserDesp[17] = "Name of the user\0";


/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t hbpAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
  // HelloBluetooth Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&hbpService            /* pValue */
  },

    // Characteristic Name Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &hbpNameProps 
    },

      // Characteristic Name Value
      { 
        { ATT_UUID_SIZE, hbpNameUUID },
        GATT_PERMIT_READ,
        0, 
        hbpName 
      },

      // Characteristic Name User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        hbpNameUserDesp 
      },
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 hbp_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t hbp_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset );


/*********************************************************************
 * PROFILE CALLBACKS
 */
// HelloBluetooth Profile Service Callbacks
CONST gattServiceCBs_t hbpCBs =
{
  hbp_ReadAttrCB,  // Read callback function pointer
  hbp_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      HBP_AddService
 *
 * @brief   Initializes the HelloBluetooth Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t HBP_AddService( uint32 services )
{
  uint8 status = SUCCESS;

  if ( services & HBP_SERVICE )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( hbpAttrTbl, 
                                          GATT_NUM_ATTRS( hbpAttrTbl ),
                                          &hbpCBs );
  }

  return ( status );
}


/*********************************************************************
 * @fn      HBP_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t HBP_RegisterAppCBs( hbpCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    hbp_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}
  

/*********************************************************************
 * @fn      HBP_SetParameter
 *
 * @brief   Set a HelloBluetooth Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t HBP_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case HBP_NAME:
      if ( len == HBP_NAME_LEN ) 
      {
        VOID osal_memcpy( hbpName, value, HBP_NAME_LEN );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
    
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn      HBP_GetParameter
 *
 * @brief   Get a HelloBluetooth Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t HBP_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case HBP_NAME:
      VOID osal_memcpy( value, hbpName, HBP_NAME_LEN );
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          hbp_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static uint8 hbp_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen )
{
  bStatus_t status = SUCCESS;

  // If attribute permissions require authorization to read, return error
  if ( gattPermitAuthorRead( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    return ( ATT_ERR_ATTR_NOT_LONG );
  }
 
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
      // gattserverapp handles those reads      
        
      default:
        *pLen = 0;
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    // Since we have only one characteristics, we always return hbpNameUUID
    if ( osal_memcmp( pAttr->type.uuid, hbpNameUUID, pAttr->type.len ) )
    {
      // Make sure it's not a blob operation
      if ( offset == 0 )
      {
          *pLen = HBP_NAME_LEN;
          VOID osal_memcpy( pValue, pAttr->pValue, HBP_NAME_LEN );        
      }
      else
      {
        status = ATT_ERR_ATTR_NOT_LONG;
      }
    }
    else
    {
      // Should never get here!
      *pLen = 0;
      status = ATT_ERR_INVALID_HANDLE;
    }
  }

  return ( status );
}

/*********************************************************************
 * @fn      hbp_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   complete - whether this is the last packet
 * @param   oper - whether to validate and/or write attribute value  
 *
 * @return  Success or Failure
 */
static bStatus_t hbp_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset )
{
  bStatus_t status = SUCCESS;
  uint8 notifyApp = 0xFF;
  
  // If attribute permissions require authorization to write, return error
  if ( gattPermitAuthorWrite( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {

      case GATT_CLIENT_CHAR_CFG_UUID:
        status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                                 offset, GATT_CLIENT_CFG_NOTIFY );
        break;
        
      default:
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    status = ATT_ERR_INVALID_HANDLE;
  }

  // If a charactersitic value changed then callback function to notify application of change
  if ( (notifyApp != 0xFF ) && hbp_AppCBs && hbp_AppCBs->pfnHBPChange )
  {
    hbp_AppCBs->pfnHBPChange( notifyApp );  
  }
  
  return ( status );
}

/*********************************************************************
*********************************************************************/
