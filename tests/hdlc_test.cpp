#include "include/definitions.h"
#include "qc/hdlc.h"
#include "util/hexdump.h"


using namespace std;

int main();
void test_basic();
void test_escape();
void test_unescape();
void test_vector_basic();
void test_vector_escape();
void test_vector_unescape();
void test_real_sample();


static const uint8_t test_hdlc_basic[] = { 0x01, 0x02, 0x03, 0x04 };
static const uint8_t test_hdlc_basic_encapsulated[] = { HDLC_CONTROL_CHAR, 0x01, 0x02, 0x03, 0x04, 0x91, 0x39, HDLC_CONTROL_CHAR };
static const uint8_t test_hdlc_escape[] = { 0x01, 0x02, 0x03, HDLC_CONTROL_CHAR, 0x04, HDLC_ESC_CHAR, 0x05, 0x06, HDLC_CONTROL_CHAR, 0x07 };
static const uint8_t test_hdlc_escape_encapsulated[] = { HDLC_CONTROL_CHAR, 0x01, 0x02, 0x03, HDLC_ESC_CHAR, 0x5e, 0x04, HDLC_ESC_CHAR, 0x5d, 0x05, 0x06, HDLC_ESC_CHAR, 0x5e, 0x07, 0xd6, 0x16, HDLC_CONTROL_CHAR };

