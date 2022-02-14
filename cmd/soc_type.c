/*
 * SOC Temperature Type
 * slash.linux.c@gmail.com
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <command.h>
#include <errno.h>
#include <dm.h>
#include <malloc.h>
#include <asm/gpio.h>
#include <linux/err.h>

__weak int name_to_gpio(const char *name)
{
	return simple_strtoul(name, NULL, 10);
}

enum gpio_cmd {
	GPIOC_INPUT,
};

static int do_soc_type(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int gpio;
	enum gpio_cmd sub_cmd;
	int value;
	const char *str_gpio = NULL;
	int ret;
	char env_buf[2];

	if (argc < 2)
 show_usage:
		return CMD_RET_USAGE;

	argv += 1;
	str_gpio = *argv;

	if (!str_gpio)
		goto show_usage;

	sub_cmd = GPIOC_INPUT;

#if defined(CONFIG_DM_GPIO)
	ret = gpio_lookup_name(str_gpio, NULL, NULL, &gpio);
	if (ret) {
		printf("GPIO: '%s' not found\n", str_gpio);
		return cmd_process_error(cmdtp, ret);
	}
#else
	/* turn the gpio name into a gpio number */
	gpio = name_to_gpio(str_gpio);
	if (gpio < 0)
		goto show_usage;
#endif

	/* grab the pin before we tweak it */
	ret = gpio_request(gpio, "cmd_soctype");
	if (ret && ret != -EBUSY) {
		printf("gpio: requesting pin %u failed\n", gpio);
		return -1;
	}

	gpio_direction_input(gpio);
	value = gpio_get_value(gpio);
	printf("soc type: pin %s (gpio %u) value is ", str_gpio, gpio);

	if (IS_ERR_VALUE(value)) {
		printf("unknown (ret=%d)\n", value);
		goto err;
	}

	sprintf(env_buf, "%d", value);
	env_set("soc_type", env_buf);

	if (value)
		printf("%d: soc type Industry\n", value);
	else
		printf("%d: soc type Consump\n", value);

	if (ret != -EBUSY)
		gpio_free(gpio);

	return (sub_cmd == GPIOC_INPUT) ? value : CMD_RET_SUCCESS;

err:
	if (ret != -EBUSY)
		gpio_free(gpio);
	return CMD_RET_FAILURE;
}

U_BOOT_CMD(soc_type, 2, 0, do_soc_type,
	   "query soc type",
	   "<pin>\n");
