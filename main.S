.bss
# This is the monitor stack
.global monitor_stack
		.space	1024
monitor_stack:
.text
.global main
# This sets up all kinds of stuff, like the special registers,
# stack, default exception handlers, etc...
main:
	subui	$1, $0, 1
	addu	$2, $0, $0
mem_clear_loop:
	sw	$1, 0($2)

	addui	$2, $2, 1
	la	$3, 0x1ffff
	seq	$3, $3, $2
	
	beqz	$3, mem_clear_loop
	
	# Setup the stack pointer for the monitor
	la	$sp, monitor_stack
	
	# Here we actually start our monitor code
	subui	$sp, $sp, 4

	# Setup the interrupts
	jal	setup_ints

	# Lock the ROMs
	jal	lock_rom
	
	# Initialise the serial port
	jal	init_serial
	
	# Enable interrupts
	jal	enable_ints
	
	# Initialise the stdio library
	jal	stdio_init
	# Setup the breakpoint table & step mode stuff
	jal	on_reset
	# Print the welcome message
	jal	welcome
	# Begin the CLI loop
	jal	cli
	
.global invoke
invoke:
	subui	$sp, $sp, 1
	sw	$ra, 0($sp)
	lw	$1, 1($sp)
	jalr	$1
	lw	$ra, 0($sp)
	addui	$sp, $sp, 1
	jr	$ra