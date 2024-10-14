#
# (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y += drm_modes.o

ifndef CONFIG_SPL_BUILD
obj-y += rockchip_display.o rockchip_display_helper.o rockchip_crtc.o rockchip_phy.o rockchip_bridge.o \
		rockchip_vop.o rockchip_vop_reg.o rockchip_vop2.o bmp_helper.o \
		rockchip_connector.o rockchip_post_csc.o libnsbmp.o

obj-$(CONFIG_DRM_MIPI_DSI) += drm_mipi_dsi.o
obj-$(CONFIG_DRM_DP_HELPER) += drm_dp_helper.o
obj-$(CONFIG_DRM_DSC) += drm_dsc.o
obj-$(CONFIG_DRM_MAXIM_MAX96745) += max96745.o
obj-$(CONFIG_DRM_MAXIM_MAX96755F) += max96755f.o
obj-$(CONFIG_DRM_PANEL_ROHM_BU18RL82) += panel-rohm-bu18rl82.o
obj-$(CONFIG_DRM_PANEL_MAXIM_MAX96752F) += panel-maxim-max96752f.o
obj-$(CONFIG_DRM_ROCKCHIP_DW_MIPI_DSI) += dw_mipi_dsi.o
obj-$(CONFIG_DRM_ROCKCHIP_DW_MIPI_DSI2) += dw_mipi_dsi2.o
obj-$(CONFIG_DRM_ROCKCHIP_DW_HDMI) += rockchip_dw_hdmi.o dw_hdmi.o
obj-$(CONFIG_DRM_ROCKCHIP_DW_HDMI_QP) += rockchip_dw_hdmi_qp.o dw_hdmi_qp.o
obj-$(CONFIG_ROCKCHIP_INNO_HDMI_PHY) += rockchip-inno-hdmi-phy.o
obj-$(CONFIG_DRM_ROCKCHIP_INNO_HDMI) += inno_hdmi.o
obj-$(CONFIG_DRM_ROCKCHIP_INNO_MIPI_PHY) += inno_mipi_phy.o
obj-$(CONFIG_DRM_ROCKCHIP_INNO_VIDEO_PHY) += inno_video_phy.o
obj-$(CONFIG_DRM_ROCKCHIP_INNO_VIDEO_COMBO_PHY) += inno_video_combo_phy.o
obj-$(CONFIG_PHY_ROCKCHIP_SAMSUNG_HDPTX_HDMI) += phy-rockchip-samsung-hdptx-hdmi.o
obj-$(CONFIG_DRM_ROCKCHIP_TVE) += rockchip_tve.o
obj-$(CONFIG_DRM_ROCKCHIP_ANALOGIX_DP) += analogix_dp.o analogix_dp_reg.o
obj-$(CONFIG_DRM_ROCKCHIP_DW_DP) += dw-dp.o
obj-$(CONFIG_DRM_ROCKCHIP_LVDS) += rockchip_lvds.o drm_of.o
obj-$(CONFIG_DRM_ROCKCHIP_RGB) += rockchip_rgb.o
obj-$(CONFIG_DRM_ROCKCHIP_PANEL) += rockchip_panel.o
obj-$(CONFIG_DRM_ROHM_BU18XL82) += rohm-bu18tl82.o rohm-bu18rl82.o
obj-$(CONFIG_DRM_ROCKCHIP_RK618) += rk618.o rk618_lvds.o rk618_dsi.o
obj-$(CONFIG_DRM_ROCKCHIP_RK1000) += rk1000.o rk1000_tve.o
obj-$(CONFIG_DRM_ROCKCHIP_SAMSUNG_MIPI_DCPHY) += samsung_mipi_dcphy.o
obj-$(CONFIG_DRM_SII902X) += sii902x.o
obj-$(CONFIG_SERDES_DISPLAY) += display-serdes/
else
obj-y += rockchip_spl_display.o rockchip_display_helper.o rockchip_crtc.o rockchip_connector.o rockchip_post_csc.o rockchip_vop2.o rockchip_phy.o rockchip-inno-hdmi-phy.o rockchip_dw_hdmi.o dw_hdmi.o
endif