static const uint8_t test_real_escaped_sample_data[] = {
	0x7e, 0x04, 0x00, 0xc8, 0x00, 0x00, 0xff, 0xe1, 0x04, 0x48, 0x49, 0x42, 0x9f, 0x00, 0x00, 0x40,
	0x96, 0x7c, 0xfe, 0x79, 0xc8, 0x5a, 0x02, 0xc0, 0x00, 0x7c, 0x1c, 0xc0, 0x00, 0x58, 0x6b, 0xc0,
	0x00, 0x69, 0x00, 0x80, 0x00, 0x7f, 0x00, 0x32, 0x00, 0x71, 0x00, 0x40, 0x7d, 0x5d, 0x70, 0x98,
	0xe2, 0xd4, 0x5b, 0x09, 0x3c, 0x81, 0x5d, 0x49, 0x42, 0x9f, 0x00, 0x80, 0x66, 0x11, 0x73, 0xe8,
	0xf9, 0xc8, 0x5a, 0x4d, 0x41, 0xc6, 0x00, 0x00, 0xc7, 0x00, 0x78, 0x08, 0x19, 0x09, 0xa3, 0x01,
	0xc4, 0x00, 0x3c, 0x26, 0xc3, 0x9b, 0x58, 0xa8, 0x7f, 0xff, 0x5b, 0x20, 0x1c, 0xf4, 0xeb, 0xe2,
	0x7f, 0xdf, 0x78, 0x01, 0xc2, 0xa5, 0x48, 0x00, 0x39, 0x31, 0x29, 0x4b, 0x41, 0xc6, 0x00, 0x00,
	0x42, 0x00, 0x78, 0x12, 0x40, 0x20, 0x5c, 0xff, 0xe1, 0x04, 0x48, 0x48, 0x42, 0x9f, 0x00, 0x00,
	0x40, 0x8a, 0x7c, 0xc6, 0x79, 0xc8, 0x5a, 0x02, 0xc0, 0x00, 0x7c, 0x22, 0xc0, 0x00, 0x58, 0x6b,
	0xc0, 0x00, 0x69, 0x00, 0x80, 0x00, 0x7f, 0x00, 0x32, 0x00, 0x71, 0x00, 0x40, 0x7d, 0x5d, 0x70,
	0x60, 0xe2, 0xd4, 0x5b, 0x08, 0x3c, 0x01, 0x5c, 0x48, 0x42, 0x9f, 0x00, 0x80, 0x63, 0x10, 0x73,
	0xb0, 0xf9, 0xc8, 0x5a, 0x3c, 0x41, 0xc6, 0x00, 0x80, 0x43, 0x00, 0x78, 0x01, 0x40, 0x00, 0x78,
	0xc2, 0xc0, 0x03, 0x4d, 0x02, 0x19, 0x08, 0xa3, 0x06, 0x41, 0x03, 0x4c, 0x01, 0xc4, 0x00, 0x3c,
	0xe8, 0xc2, 0x9b, 0x58, 0x0a, 0x6e, 0x40, 0x11, 0x01, 0x3c, 0x01, 0x28, 0x45, 0x42, 0x9f, 0x00,
	0x80, 0x67, 0x00, 0x73, 0x96, 0xf9, 0xc8, 0x58, 0x00, 0x40, 0x9f, 0x52, 0x4b, 0x41, 0xc6, 0x00,
	0x90, 0xc1, 0x00, 0xad, 0x0a, 0x6e, 0x40, 0x11, 0x01, 0x3c, 0x11, 0x28, 0x45, 0x42, 0x9f, 0x00,
	0x80, 0x66, 0x00, 0x73, 0x86, 0xf9, 0xc8, 0x58, 0x00, 0x40, 0x9f, 0x52, 0x4b, 0x41, 0xc6, 0x00,
	0x90, 0xc1, 0x00, 0xad, 0x00, 0x70, 0x00, 0x00, 0x20, 0x40, 0x00, 0x75, 0x10, 0x1c, 0xf4, 0xeb,
	0x14, 0x40, 0x00, 0x5c, 0x68, 0x3a, 0x09, 0x30, 0x00, 0x70, 0x00, 0x00, 0x40, 0x40, 0x00, 0x75,
	0x10, 0x48, 0x00, 0x5c, 0x30, 0xe0, 0x00, 0x7d, 0x5e, 0x45, 0x42, 0x9f, 0x00, 0x80, 0x64, 0x00,
	0x73, 0x01, 0x3c, 0x08, 0x3a, 0x62, 0xf9, 0xc8, 0x5a, 0x45, 0x42, 0x9f, 0x00, 0x80, 0x65, 0x11,
	0x73, 0x60, 0x79, 0xc8, 0x5a, 0x02, 0xe0, 0x30, 0x73, 0x15, 0x76, 0x04, 0x00, 0x10, 0xc0, 0x08,
	0x17, 0xa0, 0x40, 0x80, 0x75, 0x11, 0x40, 0x60, 0x70, 0x10, 0x1c, 0xf4, 0xeb, 0x12, 0xc0, 0x00,
	0x5c, 0x00, 0xc0, 0xa0, 0x70, 0xe0, 0xc0, 0x80, 0xc6, 0x00, 0x75, 0x80, 0x85, 0x14, 0x48, 0x20,
	0x5c, 0x30, 0xe0, 0x80, 0x7d, 0x5e, 0x00, 0x43, 0x20, 0x85, 0x0e, 0x48, 0x20, 0x5c, 0x10, 0xe0,
	0x80, 0x7d, 0x5e, 0x45, 0x42, 0x9f, 0x00, 0x80, 0x62, 0x11, 0x73, 0x01, 0x3c, 0x08, 0x3a, 0x36,
	0xf9, 0xc8, 0x5a, 0x45, 0x42, 0x9f, 0x00, 0x80, 0x63, 0x11, 0x73, 0x34, 0x79, 0xc8, 0x5a, 0x02,
	0xe0, 0x30, 0x73, 0x13, 0x76, 0x04, 0x00, 0x60, 0xc0, 0x08, 0x17, 0x50, 0x60, 0x40, 0x73, 0x10,
	0x1c, 0xf4, 0xeb, 0x45, 0x42, 0x9f, 0x00, 0x80, 0x61, 0x10, 0x73, 0x24, 0x79, 0xc8, 0x5a, 0x02,
	0xc0, 0x00, 0x7c, 0x13, 0x76, 0x04, 0x00, 0x20, 0xc0, 0x08, 0x17, 0x11, 0x40, 0x60, 0x70, 0x20,
	0x1c, 0xf4, 0xeb, 0xc0, 0x7f, 0xff, 0x5b, 0x00, 0xc0, 0x25, 0x49, 0x10, 0x40, 0x60, 0x70, 0xe8,
	0x7f, 0xff, 0x5b, 0x00, 0xc0, 0x25, 0x49, 0xc1, 0x40, 0x00, 0x78, 0x0c, 0x61, 0x40, 0x10, 0x00,
	0xc5, 0xa5, 0x48, 0x49, 0x42, 0x9f, 0x00, 0x00, 0x40, 0x9e, 0x7c, 0x0e, 0xc0, 0x00, 0x58, 0x20,
	0x40, 0x50, 0xdd, 0x0a, 0x48, 0x20, 0x5c, 0x4a, 0x42, 0x9f, 0x00, 0x80, 0xe0, 0x10, 0x73, 0x18,
	0xc0, 0x02, 0x16, 0x00, 0x40, 0x7d, 0x5d, 0x70, 0x9e, 0xe1, 0xd4, 0x5b, 0x26, 0x40, 0x00, 0x78,
	0x32, 0x41, 0xc6, 0x00, 0x07, 0x3c, 0x05, 0x4b, 0x64, 0x45, 0x10, 0xc4, 0x86, 0xf0, 0x05, 0x3b,
	0x91, 0x30, 0x12, 0x28, 0x4a, 0x42, 0x9f, 0x00, 0x80, 0x41, 0x00, 0x78, 0x49, 0x11, 0x47, 0xa1,
	0x03, 0x40, 0x00, 0x78, 0xde, 0xf8, 0xc8, 0x5a, 0x26, 0xc2, 0x9b, 0x58, 0x60, 0x41, 0x00, 0x75,
	0x30, 0x60, 0x00, 0x7d, 0x5e, 0x10, 0x1c, 0xf4, 0xeb, 0xa0, 0x60, 0x41, 0x73, 0x08, 0xc0, 0x00,
	0x5c, 0x30, 0xe2, 0x41, 0x73, 0x10, 0xc0, 0x30, 0xf1, 0x45, 0x42, 0x9f, 0x00, 0x80, 0x60, 0x10,
	0x73, 0xca, 0x78, 0xc8, 0x5a, 0x02, 0xc0, 0x00, 0x7c, 0x10, 0x76, 0x04, 0x00, 0x38, 0xc0, 0x08,
	0x17, 0x10, 0x41, 0x00, 0xf5, 0x10, 0x1c, 0xf4, 0xeb, 0x44, 0x42, 0x9f, 0x00, 0x80, 0x64, 0x11,
	0x73, 0xba, 0x78, 0xc8, 0x5a, 0x02, 0xe0, 0x30, 0x73, 0x00, 0x40, 0xb1, 0x70, 0x10, 0xcb, 0x09,
	0x11, 0xe0, 0xc0, 0x80, 0xc6, 0x38, 0x40, 0x00, 0x00, 0xe1, 0x43, 0x00, 0x76, 0x26, 0xc0, 0x43,
	0x24, 0x00, 0x70, 0x00, 0x76, 0x1e, 0xc0, 0x42, 0x24, 0x44, 0x42, 0x9f, 0x00, 0x80, 0x65, 0x11,
	0x73, 0xa2, 0x78, 0xc8, 0x5a, 0x02, 0xc0, 0x00, 0x7c, 0x00, 0x40, 0xb0, 0x70, 0x16, 0xd1, 0x08,
	0x11, 0xe0, 0xc0, 0x80, 0xc6, 0x08, 0x4c, 0x00, 0x00, 0x21, 0x44, 0x00, 0x76, 0x0e, 0xc0, 0x43,
	0x24, 0xc0, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x76, 0x0a, 0xc0, 0x02, 0x24, 0x0c, 0xc0, 0x08,
	0x16, 0x0a, 0xc1, 0x08, 0x16, 0x44, 0x42, 0x9f, 0x00, 0x80, 0x66, 0x10, 0x73, 0x01, 0x3c, 0x08,
	0x3a, 0x7d, 0x5e, 0xf8, 0xc8, 0x5a, 0x44, 0x42, 0x9f, 0x00, 0x80, 0x67, 0x10, 0x73, 0x7c, 0x78,
	0xc8, 0x5a, 0x02, 0xc0, 0x00, 0x7c, 0x0e, 0x76, 0x04, 0x00, 0x00, 0xc0, 0x08, 0x17, 0xe1, 0x7f,
	0x24, 0x49, 0x03, 0xc0, 0x9d, 0xa0, 0x74, 0x41, 0x9b, 0x5a, 0x20, 0xc0, 0x25, 0x49, 0xaa, 0xff,
	0xff, 0x5b, 0x10, 0x40, 0x60, 0x70, 0x8e, 0x7f, 0xff, 0x5b, 0xe1, 0x7f, 0x24, 0x49, 0x20, 0xc0,
	0x25, 0x49, 0xe2, 0x7f, 0xdf, 0x78, 0x01, 0xc2, 0xa5, 0x48, 0x01, 0x39, 0x31, 0x29, 0x49, 0x42,
	0x9f, 0x00, 0x00, 0x40, 0x82, 0x7c, 0x10, 0x40, 0x00, 0x5c, 0xff, 0xe1, 0x04, 0x48, 0x20, 0x40,
	0x50, 0xdd, 0x0c, 0x48, 0x20, 0x5c, 0x49, 0x42, 0x9f, 0x00, 0x80, 0xe1, 0x10, 0x73, 0x02, 0x40,
	0x00, 0x7c, 0x5e, 0xc0, 0x00, 0x58, 0xbe, 0xce, 0xfe, 0x5b, 0x3d, 0x41, 0xc6, 0x00, 0x94, 0x70,
	0x30, 0xd8, 0x12, 0x40, 0x60, 0x70, 0x00, 0xc0, 0x7d, 0x5d, 0x70, 0x11, 0x41, 0x10, 0xb0, 0xe4,
	0xe0, 0xd4, 0x5b, 0x23, 0x40, 0x00, 0x78, 0x49, 0x42, 0x9f, 0x00, 0x80, 0x33, 0x40, 0x69, 0xa2,
	0x43, 0xc8, 0x00, 0x04, 0x47, 0x80, 0x49, 0x67, 0xc1, 0x03, 0x4d, 0x21, 0x40, 0x44, 0x91, 0x01,
	0xcb, 0xb0, 0xa1, 0x01, 0x40, 0xc1, 0x70, 0x05, 0x1c, 0x42, 0x28, 0x22, 0x44, 0x02, 0x8c, 0x02,
	0xca, 0xb0, 0xa1, 0x02, 0x40, 0xc2, 0x70, 0xa6, 0x47, 0x44, 0x91, 0x06, 0xc2, 0xb0, 0xa1, 0xa6,
	0x42, 0x64, 0x91, 0x08, 0xc7, 0x10, 0xa1, 0x26, 0x41, 0x06, 0x8c, 0x07, 0xc2, 0xb0, 0xa1, 0x66,
	0x5d, 0x44, 0x91, 0x01, 0xca, 0xb1, 0xa1, 0x44, 0xdd, 0x44, 0x91, 0xa4, 0xdf, 0x04, 0x8e, 0x04,
	0x41, 0x04, 0x8c, 0x02, 0xca, 0xb1, 0xa1, 0x9d, 0x40, 0x00, 0x00, 0xc4, 0xc0, 0x52, 0x91, 0x2e,
	0x80, 0x7e
};

