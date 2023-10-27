#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int fd;
char * filePath = "/home/tung/Documents/usbEncrypt/test_file.txt";

void endryptFile()
{
    char write_buf[1024] = "";
    snprintf(write_buf, 1024, "%d&%s\0", 1, filePath);
    printf("concat string: %s\n", write_buf);
    write(fd, write_buf, strlen(write_buf));
}

void decryptFile()
{
    char write_buf[1024] = "";
    snprintf(write_buf, 1024, "%d&%s\0", 0, filePath);
    printf("concat string: %s\n", write_buf);
    write(fd, write_buf, strlen(write_buf));
}

int main()
{

    fd = open("/dev/ecryptDriver0", O_RDWR); // mo de doc va ghi
    if (fd < 0)
    {
        printf("Cannot open device file...\n");
        return 0;
    }

    int choice;

    do
    {
        printf("\nFile Management\n");
        printf("1. Encrypt file\n");
        printf("2. Decrypt file\n");
        printf("3. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            endryptFile();
            break;
        case 2:
            decryptFile();
            break;
            break;
        case 3:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 3);

    return 0;
}

