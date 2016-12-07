#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "vdi.h"


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <src> [dest]\n", argv[0]);
        return 0;
    }

    vdi_image* img;
    if (vdi_open(&img, argv[1]) != VDI_OK) {
        return 1;
    }

    printf("Disk Size: %"PRIu64"\n", img->header.disk_size);
    printf("Block Size: %"PRIu32"\n", img->header.block_size);
    printf("Block Count: %"PRIu32"\n", img->header.blocks_in_hdd);
    printf("Blocks Offset: 0x%"PRIX32"\n", img->header.offset_blocks);
    printf("Data Offset: 0x%"PRIX32"\n", img->header.offset_data);

    if (argc > 2) {
        printf("\n%s%s%s\n", "Cloning to ", argv[2], "...");

        void* buf;
        if ((buf = malloc(img->header.block_size)) == NULL) {
            fprintf(stderr, "%s\n", "Could not allocate the block memory buffer");
            vdi_close(img);
            return 1;
        }

        FILE* fp;
        if ((fp = fopen(argv[2], "wb")) == NULL) {
            fprintf(stderr, "%s\n", "Could not open the dest file for writing");
            vdi_close(img);
            free(buf);
            return 1;
        }

        uint32_t failed = 0;
        for (uint32_t i = 0; i < img->header.blocks_in_hdd; ++i) {
            if (i % 100 == 0) {
                putchar('.');
                fflush(stdout);
            }

            if (vdi_read_block(img, i, buf) != VDI_OK) {
                ++failed;
                putchar('!');
                memset(buf, 0, img->header.block_size);
            }

            if (fwrite(buf, img->header.block_size, 1, fp) != 1) {
                fprintf(stderr, "%s\n", "Failed to write block to file");
                vdi_close(img);
                free(buf);
                fclose(fp);
                return 1;
            }
        }

        printf("\n%s\n", "Done!");
        printf("%s%"PRIu32"%s\n", "Rcovered ", img->header.blocks_in_hdd - failed, " blocks.");

        free(buf);
        fclose(fp);
    }

    vdi_close(img);
    return 0;
}