void test_real_escaped_sample()
{
	printf("Starting Real Escaped Sample Test\n");

	printf("Testing Data:\n");

	hexdump((uint8_t*)test_real_escaped_sample_data, sizeof(test_real_escaped_sample_data));

	uint8_t* out = NULL;
	size_t outSize = 0;

	hdlc_response((uint8_t*)test_real_escaped_sample_data, sizeof(test_real_escaped_sample_data), &out, outSize);

	printf("Resulting Unescaped Data:\n");

	hexdump(out, outSize);

	free(out);

}

void test_vector_basic()
{
	printf("Starting Basic Vector Encapsulation Test\n");

	vector<uint8_t> test_hdlc_basic_v(test_hdlc_basic, test_hdlc_basic + 4);

	printf("Testing Data:\n");

	hexdump(&test_hdlc_basic_v[0], test_hdlc_basic_v.size());

	hdlc_request(test_hdlc_basic_v);

	printf("Resulting Encapsulated Data:\n");

	hexdump(&test_hdlc_basic_v[0], test_hdlc_basic_v.size());

	if (test_hdlc_basic_v.size() != sizeof(test_hdlc_basic_encapsulated)) {
		printf("Test Failed. Encapsulated data is not the expected size\n");
		return;
	}

	for (int i = 0; i < test_hdlc_basic_v.size(); i++) {
		if (test_hdlc_basic_v[i] != test_hdlc_basic_encapsulated[i]) {
			printf("Test Failed. Encapsulated data contains unexpected data\n");
			return;
		}
	}

	printf("Basic Vector Encapsulation: PASS\n");

}

