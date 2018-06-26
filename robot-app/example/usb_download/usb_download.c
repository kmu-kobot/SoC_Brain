/*
 *	USB BULK Download Appilcation for uClinux
 *
 *	Copyright (c) 2014 Advanced Digital Chips Inc.
 *
 *	1.00		
 *	1.01
 *	1.02		2014-06-20	Add rsp_proc_read_binary 
 *	1.03		2014-06-26	Add VMCD_FILE_SIZE
 *							Add VCMD_FILE_OPEN
 *							Add VCMD_FILE_CLOSE
 *							Add VCMD_FILE_READ
 *	1.04		2014-06-27	Add VCMD_FILE_WRITE
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>

#include "usb_download.h"

#define USB_DOWNLOAD_VER	"Ver 1.04"
#define RSPVERSION			100

#define DEVICE_NAME			"AMAZON2"

#define WAIT_USB_TIMEOUT    10 // 60 sec

#define MAX_PACKET_SIZE		(1024 * 64)

#define REPLY_DOWN_SIZE 	(1024 * 16)
#define FILEBUF_SIZE    REPLY_DOWN_SIZE

//#define USB_APP_DEBUG

#ifdef USB_APP_DEBUG
#define DEBUG(fmt, arg...)	printf(fmt, ##arg)
#else 
#define DEBUG(fmt, arg...)
#endif

#define ER_NUM_MEM_READ 	1
#define ER_NUM_MEM_WRITE 	2
#define ER_NUM_PACKET 		3
#define ER_NUM_HBREAK_WRITE	4
#define ER_NUM_HBREAK_READ 	5
#define ER_NUM_HWATCH_WRITE	6
#define ER_NUM_HWATCH_READ 	7
#define ER_NUM_EACCES		13
#define ER_NUM_EFAULT		14
#define ER_NUM_EBUSY		16
#define ER_NUM_EEXIST		17
#define ER_NUM_EUNKNOWN		9999

static const char *DIGITS = "0123456789abcdef";
static int flash_sector_size = 4 * 1024;
static int flash_size = 512 * 1024;
static int ram_size = 0x1000000;

static char packetbuf[MAX_PACKET_SIZE+1];
static char readbuf[MAX_PACKET_SIZE];

static int __usbbulk_dev;

FILE *g_fp = NULL;
static int g_fsize = 0;
static char *g_fname = NULL;
static unsigned int g_flag;

static int hex (char ch)
{
	if ((ch >= 'a') && (ch <= 'f'))
		return (ch - 'a' + 10);
	if ((ch >= '0') && (ch <= '9'))
		return (ch - '0');
	if ((ch >= 'A') && (ch <= 'F'))
		return (ch - 'A' + 10);
	return (-1);
}

static char* mem2hex(unsigned char* mem,char* buf,int count)
{
	int i;
	unsigned char ch;
	for (i=0;i<count;i++) {
		ch = *mem;
		*buf++ = DIGITS[ch >> 4];
		*buf++ = DIGITS[ch & 0xf];
		mem++;
	}
	return(buf);
}

int get_char(char* ch)
{
	int timeout = 1000;
	while(1) {
		if(read(__usbbulk_dev, ch, 1))
			break;
		if(timeout < 0)
			return -1;
		timeout--;
	}
	return 1;
}

int put_char(unsigned char ch)
{
	//write_usb(&ch,1);
	write(__usbbulk_dev, &ch, 1);
	return 1;
}

int put_string(unsigned char* str,unsigned int len)
{
	//write_usb(str,len);
	write(__usbbulk_dev, str, len);
	return 0;
}

/* Convert BUFFER, binary data at least LEN bytes long, into escaped
 * binary data in OUT_BUF.  Set *OUT_LEN to the length of the data
 * encoded in OUT_BUF, and return the number of bytes in OUT_BUF
 * (which may be more than *OUT_LEN due to escape characters).  
 * The total number of bytes in the output buffer will be at most OUT_MAXLEN.  
 */

