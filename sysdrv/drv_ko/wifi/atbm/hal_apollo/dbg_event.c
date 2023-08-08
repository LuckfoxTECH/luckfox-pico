//#define DBG_EVENT_LOG 
#ifdef DBG_EVENT_LOG
#include <net/atbm_mac80211.h>
#define ELOG_LEN 64
struct elog_id{
	unsigned int id;
	unsigned int valid;
	unsigned long  tim;
	unsigned int data;
};
struct elog_s {
	u8 start;
	u8 index;
	u16 len;
	struct elog_id e_id[ELOG_LEN];
};

#define EVENT_MEM_ALIGN   8
#ifndef ALIGN
#define ALIGN(a,b)			(((a) + ((b) - 1)) & (~((b)-1)))
#endif

struct elog_s elogdata;

struct elog_s *elog = (struct elog_s * )&elogdata;
void ELOG_INIT(void)
{
	int len =0;
	elog = (struct elog_s * )&elogdata;
	//elog = (struct elog_s * )ALIGN((u32)&SM_Region.data[0],EVENT_MEM_ALIGN);
	//ASSERT((SM_MEM_LEN-sizeof(elog)-EVENT_MEM_ALIGN)>sizeof(struct elog_id),0x99);
	//len = (SM_MEM_LEN-sizeof(elog)-EVENT_MEM_ALIGN)/sizeof(struct elog_id);
	//if(len > ELOG_LEN)
	len = ELOG_LEN;
	memset(elog,0,sizeof(struct elog_id));
	elog->start = 1;
	elog->index = 0;
	elog->len = len;
	atbm_printk_always("ELOG_INIT len %d \n",elog->len);

}

void ELOG_Add(u8 id,u32 data)
{
	if(elog->start ){
		elog->e_id[elog->index].id = id;
		elog->e_id[elog->index].data = data;
		elog->e_id[elog->index].tim = jiffies;
		elog->e_id[elog->index].valid = 1;
		elog->index++;
		if(elog->index >= elog->len){			
			elog->index = 0;
		}
	}
	
}
void ELOG_Stop(void)
{
	elog->start = 0;
	atbm_printk_always("E_Stop\n");
}
void ELOG_Start(void)
{
	elog->start = 1;
}

void ELOG_Clear(void)
{
	int i=0;
	elog->index=0;
	while(i<elog->len){
		elog->e_id[i].valid = 0;
		i++;
	}
}

void ELOG_Show(void)
{
	u32 tim = 0;
	u8 index = elog->index;
	int loop = 0;
	atbm_printk_always("ELOG_Show id:data:tim (%d:%d)\n",elog->index,elog->len);
	do {
		loop++;
		if(index >= elog->len){			
			index = 0;
		}
		if(!elog->e_id[index].valid){
			index++;
			continue;
		}
		atbm_printk_always("%02x|%08x|%06lx\n",elog->e_id[index].id,
								elog->e_id[index].data,
								(elog->e_id[index].tim-tim));
		tim = elog->e_id[index].tim;
		index++;
	}while(loop < elog->len);//(index!=elog->index);
	atbm_printk_always("ELOG_Show end\n");
}
#endif //if DBG_EVENT_LOG