void test_vector_escape()
{
	printf("Starting Vector Escaping Test\n");

	vector<uint8_t> test_hdlc_escape_v(test_hdlc_escape, test_hdlc_escape + 10);

	printf("Testing Data:\n");

	hexdump(&test_hdlc_escape_v[0], test_hdlc_escape_v.size());

	hdlc_request(test_hdlc_escape_v);

	printf("Resulting Encapsulated Data\n");

	hexdump(&test_hdlc_escape_v[0], test_hdlc_escape_v.size());
	
	if (test_hdlc_escape_v.size() != sizeof(test_hdlc_escape_encapsulated)) {
		printf("Test Failed. Encapsulated data is not the expected size - %lu %li", test_hdlc_escape_v.size(), sizeof(test_hdlc_escape_encapsulated));
		return;
	}

	for (int i = 0; i < test_hdlc_escape_v.size(); i++) {
		if (test_hdlc_escape_v[i] != test_hdlc_escape_encapsulated[i]) {
			printf("Test Failed. Encapsulated data contains unexpected data");
			return;
		}
	}
	
	printf("Escaping Vector: PASS\n"); 
}

void test_vector_unescape()
{
	printf("Starting Vector Unescape Test\n");

	vector<uint8_t> test_hdlc_escape_v(test_hdlc_escape, test_hdlc_escape + 10);

	printf("Testing Data:\n");

	hexdump(&test_hdlc_escape_v[0], test_hdlc_escape_v.size());

	hdlc_request(test_hdlc_escape_v);
	
	printf("Resulting Encapsulated Data\n");

	hexdump(&test_hdlc_escape_v[0], test_hdlc_escape_v.size());

	hdlc_response(test_hdlc_escape_v);

	printf("Resulting Unescaped Data\n");

	hexdump(&test_hdlc_escape_v[0], test_hdlc_escape_v.size());

	if (test_hdlc_escape_v.size() != sizeof(test_hdlc_escape)) {
		printf("Test Failed. Encapsulated data is not the expected size - %lu %li", test_hdlc_escape_v.size(), sizeof(test_hdlc_escape));
		return;
	}

	for (int i = 0; i < test_hdlc_escape_v.size(); i++) {
		if (test_hdlc_escape_v[i] != test_hdlc_escape[i]) {
			printf("Test Failed. Encapsulated data contains unexpected data");
			return;
		}
	}

	printf("Unescaping Vector: PASS\n");
}