static int remote_escape_output (const unsigned char *buffer, int len, unsigned char *out_buf, int *out_len, int out_maxlen)
{
	int input_index, output_index;

	output_index = 0;

	for (input_index = 0; input_index < len; input_index++)
	{
		unsigned char b = buffer[input_index];

		if (b == '$' || b == '#' || b == '}' || b == '*')
		{
			/* These must be escaped.  */
			if (output_index + 2 > out_maxlen)
				break;
			out_buf[output_index++] = '}';
			out_buf[output_index++] = b ^ 0x20;
		}
		else
		{
			if (output_index + 1 > out_maxlen)
				break;
			out_buf[output_index++] = b;
		}
	}

	*out_len = input_index;

	return output_index;
}

/* Convert BUFFER, escaped data LEN bytes long, into binary data
 * in OUT_BUF.  Return the number of bytes written to OUT_BUF.
 * Raise an error if the total number of bytes exceeds OUT_MAXLEN.
 *
 * This function reverses remote_escape_output.  It allows more
 * escaped characters than that function does, in particular because
 * '*' must be escaped to avoid the run-length encoding processing
 * in reading packets.  
 * */

static int	remote_unescape_input (const unsigned char *buffer, int len, unsigned char *out_buf, int out_maxlen)
{
	int input_index, output_index;
	int escaped;

	output_index = 0;
	escaped = 0;
	for (input_index = 0; input_index < len; input_index++)
	{
		unsigned char b = buffer[input_index];

		if (output_index + 1 > out_maxlen)
			printf("Received too much data from the target.");

		if (escaped)
		{
			out_buf[output_index++] = b ^ 0x20;
			escaped = 0;
		}
		else if (b == '}')
			escaped = 1;
		else
			out_buf[output_index++] = b;
	}

	if (escaped)
		printf("Unmatched escape character in target response.");

	return output_index;
}

/*
	'#' : 0x23
	'$'	: 0x24
*/
int getpkt(char* packet_buffer)
{
	char checksum;
	char xmitcsum;
	int  count;
	unsigned char ch;
	
	//printf("[%s]-[%d] : %s\n", __FILE__, __LINE__, __func__);

	do {
		/* wait around for the start character, ignore all other characters */
		while(1)
		{
			int rev = get_char(&ch);
			if(rev < 0)
				return -1;
			else if(rev ==0)
				return 0;
				
			if( (ch&0x7f) == '$')
				break;
			else if((ch&0x7f)=='\003')	//remote interrupt
			{
				return 0;
			}
		}
		
		checksum = 0;
		xmitcsum = 0;
		count = 0;

		/* now, read until a # or end of buffer is found */
		while (count < MAX_PACKET_SIZE) {
			if(get_char(&ch) == -1)
				return 0;
			if (ch == '#') break;
			
			checksum += ch;

			packet_buffer[count] = ch;
			count = count + 1;
		}
		
		packet_buffer[count] = 0;
		
		if (ch == '#') {
			if(get_char(&ch)==-1)
				return 0;
			xmitcsum = (char)hex(ch & 0x7f) << 4;
			
			if(get_char(&ch)==-1)
				return 0;
			xmitcsum += (char)hex(ch& 0x7f);
			
			if (checksum != xmitcsum ) {
				DEBUG ("checksum error\n");
				put_char('-');  /* failed checksum */	//bug?
			}
			else
			{
				put_char('+');  /* successful transfer */
			}
		} else {
			printf("packet receive error\n");
			return -1;
		}
	} while (checksum != xmitcsum);
	return count;
}

int putpkt(char* packetdata,int len)
{
	char reply;
	unsigned char checksum=0;
	int i;

	DEBUG("putpkt\n");
	DEBUG("%s\n", packetdata);
	
	for(i = 0; i < len; i++)
	{
		checksum += packetdata[i];
	}
	
	while(1)
	{
		put_char('$');
		if(len > 0)
			put_string(packetdata,len);
		put_char('#');
		put_char(DIGITS[(checksum>>4) & 0xf]);
		put_char(DIGITS[checksum & 0xf]);
		if(get_char(&reply) == -1)
			return -1;
		if(reply == '+')
			break;
		else if(reply == '-') {
			DEBUG("-");
			continue;
		}
		else if(reply == 0x3) //ctrl+c
			break;
		else if(reply == '$')
			break;//new packet?
		else
			return -1;//connection closed?
	}

	return 1;
}

