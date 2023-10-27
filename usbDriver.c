#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 12, 0)
#include <crypto/internal/cipher.h>
#endif
#define USB_VENDOR_ID (0x0483)  // USB device's vendor ID
#define USB_PRODUCT_ID (0x3748) // USB device's product ID

#define MAX_BUFFER_SIZE 1024
char pkey[] = "yourKey";

static struct usb_device *device;
static struct usb_class_driver class;
char kernel_buffer[1024];
static struct crypto_cipher *tfm;

static int encrypt_file(char *file_path)
{
	struct file *file;
	loff_t offset = 0;
	ssize_t read_bytes;
	char *buffer;
	buffer = kmalloc(MAX_BUFFER_SIZE, GFP_KERNEL);
	if (!buffer) {
		printk(KERN_ERR "Failed to allocate memory.\n");
		filp_close(file, NULL);
		return -1;
	}
	// Open the file for reading and writing
	file = filp_open(file_path, O_RDWR, 0);

	if (!file || IS_ERR(file))
	{
		printk(KERN_ERR "Error opening the file.\n");
		return -ENOENT;
	}

	// Set the file position to the beginning

	offset = 0;
	vfs_llseek(file, offset, SEEK_SET);

	// Read data from the file
	read_bytes = kernel_read(file, buffer, MAX_BUFFER_SIZE - 1, &offset);
	if (read_bytes < 0)
	{
		printk(KERN_ERR "Error reading the file.\n");
		kfree(buffer);
		filp_close(file, NULL);
		return read_bytes;
	}


	// Initialize the Crypto API cipher
	tfm = crypto_alloc_cipher("aes", 4, CRYPTO_ALG_ASYNC);
	if (IS_ERR(tfm))
	{
		printk(KERN_ERR "Error initializing AES cipher.\n");
		kfree(buffer);
		filp_close(file, NULL);
		return PTR_ERR(tfm);
	}

	crypto_cipher_setkey(tfm, pkey, sizeof(pkey));

	ssize_t temp_size = 0;

	do {
		crypto_cipher_encrypt_one(tfm, buffer + temp_size, buffer + temp_size);
		temp_size += 16;
	} while (temp_size < read_bytes);



	// Set the file position to the beginning
	offset = 0;
	vfs_llseek(file, offset, SEEK_SET);

	// Write the encrypted data back to the file
	kernel_write(file, buffer, strlen(buffer), &offset);

	// Cleanup
	crypto_free_cipher(tfm);
	filp_close(file, NULL);
	kfree(buffer);
	printk(KERN_INFO "File encrypted successfully.\n");
	return 0;
}