void test_basic()
{
	printf("Starting Basic Encapsulation Test\n");

	printf("Testing Data:\n");

	hexdump((uint8_t*)test_hdlc_basic, sizeof(test_hdlc_basic));

	uint8_t* out = NULL;
	size_t outSize = 0;

	hdlc_request((uint8_t*)test_hdlc_basic, sizeof(test_hdlc_basic), &out, outSize);
	
	printf("Resulting Encapsulated Data:\n");

	hexdump(out, outSize);

	if (outSize  != sizeof(test_hdlc_basic_encapsulated)) {
		free(out);
		printf("Test Failed. Encapsulated data is not the expected size\n");
		return;
	}

	for (int i = 0; i < outSize; i++) {
		if (out[i] != test_hdlc_basic_encapsulated[i]) {
			free(out);
			printf("Test Failed. Encapsulated data contains unexpected data\n");
			return;
		}
	}

	free(out);

	printf("Basic Encapsulation: PASS\n");
}

void test_escape()
{
	printf("Starting Escaping Test\n");

	printf("Testing Data:\n");

	hexdump((uint8_t*)test_hdlc_escape, sizeof(test_hdlc_escape));

	uint8_t* out = NULL;
	size_t outSize = 0;

	hdlc_request((uint8_t*)test_hdlc_escape, sizeof(test_hdlc_escape), &out, outSize);

	printf("Resulting Encapsulated Data\n");

	hexdump(out, outSize);

	if (outSize != sizeof(test_hdlc_escape_encapsulated)) {
		free(out); 
		printf("Test Failed. Encapsulated data is not the expected size\n");
		return;
	}

	for (int i = 0; i < outSize; i++) {
		if (out[i] != test_hdlc_escape_encapsulated[i]) {
			free(out);
			printf("Test Failed. Encapsulated data contains unexpected data\n");
			return;
		}
	}

	free(out);

	printf("Escaping Vector: PASS\n");
}

