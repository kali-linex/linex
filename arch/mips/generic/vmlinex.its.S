/* SPDX-License-Identifier: GPL-2.0 */
/dts-v1/;

/ {
	description = KERNEL_NAME;
	#address-cells = <ADDR_CELLS>;

	images {
		kernel {
			description = KERNEL_NAME;
			data = /incbin/(VMLINEX_BINARY);
			type = "kernel";
			arch = "mips";
			os = "linex";
			compression = VMLINEX_COMPRESSION;
			load = /bits/ ADDR_BITS <VMLINEX_LOAD_ADDRESS>;
			entry = /bits/ ADDR_BITS <VMLINEX_ENTRY_ADDRESS>;
			hash {
				algo = "sha1";
			};
		};
	};

	configurations {
		default = "conf-default";

		conf-default {
			description = "Generic Linex kernel";
			kernel = "kernel";
		};
	};
};