static int send_error(int err)
{
	char str[128];
	sprintf(str, "E%02d", err);
	return putpkt(str, strlen(str));
}

void sendmemorymap(void)
{
	char xml[1024];
	
	sprintf(xml,"<?xml version=\"1.0\"?>\
				<memory-map>\
				<memory type=\"flash\" start=\"0\" length=\"%d\" sectorsize=\"%d\" />\
				<memory type=\"ram\" start=\"0xc0000000\" length=\"%d\" />\
				</memory-map>", flash_size, flash_sector_size, ram_size);

	//printf("%s\n", xml);
	
	if(putpkt(xml,strlen(xml)) < 0)
		printf("putpkt error\n");
	
	//printf("sendmemorymap end\n");
}

void rsp_proc_query(char* packetbuf)
{
	packetbuf++;//skip 'q'
	if(strncmp(packetbuf,"Rcmd,",5)==0)
	{
		//remote command
		packetbuf += 5;
		if(strncmp(packetbuf,"rspv",4)==0)
		{
			int ver = RSPVERSION;
			mem2hex((unsigned char*)&ver,packetbuf,4);
			if(putpkt(packetbuf,8) < 0)
				printf("putpkt error\n");
		}
		else if(strncmp(packetbuf,"whoareyou",9)==0)
		{
			strcpy(packetbuf,DEVICE_NAME);
			if(putpkt(packetbuf,8) < 0)
				printf("putpkt error\n");
		}
	}
	else if(strncmp(packetbuf,"Xfer:",5)==0)
	{
		packetbuf += 5;
		if(strncmp(packetbuf,"memory-map:read",strlen("memory-map:read"))==0)
		{
			sendmemorymap();
		}
		printf("Host is connected\n");
	}
}

void rsp_proc_write_binary(char* packet,int packetsize)		
{
	char *separator;
	unsigned int addr = 0;
	int len = 0;
	//static int total_len = 0;
	
	//printf("[%s]-[%d] : %s\n", __FILE__, __LINE__, __func__);
	
	packet++;		// skip 'X'
	addr = strtoul(packet, &separator, 16);		// skip addr

	if (*separator != ',')
	{
		printf("incomplete write memory binary packet received, dropping connection");
		return;
	}

	strtoul(separator + 1, &separator, 16);

	if (*separator != ':')
	{
		printf("incomplete write memory binary packet received, dropping connection");
		return;
	}
	separator++;

	len = packetsize - (separator - packet) - 1;
	
	int datalen = remote_unescape_input((char*)separator,len,(char*)separator,MAX_PACKET_SIZE);
	fwrite(separator, 1, datalen, g_fp);
	//memorywrite(addr,(BYTE*)separator,datalen);
	g_fsize += datalen;
	//printf("File Write %d (%d) \r\n", datalen, total_len);
	
	if(putpkt("OK",2) < 0)
		printf("putpkt error\n");

	//return total_len;
}

int rsp_proc_read_binary(char* packet)
{
	char *separator;
	unsigned int addr = 0;
	int len = 0;
	int out_len = 0;
	static unsigned int total_len = 0;

	packet++;		// skip 'x'
	addr = strtoul(packet, &separator, 16); 	// skip addr

	if (*separator != ',')
	{
		printf("incomplete write memory binary packet received, dropping connection");
		send_error(ER_NUM_PACKET);
		return -1;
	}

	len = strtoul(separator + 1, NULL, 16);

	if(total_len > g_fsize)
		printf("File Size Error\n");

	if(fread(readbuf, 1, len, g_fp) < 0)
		printf("File Read Error\n");

	total_len += len;
	
	out_len = remote_escape_output(readbuf,len,packetbuf,&len,MAX_PACKET_SIZE);

	//printf("rsp_proc_read_binary : total_len = %d, len = %d, out_len = %d, MAX_PACKET_SIZE : %d\n", total_len, len, out_len,  MAX_PACKET_SIZE);

	if(putpkt((char*)packetbuf, out_len) < 0)
		printf("putpkt error\n");

	return total_len;
}

