// Copyright (c) 2023-2026 Christiaan (chris@boreddev.nl)
// This software is released under the GNU General Public License v3.0. See LICENSE file for details.
// This header needs to maintain in any file it is present in, as per the GPL license terms.
#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Standard syscalls available from Kernel mode
enum {
    SYS_EXIT   = 0,
    SYS_WRITE  = 1,
    SYS_FS     = 4,
    SYS_SYSTEM = 5,
    SYS_SBRK   = 9,
    SYS_KILL   = 10,
    SYS_MMAP   = 11,
    SYS_MUNMAP = 12,
    SYS_FUTEX  = 13
};

// Futex operations
enum {
    FUTEX_WAIT = 0,
    FUTEX_WAKE = 1
};

// FS Commands
enum {
    FS_CMD_OPEN = 1,
    FS_CMD_READ = 2,
    FS_CMD_WRITE = 3,
    FS_CMD_CLOSE = 4,
    FS_CMD_SEEK = 5,
    FS_CMD_TELL = 6,
    FS_CMD_LIST = 7,
    FS_CMD_DELETE = 8,
    FS_CMD_SIZE = 9,
    FS_CMD_MKDIR = 10,
    FS_CMD_EXISTS = 11,
    FS_CMD_GETCWD = 12,
    FS_CMD_CHDIR = 13,
    FS_CMD_GET_INFO = 14,
    FS_CMD_DUP = 15,
    FS_CMD_DUP2 = 16,
    FS_CMD_PIPE = 17,
    FS_CMD_FCNTL = 18,
    FS_CMD_STATFS = 19,
    FS_CMD_MOUNT_COUNT = 20,
    FS_CMD_MOUNT_INFO = 21,
    FS_CMD_POLL = 22,
    FS_CMD_IOCTL = 24,
    FS_CMD_UNIX_SOCKET_CREATE = 25,
    FS_CMD_UNIX_SOCKET_BIND = 26,
    FS_CMD_UNIX_SOCKET_LISTEN = 27,
    FS_CMD_UNIX_SOCKET_ACCEPT = 28,
    FS_CMD_UNIX_SOCKET_CONNECT = 29,
    FS_CMD_UNIX_SOCKET_SEND = 30,
    FS_CMD_UNIX_SOCKET_RECV = 31,
    FS_CMD_UNIX_SOCKET_CLOSE = 32,
    FS_CMD_UNIX_SOCKET_UNLINK = 33,
    FS_CMD_LIST_OFFSET = 34
};

// System Commands (via SYS_SYSTEM)
enum {
    SYSTEM_CMD_CLEAR_SCREEN = 10,
    SYSTEM_CMD_RTC_GET = 11,
    SYSTEM_CMD_REBOOT = 12,
    SYSTEM_CMD_SHUTDOWN = 13,
    SYSTEM_CMD_GET_MEM_INFO = 15,
    SYSTEM_CMD_GET_TICKS = 16,
    SYSTEM_CMD_PCI_LIST = 17,
    SYSTEM_CMD_UDP_SEND = 22,
    SYSTEM_CMD_ICMP_PING = 26,
    SYSTEM_CMD_SET_TEXT_COLOR = 29,
    SYSTEM_CMD_RTC_SET = 32,
    SYSTEM_CMD_TCP_CONNECT = 33,
    SYSTEM_CMD_TCP_SEND = 34,
    SYSTEM_CMD_TCP_RECV = 35,
    SYSTEM_CMD_TCP_CLOSE = 36,
    SYSTEM_CMD_DNS_LOOKUP = 37,
    SYSTEM_CMD_NET_UNLOCK = 39,
    SYSTEM_CMD_SLEEP = 46,
    SYSTEM_CMD_SET_RAW_MODE = 41,
    SYSTEM_CMD_TCP_RECV_NB = 42,
    SYSTEM_CMD_YIELD = 43,
    SYSTEM_CMD_TCP_LISTEN = 44,
    SYSTEM_CMD_TCP_ACCEPT = 45,
    SYSTEM_CMD_SET_KEYBOARD_LAYOUT = 49,
    SYSTEM_CMD_PARALLEL_RUN = 50,
    SYSTEM_CMD_GET_KEYBOARD_LAYOUT = 51,
    SYSTEM_CMD_TTY_CREATE = 60,
    SYSTEM_CMD_TTY_READ_OUT = 61,
    SYSTEM_CMD_TTY_WRITE_IN = 62,
    SYSTEM_CMD_TTY_READ_IN = 63,
    SYSTEM_CMD_SPAWN = 64,
    SYSTEM_CMD_TTY_SET_FG = 65,
    SYSTEM_CMD_TTY_GET_FG = 66,
    SYSTEM_CMD_TTY_KILL_FG = 67,
    SYSTEM_CMD_TTY_KILL_ALL = 68,
    SYSTEM_CMD_TTY_DESTROY = 69,
    SYSTEM_CMD_EXEC = 70,
    SYSTEM_CMD_WAITPID = 71,
    SYSTEM_CMD_KILL_SIGNAL = 72,
    SYSTEM_CMD_SIGACTION = 73,
    SYSTEM_CMD_SIGPROCMASK = 74,
    SYSTEM_CMD_SIGPENDING = 75,
    SYSTEM_CMD_GET_ELF_METADATA = 76,
    SYSTEM_CMD_GET_ELF_PRIMARY_IMAGE = 77,
    SYSTEM_CMD_TTY_GET_ID = 78,
    SYSTEM_CMD_PTY_CREATE = 82,
    SYSTEM_CMD_PTY_DESTROY = 83
    ,SYSTEM_CMD_GET_PID = 84
};

