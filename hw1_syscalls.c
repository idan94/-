//In this file we will write  the corresponding system calls for the three functions.
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm-i386/current.h>

/* System Call number 243 */
int sys_sc_restrict (dpsfr *DATA)
{
	
	//allocate in memory to get data from the user
	dpsfr* kernel_data = kmalloc(sizeof(dpsfr),GFP_KERNEL);
	//check for allocation error
	if(kernel_data == NULL)
	{
		return -ENOMEM;
	}
	//copy from the user and check if it was complete
	if(copy_from_user ((void*)kernel_data,(void*)DATA,sizeof(dpsfr)) != 0)
	{
		kfree(kernel_data);
		return -ENOMEM;
	}
	//now we get the parameters for the function 
	pid_t pid = kernel_data->pid;
	
	int proc_restriction_level = kernel_data->proc_restriction_level;
	
	scr* restrictions_list = kernel_data-> restrictions_list;
	
	int list_size = kernel_data->list_size;
	
	//now that we have the parameters we can free kernel_data
	kfree(kernel_data);
	//input check:
	if(pid < 0){
		return -ESRCH;
	}
	
	task_t* p = find_task_by_pid(pid);
	
	if(p == NULL){
		
		return -ESRCH;
	}
	if(proc_restriction_level<0 || proc_restriction_level>2 || list_size<0){
		return -EINVAL;
	}

	//implemention of the function:
	p->restriction_level = proc_restriction_level;
	if(p->restrictions_list != NULL){
		kfree(p->restrictions_list);
		p->restrictions_list = NULL;
	}
	//allocate the memory needed 
	p->restrictions_list = kmalloc(list_size*(sizeof(*restrictions_list)),GFP_KERNEL);
	if(p->restrictions_list == NULL){
		return -ENOMEM;
	}
	
	if(copy_from_user(p->restrictions_list,restrictions_list,
	list_size*(sizeof(*restrictions_list))) != 0){
		kfree(p->restrictions_list);
		return -ENOMEM;
	}
	//update the scr_array_size
	p->restrictions_list_size = list_size;
	
	return 0;//TODO:On any other failure errno should contain ​EINVAL
}

/* System Call number 244 */
int sys_set_proc_restriction(pid_t pid, int proc_restriction_level) {
	if(pid < 0){
		return -ESRCH;
	}
	task_t* p = find_task_by_pid(pid);
	if(p == NULL){
		return -ESRCH;
	}
	if(proc_restriction_level<0 || proc_restriction_level>2){
		return -EINVAL;
	}
	
	//implemention of the function:
	p->restriction_level = proc_restriction_level;
	return 0;//TODO:On any other failure errno should contain ​EINVAL
}

/* System Call number 245 */
int sys_get_process_log (pid_t pid, int size, fai* user_mem){

	if(pid < 0){
		return -ESRCH;
	}
	task_t* p = find_task_by_pid(pid);
	if(p == NULL){
		return -ESRCH;
	}
	if(size > p->log_size || size<0){
		return -EINVAL;
	}
	fai temp[100];
	//implemention of the function:
	if((p->log_records_index - size) >= 0){
		if(copy_to_user(user_mem, &(p->log_records[p->log_records_index-size]), size*(sizeof(fai))) != 0){
			return -ENOMEM;
		}
	}else{//for example: we entered 120(index=119) and want last 30. need to get 89-99 and then 0-20
		int from_end_of_array = -(p->log_records_index - size) ;
		memcpy(temp, &(p->log_records[(100-from_end_of_array)]), (from_end_of_array)*(sizeof(fai)));
		int from_start_of_array = size- from_end_of_array;
		memcpy(&(temp[from_end_of_array]), (p->log_records), (from_start_of_array)*(sizeof(fai)));
		if(copy_to_user(user_mem, temp, size*(sizeof(fai))) != 0){
			return -ENOMEM;
		}
	}
	return 0;//TODO:On any other failure errno should contain ​EINVAL
}