#define VCMD_RUNAT	"RunAt"

void rsp_proc_v(char* packet,int packetsize)
{
	packet++;	//skip 'v'
	unsigned char buf[MAX_PACKET_SIZE];
	static int up_len = 0;

	DEBUG("rsp_proc_v : %s\n", packet);
	
	if(strncmp(packet, VCMD_RUNAT, strlen(VCMD_RUNAT))==0)
	{
		DEBUG("VCMD_RUNAT\n");
		printf("Exit USB BULK Program\n");

		if(g_fp != NULL) {
			fclose(g_fp);
		}

		// usb off
		ioctl(__usbbulk_dev, USB_OFF, 0);
		exit(1);
	}
	else if (strncmp(packet, VCMD_FILE_SIZE, strlen(VCMD_FILE_SIZE)) == 0)
	{
		FILE *fd;
		char *filename;
		unsigned int size;

		DEBUG("VCMD_FILE_SIZE\n");
		packet += strlen(VCMD_FILE_SIZE);

		filename = packet;
		DEBUG("filename : %s\n", filename);

		if((fd = fopen(filename, "rb")) == (FILE *)NULL) {
			printf("Cannot open read file, %s\n", filename);
			// send_error
			send_error(ER_NUM_EACCES);
			return;
		}

		fseek(fd, 0, SEEK_END);
		size = ftell(fd);
		fclose(fd);

		// send resp
		sprintf(buf, "F %08x", size);
		putpkt(buf, strlen(buf));
	}
	else if (strncmp(packet, VCMD_FILE_OPEN, strlen(VCMD_FILE_OPEN)) == 0)
	{
		char *separator;
		char *filename;
		unsigned int flag;
		
		DEBUG("VCMD_FILE_OPEN\n");
		packet += strlen(VCMD_FILE_OPEN);

		filename = strtok(packet,","); 
		DEBUG("filename : %s\n", filename);

		packet = packet + strlen(filename) + 1;

		flag = strtoul(packet, &separator, 16);

		DEBUG("flag : %d\n", flag);

		if(g_fp != NULL) {
			DEBUG("FILE OPEN ERROR : already open\n");
			// send error
			
			return;
		} else {
			if(flag == 0) {		// Read
				if ((g_fp = fopen(filename, "rb")) == (FILE *)NULL) {
        			printf("Cannot open read file, %s\n", filename);
					// send_error
					send_error(ER_NUM_EACCES);
        			return;
				} else {
					g_fname = (char *)malloc(strlen(filename) + 1);
					strcpy(g_fname, filename);

					fseek(g_fp, 0, SEEK_END);
					g_fsize = ftell(g_fp);
					fseek(g_fp, 0, SEEK_SET);

					g_flag = flag;

					printf("File Upload %s (%d Byte)...\n",g_fname, g_fsize);

					// send resp
					sprintf(buf, "F %08x", (unsigned int)g_fp);
					putpkt(buf, strlen(buf));
				}
			} else if(flag == 1) {
				if ((g_fp = fopen(filename, "wb")) == (FILE *)NULL) {
        			printf("Cannot open read file, %s\n", filename);
					// send_error
					send_error(ER_NUM_EACCES);
        			return;
				} else {
					g_fname = (char *)malloc(strlen(filename) + 1);
					strcpy(g_fname, filename);

					g_fsize = 0;
					g_flag = flag;

					printf("File Download %s ...\n",g_fname);

					// send resp
					sprintf(buf, "F %08x", (unsigned int)g_fp);
					putpkt(buf, strlen(buf));
				}
			}
		}
	}
	else if (strncmp(packet, VCMD_FILE_CLOSE, strlen(VCMD_FILE_CLOSE)) == 0)
	{
		//FILE *fd;
		DEBUG("VCMD_FILE_CLOSE\n");
		packet += strlen(VCMD_FILE_CLOSE);

		if(g_fp != NULL) {
			if(g_flag == 0)
			printf("\t------ File Upload Complete -----\n");
			else if(g_flag == 1)
			printf("\t------ File Download Complete -----\n");
			else
			printf("\t-----------\n");

			printf("\tFileName : %s\n", g_fname);
			printf("\tSize : %d Byte\n", g_fsize);
			free(g_fname);
			g_fsize = 0;
			up_len = 0;
			fclose(g_fp);
			g_fp = (FILE *)NULL;

			// send resp
			sprintf(buf, "F 0");
			putpkt(buf, strlen(buf));

		} else {
			DEBUG("VCMD_FILE_CLOSE : File not opend\n");
			// send error
			send_error(ER_NUM_EACCES);
		}
	}
	else if (strncmp(packet, VCMD_FILE_READ, strlen(VCMD_FILE_READ)) == 0)
	{
		char *separator;
		unsigned int fd;
		unsigned int count;
		unsigned int offset;
		int out_len;

		DEBUG("VCMD_FILE_READ\n");
		packet += strlen(VCMD_FILE_READ);

		fd = strtoul(packet, &separator, 16);
		DEBUG("fd : 0x%08x ", fd);

		if(*separator != ',')
		{
			DEBUG("incomplete packet received, drooping conection\n");
			// send error
			send_error(ER_NUM_PACKET);
			return;
		}
		separator++;
		count = strtoul(separator, &separator, 16);
		DEBUG("count : 0%08x ", count);

		if(*separator != ',')
		{
			DEBUG("incomplete packet received, drooping conection\n");
			// send error
			send_error(ER_NUM_PACKET);
			return;
		}

		separator++;
		offset = strtoul(separator, &separator, 16);
		DEBUG("offset: 0%08x ", offset);

		if(g_fp != NULL) {
			if(fread(readbuf, 1, count, g_fp) < 0)
				DEBUG("File Read Error\n");

			out_len = remote_escape_output(readbuf,count,packetbuf,&count,MAX_PACKET_SIZE);
			sprintf(buf, "F %08x,0;", count);
			int tmp = strlen(buf);
			memcpy(buf + tmp, packetbuf, out_len);
			// send resp
			putpkt(buf, tmp + out_len);

			up_len += count;
			printf("\t %d%% (%d / %d) \n", (int)(((float)up_len / (float)g_fsize) * 100), up_len, g_fsize);
		} else {
			DEBUG("VCMD_FILE_READ : File not opend\n");
			// send error
			send_error(ER_NUM_EACCES);
		}
	}
	else if (strncmp(packet, VCMD_FILE_WRITE, strlen(VCMD_FILE_READ)) == 0)
	{
		char *separator;
		unsigned int fd;
		unsigned int offset;
		int in_len;

		DEBUG("VCMD_FILE_WRITE\n");
		packet += strlen(VCMD_FILE_WRITE);

		fd = strtoul(packet, &separator, 16);
		DEBUG("fd : 0x%08x ", fd);

		if(*separator != ',')
		{
			DEBUG("incomplete packet received, drooping conection\n");
			// send error
			send_error(ER_NUM_PACKET);
			return;
		}
		separator++;

		offset = strtoul(separator, &separator, 16);
		DEBUG("offset : 0x%08x \n", offset);

		if(*separator != ',')
		{
			DEBUG("incomplete packet received, drooping conection\n");
			// send error
			send_error(ER_NUM_PACKET);
			return;
		}
		separator++;

		in_len = packetsize - strlen(VCMD_FILE_WRITE) - (separator - packet) -1;

		int datalen = remote_unescape_input((char*)separator,in_len,(char*)separator,MAX_PACKET_SIZE);

		DEBUG("packetsize : %d, in_len : %d, datalen : %d\n", packetsize, in_len, datalen);
		
		if(fwrite(separator, 1, datalen, g_fp) < 0)
			printf("File Write Error\n");
		g_fsize += datalen;

		// send resp
		sprintf(buf, "F %08x", datalen);
		putpkt(buf, strlen(buf));
	}
	else 
	{
		send_error(ER_NUM_EUNKNOWN);
	}
}

