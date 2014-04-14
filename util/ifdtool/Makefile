#
# Makefile for ifdtool utility
#
# Copyright (C) 2011 by Google Inc
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published b
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

PROGRAM = ifdtool

CC      = gcc
INSTALL = /usr/bin/install
PREFIX  = /usr/local
CFLAGS  = -O2 -g -Wall -W
LDFLAGS = 

OBJS = ifdtool.o

all: dep $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(PROGRAM) *.o *~

distclean: clean
	rm -f .dependencies

dep:
	@$(CC) $(CFLAGS) -MM *.c > .dependencies

install: $(PROGRAM)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) $(PROGRAM) $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man8
	$(INSTALL) $(PROGRAM).8 $(DESTDIR)$(PREFIX)/share/man/man8

.PHONY: all clean distclean dep 

-include .dependencies

