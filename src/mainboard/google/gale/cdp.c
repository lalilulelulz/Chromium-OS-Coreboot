
/* * Copyright (c) 2012 - 2013 The Linux Foundation. All rights reserved.* */

#include <gpio.h>
#include <soc/cdp.h>
#include <soc/ebi2.h>
#include <soc/clock.h>
#include <types.h>
#include <boardid.h>

void ipq_configure_gpio(const gpio_func_data_t *gpio, unsigned count)
{
	int i;

	for (i = 0; i < count; i++) {
		gpio_tlmm_config(gpio->gpio, gpio->func, gpio->dir,
				gpio->pull, gpio->drvstr, gpio->enable);
		gpio++;
	}
}
