#ifndef _HW1_UTILS
#define _HW1_UTILS

typedef struct sys_call_restriction {
	int syscall_num;
	int restriction_threshold;
} scr;
typedef struct forbidden_activity_info {
	int syscall_num;
	int syscall_restriction_threshold;
	int proc_restriction_level;
	int time;
} fai;
typedef struct data_passing_stracture_for_restriction {
		pid_t pid;
		int proc_restriction_level;
		scr* restrictions_list;
		int list_size;
} dpsfr;

#endif /* !(_HW1_UTILS) */

