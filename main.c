#include <stdlib.h>
#include <stdio.h>  
#include <string.h>
#include "cJSON.h"
#include "fop.h"
#include "json_op.h"

int node_get(struct op *o)
{
	int retvalue = 1;
	if (o == NULL) {
		retvalue = -1;
		goto end;
	}

	op_high_input(0, o, "a", "b", "c");
	op_high_input(1, o, "d", "e", "f");
	op_high_input(2, o, "g", "h", "i");
	op_high_input(3, o, "j", "k", "l");
end:
	return retvalue;
}

/*user callback*/
int low_output_cb(int arg, char *s, int size)
{
	int retvalue = 1;
	if (s == NULL) {
		retvalue = -1;
		goto end;
	}
	//print("%s [%d]\n", s, size);
	file_write(arg, s, size);
end:
	return retvalue;
}

/*user callback*/
int high_output_cb(char *a, char *b, char *c)
{
	int retvalue = 1;
	if ((a == NULL) || (b == NULL) || (c == NULL)) {
		retvalue = -1;
		goto end;
	}
	print("[%s] [%s] [%s]\n", a, b, c);
end:
	return retvalue;
}

/*user callback*/
int low_input_cb(int arg, char *s, int size)
{
	int retvalue = 1;

	print("\n");
	size = file_read(arg, s, size);
	print("\n");
	if (size == -1) {
		retvalue = -1;
		goto end;
	}
end:
	return retvalue;
}

int main(int argc, char **argv)
{
	int retvalue = -1;
	int fd;
	if (argc != 2) {
		print("usage: %s file\n", argv[0]);
		return -1;
	}

	fd = file_create(argv[1]);
	if (fd == -1) {
		retvalue = -1;
		goto end;
	}

	struct op *o_obj;
	op_init(&o_obj, fd);
	op_reg_low_output(o_obj, low_output_cb);
	op_reg_high_output(o_obj, high_output_cb);
	op_reg_low_input(o_obj, low_input_cb);

	node_get(o_obj);
	op_low_output(o_obj);

	retvalue = op_low_input(o_obj);
	if (retvalue == -1) {
		goto end;
	}
	int i;
	for (i = 0; i < 10; i++) {
		op_high_output(o_obj, i);
	}
	op_delete(&o_obj);
end:
	return retvalue;
}
