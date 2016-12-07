#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vdi.h"


vdi_result _vdi_read_header(vdi_image* img) {
    if (fseek(img->handle, 0, SEEK_SET) != 0) {
        return VDI_KO;
    }

    if (fread(&img->header, sizeof(vdi_header), 1, img->handle) != 1) {
        return VDI_KO;
    }

    if (img->header.sig != 0xBEDA107F) {
        return VDI_KO;
    }

    if (img->header.header_size != 0x0190) {
        return VDI_KO;
    }

    return VDI_OK;
}

vdi_result _vdi_read_blockmap(vdi_image* img) {
    if (fseek(img->handle, img->header.offset_blocks, SEEK_SET) != 0) {
        return VDI_KO;
    }

    if ((img->blockmap = malloc(sizeof(_vdi_blockmap) * img->header.blocks_in_hdd)) == NULL) {
        return VDI_KO;
    }

    if (fread(img->blockmap, sizeof(_vdi_blockmap), img->header.blocks_in_hdd, img->handle) != img->header.blocks_in_hdd) {
        free(img->blockmap);
        img->blockmap = NULL;
        return VDI_KO;
    }

    return VDI_OK;
}

vdi_result vdi_open(vdi_image** img, const char* filename) {
    if ((*img = malloc(sizeof(vdi_image))) == NULL) {
        return VDI_KO;
    }

    (*img)->handle = NULL;
    (*img)->blockmap = NULL;

    if (((*img)->handle = fopen(filename, "rb")) == NULL) {
        vdi_close(*img);
        return VDI_KO;
    }

    if (_vdi_read_header(*img) != VDI_OK) {
        vdi_close(*img);
        return VDI_KO;
    }

    if (_vdi_read_blockmap(*img) != VDI_OK) {
        vdi_close(*img);
        return VDI_KO;
    }

    return VDI_OK;
}

vdi_result vdi_read_block(vdi_image* img, uint32_t block, void* buf) {
    if (block >= img->header.blocks_in_hdd) {
        return VDI_KO;
    }

    uint32_t phys_block = img->blockmap[block];

    if (phys_block == UINT32_MAX) {
        memset(buf, 0, img->header.block_size);
        return VDI_OK;
    }

    size_t offset = img->header.offset_data + img->header.block_size * phys_block;

    if (fseek(img->handle, offset, SEEK_SET) != 0) {
        return VDI_KO;
    }

    if (fread(buf, img->header.block_size, 1, img->handle) != 1) {
        return VDI_KO;
    }

    return VDI_OK;
}

vdi_result vdi_close(vdi_image* img) {
    if (img != NULL) {
        if (img->handle != NULL) {
            fclose(img->handle);
            img->handle = NULL;
        }

        free(img->blockmap);
        img->blockmap = NULL;

        free(img);
    }
    return VDI_OK;
}
