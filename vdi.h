#ifndef VDI_H
#define VDI_H

#include <stdio.h>
#include <stdint.h>


#define VDI_OK 0
#define VDI_KO 1
typedef uint8_t vdi_result;


typedef struct __attribute__((__packed__)) {
    uint8_t header[64];
    uint32_t sig;
    uint16_t version_major;
    uint16_t version_minor;
    uint32_t header_size;
    uint32_t image_type;
    uint32_t image_flags;
    uint8_t description[256];
    uint32_t offset_blocks;
    uint32_t offset_data;
    uint32_t cylinders;
    uint32_t heads;
    uint32_t sectors;
    uint32_t sector_size;
    uint8_t _pad_a[4];
    uint64_t disk_size;
    uint32_t block_size;
    uint32_t block_extra_data;
    uint32_t blocks_in_hdd;
    uint32_t blocks_allocated;
    uint8_t uuid[16];
    uint8_t uuid_snap[16];
    uint8_t uuid_link[16];
    uint8_t uuid_parent[16];
    uint8_t _pad_b[56];
} vdi_header;


typedef uint32_t _vdi_blockmap;
typedef _vdi_blockmap* vdi_blockmap;


typedef struct {
    FILE* handle;
    vdi_header header;
    vdi_blockmap blockmap;
} vdi_image;


vdi_result vdi_open(vdi_image**, const char*);
vdi_result vdi_read_block(vdi_image*, uint32_t, void*);
vdi_result vdi_close(vdi_image*);

#endif /* VDI_H */
