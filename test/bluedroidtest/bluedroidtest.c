/******************************************************************************
 *
 *  Copyright (C) 2009-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/************************************************************************************
 *
 *  Filename:      bluedroidtest.c
 *
 *  Description:   Bluedroid Test application
 *
 ***********************************************************************************/

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/prctl.h>
#include <linux/capability.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#ifndef LINUX_NATIVE
#include <private/android_filesystem_config.h>
#include <android/log.h>
#endif
#define LOGE(...)
#define LOGV(...)
#include <hardware/hardware.h>
#include <hardware/bluetooth.h>

/************************************************************************************
**  Constants & Macros
************************************************************************************/

#define PID_FILE "/data/.bdt_pid"

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#define CASE_RETURN_STR(const) case const: return #const;

extern const bt_interface_t* bluetooth__get_bluetooth_interface ();

/************************************************************************************
**  Local type definitions
************************************************************************************/
/* Remote Device */
typedef struct rdev {
	struct rdev	*next;
	bt_bdaddr_t bdaddr;
	uint8_t 	bdname[249];
	uint8_t		cls[3];
	uint32_t	tod;
	uint8_t		rssi;
} rdev_t;

static rdev_t *rdev_list = NULL;
/************************************************************************************
**  Static variables
************************************************************************************/

static unsigned char main_done = 0;
static bt_status_t status;

/* Main API */
static bluetooth_device_t* bt_device;

const bt_interface_t* sBtInterface = NULL;

#ifndef LINUX_NATIVE
static gid_t groups[] = { AID_NET_BT, AID_INET, AID_NET_BT_ADMIN,
                          AID_SYSTEM, AID_MISC, AID_SDCARD_RW,
                          AID_NET_ADMIN, AID_VPN};
#endif

/* Set to 1 when the Bluedroid stack is enabled */
static unsigned char bt_enabled = 0;

/************************************************************************************
**  Static functions
************************************************************************************/

static void process_cmd(char *p, unsigned char is_job);
static void job_handler(void *param);
static void bdt_log(const char *fmt_str, ...);


/************************************************************************************
**  Externs
************************************************************************************/

/************************************************************************************
**  Functions
************************************************************************************/

void add_rdev(rdev_t *dev)
{
	dev->next = rdev_list;
	rdev_list = dev;
}

void flush_rdev(void)
{
	rdev_t *t = NULL;

	while(rdev_list) {
		t = rdev_list->next;
		free(rdev_list);
		rdev_list = t;
	}
}

rdev_t* find_rdev(bt_bdaddr_t *bdaddr)
{
	rdev_t *t = NULL;
	for(t = rdev_list; t; t = t->next) {
		if (memcmp(&t->bdaddr, bdaddr, sizeof(bt_bdaddr_t)) == 0) 
			return t;
	}
	return NULL;
}

/************************************************************************************
**  Shutdown helper functions
************************************************************************************/

static void bdt_shutdown(void)
{
    bdt_log("shutdown bdroid test app\n");
    main_done = 1;
}


/*****************************************************************************
** Android's init.rc does not yet support applying linux capabilities
*****************************************************************************/

static void config_permissions(void)
{
#ifndef LINUX_NATIVE
    struct __user_cap_header_struct header;
    struct __user_cap_data_struct cap;

    bdt_log("set_aid_and_cap : pid %d, uid %d gid %d", getpid(), getuid(), getgid());

    header.pid = 0;

    prctl(PR_SET_KEEPCAPS, 1, 0, 0, 0);

    setuid(AID_BLUETOOTH);
    setgid(AID_BLUETOOTH);

    header.version = _LINUX_CAPABILITY_VERSION;

    cap.effective = cap.permitted =  cap.inheritable =
                    1 << CAP_NET_RAW |
                    1 << CAP_NET_ADMIN |
                    1 << CAP_NET_BIND_SERVICE |
                    1 << CAP_SYS_RAWIO |
                    1 << CAP_SYS_NICE |
                    1 << CAP_SETGID;

    capset(&header, &cap);
    setgroups(sizeof(groups)/sizeof(groups[0]), groups);
#endif
}



