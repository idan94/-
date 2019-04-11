//In this file we will write the CODE WRAPPERS for the three functions.
//user code
#include <errno.h>
#include <sys/types.h>
#include "hw1_utils.h"



/* System Call number 243 */
int sc_restrict(pid_t pid, int proc_restriction_level, scr* restrictions_list, int list_size) {
	int __res;
	//build a data transfering structure in order to move parameters to function
	dpsfr DATA = {pid,proc_restriction_level,restrictions_list,list_size};
	__asm__ volatile(
		"int $0x80;"
		: "=a" (__res)
		: "0" (243),"b"(&DATA)
		: "memory");
	if ((__res) < 0) {
		errno = (-__res);
		return -1;
	}
	return 0;
}

/* System Call number 244 */
int set_proc_restriction(pid_t pid, int proc_restriction_level) {
	int __res;
	__asm__ volatile(
		"int $0x80;"
		: "=a" (__res)
		: "0" (244), "b" (pid), "c" (proc_restriction_level)
		: "memory");
	if ((__res) < 0) {
		errno = (-__res);
		return -1;
	}
	return 0;
}

/* System Call number 245 */
int get_process_log(pid_t pid, int size, fai* user_mem){
	int __res;
	__asm__ volatile(
		"int $0x80;"
		: "=a" (__res)
		: "0" (245), "b" (pid), "c" (size), "d" (user_mem)
		: "memory");
	if ((__res) < 0) {
		errno = (-__res);
		return -1;
	}
	return 0;
}
