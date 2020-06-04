#define GSI_UACPROC                     8
#define GSI_IEEE_FP_CONTROL             45
#define GSI_IEEE_STATE_AT_SIGNAL        46
#define GSI_PROC_TYPE                   60
#define GSI_GET_HWRPB                   101

#define EFAULT          14      /* Bad address */
#define EINVAL          22      /* Invalid argument */
#define EOPNOTSUPP      122     /* Operation not supported on transport endpoint */

#define IEEE_SW_MASK			0
#define IEEE_STATUS_MASK		1
#define UAC_BITMASK                     7

#define VERIFY_WRITE			1

enum implver_enum {
	IMPLVER_EV4, IMPLVER_EV5, IMPLVER_EV6
};

/* This is the shift that is applied to the UAC bits as stored in the
 per-thread flags.  See thread_info.h.  */
#define UAC_SHIFT                       6

struct thread_struct {
	unsigned long flags;
};

struct current_struct {
	struct thread_struct thread;
};

struct hwrpb_struct {
	unsigned long phys_addr; /* check: physical address of the hwrpb */
	unsigned long id; /* check: "HWRPB\0\0\0" */
	unsigned long revision;
	unsigned long size; /* size of hwrpb */
	unsigned long cpuid;
	unsigned long pagesize; /* 8192, I hope */
	unsigned long pa_bits; /* number of physical address bits */
	unsigned long max_asn;
	unsigned char ssn[16]; /* system serial number: big bother is watching */
	unsigned long sys_type;
	unsigned long sys_variation;
	unsigned long sys_revision;
	unsigned long intr_freq; /* interval clock frequency * 4096 */
	unsigned long cycle_freq; /* cycle counter frequency */
	unsigned long vptb; /* Virtual Page Table Base address */
	unsigned long res1;
	unsigned long tbhb_offset; /* Translation Buffer Hint Block */
	unsigned long nr_processors;
	unsigned long processor_size;
	unsigned long processor_offset;
	unsigned long ctb_nr;
	unsigned long ctb_size; /* console terminal block size */
	unsigned long ctbt_offset; /* console terminal block table offset */
	unsigned long crb_offset; /* console callback routine block */
	unsigned long mddt_offset; /* memory data descriptor table */
	unsigned long cdb_offset; /* configuration data block (or NULL) */
	unsigned long frut_offset; /* FRU table (or NULL) */
	void (*save_terminal)(unsigned long);
	unsigned long save_terminal_data;
	void (*restore_terminal)(unsigned long);
	unsigned long restore_terminal_data;
	void (*CPU_restart)(unsigned long);
	unsigned long CPU_restart_data;
	unsigned long res2;
	unsigned long res3;
	unsigned long chksum;
	unsigned long rxrdy;
	unsigned long txrdy;
	unsigned long dsr_offset; /* "Dynamic System Recognition Data Block Table" */
};

#define INIT_HWRPB ((struct hwrpb_struct *) 0x10000000)

struct current_struct *current = { { 0 } };
struct hwrpb_struct *hwrpb = INIT_HWRPB;

struct percpu_struct {
	unsigned long hwpcb[16];
	unsigned long flags;
	unsigned long pal_mem_size;
	unsigned long pal_scratch_size;
	unsigned long pal_mem_pa;
	unsigned long pal_scratch_pa;
	unsigned long pal_revision;
	unsigned long type;
	unsigned long variation;
	unsigned long revision;
	unsigned long serial_no[2];
	unsigned long logout_area_pa;
	unsigned long logout_area_len;
	unsigned long halt_PCBB;
	unsigned long halt_PC;
	unsigned long halt_PS;
	unsigned long halt_arg;
	unsigned long halt_ra;
	unsigned long halt_pv;
	unsigned long halt_reason;
	unsigned long res;
	unsigned long ipc_buffer[21];
	unsigned long palcode_avail[16];
	unsigned long compatibility;
	unsigned long console_data_log_pa;
	unsigned long console_data_log_length;
	unsigned long bcache_info;
};

static inline unsigned long rdfpcr() {
	return 1;
}

static inline int implver() {
	return IMPLVER_EV4;
}

static inline int access_ok(int type, const void *addr, unsigned long size) {
	return 1;
}

static inline unsigned long swcr_update_status(unsigned long swcr,
		unsigned long fpcr) {
	/* EV6 implements most of the bits in hardware.  Collect
	 the acrued exception bits from the real fpcr.  */
	if (implver() == IMPLVER_EV6) {
		swcr &= ~IEEE_STATUS_MASK;
		swcr |= (fpcr >> 35) & IEEE_STATUS_MASK;
	}
	return swcr;
}

void might_fault() {
}

#define __put_user(x, ptr) \
({                                                              \
        int __pu_err = -EFAULT;                                 \
        __pu_err;                                               \
})

#define put_user(x, ptr)                                        \
({                                                              \
        might_fault();                                          \
        __put_user(x, ptr);                                     \
})

typedef unsigned int size_t;

static inline unsigned long copy_to_user(void *to, const void *from,
		unsigned long n) {
	memcpy(to, from, n);
	return n;
}

unsigned long osf_getsysinfo(unsigned long op, void *buffer,
		unsigned long nbytes, int *start, void *arg) {
	unsigned long w;
	struct percpu_struct *cpu;

	switch (op) {
	case GSI_IEEE_FP_CONTROL:
		/* Return current software fp control & status bits.  */
		/* Note that DU doesn't verify available space here.  */

		w = current->thread.flags & IEEE_SW_MASK;
		w = swcr_update_status(w, rdfpcr());
		if (put_user(w, (unsigned long *) buffer))
			return -EFAULT;
		return 0;

	case GSI_IEEE_STATE_AT_SIGNAL:
		/*
		 * Not sure anybody will ever use this weird stuff.  These
		 * ops can be used (under OSF/1) to set the fpcr that should
		 * be used when a signal handler starts executing.
		 */
		break;

	case GSI_UACPROC:
		if (nbytes < sizeof(unsigned int))
			return -EINVAL;
		w = (current->thread.flags >> UAC_SHIFT) & UAC_BITMASK;
		if (put_user(w, (unsigned int *) buffer))
			return -EFAULT;
		return 1;

	case GSI_PROC_TYPE:
		if (nbytes < sizeof(unsigned long))
			return -EINVAL;
		cpu = (struct percpu_struct*) ((char*) hwrpb + hwrpb->processor_offset);
		w = cpu->type;
		if (put_user(w, (unsigned long *) buffer))
			return -EFAULT;
		return 1;

	case GSI_GET_HWRPB:
		if (nbytes < sizeof(*hwrpb))
			// if (nbytes > sizeof(*hwrpb))
			return -EINVAL;
		if (copy_to_user(buffer, hwrpb, nbytes) != 0)
			return -EFAULT;
		return 1;

	default:
		break;
	}

	return -EOPNOTSUPP;
}

int main(void) {
	return 0;
}