/*****************************************************************************
**   Logger API
*****************************************************************************/

void bdt_log(const char *fmt_str, ...)
{
    static char buffer[1024];
    va_list ap;

    va_start(ap, fmt_str);
    vsnprintf(buffer, 1024, fmt_str, ap);
    va_end(ap);

    fprintf(stdout, "%s\n", buffer);
}

/*******************************************************************************
 ** Misc helper functions
 *******************************************************************************/
static const char* dump_bt_status(bt_status_t status)
{
    switch(status)
    {
        CASE_RETURN_STR(BT_STATUS_SUCCESS)
        CASE_RETURN_STR(BT_STATUS_FAIL)
        CASE_RETURN_STR(BT_STATUS_NOT_READY)
        CASE_RETURN_STR(BT_STATUS_NOMEM)
        CASE_RETURN_STR(BT_STATUS_BUSY)
        CASE_RETURN_STR(BT_STATUS_UNSUPPORTED)

        default:
            return "unknown status code";
    }
}

static void hex_dump(char *msg, void *data, int size, int trunc)
{
    unsigned char *p = data;
    unsigned char c;
    int n;
    char bytestr[4] = {0};
    char addrstr[10] = {0};
    char hexstr[ 16*3 + 5] = {0};
    char charstr[16*1 + 5] = {0};

    bdt_log("%s  \n", msg);

    /* truncate */
    if(trunc && (size>32))
        size = 32;

    for(n=1;n<=size;n++) {
        if (n%16 == 1) {
            /* store address for this line */
            snprintf(addrstr, sizeof(addrstr), "%.4x",
               ((unsigned int)p-(unsigned int)data) );
        }

        c = *p;
        if (isalnum(c) == 0) {
            c = '.';
        }

        /* store hex str (for left side) */
        snprintf(bytestr, sizeof(bytestr), "%02X ", *p);
        strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);

        /* store char str (for right side) */
        snprintf(bytestr, sizeof(bytestr), "%c", c);
        strncat(charstr, bytestr, sizeof(charstr)-strlen(charstr)-1);

        if(n%16 == 0) {
            /* line completed */
            bdt_log("[%4.4s]   %-50.50s  %s\n", addrstr, hexstr, charstr);
            hexstr[0] = 0;
            charstr[0] = 0;
        } else if(n%8 == 0) {
            /* half line: add whitespaces */
            strncat(hexstr, "  ", sizeof(hexstr)-strlen(hexstr)-1);
            strncat(charstr, " ", sizeof(charstr)-strlen(charstr)-1);
        }
        p++; /* next byte */
    }

    if (strlen(hexstr) > 0) {
        /* print rest of buffer if not empty */
        bdt_log("[%4.4s]   %-50.50s  %s\n", addrstr, hexstr, charstr);
    }
}

/*******************************************************************************
 ** Console helper functions
 *******************************************************************************/

void skip_blanks(char **p)
{
  while (**p == ' ')
    (*p)++;
}

uint32_t get_int(char **p, int DefaultValue)
{
  uint32_t Value = 0;
  unsigned char   UseDefault;

  UseDefault = 1;
  skip_blanks(p);

  while ( ((**p)<= '9' && (**p)>= '0') )
    {
      Value = Value * 10 + (**p) - '0';
      UseDefault = 0;
      (*p)++;
    }

  if (UseDefault)
    return DefaultValue;
  else
    return Value;
}

int get_signed_int(char **p, int DefaultValue)
{
  int    Value = 0;
  unsigned char   UseDefault;
  unsigned char  NegativeNum = 0;

  UseDefault = 1;
  skip_blanks(p);

  if ( (**p) == '-')
    {
      NegativeNum = 1;
      (*p)++;
    }
  while ( ((**p)<= '9' && (**p)>= '0') )
    {
      Value = Value * 10 + (**p) - '0';
      UseDefault = 0;
      (*p)++;
    }

  if (UseDefault)
    return DefaultValue;
  else
    return ((NegativeNum == 0)? Value : -Value);
}

