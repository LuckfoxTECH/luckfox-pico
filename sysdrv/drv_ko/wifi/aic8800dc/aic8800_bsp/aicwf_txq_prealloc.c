#include <linux/slab.h>
#include "aicsdio_txrxif.h"
#include "aic_bsp_driver.h"

struct prealloc_txq{
    int prealloced;
    void *txq;
    size_t size;
};

struct prealloc_txq prealloc_txq;
#define MAX_TXQ_SIZE 100 * 1024

void *aicwf_prealloc_txq_alloc(size_t size)
{

    BUG_ON(size > MAX_TXQ_SIZE);

    //check prealloc_txq.size
    if((int)prealloc_txq.size != (int)size)
    {
        AICWFDBG(LOGINFO, "%s size is diff will to be kzalloc \r\n", __func__);

        if(prealloc_txq.txq != NULL)
        {
            AICWFDBG(LOGINFO, "%s txq to kfree \r\n", __func__);
            kfree(prealloc_txq.txq);
            prealloc_txq.txq = NULL;
        }
        
        prealloc_txq.size = size;
        prealloc_txq.prealloced = 0;
    }

    //check prealloc or not
    if(!prealloc_txq.prealloced)
    {
        prealloc_txq.txq = kzalloc(size, GFP_KERNEL);
        if(!prealloc_txq.txq){
            AICWFDBG(LOGERROR, "%s txq kzalloc fail \r\n", __func__);
        }else{
            AICWFDBG(LOGINFO, "%s txq kzalloc successful \r\n", __func__);
            prealloc_txq.prealloced = 1;
        }
    }else{
         AICWFDBG(LOGINFO, "%s txq not need to kzalloc \r\n", __func__);
    }

    return prealloc_txq.txq;
}
void aicwf_prealloc_txq_free(void)
{
    if(prealloc_txq.txq != NULL)
    {
        AICWFDBG(LOGINFO, "%s txq to kfree \r\n", __func__);
        kfree(prealloc_txq.txq);
        prealloc_txq.txq = NULL;
    }
}

EXPORT_SYMBOL(aicwf_prealloc_txq_alloc);

