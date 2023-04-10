#include <main.h>

extern int tester_Simulated_traffic_lights(int argc, char * argv[]);

static void do_system_initial(void)
{
	malloc_init();

	s5pv210_clk_initial();
	s5pv210_irq_initial();
	s5pv210_tick_initial();
	s5pv210_tick_delay_initial();
	s5pv210_serial_initial();
	s5pv210_fb_initial();

	led_initial();
	beep_initial();
	key_initial();
}

int main(int argc, char * argv[])
{
	do_system_initial();

	tester_Simulated_traffic_lights(argc, argv);
	return 0;
}