void get_str(char **p, char *Buffer)
{
  skip_blanks(p);

  while (**p != 0 && **p != ' ')
    {
      *Buffer = **p;
      (*p)++;
      Buffer++;
    }

  *Buffer = 0;
}

uint32_t get_hex(char **p, int DefaultValue)
{
  uint32_t Value = 0;
  unsigned char   UseDefault;

  UseDefault = 1;
  skip_blanks(p);

  while ( ((**p)<= '9' && (**p)>= '0') ||
          ((**p)<= 'f' && (**p)>= 'a') ||
          ((**p)<= 'F' && (**p)>= 'A') )
    {
      if (**p >= 'a')
        Value = Value * 16 + (**p) - 'a' + 10;
      else if (**p >= 'A')
        Value = Value * 16 + (**p) - 'A' + 10;
      else
        Value = Value * 16 + (**p) - '0';
      UseDefault = 0;
      (*p)++;
    }

  if (UseDefault)
    return DefaultValue;
  else
    return Value;
}

void get_bdaddr(const char *str, bt_bdaddr_t *bd) {
    char *d = ((char *)bd), *endp;
    int i;
    for(i = 0; i < 6; i++) {
        *d++ = strtol(str, &endp, 16);
        if (*endp != ':' && i != 5) {
            memset(bd, 0, sizeof(bt_bdaddr_t));
            return;
        }
        str = endp + 1;
    }
}

#define is_cmd(str) ((strlen(str) == strlen(cmd)) && strncmp((const char *)&cmd, str, strlen(str)) == 0)
#define if_cmd(str)  if (is_cmd(str))

typedef void (t_console_cmd_handler) (char *p);

typedef struct {
    const char *name;
    t_console_cmd_handler *handler;
    const char *help;
    unsigned char is_job;
} t_cmd;


const t_cmd console_cmd_list[];
static int console_cmd_maxlen = 0;

static void cmdjob_handler(void *param)
{
    char *job_cmd = (char*)param;

    bdt_log("cmdjob starting (%s)", job_cmd);

    process_cmd(job_cmd, 1);

    bdt_log("cmdjob terminating");

    free(job_cmd);
}

static int create_cmdjob(char *cmd)
{
    pthread_t thread_id;
    char *job_cmd;

    job_cmd = malloc(strlen(cmd)+1); /* freed in job handler */
    strcpy(job_cmd, cmd);

    if (pthread_create(&thread_id, NULL,
                       (void*)cmdjob_handler, (void*)job_cmd)!=0)
      perror("pthread_create");

    return 0;
}

/*******************************************************************************
 ** Load stack lib
 *******************************************************************************/
#ifdef LINUX_NATIVE
static int load(const char *id,
        const char *path,
        const struct hw_module_t **pHmi)
{
    int status;
    void *handle;
    struct hw_module_t *hmi;

    /*
     * load the symbols resolving undefined symbols before
     * dlopen returns. Since RTLD_GLOBAL is not or'd in with
     * RTLD_NOW the external symbols will not be global
     */
    handle = dlopen(path, RTLD_NOW);
    if (handle == NULL) {
        char const *err_str = dlerror();
        printf("load: module=%s\n%s", path, err_str?err_str:"unknown");
        status = -EINVAL;
        goto done;
    }

    /* Get the address of the struct hal_module_info. */
    const char *sym = HAL_MODULE_INFO_SYM_AS_STR;
    hmi = (struct hw_module_t *)dlsym(handle, sym);
    if (hmi == NULL) {
        printf("load: couldn't find symbol %s", sym);
        status = -EINVAL;
        goto done;
    }

    /* Check that the id matches */
    if (strcmp(id, hmi->id) != 0) {
        printf("load: id=%s != hmi->id=%s", id, hmi->id);
        status = -EINVAL;
        goto done;
    }

    hmi->dso = handle;

    /* success */
    status = 0;

    /* Check that the id matches */
    if (strcmp(id, hmi->id) != 0) {
        printf("load: id=%s != hmi->id=%s", id, hmi->id);
        status = -EINVAL;
        goto done;
    }

    hmi->dso = handle;

    /* success */
    status = 0;

done:
    if (status != 0) {
        hmi = NULL;
        if (handle != NULL) {
            dlclose(handle);
            handle = NULL;
        }
    } else {
        printf("loaded HAL id=%s path=%s hmi=%p handle=%p",
                id, path, *pHmi, handle);
    }

    *pHmi = hmi;

    return status;
}

