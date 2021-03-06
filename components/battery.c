#include <dbus/dbus.h>
#include <gio/gio.h>
#include <stdio.h>

#include "../types.h"

#define PROXY_FLAGS G_DBUS_PROXY_FLAGS_NONE
#define CALL_FLAGS G_DBUS_CALL_FLAGS_NONE

#define UPOWER_BUS "org.freedesktop.UPower"
#define UPOWER_PATH "/org/freedesktop/UPower"
#define UPOWER_IFACE UPOWER_BUS
#define UPOWER_DISPLAY_DEVICE (UPOWER_PATH "/devices/DisplayDevice")
#define UPOWER_DEVICE_IFACE (UPOWER_BUS ".Device")
#define DBUS_PROPS "org.freedesktop.DBus.Properties"

enum {
	Unknown = 0,
	Charging,
	Discharging,
	Empty,
	FullyCharged,
	PendingCharge,
	PendingDischarge
} BatteryState;

static const char *icons[FullyCharged + 1][5] = {
	{""},                          // Unknown
	{" ", " ", " ", " ", " "}, // Charging
	{"", "", "", "", ""},      // Discharging
	{""},                          // Empty
	{""}                           // FullyCharged
};

static GError *baterr = NULL;
static GDBusProxy *proxy = NULL;

int battery_status(char *output, unsigned int size, const Arg *arg) {
	GVariant *retval;
	GVariantDict *devprops;
	int written = 0, index;

	double percentage;
	dbus_uint32_t state;

	if (proxy == NULL) {
		proxy = g_dbus_proxy_new_for_bus_sync(
			G_BUS_TYPE_SYSTEM, PROXY_FLAGS, NULL, UPOWER_BUS,
			UPOWER_DISPLAY_DEVICE, DBUS_PROPS, NULL, &baterr);

		if (proxy == NULL) {
			strncpy(output, " Error", size);
			written = strlen(output);
			fprintf(stderr, "Error creating dbus proxy: %s", baterr->message);
			return 1;
		}
	}

	retval = g_dbus_proxy_call_sync(proxy, "GetAll",
	                                g_variant_new("(s)", UPOWER_DEVICE_IFACE),
	                                CALL_FLAGS, -1, NULL, &baterr);
	devprops = g_variant_dict_new(g_variant_get_child_value(retval, 0));

	if (g_variant_dict_lookup(devprops, "Percentage", "d", &percentage) &&
	    g_variant_dict_lookup(devprops, "State", "u", &state)) {
		index = (int)percentage / 20;

		if (state == Unknown || state == Empty || state == FullyCharged)
			index = 0;
		else if (state == Discharging && index == 5)
			index = 4;

		written =
			snprintf(output, size, "%s %d", icons[state][index], (int)percentage);
	}

	return written;
}

void battery_status_clean(void *arg) {
	if (baterr)
		g_error_free(baterr);
	if (proxy)
		g_object_unref(proxy);
}