static int decrypt_file(char *file_path)
{
	struct file *file;
	loff_t offset = 0;
	ssize_t read_bytes;
 	char *buffer;
    	char *encrypt;
	buffer = kmalloc(MAX_BUFFER_SIZE, GFP_KERNEL);
	if (!buffer) {
		printk(KERN_ERR "Failed to allocate memory.\n");
		filp_close(file, NULL);
		return -1;
	}
	
	encrypt = kmalloc(MAX_BUFFER_SIZE, GFP_KERNEL);
	if (!encrypt) {
		printk(KERN_ERR "Failed to allocate memory.\n");
		filp_close(file, NULL);
		return -1;
	}
	// Open the file for reading and writing
	file = filp_open(file_path, O_RDWR, 0);

	if (!file || IS_ERR(file))
	{
		printk(KERN_ERR "Error opening the file.\n");
		return -ENOENT;
	}

	// Set the file position to the beginning
	offset = 0;
	vfs_llseek(file, offset, SEEK_SET);

	// Read data from the file
	read_bytes = kernel_read(file, buffer, MAX_BUFFER_SIZE - 1, &offset);
	if (read_bytes < 0)
	{
		printk(KERN_ERR "Error reading the file.\n");
		kfree(buffer);
				kfree(encrypt);
		filp_close(file, NULL);
		return read_bytes;
	}

	// Initialize the Crypto API cipher (same key as used for encryption)
	tfm = crypto_alloc_cipher("aes", 4, CRYPTO_ALG_ASYNC);
	if (IS_ERR(tfm))
	{
		printk(KERN_ERR "Error initializing AES cipher.\n");
		kfree(buffer);
				kfree(encrypt);
		filp_close(file, NULL);
		return PTR_ERR(tfm);
	}

	crypto_cipher_setkey(tfm, pkey, sizeof(pkey));

	// Decrypt the data in-place
	ssize_t temp_size = 0;

	do {
		crypto_cipher_decrypt_one(tfm, encrypt + temp_size, buffer + temp_size);
		temp_size += 16;
	} while (temp_size < read_bytes);
	
	
 	filp_close(file, NULL);
    	file = filp_open(file_path, O_WRONLY | O_TRUNC, 0);

	// Set the file position to the beginning
	offset = 0;
	vfs_llseek(file, offset, SEEK_SET);

	// Write the decrypted data back to the file
	kernel_write(file, encrypt, strlen(encrypt), &offset);

	// Cleanup
	crypto_free_cipher(tfm);
	filp_close(file, NULL);
	kfree(buffer);
			kfree(encrypt);
	printk(KERN_INFO "File decrypted successfully.\n");
	return 0;
}

static int usb_open(struct inode *i, struct file *f)
{
	printk("usb open from driver");
	return 0;
}
static int usb_close(struct inode *i, struct file *f)
{
	printk("usb close from driver");
	return 0;
}
static ssize_t usb_read(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
	printk("usb read from driver");
	return 0;
}
static ssize_t usb_write(struct file *f, const char __user *buf, size_t cnt,
		loff_t *off)
{
	int isEncrypt;
	char fileName[100];
	copy_from_user(kernel_buffer, buf, cnt);

	kernel_buffer[cnt] = '\0'; // Null-terminate the data

	sscanf(kernel_buffer, "%d&%99s", &isEncrypt,
			fileName);

	if (isEncrypt == 1)
	{
		printk("Encrypt: %d, %s", isEncrypt, fileName);
		encrypt_file(fileName);
	}
	else
	{
		printk("Decrypt: %d, %s", isEncrypt, fileName);
		decrypt_file(fileName);
	}

	return 0;
}

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = usb_open,
	.release = usb_close,
	.read = usb_read,
	.write = usb_write,
};

static int usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	int retval;

	device = interface_to_usbdev(interface);

	class.name = "usb/ecryptDriver%d";
	class.fops = &fops;
	if ((retval = usb_register_dev(interface, &class)) < 0)
	{
		/* Something prevented us from registering this driver */
		printk(KERN_ERR "Not able to get a minor for this device.");
	}
	else
	{
		printk(KERN_INFO "Minor obtained: %d\n", interface->minor);
	}

	return retval;
}

static void usb_disconnect(struct usb_interface *interface)
{
	usb_deregister_dev(interface, &class);
}

static struct usb_device_id usb_table[] =
{
	{USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID)},
	{} /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, usb_table);

static struct usb_driver usb_driver =
{
	.name = "use driver",
	.id_table = usb_table,
	.probe = usb_probe,
	.disconnect = usb_disconnect,
};

static int __init usb_init(void)
{
	int result;
	result = usb_register(&usb_driver);
	if (result)
	{
		printk("Register Error: %d", result);
		return -result;
	}
	printk("Register success: %d", result);
	return 0;
}

static void __exit usb_exit(void)
{
	usb_deregister(&usb_driver);
}

module_init(usb_init);
module_exit(usb_exit);

MODULE_LICENSE("GPL");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 12, 0)
MODULE_IMPORT_NS(CRYPTO_INTERNAL);
#endif
MODULE_AUTHOR("Use driver");
MODULE_DESCRIPTION("USB Registration Driver");
