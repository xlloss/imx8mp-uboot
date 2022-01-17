#
# Copyright 2021 DFI
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y += imx8mp_m8mp553.o

ifdef CONFIG_SPL_BUILD
obj-y += spl.o
obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_2g_id0_timing.o
obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_4g_id0_timing.o
obj-$(CONFIG_IMX8M_LPDDR4) += lpddr4_4g_id1_timing.o
endif
