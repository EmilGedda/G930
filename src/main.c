#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdint.h>

int connect(libusb_device* device, libusb_device_handle **handle) {
	int r = libusb_open(device, handle);
	if(r != 0) {
		printf("Unable to open USB connection\n");
		printf("Error: %s\n", libusb_error_name(r));
		return 1;
	}
	r = libusb_set_auto_detach_kernel_driver(*handle, 1);

	if(r != 0) {
		printf("Unable to detach the kernel driver\n");
		printf("Error: %s\n", libusb_error_name(r));
		return 1;
	}

	r = libusb_claim_interface(*handle, 2);

	if(r != 0) {
		printf("Unable to claim the interface.\n");
		printf("Error: %s\n", libusb_error_name(r));
		return 1;
	}

	return 0;
}

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

void mic_enabled(uint8_t data, libusb_device_handle **handle) {
	if(data != 0) {
		data = 1;
	}
	int r = libusb_control_transfer(*handle, 2, 1, 2, 0, &data, 1, 10);
	if (r > 0) {
		printf("Successful transfer!\n");
		return;
	}
	printf("Unable to send!\n");
	printf("%s\n", libusb_error_name(r));	
}

int main () {
	libusb_device **devices;
	libusb_context *ctx = NULL;
	libusb_device_handle *handle = NULL;
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
	
	if (headset == NULL) {
		printf("Unable to find a connected Logitech G930\n");
		return 1;
	}

	r = connect(headset, &handle);	
	if (r != 0) return r;
	mic_enabled(1, &handle);
	libusb_release_interface(handle, 2);
	libusb_close(handle);
	libusb_free_device_list(devices, 1);
	libusb_exit(ctx);
	return 0;
}

