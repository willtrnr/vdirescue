vdirescue
=========

So things exploded and I ended up with a messed up and truncated VDI file (for reasons unknown so far).

This is a small thing I put together to expand a dynamic (or static, technically it should work, but untested) VDI file into a raw disk image.

Similarly to ddrescue whacked out blocks will be zeroed out in the output.

Building
--------

I didn't bother making a makefile, so you can just call the `./build.sh`.