int hw_get_module(const char *id, const struct hw_module_t **module)
{
#if 0
    int status;
    int i;
    const struct hw_module_t *hmi = NULL;
    char prop[PATH_MAX];
    char path[PATH_MAX];

    /*
     * Here we rely on the fact that calling dlopen multiple times on
     * the same .so will simply increment a refcount (and not load
     * a new copy of the library).
     * We also assume that dlopen() is thread-safe.
     */

    /* Loop through the configuration variants looking for a module */
    for (i=0 ; i<HAL_VARIANT_KEYS_COUNT+1 ; i++) {
        if (i < HAL_VARIANT_KEYS_COUNT) {
            if (property_get(variant_keys[i], prop, NULL) == 0) {
                continue;
            }
            snprintf(path, sizeof(path), "%s/%s.%s.so",
                    HAL_LIBRARY_PATH, id, prop);
        } else {
            snprintf(path, sizeof(path), "%s/%s.default.so",
                    HAL_LIBRARY_PATH, id);
        }
        if (access(path, R_OK)) {
            continue;
        }
        /* we found a library matching this id/variant */
        break;
    }

    status = -ENOENT;
    if (i < HAL_VARIANT_KEYS_COUNT+1) {
        /* load the module, if this fails, we're doomed, and we should not try
         * to load a different variant. */
        status = load(id, path, module);
    }
#endif
	status = load(id, "bluetooth.default.so", module);
    return status;
}

#endif


int HAL_load(void)
{
    int err = 0;

    hw_module_t* module;
    hw_device_t* device;

    bdt_log("Loading HAL lib + extensions");

#ifndef LINUX_NATIVE
    err = hw_get_module(BT_HARDWARE_MODULE_ID, (hw_module_t const**)&module);
    if (err == 0)
    {
		
        err = module->methods->open(module, BT_HARDWARE_MODULE_ID, &device);
        if (err == 0) {
            bt_device = (bluetooth_device_t *)device;
            sBtInterface = bt_device->get_bluetooth_interface();
        }
    }
#else
	sBtInterface = bluetooth__get_bluetooth_interface();
#endif

    bdt_log("HAL library loaded (%s) %p \n", strerror(err),sBtInterface);

    return err;
}

int HAL_unload(void)
{
    int err = 0;

    bdt_log("Unloading HAL lib");

    sBtInterface = NULL;

    bdt_log("HAL library unloaded (%s)", strerror(err));

    return err;
}

/*******************************************************************************
 ** HAL test functions & callbacks
 *******************************************************************************/

void setup_test_env(void)
{
    int i = 0;

    while (console_cmd_list[i].name != NULL)
    {
        console_cmd_maxlen = MAX(console_cmd_maxlen, (int)strlen(console_cmd_list[i].name));
        i++;
    }
}

void check_return_status(bt_status_t status)
{
    if (status != BT_STATUS_SUCCESS)
    {
        bdt_log("HAL REQUEST FAILED status : %d (%s)", status, dump_bt_status(status));
    }
    else
    {
        bdt_log("HAL REQUEST SUCCESS");
    }
}

static void adapter_state_changed(bt_state_t state)
{
    bdt_log("ADAPTER STATE UPDATED : %s", (state == BT_STATE_OFF)?"OFF":"ON");
    if (state == BT_STATE_ON) {
        bt_enabled = 1;
    } else {
        bt_enabled = 0;
    }
}

