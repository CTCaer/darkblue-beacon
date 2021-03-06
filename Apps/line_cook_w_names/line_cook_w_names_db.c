/*
 * This file has been automatically generated by the WICED Smart Designer.
 * BLE device's GATT database, device configuration, function that can be used
 * to send notifications and indications.
 *
 */

// line_cook_w_names_db.c

#include "bleprofile.h"
#include "bleapp.h"
#include "gpiodriver.h"
#include "string.h"
#include "stdio.h"
#include "platform.h"
#include "line_cook_w_names_db.h"

const UINT8 gatt_database[]= // Define GATT database
{
// ***** Primary service 'Generic Access'
    //<Name>Generic Access</Name>
    //<Uuid>1800</Uuid>
    //Service handle: HDLS_GENERIC_ACCESS
    //Service UUID: UUID_SERVICE_GAP
    PRIMARY_SERVICE_UUID16 (HDLS_GENERIC_ACCESS, UUID_SERVICE_GAP),

    //<Name>Device Name</Name>
    //<Uuid>2A00</Uuid>
    CHARACTERISTIC_UUID16 (HDLC_GENERIC_ACCESS_DEVICE_NAME,
                           HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE,
                           UUID_CHARACTERISTIC_DEVICE_NAME,
                           LEGATTDB_CHAR_PROP_READ,
                           LEGATTDB_PERM_READABLE,
                           17),
    'l','i','n','e',0x2d,'c','o','o','k',0x2d,'w',0x2d,'n','a','m','e','s',

    //<Name>Appearance</Name>
    //<Uuid>2A01</Uuid>
    CHARACTERISTIC_UUID16 (HDLC_GENERIC_ACCESS_APPEARANCE,
                           HDLC_GENERIC_ACCESS_APPEARANCE_VALUE,
                           UUID_CHARACTERISTIC_APPEARANCE,
                           LEGATTDB_CHAR_PROP_READ,
                           LEGATTDB_PERM_READABLE,
                           2),
    0x00,0x00,

// ***** Primary service 'Generic Attribute'
    //<Name>Generic Attribute</Name>
    //<Uuid>1801</Uuid>
    //Service handle: HDLS_GENERIC_ATTRIBUTE
    //Service UUID: UUID_SERVICE_GATT
    PRIMARY_SERVICE_UUID16 (HDLS_GENERIC_ATTRIBUTE, UUID_SERVICE_GATT),

// ***** Primary service 'Line Cook Cooking Step'
    //<Name>Line Cook Cooking Step</Name>
    //<Uuid>cf 15 fe e0 4a e6 44 7f 8b 44 57 1f d8 44 38 f2</Uuid>
    //Service handle: HDLS_LINE_COOK_COOKING_STEP
    //Service UUID: __UUID_LINE_COOK_COOKING_STEP
    PRIMARY_SERVICE_UUID128 (HDLS_LINE_COOK_COOKING_STEP, __UUID_LINE_COOK_COOKING_STEP),

    //<Name>Cooking Step</Name>
    //<Uuid>f9 a8 b1 1a 10 6a 43 ea b2 82 18 9a 9c c3 9f 51</Uuid>
    CHARACTERISTIC_UUID128_WRITABLE (HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP,
                           HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE,
                           __UUID_LINE_COOK_COOKING_STEP_COOKING_STEP,
                           LEGATTDB_CHAR_PROP_READ | LEGATTDB_CHAR_PROP_WRITE | LEGATTDB_CHAR_PROP_NOTIFY | LEGATTDB_CHAR_PROP_INDICATE,
                           LEGATTDB_PERM_READABLE | LEGATTDB_PERM_WRITE_REQ,
                           10),
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,

    //<UserDescription>
    //<Value>Cooking Step</Value>
    CHAR_DESCRIPTOR_UUID16 (HDLD_LINE_COOK_COOKING_STEP_COOKING_STEP_USER_DESCRIPTION,
                    UUID_DESCRIPTOR_CHARACTERISTIC_USER_DESCRIPTION,
                    LEGATTDB_PERM_READABLE, 12),
    //UTF-8 <User Description> Cooking Step
    'C','o','o','k','i','n','g',' ','S','t','e','p',

    //<ClientConfiguration>
    //<Notification>false</Notification>
    //<Indication>false</Indication>
    CHAR_DESCRIPTOR_UUID16_WRITABLE (HDLD_LINE_COOK_COOKING_STEP_COOKING_STEP_CLIENT_CONFIGURATION,
                                     UUID_DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION,
                                     LEGATTDB_PERM_READABLE | LEGATTDB_PERM_WRITE_CMD | LEGATTDB_PERM_WRITE_REQ | LEGATTDB_PERM_RELIABLE_WRITE | LEGATTDB_PERM_AUTH_WRITABLE,
                                     2),
    BIT16_TO_8 (CCC_NONE),

};
// Indication sent, waiting for ack
UINT8 line_cook_w_names_indication_sent = 0;
// Length of the GATT database
const UINT16 gatt_database_len = sizeof(gatt_database);
// Pointer to the bonded peer info or NULL if not bonded
__HOSTINFO_PEER *p_bonded = NULL;
// Following structure defines GPIO configuration used by the application
const BLE_PROFILE_GPIO_CFG line_cook_w_names_gpio_cfg =
{
    {
        GPIO_PIN_WP,                               // This need to be used to enable/disable NVRAM write protect
        GPIO_PIN_BUTTON, GPIO_PIN_LED, -1, -1, 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 // Other GPIOs are not used
    },
    /*.gpio_flag =*/
    {
        GPIO_SETTINGS_WP,
        GPIO_SETTINGS_BUTTON, GPIO_SETTINGS_LED, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

// Main vendor specific service
UINT8 line_cook_w_names_uuid_main_vsc_service[16] = {__UUID_LINE_COOK_COOKING_STEP};
// Indication confirmation callback
static void __indication_cfm()
{
    line_cook_w_names_indication_sent = 0;
    line_cook_w_names_indication_cfm();
}

// Timer callback function for line_cook_w_names_reg_timer
void line_cook_w_names_fine_timeout(UINT32 arg)
{
    line_cook_w_names_timer_fine();
}

// Registers timer. Should be called from line_cook_w_names_create()
void line_cook_w_names_reg_timer()
{
    bleprofile_regTimerCb(line_cook_w_names_fine_timeout, NULL);
    bleprofile_StartTimer();
}

// Finds bonded peer in the hostinfo. On exit p_bonded points on bonded peer or NULL
__HOSTINFO_PEER *__find_bonded_peer(UINT8 *bda)
{
    int i;
    p_bonded = NULL;
    // Check if that peer is bonded and get its pointer
    for (i=0; i<GATT_DB_MAX_BONDED_PEERS; i++)
    {
        if (0 == memcmp(bda, p_hostinfo_generated->peer[i].bdaddr, 6))
        {
            p_bonded = &p_hostinfo_generated->peer[i];
            break;
        }
    }
    return p_bonded;
}

// initializes persistent values in the hostinfo to add bonded peer
void line_cook_w_names_add_bond(UINT8 *bda)
{
    // Find this peer among bonded peers in the p_hostinfo_generated
    if (!__find_bonded_peer(bda));
    {
        // Peer is not found. Remember new one.
        memcpy(&p_hostinfo_generated->peer[GATT_DB_MAX_BONDED_PEERS-1], &p_hostinfo_generated->peer[GATT_DB_MAX_BONDED_PEERS-2], sizeof(__HOSTINFO_PEER) * (GATT_DB_MAX_BONDED_PEERS - 1));
        p_bonded = &p_hostinfo_generated->peer[0];
        memcpy(p_bonded->bdaddr, bda, 6);
    }

    // Clear persistent generated values in the hostinfo for just bonded peer

    // Set the initial value of the client configuration descriptor for value 'Cooking Step'
    p_bonded->line_cook_cooking_step_cooking_step_client_configuration = 0;
}

// Prepares generated code for connection - writes persistent values from __HOSTINFO to GATT DB
void __on_connection_up()
{
    line_cook_w_names_indication_sent = 0;
    // Find this peer among bonded peers in the p_hostinfo_generated
    if (__find_bonded_peer(line_cook_w_names_remote_addr))
    {
        BLEPROFILE_DB_PDU db_pdu;

        // Write the value of the client configuration descriptor for value 'Cooking Step'
        db_pdu.len = 2;
        db_pdu.pdu[0] = p_bonded->line_cook_cooking_step_cooking_step_client_configuration & 0xff;
        db_pdu.pdu[1] = (p_bonded->line_cook_cooking_step_cooking_step_client_configuration >> 8) & 0xff;
        bleprofile_WriteHandle(HDLD_LINE_COOK_COOKING_STEP_COOKING_STEP_CLIENT_CONFIGURATION, &db_pdu);
    }
}

// Updates __HOSTINFO by the value written by peer.
// Returns true if any persistent value is changed
BOOL __write_handler(UINT16 handle, int len, UINT8 *attrPtr)
{
    BOOL res = FALSE;
    if (handle == HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE)
    {
        if (len > 10)
        {
            ble_trace2("bad length:%d handle:%04x", len, handle);
        }
        else
        {
            //call custom on_write function
            ble_trace1("write handle:%04x", handle);
            res = on_write_line_cook_cooking_step_cooking_step(len, attrPtr);
        }
    }
    if ((len == 2) && (handle == HDLD_LINE_COOK_COOKING_STEP_COOKING_STEP_CLIENT_CONFIGURATION))
    {
        // find this peer among bonded peers in the p_hostinfo_generated
        if (__find_bonded_peer(line_cook_w_names_remote_addr))
        {
            p_bonded->line_cook_cooking_step_cooking_step_client_configuration = attrPtr[0] + (attrPtr[1] << 8);
            ble_trace2("handle:%02x val:%04x", handle, p_bonded->line_cook_cooking_step_cooking_step_client_configuration);
            res = TRUE;
        }
    }
    return res;
}

// It should be called when 'Device Name' is changed.
BOOL store_in_db_generic_access_device_name(UINT8* p_value, UINT8 value_len)
{
    BLEPROFILE_DB_PDU db_pdu;
    // Write value to the GATT DB
    ble_trace2("write len:%d handle:%02x", value_len, HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE);
    memcpy(&db_pdu.pdu[0], p_value, value_len);
    db_pdu.len = value_len;
    bleprofile_WriteHandle(HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE, &db_pdu);
    return TRUE;
}

// It should be called when 'Appearance' is changed.
BOOL store_in_db_generic_access_appearance(UINT8* p_value, UINT8 value_len)
{
    BLEPROFILE_DB_PDU db_pdu;
    // Write value to the GATT DB
    ble_trace2("write len:%d handle:%02x", value_len, HDLC_GENERIC_ACCESS_APPEARANCE_VALUE);
    memcpy(&db_pdu.pdu[0], p_value, value_len);
    db_pdu.len = value_len;
    bleprofile_WriteHandle(HDLC_GENERIC_ACCESS_APPEARANCE_VALUE, &db_pdu);
    return TRUE;
}

// It should be called when 'Cooking Step' is changed.
BOOL store_in_db_line_cook_cooking_step_cooking_step(UINT8* p_value, UINT8 value_len, BOOL write, BOOL notify)
{
    BLEPROFILE_DB_PDU db_pdu;
    //if write is requested
    if (write)
    {
        // Write value to the GATT DB
        ble_trace2("write len:%d handle:%02x", value_len, HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE);
        memcpy(&db_pdu.pdu[0], p_value, value_len);
        db_pdu.len = value_len;
        bleprofile_WriteHandle(HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE, &db_pdu);
    }
    // If notification is requested
    if (notify)
    {
        if (__find_bonded_peer(line_cook_w_names_remote_addr))
        {
            // Exit if notify and indicate are not configured in the Client Configuration Descriptor
            if (0 == (p_bonded->line_cook_cooking_step_cooking_step_client_configuration & (CCC_NOTIFICATION | CCC_INDICATION)))
            {
                ble_trace1("don't notify handle:%02x", HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE);
                return TRUE;
            }
            // Just return FALSE if connection is down
            if (line_cook_w_names_connection_handle == 0)
            {
                ble_trace1("not connected handle:%02x", HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE);
                return FALSE;
            }
            // Just return FALSE if we are waiting for confirmation
            if (line_cook_w_names_indication_sent != 0)
            {
                ble_trace1("wait confirmation handle:%02x", HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE);
                return FALSE;
            }
            //if write is not requested then we did't write the value. Read it
            if (!write)
            {
                bleprofile_ReadHandle(HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE, &db_pdu);
            }
            // Notify property is true. If client has registered for notification
            if (p_bonded->line_cook_cooking_step_cooking_step_client_configuration & CCC_NOTIFICATION)
            {
                ble_trace2("notify len:%d handle:%02x", db_pdu.len, HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE);
                bleprofile_sendNotification(HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE, (UINT8 *)db_pdu.pdu, db_pdu.len);
            }
            // Indicate property is true. If client has registered for indication,
            // We can send only one and need to wait for ack.
            else
            {
                line_cook_w_names_indication_sent = 1;
                ble_trace2("indicate len:%d handle=%02x", db_pdu.len, HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE);
                bleprofile_sendIndication(HDLC_LINE_COOK_COOKING_STEP_COOKING_STEP_VALUE, (UINT8 *)db_pdu.pdu, db_pdu.len, __indication_cfm);
            }
        }
    }
    return TRUE;
}

