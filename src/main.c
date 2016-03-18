#include <libusb-1.0/libusb.h>
#include <stdio.h>

libusb_device* get_headset(libusb_device **devices, ssize_t len) {
	const int LOGITECH = 0x046D;
	const int G930 = 0x0A1F; 


	for(ssize_t i = 0; i < len; i++) {
		struct libusb_device_descriptor d;
		int r = libusb_get_device_descriptor(devices[i], &d);
		if (r < 0) {
			printf("Failed to load descriptor for device %zd\n", i);
			printf("%s\n", libusb_error_name(r));	
		}
		if (d.idProduct == G930 && d.idVendor == LOGITECH) {
			printf("Found Logitech G930 device.\n");
			return devices[i];
		}

	}
	return NULL;
}

int main () {
	libusb_device **devices;
	libusb_context *ctx = NULL;
	int r = libusb_init(&ctx);

	if(r < 0) {
		printf("Unable to init libusb\n");
		return 1;
	}

	libusb_set_debug(ctx, 3); /* Recommended by documentation */
	ssize_t list_len = libusb_get_device_list(ctx, &devices);

	if (list_len < 0) {
		printf("Unable to fetch all USB devices\n");
		return 1;
	}

	printf("Number of devices found: %zd\n", list_len);

	libusb_device* headset = get_headset(devices, list_len);

	libusb_free_device_list(devices, 1);
	libusb_exit(ctx);
	return 0;
}

