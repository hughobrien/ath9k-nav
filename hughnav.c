#include <linux/module.h>
//#include <linux/init.h> // seems unnecessary
//#include <linux/kernel.h> // seems unnecessary
#include "../ath9k/debug.h"
#include "../ath9k/reg.h"
#include "../ath9k/hw.h"

static int hughnav_value = 0;
int nav_update_interval __read_mostly = HZ/30;
struct delayed_work nav_read_work;

extern int read_hughnav(void);

static void nav_reading_update(struct work_struct *w)
{
	hughnav_value = read_hughnav(); // call update function here
	printk(KERN_ALERT "hughnav running, nav:%d\n", hughnav_value);
	schedule_delayed_work(&nav_read_work, nav_update_interval); // reschedule
}

static int start_nav_update_timer(void) // setup periodic reading
{
	INIT_DEFERRABLE_WORK(&nav_read_work, nav_reading_update); //macro to setup the work?
	schedule_delayed_work(&nav_read_work, nav_update_interval);
	return 0;
}

static int hughnav_init(void)
{
	printk(KERN_ALERT "hughnav started, nav:%d, interval=%d\n", hughnav_value, nav_update_interval);
	start_nav_update_timer();
	return 0;
}

static void hughnav_exit(void)
{
	cancel_delayed_work(&nav_read_work);
	printk(KERN_ALERT "hughnav exited, nav:%d\n", hughnav_value);
	printk(KERN_ALERT "reg: %d\n", AR_NAV);
}

module_init(hughnav_init);
module_exit(hughnav_exit);