int main(int argc, char *argv[])
{
	int packetlen = 0;
	int file_len = 0;
	static int mode = 0;

	/*
	 * mode
	 * 1 : rsp_proc_read_memory
	 * 2 : rsp_proc_write_memory
	 * 3 : rsp_proc_query
	 * 4 : rsp_proc_write_binary
	 * 5 : rsp_proc_read_binary
	 * 6 : rsp_proc_file_io
	 * 7 : rsp_proc_v
	 */

	//char packetbuf[MAX_PACKET_SIZE+1];

    printf("\nUSB BULK Download Program (%s)\n", USB_DOWNLOAD_VER);

	/*
    if (argc != 2) {
        printf("Usage: \n");
		printf("\t Download : usb_download [filename]\n");
		printf("\t Upload : usb_download up\n");
        return 0;
    }
	*/

    if ((__usbbulk_dev = open(USBBULK_DEV_NAME, O_RDWR)) < 0) {
        printf("Open Error %s\n", USBBULK_DEV_NAME);
        return 0;
    }

	ioctl(__usbbulk_dev, USB_RESET, 0);

	ioctl(__usbbulk_dev, USB_INIT, 0);

    /* wait version command */
	while(1)
	{
		ioctl(__usbbulk_dev, USB_BUF_INIT, 0);
		packetlen = getpkt(packetbuf);
#if 0
		int i;
		for(i = 0; i < packetlen; i++)
			printf("%c", packetbuf[i]);
		printf("\r\n");
#endif		
		
		switch(packetbuf[0])
		{
		case 'm' :
			mode = 1;
			DEBUG("rsp_proc_read_memory \n");
			break;
		case 'M' :
			mode = 2;
			DEBUG("rsp_proc_write_memory \n");
			break;
		case 'q' :
			mode = 3;
			DEBUG("rsp_proc_query \n");
			rsp_proc_query(packetbuf);
			break;
		case 'X' :
			DEBUG("rsp_proc_write_binary \n");
			if(mode == 3) {
				if ((g_fp = fopen(argv[1], "wb")) == (FILE *)NULL) {
        			printf("Cannot open output file, %s\n", argv[1]);

        			return -1;
    			} else {
					g_fname = argv[1];
					g_fsize = 0;
				}
			}
			mode = 4;
			rsp_proc_write_binary(packetbuf,packetlen);
			break;
		case 'x' : 
			DEBUG("rsp_proc_read_binary \n");
			if(mode == 3) {
				if ((g_fp = fopen(argv[1], "rb")) == (FILE *)NULL) {
					printf("Cannot open input file, %s\n", argv[1]);
					send_error(ER_NUM_EACCES);
					return -1;
				} else {
					g_fname = argv[1];
					fseek(g_fp, 0, SEEK_END);
					g_fsize = ftell(g_fp);
					fseek(g_fp, 0, SEEK_SET);
					printf("Start Download (%d Byte)...\n", g_fsize);
				}
			}

			mode = 5;
			file_len = rsp_proc_read_binary(packetbuf);
			printf("\t %d%% (%d / %d) \n", (int)(((float)file_len / (float)g_fsize) * 100), file_len, g_fsize);
			break;
		case 'F' :
			mode = 6;
			//printf("rsp_proc_file_io \n");
			break;
		case 'v' :
			DEBUG("rsp_proc_v\n");
			rsp_proc_v(packetbuf,packetlen);
/*
			if(mode == 4 || mode == 5) {
				if(rsp_proc_v(packetbuf, packetlen) == 0) {
					fclose(fp);
					printf("\tFileName : %s\n", argv[1]);
					printf("\tSize : %d Byte\n", file_len);
					ioctl(__usbbulk_dev, USB_OFF, 0);
					exit(1);
				}
			}
*/
			mode = 7;
			break;
 		default:
			printf("Unknown Command\n");
			//putpkt(NULL, 0);
			break;
		}
	}

    return -1;
}
