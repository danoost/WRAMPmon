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

# This routine is the default interrupt handler for the monitor
.text
.global interrupt_handler
interrupt_handler:
	# Firstly we save the registers
	sw	$1, regsave+1($0)
	la	$1, regsave
	sw	$2, 2($1)
	sw	$3, 3($1)
	sw	$4, 4($1)
	sw	$5, 5($1)
	sw	$6, 6($1)
	sw	$7, 7($1)
	sw	$8, 8($1)
	sw	$9, 9($1)
	sw	$10, 10($1)
	sw	$11, 11($1)
	sw	$12, 12($1)

	movsg	$2, $ers
	sw	$2, 13($1)
	
	sw	$sp, 14($1)
	sw	$ra, 15($1)

	# Start using the monitor stack again
	la	$sp, monitor_stack
	subui	$sp, $sp, 4

	movsg	$1, $estat
	sw	$1, 0($sp)
	movsg	$1, $cctrl
	sw	$1, 1($sp)
	movsg	$1, $ear
	sw	$1, program_counter($0)
	sw	$1, 2($sp)
	jal	handle_interrupt

	# And then jump into the command line interface
	jal	enable_ints
	
	j	cli

.global setup_ints
setup_ints:
	# Setup the exception vector
	la	$1, interrupt_handler
	movgs	$evec, $1

	# Setup the CPU control register
	movsg	$1, $cctrl
	andi	$1, $1, 0xf
	ori	$1, $1, 0xa
	movgs	$cctrl, $1

	jr	$ra

.global enable_ints
enable_ints:
	movsg	$1, $cctrl
	ori	$1, $1, 0x2
	movgs	$cctrl, $1
	jr	$ra