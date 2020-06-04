#define __user

#define SCO_OPTIONS     0x01
#define SCO_CONNINFO    0x02

#define BT_CONNECTED    14

#define EFAULT          14      /* Bad address */
#define ENOTCONN        134     /* Transport endpoint is not connected */
#define ENOPROTOOPT     99      /* Protocol not available */

typedef unsigned char   __u8;
typedef unsigned short __u16;
typedef unsigned int   __u32;

struct sco_options {
        __u16 mtu;
};

struct sco_conninfo {
        __u16 hci_handle;
        __u8  dev_class[3];
};

struct sock {
	__u16  sk_state;
};  

struct socket {
  // socket_state state;
  unsigned long flags;
  // const struct proto_ops * ops;
  // struct fasync_struct * fasync_list;
  // struct file * file;
  struct sock * sk;
  // wait_queue_head_t wait;
  short type;
};  

struct hci_conn {
        __u16           handle;
        __u8            dev_class[3];
};

struct sco_conn {
        struct hci_conn *hcon;
        unsigned int    mtu;
};

struct sco_pinfo {
        // struct bt_sock  bt;
        __u32           flags;
        struct sco_conn *conn;
};

// define a dummy variable
#define DUMMY_STRING 	"ABC"
struct hci_conn hcon = {0,DUMMY_STRING};
struct sco_conn conn = {&hcon,0};
struct sco_pinfo dummy = {0,&conn};
struct sco_pinfo dummy1;

struct sco_pinfo* sco_pi(struct sock* sk) {
	return &dummy;
}

#define min_t(type, x, y) ({                    \
        type __min1 = (x);                      \
        type __min2 = (y);                      \
        __min1 < __min2 ? __min1 : __min2; })

static inline void lock_sock(struct sock *sk){}
static inline void release_sock(struct sock *sk){}

void BT_DBG(const char*,struct sock *sk){}

#define __get_user(src, dest)                                           \
({                                                                      \
       (src) = 16;                                                  \
       0;                                                              \
})

#define get_user        __get_user

/* libc library */
int memcmp(char *s1, char *s2, unsigned int n) {
  int i;
  for(i=0;i<n;i++) {
     if(s1[i] != s2[i]) return -1;
  }
  return 0;
}

//*
void memcpy(char *dst, char *src, unsigned int n) {
  if(n < 30){
	   int i;
	   for(i=0;i<n;i++) {
	      dst[i] = src[i];
	   }
   }
}
//*/

void memset(char *dst, char c, unsigned int n) {
  int i;
  for(i=0;i<n;i++) {
       dst[i] = c;
  }
}
/* ------ */
static inline unsigned long copy_to_user(void *to, const void *from,
		unsigned long n) {
	memcpy((char*)to, (char*)from, n);
	return n;
}

static int sco_sock_getsockopt_old(struct socket *sock, int optname, char __user *optval, int __user *optlen)
{
	struct sock *sk = sock->sk;
	struct sco_options opts;
	struct sco_conninfo cinfo;
	int len, err = 0;

	BT_DBG("sk %p", sk);

	if (get_user(len, optlen))
	return -EFAULT;

	lock_sock(sk);

	switch (optname) {
		case SCO_OPTIONS:
		if (sk->sk_state != BT_CONNECTED) {
			err = -ENOTCONN;
			break;
		}

		opts.mtu = sco_pi(sk)->conn->mtu;

		BT_DBG("mtu %d", opts.mtu);

		len = min_t(unsigned int, len, sizeof(opts));
		if (copy_to_user(optval, (char *)&opts, len))
		err = -EFAULT;

		break;

		case SCO_CONNINFO:
		if (sk->sk_state != BT_CONNECTED) {
			err = -ENOTCONN;
			break;
		}
		
		// formal patch
		memset((char*)&cinfo, 0, sizeof(cinfo));

		cinfo.hci_handle = sco_pi(sk)->conn->hcon->handle;
		memcpy(cinfo.dev_class, sco_pi(sk)->conn->hcon->dev_class, 3);

		len = min_t(unsigned int, len, sizeof(cinfo));
		if (copy_to_user(optval, (char *)&cinfo, len))
			err = -EFAULT;

		break;

		default:
		err = -ENOPROTOOPT;
		break;
	}

	release_sock(sk);
	return err;
}
