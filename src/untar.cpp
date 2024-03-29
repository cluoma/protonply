//
// Created by colin on 08/01/23.
//
// Modified version of libarchive/examples/untar.c published under public domain
// https://github.com/libarchive/libarchive/blob/master/examples/untar.c
//

#include "untar.h"

#include <archive.h>
#include <archive_entry.h>
#include <cstdlib>
#include <iostream>
#include <filesystem>

static int copy_data(struct archive *ar, struct archive *aw)
{
    int r;
    const void *buff;
    size_t size;
    la_int64_t offset;

    for (;;) {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r < ARCHIVE_OK)
            return (r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK) {
            fprintf(stderr, "%s\n", archive_error_string(aw));
            return (r);
        }
    }
}

int untar(const char * arch_filename, const char * out_dir)
{
    int ret = UNTAR_SUCCCES;

    std::filesystem::path dir(out_dir);

    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags;
    int r;

    /* Select which attributes we want to restore. */
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);

    /* Open tar file */
    if (archive_read_open_filename(a, arch_filename, 10240)) {
        ret = UNTAR_ERROR;
        goto bad;
    }


    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(a));
        if (r < ARCHIVE_WARN) {
            ret = UNTAR_ERROR;
            goto bad;
        }

        std::cout << archive_entry_pathname(entry) << '\n';
        archive_entry_set_pathname(entry, (dir / archive_entry_pathname(entry)).c_str());

        r = archive_write_header(ext, entry);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        else if (archive_entry_size(entry) > 0) {
            r = copy_data(a, ext);
            if (r < ARCHIVE_OK)
                fprintf(stderr, "%s\n", archive_error_string(ext));
            if (r < ARCHIVE_WARN) {
                ret = UNTAR_ERROR;
                goto bad;
            }
        }
        r = archive_write_finish_entry(ext);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN) {
            ret = UNTAR_ERROR;
            goto bad;
        }
    }

bad:
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);

    return ret;
}