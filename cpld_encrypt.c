#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "cpld_encrypt.h"

/*
 *	CPLD代码参考
 *
 *	写进程块
=================================================================
reg [7:0] encrypt_tmp;
always @ (posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        encrypt_tmp   <= 8'H00;
    end else if (bus_cpld_we && (bus_addr==ADDR_ENCRYPT)) begin
        encrypt_tmp[0] <= bus_data[1] ^ bus_data[7];
        encrypt_tmp[1] <= bus_data[6] ^ bus_data[4];
        encrypt_tmp[2] <= bus_data[5] ^ bus_data[6];
        encrypt_tmp[3] <= bus_data[0] ^ bus_data[3];
        encrypt_tmp[4] <= bus_data[2] ^ bus_data[5];
        encrypt_tmp[5] <= bus_data[3] ^ bus_data[2];
        encrypt_tmp[6] <= bus_data[7] ^ bus_data[0];
        encrypt_tmp[7] <= bus_data[4] ^ bus_data[1];
    end else;
end
=================================================================
 * 	读进程块
=================================================================

always@(posedge clk or negedge rst_n)
    if (!rst_n)
        bus_data_out <= 8'h00;
    else if (bus_cpld_re)
        case(bus_addr)
            ADDR_ENCRYPT            : bus_data_out <= encrypt_tmp ^ 8'H35;
            default                 : bus_data_out <= {8'H00};
        endcase
endmodule
=================================================================
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
#define CPLD_BASE	0xE2000000
#define REG_ENCRYPT	0x1F

int cpld_encrypt_check(char dat)
{
	int fd;
	unsigned char *pbase=NULL;
	char encrypt=0;
	int ret;

	encrypt = 0;
	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (!fd) {
		return -2;
	}
	pbase = mmap(NULL, 0xFF, PROT_READ | PROT_WRITE, MAP_SHARED, fd, CPLD_BASE);
	if (pbase==MAP_FAILED) {
		return -3;
	}

	//手动位异或加密（在写CPLD时进行）
	encrypt |= (((dat>>1)&0x1) ^ ((dat>>7)&0x1))<<0;
	encrypt |= (((dat>>6)&0x1) ^ ((dat>>4)&0x1))<<1;
	encrypt |= (((dat>>5)&0x1) ^ ((dat>>6)&0x1))<<2;
	encrypt |= (((dat>>0)&0x1) ^ ((dat>>3)&0x1))<<3;
	encrypt |= (((dat>>2)&0x1) ^ ((dat>>5)&0x1))<<4;
	encrypt |= (((dat>>3)&0x1) ^ ((dat>>2)&0x1))<<5;
	encrypt |= (((dat>>7)&0x1) ^ ((dat>>0)&0x1))<<6;
	encrypt |= (((dat>>4)&0x1) ^ ((dat>>1)&0x1))<<7;
	//手动字节异或加密（在读CPLD时进行）
	encrypt ^= 0x35;

	//写数据并读回比较
	pbase[REG_ENCRYPT] = dat;

	if (encrypt == pbase[REG_ENCRYPT]) {
		ret = 0;
	} else {
		ret = -1;
	}

	munmap((void *)pbase, 0xFF);

	close(fd);

	return ret;
}

#ifdef ENCRYPT_DEBUG
int main(int argc, char **argv)
{
	int ret;
	if (argc<2) {
		printf("Usage: %s value\n", argv[0]);
		return -1;
	}
	ret = cpld_encrypt_check(atoi(argv[1]));
	if (!ret) {
		printf("Encrypt check failed.\n");
	} else {
		printf("Encrypt check success.\n");
	}
}
#endif