enum {
    SPAWN_FLAG_TERMINAL = 0x1,
    SPAWN_FLAG_INHERIT_TTY = 0x2,
    SPAWN_FLAG_TTY_ID = 0x4,
    SPAWN_FLAG_BACKGROUND = 0x8
};

// ELF app metadata (mirrors src/sys/elf.h, kept in sync manually)
#define BOREDOS_APP_METADATA_MAX_APP_NAME   64
#define BOREDOS_APP_METADATA_MAX_DESCRIPTION 192
#define BOREDOS_APP_METADATA_MAX_IMAGES     4
#define BOREDOS_APP_METADATA_MAX_IMAGE_PATH 160

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint16_t version;
    uint16_t image_count;
    uint16_t reserved;
    char app_name[BOREDOS_APP_METADATA_MAX_APP_NAME];
    char description[BOREDOS_APP_METADATA_MAX_DESCRIPTION];
    char images[BOREDOS_APP_METADATA_MAX_IMAGES][BOREDOS_APP_METADATA_MAX_IMAGE_PATH];
} boredos_app_metadata_t;

// Internal assembly entry into Ring 0
extern uint64_t syscall0(uint64_t sys_num);
extern uint64_t syscall1(uint64_t sys_num, uint64_t arg1);
extern uint64_t syscall2(uint64_t sys_num, uint64_t arg1, uint64_t arg2);
extern uint64_t syscall3(uint64_t sys_num, uint64_t arg1, uint64_t arg2, uint64_t arg3);
extern uint64_t syscall4(uint64_t sys_num, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);
extern uint64_t syscall5(uint64_t sys_num, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);
extern uint64_t syscall6(uint64_t sys_num, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);

// Public API
void sys_exit(int status);
int sys_write(int fd, const char *buf, int len);
void *sys_sbrk(int incr);
void sys_kill(int pid);
int sys_system(int cmd, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);
void *sys_mmap(void *addr, size_t length, int prot, int flags, int fd, int64_t offset);
int sys_munmap(void *addr, size_t length);

// FS API
typedef struct {
    uint64_t total_blocks;
    uint64_t free_blocks;
    uint64_t block_size;
} vfs_statfs_t;

typedef struct {
    char path[256];
    char device[32];
    char fs_type[16];
} mount_info_t;

int sys_open(const char *path, const char *mode);
int sys_read(int fd, void *buf, uint32_t len);
int sys_write_fs(int fd, const void *buf, uint32_t len);
void sys_close(int fd);
int sys_seek(int fd, int offset, int whence);
uint32_t sys_tell(int fd);
uint32_t sys_size(int fd);
int sys_delete(const char *path);
int sys_mkdir(const char *path);
int sys_exists(const char *path);
int sys_getcwd(char *buf, int size);
int sys_chdir(const char *path);
int sys_dup(int oldfd);
int sys_dup2(int oldfd, int newfd);
int sys_pipe(int pipefd[2]);
int sys_fcntl(int fd, int cmd, int val);
int sys_fs_statfs(const char *path, vfs_statfs_t *stat);
int sys_fs_mount_count(void);
int sys_fs_mount_info(int index, mount_info_t *info);

int sys_tty_create(void);
int sys_tty_read_out(int tty_id, char *buf, int len);
int sys_tty_write_in(int tty_id, const char *buf, int len);
int sys_tty_read_in(char *buf, int len);
int sys_spawn(const char *path, const char *args, uint64_t flags, uint64_t tty_id);
int sys_exec(const char *path, const char *args);
int sys_waitpid(int pid, int *status, int options);
int sys_kill_signal(int pid, int sig);
int sys_sigaction(int sig, const void *act, void *oldact);
int sys_sigprocmask(int how, const unsigned long *set, unsigned long *oldset);
int sys_sigpending(unsigned long *set);
int sys_tty_set_fg(int tty_id, int pid);
int sys_tty_get_fg(int tty_id);
int sys_tty_kill_fg(int tty_id);
int sys_tty_kill_all(int tty_id);
int sys_tty_destroy(int tty_id);
int sys_pty_create(void);
int sys_pty_destroy(int pty_id);

