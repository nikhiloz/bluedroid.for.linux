set(CMAKE_BUILD_TYPE Debug)

set(LOCAL_C_INCLUDES
		. 
		..
	   include 
	   avct 
	   btm 
	   avrc 
	   l2cap 
	   avdt 
	   gatt 
	   gap 
	   pan 
	   bnep 
	   hid 
	   sdp 
	   smp 
	   ../include 
	   ../gki/common 
	   ../gki/ulinux 
	   ../udrv/include 
	   ../rpc/include 
	   ../hcis 
	   ../ctrlr/include 
	   ../bta/include 
	   ../bta/sys 
	   ../brcm/include 
	   ../utils/include 
	   $(bdroid_C_INCLUDES) 
	)

#LOCAL_CFLAGS += $(bdroid_CFLAGS)

#ifeq ($(BOARD_HAVE_BLUETOOTH_BCM),true)
#LOCAL_CFLAGS += 
#	-DBOARD_HAVE_BLUETOOTH_BCM
#endif

set(LOCAL_SRC_FILES
    ./a2dp/a2d_api.c 
    ./a2dp/a2d_sbc.c 
    ./avrc/avrc_api.c 
    ./avrc/avrc_sdp.c 
    ./avrc/avrc_opt.c 
    ./hid/hidh_api.c 
    ./hid/hidh_conn.c 
    ./bnep/bnep_main.c 
    ./bnep/bnep_utils.c 
    ./bnep/bnep_api.c 
    ./hcic/hciblecmds.c 
    ./hcic/hcicmds.c 
    ./btm/btm_ble.c 
    ./btm/btm_sec.c 
    ./btm/btm_inq.c 
    ./btm/btm_ble_addr.c 
    ./btm/btm_ble_bgconn.c 
    ./btm/btm_main.c 
    ./btm/btm_dev.c 
    ./btm/btm_ble_gap.c 
    ./btm/btm_acl.c 
    ./btm/btm_sco.c 
    ./btm/btm_pm.c 
    ./btm/btm_devctl.c 
    ./rfcomm/rfc_utils.c 
    ./rfcomm/port_rfc.c 
    ./rfcomm/rfc_l2cap_if.c 
    ./rfcomm/rfc_mx_fsm.c 
    ./rfcomm/port_utils.c 
    ./rfcomm/rfc_port_fsm.c 
    ./rfcomm/rfc_port_if.c 
    ./rfcomm/port_api.c 
    ./rfcomm/rfc_ts_frames.c 
    ./mcap/mca_dact.c 
    ./mcap/mca_dsm.c 
    ./mcap/mca_l2c.c 
    ./mcap/mca_main.c 
    ./mcap/mca_csm.c 
    ./mcap/mca_cact.c 
    ./mcap/mca_api.c 
    ./gatt/gatt_sr.c 
    ./gatt/gatt_cl.c 
    ./gatt/gatt_api.c 
    ./gatt/gatt_auth.c 
    ./gatt/gatt_utils.c 
    ./gatt/gatt_main.c 
    ./gatt/att_protocol.c 
    ./gatt/gatt_attr.c 
    ./gatt/gatt_db.c 
    ./avct/avct_api.c 
    ./avct/avct_l2c.c 
    ./avct/avct_lcb.c 
    ./avct/avct_ccb.c 
    ./avct/avct_lcb_act.c 
    ./smp/smp_main.c 
    ./smp/smp_l2c.c 
    ./smp/smp_cmac.c 
    ./smp/smp_utils.c 
    ./smp/smp_act.c 
    ./smp/smp_keys.c 
    ./smp/smp_api.c 
    ./avdt/avdt_ccb.c 
    ./avdt/avdt_scb_act.c 
    ./avdt/avdt_msg.c 
    ./avdt/avdt_ccb_act.c 
    ./avdt/avdt_api.c 
    ./avdt/avdt_scb.c 
    ./avdt/avdt_ad.c 
    ./avdt/avdt_l2c.c 
    ./sdp/sdp_server.c 
    ./sdp/sdp_main.c 
    ./sdp/sdp_db.c 
    ./sdp/sdp_utils.c 
    ./sdp/sdp_api.c 
    ./sdp/sdp_discovery.c 
    ./pan/pan_main.c 
    ./pan/pan_api.c 
    ./pan/pan_utils.c 
    ./btu/btu_hcif.c 
    ./btu/btu_init.c 
    ./btu/btu_task.c 
    ./l2cap/l2c_fcr.c 
    ./l2cap/l2c_ucd.c 
    ./l2cap/l2c_main.c 
    ./l2cap/l2c_api.c 
    ./l2cap/l2c_utils.c 
    ./l2cap/l2c_csm.c 
    ./l2cap/l2c_link.c 
    ./l2cap/l2c_ble.c)

set(LOCAL_MODULE libbt-brcm_stack)
add_definitions("-DLINUX_NATIVE -DSBC_FOR_EMBEDDED_LINUX")

include_directories(${LOCAL_C_INCLUDES})
add_library(${LOCAL_MODULE} ${LOCAL_SRC_FILES})
set_target_properties(${LOCAL_MODULE} PROPERTIES PREFIX "")
