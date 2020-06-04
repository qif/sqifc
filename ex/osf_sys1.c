#ifndef UTS_SYSNAME
#define UTS_SYSNAME "Linux"
#endif

#ifndef UTS_MACHINE
#define UTS_MACHINE "unknown"
#endif

#ifndef UTS_NODENAME
#define UTS_NODENAME "(none)"   /* set by sethostname() */
#endif

#ifndef UTS_DOMAINNAME
#define UTS_DOMAINNAME "@@@@@@@@A dummy 32-character domain name@@@@@@@@" /* set by setdomainname() */
#endif

#define UTS_VERSION "#1 SMP Sat Aug 27 16:21:11 UTC 2012"
#define UTS_RELEASE "2.6.35-22-generic"

#define VERIFY_WRITE 1

typedef unsigned char __u8;
typedef __u8 uint8_t;
typedef uint8_t u8;

static const u8 magic[] = { 0xc5, 0x3a, 0xa3, 0x5c, 0xc5, 0x3a, 0xa3, 0x5c };

typedef struct {
	volatile unsigned int slock;
} raw_spinlock_t;

struct lockdep_map;

struct list_head {
	struct list_head *next, *prev;
};

typedef struct {
	raw_spinlock_t raw_lock;
#ifdef CONFIG_GENERIC_LOCKBREAK
unsigned int break_lock;
#endif
#ifdef CONFIG_DEBUG_SPINLOCK
unsigned int magic, owner_cpu;
void *owner;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
struct lockdep_map dep_map;
#endif
} spinlock_t;

struct rw_semaphore {
	long count;
#define RWSEM_UNLOCKED_VALUE            0x00000000
#define RWSEM_ACTIVE_BIAS               0x00000001
#define RWSEM_ACTIVE_MASK               0x0000ffff
#define RWSEM_WAITING_BIAS              (-0x00010000)
#define RWSEM_ACTIVE_READ_BIAS          RWSEM_ACTIVE_BIAS
#define RWSEM_ACTIVE_WRITE_BIAS         (RWSEM_WAITING_BIAS + RWSEM_ACTIVE_BIAS)
	spinlock_t wait_lock;
	struct list_head wait_list;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
struct lockdep_map dep_map;
#endif
};

struct rw_semaphore uts_sem;

struct new_utsname {
	char sysname[65];
	char nodename[65];
	char release[65];
	char version[65];
	char machine[65];
	char domainname[65];
};

struct new_utsname system_utsname = { UTS_SYSNAME, UTS_NODENAME, UTS_RELEASE,
		UTS_VERSION, UTS_MACHINE, UTS_DOMAINNAME };

char nondet_char();

void __put_user(char c, char* name) {
	// *name = c;
	// create nondeterministic
	*name = nondet_char();
}

static inline int verify_area(int type, const void *addr, unsigned long size) {
	return 0;
}

void down_read(struct rw_semaphore *sem) {
}
void up_read(struct rw_semaphore *sem) {
}

int osf_getdomainname(char *name, int namelen) {
	unsigned len;
	int i, error;

	error = verify_area(VERIFY_WRITE, name, namelen);
	if (error)
		goto out;

	len = namelen;

	// Official patch
	if(len>32)

	// if (namelen > 32)
		len = 32;

	down_read(&uts_sem);
	for (i = 0; i < len; ++i) {
		__put_user(system_utsname.domainname[i], name + i);
		if (system_utsname.domainname[i] == '\0')
			break;
	}
	up_read(&uts_sem);
	out: return error;
}