const char* dump_property_type(bt_property_type_t type);

const char* dump_property_type(bt_property_type_t type)
{
    switch(type)
    {
        CASE_RETURN_STR(BT_PROPERTY_BDNAME)
        CASE_RETURN_STR(BT_PROPERTY_BDADDR)
        CASE_RETURN_STR(BT_PROPERTY_UUIDS)
        CASE_RETURN_STR(BT_PROPERTY_CLASS_OF_DEVICE)
        CASE_RETURN_STR(BT_PROPERTY_TYPE_OF_DEVICE)
        CASE_RETURN_STR(BT_PROPERTY_REMOTE_RSSI)
        CASE_RETURN_STR(BT_PROPERTY_ADAPTER_DISCOVERY_TIMEOUT)
        CASE_RETURN_STR(BT_PROPERTY_ADAPTER_BONDED_DEVICES)
        CASE_RETURN_STR(BT_PROPERTY_ADAPTER_SCAN_MODE)
        CASE_RETURN_STR(BT_PROPERTY_REMOTE_FRIENDLY_NAME)

        default:
            return "UNKNOWN PROPERTY ID";
    }
}

/*
 * see http://www.bluetooth.org/assigned-numbers/baseband.htm --- all
 * strings are reproduced verbatim
 */
static char *get_minor_device_name(int major, int minor)
{
	switch (major) {
	case 0:	/* misc */
		return "";
	case 1:	/* computer */
		switch (minor) {
		case 0:
			return "Uncategorized";
		case 1:
			return "Desktop workstation";
		case 2:
			return "Server";
		case 3:
			return "Laptop";
		case 4:
			return "Handheld";
		case 5:
			return "Palm";
		case 6:
			return "Wearable";
		}
		break;
	case 2:	/* phone */
		switch (minor) {
		case 0:
			return "Uncategorized";
		case 1:
			return "Cellular";
		case 2:
			return "Cordless";
		case 3:
			return "Smart phone";
		case 4:
			return "Wired modem or voice gateway";
		case 5:
			return "Common ISDN Access";
		case 6:
			return "Sim Card Reader";
		}
		break;
	case 3:	/* lan access */
		if (minor == 0)
			return "Uncategorized";
		switch (minor / 8) {
		case 0:
			return "Fully available";
		case 1:
			return "1-17% utilized";
		case 2:
			return "17-33% utilized";
		case 3:
			return "33-50% utilized";
		case 4:
			return "50-67% utilized";
		case 5:
			return "67-83% utilized";
		case 6:
			return "83-99% utilized";
		case 7:
			return "No service available";
		}
		break;
	case 4:	/* audio/video */
		switch (minor) {
		case 0:
			return "Uncategorized";
		case 1:
			return "Device conforms to the Headset profile";
		case 2:
			return "Hands-free";
			/* 3 is reserved */
		case 4:
			return "Microphone";
		case 5:
			return "Loudspeaker";
		case 6:
			return "Headphones";
		case 7:
			return "Portable Audio";
		case 8:
			return "Car Audio";
		case 9:
			return "Set-top box";
		case 10:
			return "HiFi Audio Device";
		case 11:
			return "VCR";
		case 12:
			return "Video Camera";
		case 13:
			return "Camcorder";
		case 14:
			return "Video Monitor";
		case 15:
			return "Video Display and Loudspeaker";
		case 16:
			return "Video Conferencing";
			/* 17 is reserved */
		case 18:
			return "Gaming/Toy";
		}
		break;
	case 5:	/* peripheral */ {
		static char cls_str[48];

		cls_str[0] = '\0';

		switch (minor & 48) {
		case 16:
			strncpy(cls_str, "Keyboard", sizeof(cls_str));
			break;
		case 32:
			strncpy(cls_str, "Pointing device", sizeof(cls_str));
			break;
		case 48:
			strncpy(cls_str, "Combo keyboard/pointing device", sizeof(cls_str));
			break;
		}
		if ((minor & 15) && (strlen(cls_str) > 0))
			strcat(cls_str, "/");

		switch (minor & 15) {
		case 0:
			break;
		case 1:
			strncat(cls_str, "Joystick", sizeof(cls_str) - strlen(cls_str));
			break;
		case 2:
			strncat(cls_str, "Gamepad", sizeof(cls_str) - strlen(cls_str));
			break;
		case 3:
			strncat(cls_str, "Remote control", sizeof(cls_str) - strlen(cls_str));
			break;
		case 4:
			strncat(cls_str, "Sensing device", sizeof(cls_str) - strlen(cls_str));
			break;
		case 5:
			strncat(cls_str, "Digitizer tablet", sizeof(cls_str) - strlen(cls_str));
			break;
		case 6:
			strncat(cls_str, "Card reader", sizeof(cls_str) - strlen(cls_str));
			break;
		default:
			strncat(cls_str, "(reserved)", sizeof(cls_str) - strlen(cls_str));
			break;
		}
		if (strlen(cls_str) > 0)
			return cls_str;
	}
	case 6:	/* imaging */
		if (minor & 4)
			return "Display";
		if (minor & 8)
			return "Camera";
		if (minor & 16)
			return "Scanner";
		if (minor & 32)
			return "Printer";
		break;
	case 7: /* wearable */
		switch (minor) {
		case 1:
			return "Wrist Watch";
		case 2:
			return "Pager";
		case 3:
			return "Jacket";
		case 4:
			return "Helmet";
		case 5:
			return "Glasses";
		}
		break;
	case 8: /* toy */
		switch (minor) {
		case 1:
			return "Robot";
		case 2:
			return "Vehicle";
		case 3:
			return "Doll / Action Figure";
		case 4:
			return "Controller";
		case 5:
			return "Game";
		}
		break;
	case 63:	/* uncategorised */
		return "";
	}
	return "Unknown (reserved) minor device class";
}

