#include <linux/version.h>
#include <linux/skbuff.h>


void rwnx_gki_skb_append(struct sk_buff *old, struct sk_buff *newsk, struct sk_buff_head *list)
{
	unsigned long flags;
	struct sk_buff *prev = old;
	struct sk_buff *next = prev->next;
	spin_lock_irqsave(&list->lock, flags);
	WRITE_ONCE(newsk->next, next);
	WRITE_ONCE(newsk->prev, prev);
	WRITE_ONCE(next->prev, newsk);
	WRITE_ONCE(prev->next, newsk);
	list->qlen++;
	spin_unlock_irqrestore(&list->lock, flags);
}

