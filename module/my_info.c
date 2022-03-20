#include <linux/init.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <generated/utsrelease.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#define MAX_PROC_SIZE 100
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mm.h>
#include <linux/cpumask.h>
#include <linux/kernel_stat.h>
#include <linux/ktime.h>
//#include <linux/cpu.h>
//struct cpuinfo_x86 *c;
static int hello_proc_show(struct seq_file *m, void *v)
{
    //seq_printf(m, "Hello proc!\n");
    struct cpuinfo_x86 *c;
    int id = 0;
    seq_puts(m,"\n============Version============\n");
    seq_printf(m,"Linux version\t:%s\n", UTS_RELEASE);
    seq_puts(m,"\n==============CPU==============\n");
    for_each_possible_cpu(id)
    {
        c = &cpu_data(id);
        seq_printf(m,"processor\t:%d\n",c->cpu_index);
        seq_printf(m,"model name\t:%s\n",c->x86_model_id);
        seq_printf(m,"physical id\t:%d\n",c->phys_proc_id);
        seq_printf(m,"core id\t\t:%d\n",c->cpu_core_id);
        seq_printf(m,"cpu cores\t:%d\n",c->booted_cores);
        seq_printf(m,"cache size\t:%u KB\n",c->x86_cache_size);
        seq_printf(m,"clflush size\t:%u\n",c->x86_clflush_size);
        seq_printf(m,"cache_alignment\t:%d\n",c->x86_cache_alignment);
        seq_printf(m,"address sizes\t:%u bits physical, %u bits virtual\n",c->x86_phys_bits,c->x86_virt_bits);
        seq_puts(m,"\n");
    }
    seq_puts(m,"\n=============Memory=============\n");
    struct sysinfo i;
    unsigned long committed;
    unsigned long pages[NR_LRU_LISTS];
    int lru;

    si_meminfo(&i);
    for (lru = LRU_BASE; lru < NR_LRU_LISTS; lru++)
        pages[lru] = global_node_page_state(NR_LRU_BASE + lru);
    seq_printf(m,"MemTotal\t:%lu kB\n", i.totalram);
    seq_printf(m,"MemFree\t\t:%lu kB\n", i.freeram);
    seq_printf(m,"Buffers\t\t:%lu kB\n", i.bufferram);
    seq_printf(m,"Active\t\t:%lu kB\n", pages[LRU_ACTIVE_ANON] + pages[LRU_ACTIVE_FILE]);
    seq_printf(m,"Inactive\t:%lu kB\n", pages[LRU_INACTIVE_ANON] + pages[LRU_INACTIVE_FILE]);
    seq_printf(m,"Shmem\t\t:%lu kB\n", i.sharedram);
    seq_printf(m,"Dirty\t\t:%lu kB\n",global_node_page_state(NR_FILE_DIRTY));
    seq_printf(m,"Writeback\t:%lu kB\n",global_node_page_state(NR_WRITEBACK));
    seq_printf(m,"KernelStack\t:%lu kB\n",global_zone_page_state(NR_KERNEL_STACK_KB));
    seq_printf(m,"PageTables\t:%lu kB\n",global_zone_page_state(NR_PAGETABLE));
    seq_puts(m,"\n==============Time==============\n");
    int idd;
    struct timespec64 uptime;
    struct timespec idle;
    u64 nsec ;
    u32 rem;
    nsec = 0;
    get_monotonic_boottime(&uptime);
    for_each_possible_cpu(idd)
    nsec += (__force u64)kcpustat_cpu(idd).cpustat[CPUTIME_IDLE];
    idle.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
    idle.tv_nsec = rem;
    seq_printf(m,"Uptime\t\t:%lu.%02lu (s)\n", (unsigned long)uptime.tv_sec, uptime.tv_nsec / (NSEC_PER_SEC / 100));
    seq_printf(m,"Idletime\t:%lu.%02lu (s)\n", (unsigned long)idle.tv_sec, idle.tv_nsec / (NSEC_PER_SEC / 100));
    return 0;
}

static int hello_proc_open(struct inode *inode, struct  file *file)
{
    return single_open(file, hello_proc_show, NULL);
}

static const struct file_operations hello_proc_fops =
{
    .owner = THIS_MODULE,
    .open = hello_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static int __init hello_proc_init(void)
{
    proc_create("my_info", 0, NULL, &hello_proc_fops);
    return 0;
}

static void __exit hello_proc_exit(void)
{
    remove_proc_entry("my_info", NULL);
}
MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