static void parse_property(rdev_t *rdev, bt_property_type_t type, void *buf0, int len)
{
	static unsigned char prop_buf[1024];
	unsigned char *buf = (unsigned char *)buf0;
	switch (type) {
	case BT_PROPERTY_BDADDR:
		break;
	case BT_PROPERTY_BDNAME:
		if (len >= 0 && len <= 248) {
			memcpy(rdev->bdname, buf, len);
			rdev->bdname[len] = 0;
		}
		break;
	case BT_PROPERTY_CLASS_OF_DEVICE:
	{
		unsigned char *cls = buf;
		rdev->cls[0] = cls[0];
		rdev->cls[1] = cls[1];
		rdev->cls[2] = cls[2];
		break;
	}
	case BT_PROPERTY_TYPE_OF_DEVICE:
	{
		uint32_t tod;
		tod = buf[0] | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24);
		rdev->tod = tod;
		break;
	}
	case BT_PROPERTY_REMOTE_RSSI:
		rdev->rssi = buf[0];
		break;
	default:
		break;
	}
}

void dump_rdev_info(rdev_t *rdev)
{
	char *addr = (char *)&rdev->bdaddr;
	unsigned char *cls = rdev->cls;
	static const char *services[] = { "Positioning",
					"Networking",
					"Rendering",
					"Capturing",
					"Object Transfer",
					"Audio",
					"Telephony",
					"Information" };
	static const char *major_devices[] = { "Miscellaneous",
					"Computer",
					"Phone",
					"LAN Access",
					"Audio/Video",
					"Peripheral",
					"Imaging",
					"Uncategorized" };

	bdt_log("Name: %s", rdev->bdname);
	
    bdt_log("\tAddr: %02x:%02x:%02x:%02x:%02x:%02x",
                   (int)addr[0],(int)addr[1],(int)addr[2],
                   (int)addr[3],(int)addr[4],(int)addr[5]);
	
	printf("\tClass: 0x%02x%02x%02x\n", cls[2], cls[1], cls[0]);
	printf("\tService Classes: ");
	if (cls[2]) {
		unsigned int i;
		int first = 1;
		for (i = 0; i < (sizeof(services) / sizeof(*services)); i++)
			if (cls[2] & (1 << i)) {
				if (!first)
					printf(", ");
				printf("%s", services[i]);
				first = 0;
			}
	} else
		printf("Unspecified");
	printf("\n\tDevice Class: ");
	if ((cls[1] & 0x1f) >= sizeof(major_devices) / sizeof(*major_devices))
		printf("Invalid Device Class!\n");
	else
		printf("%s, %s\n", major_devices[cls[1] & 0x1f],
			get_minor_device_name(cls[1] & 0x1f, cls[0] >> 2));

}

