#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm-i386/current.h>
#include <linux/hw1_utils.h>
//#include <linux/jiffies.h>
int restrict_fun(int sys_call_number){
	//lets get this syscall restriction threshold:
	//idel and init has a staticly defined task stract that cannot be changed so if they are the calling system call we will return immidetly
	if(current->pid == 0 || current->pid == 1)
	{
		return sys_call_number;
	}
	int i=0;
	for(i=0;i<current->restrictions_list_size;i++)
	{
		if((current->restrictions_list)[i].syscall_num == sys_call_number)
		{
				//means the syscall is in the list
				if((current->restriction_level) < ((current->restrictions_list)[i].restriction_threshold))
				{
				//only if the process restriction_level​ which is ​strictly lower​ than the ​restriction_threshold​ of that system call
				//now recording forbidden activity
				(current->log_records[current->log_records_index]).syscall_num = sys_call_number;
				(current->log_records[current->log_records_index]).syscall_restriction_threshold = (current->restrictions_list)[i].restriction_threshold;
				(current->log_records[current->log_records_index]).proc_restriction_level = current->restriction_level;
				(current->log_records[current->log_records_index]).time = jiffies;

				current->log_records_index =  (current->log_records_index + 1) % 100;
				if(current->log_size <100)
				{
					current->log_size++;
				}
				return -1;
				}
			return sys_call_number;
		}
	}
	return sys_call_number;
}

