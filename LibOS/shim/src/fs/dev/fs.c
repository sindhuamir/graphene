/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2014 Stony Brook University */

/*!
 * \file
 *
 * This file contains the implementation of `/dev` pseudo-filesystem.
 */

#include "shim_fs.h"
#include "shim_fs_pseudo.h"

extern const struct pseudo_fs_ops dev_null_fs_ops;
extern const struct pseudo_fs_ops dev_tty_fs_ops;
extern const struct pseudo_fs_ops dev_zero_fs_ops;
extern const struct pseudo_fs_ops dev_random_fs_ops;
extern const struct pseudo_fs_ops dev_urandom_fs_ops;
extern const struct pseudo_fs_ops dev_stdin_fs_ops;
extern const struct pseudo_fs_ops dev_stdout_fs_ops;
extern const struct pseudo_fs_ops dev_stderr_fs_ops;

extern const struct pseudo_fs_ops dev_attestation_fs_ops;
extern const struct pseudo_dir dev_attestation_dir;

static const struct pseudo_dir dev_root_dir = {
    .size = 9,
    .ent = {
        {.name   = "null",
         .fs_ops = &dev_null_fs_ops,
         .type   = LINUX_DT_CHR},
        {.name   = "tty",
         .fs_ops = &dev_tty_fs_ops,
         .type   = LINUX_DT_CHR},
        {.name   = "zero",
         .fs_ops = &dev_zero_fs_ops,
         .type   = LINUX_DT_CHR},
        {.name   = "random",
         .fs_ops = &dev_random_fs_ops,
         .type   = LINUX_DT_CHR},
        {.name   = "urandom",
         .fs_ops = &dev_urandom_fs_ops,
         .type   = LINUX_DT_CHR},
        {.name   = "stdin",
         .fs_ops = &dev_stdin_fs_ops,
         .type   = LINUX_DT_LNK},
        {.name   = "stdout",
         .fs_ops = &dev_stdout_fs_ops,
         .type   = LINUX_DT_LNK},
        {.name   = "stderr",
         .fs_ops = &dev_stderr_fs_ops,
         .type   = LINUX_DT_LNK},
        {.name   = "attestation",
         .fs_ops = &dev_attestation_fs_ops,
         .type   = LINUX_DT_DIR,
         .dir    = &dev_attestation_dir},
        },
};

static const struct pseudo_fs_ops dev_root_fs = {
    .open = &pseudo_dir_open,
    .mode = &pseudo_dir_mode,
    .stat = &pseudo_dir_stat,
};

static const struct pseudo_ent dev_root_ent = {
    .name   = "",
    .fs_ops = &dev_root_fs,
    .dir    = &dev_root_dir,
};

static int dev_open(struct shim_handle* hdl, struct shim_dentry* dent, int flags) {
    return pseudo_open(hdl, dent, flags, &dev_root_ent);
}

static int dev_lookup(struct shim_dentry* dent) {
    return pseudo_lookup(dent, &dev_root_ent);
}

static int dev_mode(struct shim_dentry* dent, mode_t* mode) {
    return pseudo_mode(dent, mode, &dev_root_ent);
}

static int dev_readdir(struct shim_dentry* dent, readdir_callback_t callback, void* arg) {
    return pseudo_readdir(dent, callback, arg, &dev_root_ent);
}

static int dev_stat(struct shim_dentry* dent, struct stat* buf) {
    return pseudo_stat(dent, buf, &dev_root_ent);
}

static int dev_hstat(struct shim_handle* hdl, struct stat* buf) {
    return pseudo_hstat(hdl, buf, &dev_root_ent);
}

static int dev_follow_link(struct shim_dentry* dent, struct shim_qstr* link) {
    return pseudo_follow_link(dent, link, &dev_root_ent);
}

static ssize_t dev_read(struct shim_handle* hdl, void* buf, size_t count) {
    if (hdl->type == TYPE_STR) {
        return str_read(hdl, buf, count);
    }

    assert(hdl->type == TYPE_DEV);
    if (!hdl->info.dev.dev_ops.read)
        return -EACCES;
    return hdl->info.dev.dev_ops.read(hdl, buf, count);
}

static ssize_t dev_write(struct shim_handle* hdl, const void* buf, size_t count) {
    if (hdl->type == TYPE_STR) {
        return str_write(hdl, buf, count);
    }

    assert(hdl->type == TYPE_DEV);
    if (!hdl->info.dev.dev_ops.write)
        return -EACCES;
    return hdl->info.dev.dev_ops.write(hdl, buf, count);
}

static off_t dev_seek(struct shim_handle* hdl, off_t offset, int whence) {
    if (hdl->type == TYPE_STR) {
        return str_seek(hdl, offset, whence);
    }

    assert(hdl->type == TYPE_DEV);
    if (!hdl->info.dev.dev_ops.seek)
        return -EACCES;
    return hdl->info.dev.dev_ops.seek(hdl, offset, whence);
}