static void device_found_cb(int num_properties, bt_property_t *properties) {
	int i;
    int addr_index = -1;
    rdev_t *rdev;

    for (i = 0; i < num_properties; i++) {
    	if (properties[i].type == BT_PROPERTY_BDADDR) {
    		addr_index = i;
    	}
    }
    if (addr_index < 0) {
		bdt_log("Address is NULL (unable to allocate) in %s", __FUNCTION__);
		return;
	}

	rdev = find_rdev(properties[addr_index].val);
	if (!rdev) {
		rdev = (rdev_t *)malloc(sizeof(rdev_t));
		if (rdev == NULL) {
			bdt_log("rdev is NULL in %s", __FUNCTION__);
			return;
		}
		memcpy(&rdev->bdaddr, properties[addr_index].val, sizeof(bt_bdaddr_t));

		// parse other infomation
		for (i = 0; i < num_properties; i++) {
			if (properties[i].type == BT_PROPERTY_BDADDR) 
				continue;
			parse_property(rdev, properties[i].type, properties[i].val, properties[i].len);
		}

		// print rdev
		dump_rdev_info(rdev);
	}
}

static void discovery_state_changed_cb(bt_discovery_state_t state)
{
	bdt_log("%s", state == BT_DISCOVERY_STARTED ? "starting":"complete");
}

static void dut_mode_recv(uint16_t opcode, uint8_t *buf, uint8_t len)
{
    bdt_log("DUT MODE RECV : NOT IMPLEMENTED");
}

static bt_callbacks_t bt_callbacks = {
    sizeof(bt_callbacks_t),
    adapter_state_changed,
    NULL, /*adapter_properties_cb */
    NULL, /* remote_device_properties_cb */
    device_found_cb, /* device_found_cb */
    discovery_state_changed_cb, /* discovery_state_changed_cb */
    NULL, /* pin_request_cb  */
    NULL, /* ssp_request_cb  */
    NULL, /*bond_state_changed_cb */
    NULL, /* acl_state_changed_cb */
    NULL, /* thread_evt_cb */
    dut_mode_recv, /*dut_mode_recv_cb */
};

void bdt_init(void)
{
    bdt_log("INIT BT ");
    status = sBtInterface->init(&bt_callbacks);
    check_return_status(status);
}

void bdt_enable(void)
{
    bdt_log("ENABLE BT");
    if (bt_enabled) {
        bdt_log("Bluetooth is already enabled");
        return;
    }
    status = sBtInterface->enable();

    check_return_status(status);
}

void bdt_disable(void)
{
    bdt_log("DISABLE BT");
    if (!bt_enabled) {
        bdt_log("Bluetooth is already disabled");
        return;
    }
    status = sBtInterface->disable();

    check_return_status(status);
}

void bdt_discover(void)
{
	bdt_log("DISCOVERING");
	printf("\n %s: %d \n",__FUNCTION__,__LINE__);

#if 0	
    if (!bt_enabled) {
        bdt_log("Bluetooth is disabled, enable it plz.");
        return;
    } 
    flush_rdev();	// clear cache
#endif

    status = sBtInterface->start_discovery();
    check_return_status(status);
}

