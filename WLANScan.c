/*
  This sample code is in public domain.

  This sample code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* 
  This sample scans the nearby APs and prints their information to log.

  It set the device to station mode by API vm_wlan_mode_set(), then call vm_wlan_scan()
  to scan the nearby APs, the result will be gotten in the scan callback function.

*/

#include "vmtype.h" 
#include "vmsystem.h"
#include "vmlog.h" 
#include "ResID.h"
#include "WLANScan.h"
#include "vmwlan.h"


static VM_RESULT g_wlan_job_id;

/********************************************************
 *
Members                                                      Description
VM_WLAN_SCAN_RESULT result;                                  Scan result enum.
VMUINT8 ap_list_number;                                      The total number of searched AP list.
VMINT32 scan_job_id;                                         Scan job ID. 0xFFFFFF means scan result broadcasting.
vm_wlan_scan_ap_info_t ap_list[VM_WLAN_MAX_AP_LIST_NUMBER];  BSS info array.
********************************************************/

static void wlan_scan_callback(
            VMUINT32 job_id,
            void *user_data,
            vm_wlan_scan_result_t* scan_result)
{
    VMINT i;
    vm_log_debug("scan result:%d", scan_result->result);
    if(VM_WLAN_SCAN_SUCCESS == scan_result->result)
    {
        for(i = 0; i < scan_result->ap_list_number; i++)
        {
            vm_log_debug("-----------------------------------");
            vm_log_debug("SSID:%s", scan_result->ap_list[i].ssid);//只有通过身份验证的用户才可以进入相应的子网络，防止未被授权的用户进入本网络
            vm_log_debug("bssid: %s", scan_result->ap_list[i].bssid);//对端路由器无线网的MAC地址
            vm_log_debug("rssi:%d", scan_result->ap_list[i].rssi);
            vm_log_debug("channel_number:%d", scan_result->ap_list[i].channel_number);//AP Channel number
            vm_log_debug("max_rate:%d", scan_result->ap_list[i].max_rate);
            vm_log_debug("privacy:%d", scan_result->ap_list[i].privacy);
            vm_log_debug("network_type:%d", scan_result->ap_list[i].network_type);
            vm_log_debug("authorize_mode:%d", scan_result->ap_list[i].authorize_mode);//Profile authorization mode.
            vm_log_debug("encrypt_mode:%d", scan_result->ap_list[i].encrypt_mode);//配置文件加密模式
        }
    }
}
static void wlan_callback(void* user_data, VM_WLAN_REQUEST_RESULT result_type)
{
    vm_log_debug("wlan result:%d", result_type);
    if(VM_WLAN_REQUEST_DONE == result_type)
    {
        g_wlan_job_id = vm_wlan_scan(wlan_scan_callback, NULL);
    }
}
static void wlan_scan(void)
{
    vm_wlan_mode_set(VM_WLAN_MODE_STA, wlan_callback, NULL);//STA (Station) mode only. Use this mode to join an access point.
}

void handle_sysevt(VMINT message, VMINT param)
{
    switch (message) 
    {
        case VM_EVENT_CREATE:
            wlan_scan();
            break;

        case VM_EVENT_QUIT:
            break;
    }
}

void vm_main(void)
{
    vm_pmng_register_system_event_callback(handle_sysevt);
}





