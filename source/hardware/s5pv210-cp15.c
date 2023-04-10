#include <types.h>
#include <s5pv210-cp15.h>

/*
 * read co-processor 15, register #1 (control register)
 */
static u32_t read_p15_c1(void)
{
	u32_t value;

	__asm__ __volatile__(
		"mrc p15, 0, %0, c1, c0, 0"
		: "=r" (value)
		:
		: "memory");

	return value;
}

/*
 * write to co-processor 15, register #1 (control register)
 */
static void write_p15_c1(u32_t value)
{
	__asm__ __volatile__(
		"mcr p15, 0, %0, c1, c0, 0"
		:
		: "r" (value)
		: "memory");

	read_p15_c1();
}

/*
 * enable irq interrupt.
 */
void irq_enable(void)
{
	u32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"bic %0, %0, #(1<<7)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "memory");
}

/*
 * disable irq interrupt.
 */
void irq_disable(void)
{
	u32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"orr %0, %0, #(1<<7)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "memory");
}

/*
 * enable fiq interrupt.
 */
void fiq_enable(void)
{
	u32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"bic %0, %0, #(1<<6)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "memory");
}

/*
 * disable fiq interrupt.
 */
void fiq_disable(void)
{
	u32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"orr %0, %0, #(1<<6)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "memory");
}

/*
 * enable icache
 */
void icache_enable(void)
{
	u32_t reg;

	reg = read_p15_c1();
	write_p15_c1(reg | (1<<12));
}

/*
 * disable icache
 */
void icache_disable(void)
{
	u32_t reg;

	reg = read_p15_c1();
	write_p15_c1(reg & ~(1<<12));
}

/*
 * enable dcache
 */
void dcache_enable(void)
{
	u32_t reg;

	reg = read_p15_c1();
	write_p15_c1(reg | (1<<2));
}

/*
 * disable dcache
 */
void dcache_disable(void)
{
	u32_t reg;

	reg = read_p15_c1();
	write_p15_c1(reg & ~(1<<2));
}

/*
 * enable mmu
 */
void mmu_enable(void)
{
	u32_t reg;

	reg = read_p15_c1();
	write_p15_c1(reg | (1<<0));
}

/*
 * disable mmu
 */
void mmu_disable(void)
{
	u32_t reg;

	reg = read_p15_c1();
	write_p15_c1(reg & ~(1<<0));
}

/*
 * enable vector interrupt controller
 */
void vic_enable(void)
{
	u32_t reg;

	reg = read_p15_c1();
	write_p15_c1(reg | (1<<24));
}

/*
 * disable vector interrupt controller
 */
void vic_disable(void)
{
	u32_t reg;

	reg = read_p15_c1();
	write_p15_c1(reg & ~(1<<24));
}

/*
 * enable branch prediction
 */
void branch_enable(void)
{
	u32_t reg;

	reg = read_p15_c1();
	write_p15_c1(reg | (1<<11));
}

/*
 * disable branch prediction
 */
void branch_disable(void)
{
	u32_t reg;

	reg = read_p15_c1();
	write_p15_c1(reg & ~(1<<11));
}
