mkfilepath := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir :=$(dir $(mkfilepath))

 include $(current_dir)/balong_product_config_drv.mk
 include $(current_dir)/balong_product_config_pam.mk
 include $(current_dir)/balong_product_config_gucas.mk
 include $(current_dir)/balong_product_config_gucnas.mk
 include $(current_dir)/balong_product_config_tlps.mk
 include $(current_dir)/balong_product_config_audio.mk
 include $(current_dir)/balong_product_config_tool.mk
 include $(current_dir)/balong_product_config_version.mk
ifneq ($(choose_variant_modem),user)
 include $(current_dir)/balong_product_config_drv_eng.mk
 include $(current_dir)/balong_product_config_pam_eng.mk
 include $(current_dir)/balong_product_config_gucas_eng.mk
 include $(current_dir)/balong_product_config_gucnas_eng.mk
 include $(current_dir)/balong_product_config_tlps_eng.mk
 include $(current_dir)/balong_product_config_audio_eng.mk
 include $(current_dir)/balong_product_config_tool_eng.mk
endif


ifeq ($(choose_variant_modem),user)

ifeq ($(strip $(MODEM_FULL_DUMP)),true)
 include $(current_dir)/balong_product_config_modem_full_dump.mk
endif

ifeq ($(strip $(MODEM_DDR_MINI_DUMP)),true)
 include $(current_dir)/balong_product_config_modem_ddr_mini_dump.mk
endif

ifeq ($(strip $(MODEM_LOGCAT_INUSER)),true)
 include $(current_dir)/balong_product_config_modem_netlink.mk
endif

ifeq ($(strip $(modem_log_filter_nv_control)),true)
 include $(current_dir)/balong_product_config_modem_log_filter.mk
endif

ifeq ($(strip $(MODEM_CHR_INUSER)),true)
 include $(current_dir)/balong_product_config_modem_chr_ptm.mk
else
 include $(current_dir)/balong_product_config_modem_chr_ptm_off.mk
endif

ifeq ($(strip $(MODEM_PM_OM_INUSER)),true)
 include $(current_dir)/balong_product_config_modem_pm_om.mk
endif

endif


