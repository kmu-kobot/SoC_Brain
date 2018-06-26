#ifndef __USB_DOWNLOAD_H__
#define __USB_DOWNLOAD_H__

#define USBBULK_DEV_NAME "/dev/usb_bulk"

#define USB_RESET		0x12300
#define USB_INIT		0x12301
#define USB_BUF_INIT	0x12302
#define USB_OFF			0x12303

#define VCMD_FILE_SIZE 	"File:size:"		//File:open: filename
#define VCMD_FILE_OPEN 	"File:open:"		//File:open: filename, flags
#define VCMD_FILE_READ 	"File:pread:"	//File:pread: fd, count, offset
#define VCMD_FILE_WRITE	"File:pwrite:"	//File:pwrite: fd, offset, data
#define VCMD_FILE_CLOSE "File:close:"	//File:close: fd

#endif // __USB_DOWNLOAD_H__
