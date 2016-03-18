#include <libusb-1.0/libusb.h>
#include <stdio.h>

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

	printf("Hello world!\n");
	return 0;
}