static int dev_truncate(struct shim_handle* hdl, off_t len) {
    if (hdl->type == TYPE_STR) {
        /* e.g. fopen("w") wants to truncate; since these are pre-populated files, just ignore */
        return 0;
    }

    assert(hdl->type == TYPE_DEV);
    if (!hdl->info.dev.dev_ops.truncate)
        return -EACCES;
    return hdl->info.dev.dev_ops.truncate(hdl, len);
}

static int dev_flush(struct shim_handle* hdl) {
    if (hdl->type == TYPE_STR) {
        return str_flush(hdl);
    }

    assert(hdl->type == TYPE_DEV);
    if (!hdl->info.dev.dev_ops.flush)
        return 0;
    return hdl->info.dev.dev_ops.flush(hdl);
}

static int dev_close(struct shim_handle* hdl) {
    if (hdl->type == TYPE_STR) {
        return str_close(hdl);
    }

    if (hdl->type == TYPE_PSEUDO) {
        /* e.g. a directory */
        return 0;
    }

    assert(hdl->type == TYPE_DEV);
    if (!hdl->info.dev.dev_ops.close)
        return 0;
    return hdl->info.dev.dev_ops.close(hdl);
}

static off_t dev_poll(struct shim_handle* hdl, int poll_type) {
    if (poll_type == FS_POLL_SZ)
        return 0;

    assert(hdl->type == TYPE_DEV);

    off_t ret = 0;
    if ((poll_type & FS_POLL_RD) && hdl->info.dev.dev_ops.read)
        ret |= FS_POLL_RD;
    if ((poll_type & FS_POLL_WR) && hdl->info.dev.dev_ops.write)
        ret |= FS_POLL_WR;

    return ret;
}

int dev_update_dev_ops(struct shim_handle* hdl) {
    struct shim_dentry* dent = hdl->dentry;
    assert(dent);

    /* simply reopen pseudo-file, this will update dev_ops function pointers to correct values */
    return pseudo_open(hdl, dent, /*flags=*/0, &dev_root_ent);
}

struct shim_fs_ops dev_fs_ops = {
    .mount    = &pseudo_mount,
    .unmount  = &pseudo_unmount,
    .flush    = &dev_flush,
    .close    = &dev_close,
    .read     = &dev_read,
    .write    = &dev_write,
    .seek     = &dev_seek,
    .hstat    = &dev_hstat,
    .poll     = &dev_poll,
    .truncate = &dev_truncate,
};

struct shim_d_ops dev_d_ops = {
    .open        = &dev_open,
    .lookup      = &dev_lookup,
    .mode        = &dev_mode,
    .readdir     = &dev_readdir,
    .stat        = &dev_stat,
    .follow_link = &dev_follow_link,
};

struct shim_fs dev_builtin_fs = {
    .name   = "dev",
    .fs_ops = &dev_fs_ops,
    .d_ops  = &dev_d_ops,
};

int init_devfs(void) {
    struct pseudo2_ent* root = pseudo_add_root_dir("dev");

    /* Device minor numbers for pseudo-devices:
     * https://elixir.bootlin.com/linux/v5.9/source/drivers/char/mem.c#L950 */

    struct pseudo2_ent* null = pseudo_add_dev(root, "null");
    null->perm = PSEUDO_MODE_FILE_RW;
    null->dev.major = 1;
    null->dev.minor = 3;
    null->dev.dev_ops.read = &dev_null_read;
    null->dev.dev_ops.write = &dev_null_write;
    null->dev.dev_ops.seek = &dev_null_seek;
    null->dev.dev_ops.truncate = &dev_null_truncate;

    struct pseudo2_ent* zero = pseudo_add_dev(root, "zero");
    zero->perm = PSEUDO_MODE_FILE_RW;
    zero->dev.major = 1;
    zero->dev.minor = 5;
    zero->dev.dev_ops.read = &dev_zero_read;
    zero->dev.dev_ops.write = &dev_null_write;
    zero->dev.dev_ops.seek = &dev_null_seek;
    zero->dev.dev_ops.truncate = &dev_null_truncate;

    struct pseudo2_ent* random = pseudo_add_dev(root, "random");
    random->perm = PSEUDO_MODE_FILE_RW;
    random->dev.major = 1;
    random->dev.minor = 8;
    random->dev.dev_ops.read = &dev_random_read;
    /* writes in /dev/random add entropy in normal Linux, but not implemented in Graphene */
    random->dev.dev_ops.write = &dev_null_write;
    random->dev.dev_ops.seek = &dev_null_seek;

    struct pseudo2_ent* urandom = pseudo_add_dev(root, "urandom");
    urandom->perm = PSEUDO_MODE_FILE_RW;
    urandom->dev.major = 1;
    urandom->dev.minor = 9;
    /* /dev/urandom is implemented the same as /dev/random, so it has the same operations */
    urandom->dev.dev_ops = random->dev.dev_ops;

    struct pseudo2_ent* stdin = pseudo_add_link(root, "stdin", NULL);
    stdin->link.target = "/proc/self/fd/0";
    struct pseudo2_ent* stdout = pseudo_add_link(root, "stdout", NULL);
    stdout->link.target = "/proc/self/fd/0";
    struct pseudo2_ent* stderr = pseudo_add_link(root, "stderr", NULL);
    stderr->link.target = "/proc/self/fd/0";

    return 0;
}