void test_unescape()
{
	printf("Starting Unescape Test\n");


	printf("Testing Data:\n");

	hexdump((uint8_t*)test_hdlc_escape, sizeof(test_hdlc_escape));

	uint8_t* out = NULL;
	size_t outSize = 0;


	hdlc_request((uint8_t*)test_hdlc_escape, sizeof(test_hdlc_escape), &out, outSize);

	printf("Resulting Encapsulated Data\n");

	hexdump(out, outSize);

	uint8_t* unescaped = NULL;
	size_t unescapedSize = 0;

	hdlc_response(out, outSize, &unescaped, unescapedSize);

	printf("Resulting Unescaped Data\n");

	hexdump(&unescaped[0], unescapedSize);

	if (unescapedSize != sizeof(test_hdlc_escape)) {
		free(out);
		free(unescaped);
		printf("Test Failed. Encapsulated data is not the expected size\n");
		return;
	}

	for (int i = 0; i < unescapedSize; i++) {
		if (unescaped[i] != test_hdlc_escape[i]) {
			free(out);
			free(unescaped); 
			printf("Test Failed. Encapsulated data contains unexpected data\n");
			return;
		}
	}

	free(out);
	free(unescaped);

	printf("Unescaping: PASS\n");
}


int main() {

	test_real_escaped_sample();

	printf("\n\n------------\nStarting Standard Tests\n------------\n\n");
	test_basic();
	test_escape();
	test_unescape(); 
	
	printf("\n\n------------\nStarting Vector Tests\n------------\n\n");
	test_vector_basic();
	test_vector_escape();
	test_vector_unescape();

	
	cout << "\n\nPress Enter To Exit" << endl;
	int pause = getchar();
	return 0;
}
