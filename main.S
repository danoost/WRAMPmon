########################################################################
# This file is part of WRAMPmon, the WRAMP monitor programe.
#
# Copyright (C) 2019 The University of Waikato, Hamilton, New Zealand.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
########################################################################


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
	# Set all memory from 0-03fff to 0xFFFFFFFF
	lhi	$1, 0xffff
	ori	$1, $1, 0xffff
	addu	$2, $0, $0
mem_clear_loop:
	sw	$1, 0($2)

	addui	$2, $2, 1
	la	$3, 0x03fff
	seq	$3, $3, $2
	
	beqz	$3, mem_clear_loop
	
	# Setup the stack pointer for the monitor
	la	$sp, monitor_stack
	
	# Here we actually start our monitor code
	subui	$sp, $sp, 4

	# Setup the interrupts
	jal	setup_ints

	# Lock the ROMs
	# Rom unlock not supported on Basys boards, so don't bother
	# interacting with memory that does nothing.
	#jal	lock_rom
	
	# Initialise the serial port
	jal	init_serial
	
	# Enable interrupts
	jal	enable_ints
	
	#setup the permission for the ptable
	jal init_ptable
	
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