void bdt_dut_mode_configure(char *p)
{
    int32_t mode = -1;

    bdt_log("BT DUT MODE CONFIGURE");
    if (!bt_enabled) {
        bdt_log("Bluetooth must be enabled for test_mode to work.");
        return;
    }
    mode = get_signed_int(&p, mode);
    if ((mode != 0) && (mode != 1)) {
        bdt_log("Please specify mode: 1 to enter, 0 to exit");
        return;
    }
    status = sBtInterface->dut_mode_configure(mode);

    check_return_status(status);
}

void bdt_cleanup(void)
{
    bdt_log("CLEANUP");
    sBtInterface->cleanup();
}

/*******************************************************************************
 ** Console commands
 *******************************************************************************/

void do_help(char *p)
{
    int i = 0;
    int max = 0;
    char line[128];
    int pos = 0;

    while (console_cmd_list[i].name != NULL)
    {
        pos = sprintf(line, "%s", (char*)console_cmd_list[i].name);
        bdt_log("%s %s\n", (char*)line, (char*)console_cmd_list[i].help);
        i++;
    }
}

void do_quit(char *p)
{
    bdt_shutdown();
}

/*******************************************************************
 *
 *  BT TEST  CONSOLE COMMANDS
 *
 *  Parses argument lists and passes to API test function
 *
*/

void do_init(char *p)
{
    bdt_init();
}

void do_enable(char *p)
{
    bdt_enable();
}

void do_disable(char *p)
{
    bdt_disable();
}

void do_discover(char *p)
{
	bdt_discover();
}

void do_dut_mode_configure(char *p)
{
    bdt_dut_mode_configure(p);
}

void do_cleanup(char *p)
{
    bdt_cleanup();
}

/*******************************************************************
 *
 *  CONSOLE COMMAND TABLE
 *
*/

const t_cmd console_cmd_list[] =
{
    /*
     * INTERNAL
     */

    { "help", do_help, "lists all available console commands", 0 },
    { "quit", do_quit, "", 0},

    /*
     * API CONSOLE COMMANDS
     */

     /* Init and Cleanup shall be called automatically */
    { "enable", do_enable, ":: enables bluetooth", 0 },
    { "disable", do_disable, ":: disables bluetooth", 0 },
	{ "discover", do_discover, ":: discover", 0 },
    { "dut_mode_configure", do_dut_mode_configure, ":: DUT mode - 1 to enter,0 to exit", 0 },

    /* add here */

    /* last entry */
    {NULL, NULL, "", 0},
};

/*
 * Main console command handler
*/

static void process_cmd(char *p, unsigned char is_job)
{
    char cmd[64];
    int i = 0;
    char *p_saved = p;

    get_str(&p, cmd);

    /* table commands */
    while (console_cmd_list[i].name != NULL)
    {
        if (is_cmd(console_cmd_list[i].name))
        {
            if (!is_job && console_cmd_list[i].is_job)
                create_cmdjob(p_saved);
            else
            {
                console_cmd_list[i].handler(p);
            }
            return;
        }
        i++;
    }
    bdt_log("%s : unknown command\n", p_saved);
    do_help(NULL);
}

int main (int argc, char * argv[])
{
    int opt;
    char cmd[128];
    int args_processed = 0;
    int pid = -1;

    config_permissions();
    bdt_log("\n:::::::::::::::::::::::::::::::::::::::::::::::::::");
    bdt_log(":: Bluedroid test app starting");

    if ( HAL_load() < 0 ) {
        perror("HAL failed to initialize, exit\n");
        unlink(PID_FILE);
        exit(0);
    }

    setup_test_env();

    /* Automatically perform the init */
    bdt_init();

    while(!main_done)
    {
        char line[128];

        /* command prompt */
        printf( ">" );
        fflush(stdout);

        fgets (line, 128, stdin);

        if (line[0]!= '\0')
        {
            /* remove linefeed */
            line[strlen(line)-1] = 0;

            process_cmd(line, 0);
            memset(line, '\0', 128);
        }
    }

    /* FIXME: Commenting this out as for some reason, the application does not exit otherwise*/
    bdt_cleanup();

    HAL_unload();

    bdt_log(":: Bluedroid test app terminating");

    return 0;
}
