
#include <linux/kernel.h>	
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/irq.h>

void my_do_tasklet(unsigned long);
DECLARE_TASKLET(my_tasklet, my_do_tasklet, 0);

void my_do_tasklet(unsigned long temp)
{
	printk(KERN_INFO "Keyboard Interrupt!!");
}

/* 
 * This function services keyboard interrupts. It reads the relevant
 * information from the keyboard and then puts the non time critical
 * part into the work queue. This will be run when the kernel considers it safe.
 */
irqreturn_t irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
	/* 
	 * This variables are static because they need to be
	 * accessible (through pointers) to the bottom half routine.
	 */
	static int initialised = 0;
	static unsigned char scancode;
	static struct work_struct task;
	unsigned char status;

	/* 
	 * Read keyboard status
	 */
	status = inb(0x64);
	scancode = inb(0x60);

	tasklet_schedule(&my_tasklet);

	return IRQ_HANDLED;
}

/* 
 * Initialize the module - register the IRQ handler 
 */
int init_module()
{
	/* 
	 * Since the keyboard handler won't co-exist with another handler,
	 * such as us, we have to disable it (free its IRQ) before we do
	 * anything.  Since we don't know where it is, there's no way to
	 * reinstate it later - so the computer will have to be rebooted
	 * when we're done.
	 */
	free_irq(1, NULL);

	/* 
	 * Request IRQ 1, the keyboard IRQ, to go to our irq_handler.
	 * SA_SHIRQ means we're willing to have othe handlers on this IRQ.
	 * SA_INTERRUPT can be used to make the handler into a fast interrupt.
	 */
	return request_irq(1,	/* The number of the keyboard IRQ on PCs */
			   irq_handler,	/* our handler */
			   IRQF_SHARED, "test_keyboard_irq_handler",
			   (void *)(irq_handler));
}

/* 
 * Cleanup 
 */
void cleanup_module()
{
	/* 
	 * This is only here for completeness. It's totally irrelevant, since
	 * we don't have a way to restore the normal keyboard interrupt so the
	 * computer is completely useless and has to be rebooted.
	 */
	free_irq(1, NULL);
}

/* 
 * some work_queue related functions are just available to GPL licensed Modules
 */
MODULE_LICENSE("GPL");
