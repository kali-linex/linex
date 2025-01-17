==========================
Samsung ARM Linex Overview
==========================

Introduction
------------

  The Samsung range of ARM SoCs spans many similar devices, from the initial
  ARM9 through to the newest ARM cores. This document shows an overview of
  the current kernel support, how to use it and where to find the code
  that supports this.

  The currently supported SoCs are:

  - S3C64XX: S3C6400 and S3C6410
  - S5PC110 / S5PV210


Configuration
-------------

  A number of configurations are supplied, as there is no current way of
  unifying all the SoCs into one kernel.

  s5pc110_defconfig
	- S5PC110 specific default configuration
  s5pv210_defconfig
	- S5PV210 specific default configuration


Layout
------

  The directory layout is currently being restructured, and consists of
  several platform directories and then the machine specific directories
  of the CPUs being built for.

  plat-samsung provides the base for all the implementations, and is the
  last in the line of include directories that are processed for the build
  specific information. It contains the base clock, GPIO and device definitions
  to get the system running.

  plat-s5p is for s5p specific builds, and contains common support for the
  S5P specific systems. Not all S5Ps use all the features in this directory
  due to differences in the hardware.


Layout changes
--------------

  The old plat-s3c and plat-s5pc1xx directories have been removed, with
  support moved to either plat-samsung or plat-s5p as necessary. These moves
  where to simplify the include and dependency issues involved with having
  so many different platform directories.


Port Contributors
-----------------

  Ben Dooks (BJD)
  Vincent Sanders
  Herbert Potzl
  Arnaud Patard (RTP)
  Roc Wu
  Klaus Fetscher
  Dimitry Andric
  Shannon Holland
  Guillaume Gourat (NexVision)
  Christer Weinigel (wingel) (Acer N30)
  Lucas Correia Villa Real (S3C2400 port)


Document Author
---------------

Copyright 2009-2010 Ben Dooks <ben-linex@fluff.org>