struct pollfd {
    int fd;
    short events;
    short revents;
};

#define POLLIN     0x0001
#define POLLOUT    0x0004
#define POLLERR    0x0008
#define POLLHUP    0x0010
#define POLLNVAL   0x0020

int sys_poll(struct pollfd *fds, int nfds, int timeout);
int poll(struct pollfd *fds, int nfds, int timeout);

typedef struct {
    char name[256];
    uint32_t size;
    uint8_t is_directory;
    uint32_t start_cluster;
    uint16_t write_date;
    uint16_t write_time;
} FAT32_FileInfo;

int sys_list(const char *path, FAT32_FileInfo *entries, int max_entries);
int sys_list_offset(const char *path, FAT32_FileInfo *entries, int max_entries, int offset);
int sys_get_file_info(const char *path, FAT32_FileInfo *info);

typedef struct {
    uint32_t pid;
    char name[64];
    uint64_t ticks;
    size_t used_memory;
    uint32_t is_idle;
} ProcessInfo;

// Network API
typedef struct { uint8_t bytes[6]; } net_mac_address_t;
typedef struct { uint8_t bytes[4]; } net_ipv4_address_t;

int sys_udp_send(const net_ipv4_address_t *dest_ip, uint16_t dest_port, uint16_t src_port, const void *data, size_t data_len);
int sys_icmp_ping(const net_ipv4_address_t *dest_ip);
void sys_set_text_color(uint32_t color);

int sys_tcp_connect(const net_ipv4_address_t *ip, uint16_t port);
int sys_tcp_listen(uint16_t port);
int sys_tcp_accept(void);
int sys_tcp_send(const void *data, size_t len);
int sys_tcp_recv(void *buf, size_t max_len);
int sys_tcp_recv_nb(void *buf, size_t max_len);
int sys_tcp_close(void);
int sys_dns_lookup(const char *name, net_ipv4_address_t *out_ip);
void sys_network_force_unlock(void);
void sys_yield(void);

// ELF metadata API
int sys_get_elf_metadata(const char *path, boredos_app_metadata_t *out_metadata);
int sys_get_elf_primary_image(const char *path, char *out_path, size_t out_path_size);

// Disk Management Syscalls

#define SYSTEM_CMD_DISK_GET_COUNT  100
#define SYSTEM_CMD_DISK_GET_INFO   101
#define SYSTEM_CMD_DISK_MOUNT      105
#define SYSTEM_CMD_DISK_UMOUNT     106
#define SYSTEM_CMD_DISK_RESCAN     107
#define SYSTEM_CMD_DISK_SYNC       109  

#define SYSTEM_CMD_DISK_WRITE_GPT      102
#define SYSTEM_CMD_DISK_WRITE_MBR      103
#define SYSTEM_CMD_DISK_MKFS_FAT32     104
#define SYSTEM_CMD_DISK_REPLACE_KERNEL 108

typedef struct {
    char     devname[16];
    char     label[32];
    uint32_t type;           
    uint32_t total_sectors;
    bool     is_partition;
    bool     is_fat32;
    bool     is_esp;
    uint32_t lba_offset;
} disk_info_t;

typedef struct {
    uint32_t lba_start;
    uint32_t sector_count;
    uint8_t  part_type;
    uint8_t  flags;
    char     label[36];
} partition_spec_t;

#define PART_FLAG_ESP       0x01
#define MIN_INSTALL_SECTORS 2097152  

int sys_disk_get_count(void);
int sys_disk_get_info(int index, disk_info_t *out);
int sys_disk_write_gpt(const char *devname, partition_spec_t *parts, int count);
int sys_disk_write_mbr(const char *devname, partition_spec_t *parts, int count);
int sys_disk_mkfs_fat32(const char *devname, const char *label);
int sys_disk_mount(const char *devname, const char *mountpoint);
int sys_disk_umount(const char *mountpoint);
int sys_disk_sync(const char *mountpoint);
int sys_disk_rescan(const char *devname);
int sys_disk_replace_kernel(const char *src_path, const char *esp_mountpoint);

// Futex
int sys_futex(uint32_t *uaddr, int op, uint32_t val);

// Ioctl helpers
int sys_ioctl(int fd, unsigned long request, void *arg);
int ioctl(int fd, unsigned long request, ...);

#endif

