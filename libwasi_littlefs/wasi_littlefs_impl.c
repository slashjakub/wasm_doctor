#include <assert.h>
#include <errno.h>

#include "wasi_littlefs_impl.h"
#include "wasi_vfs_impl_littlefs.h"

int
lfs_error_to_errno(enum lfs_error lfs_error)
{
        assert(lfs_error <= 0);
        int error = 0;
        switch (lfs_error) {
        case LFS_ERR_OK:
                error = 0;
                break;
        case LFS_ERR_IO:
                error = EIO;
                break;
        case LFS_ERR_CORRUPT:
                error = EIO; /* XXX */
                break;
        case LFS_ERR_NOENT:
                error = ENOENT;
                break;
        case LFS_ERR_EXIST:
                error = EEXIST;
                break;
        case LFS_ERR_NOTDIR:
                error = ENOTDIR;
                break;
        case LFS_ERR_ISDIR:
                error = EISDIR;
                break;
        case LFS_ERR_NOTEMPTY:
                error = ENOTEMPTY;
                break;
        case LFS_ERR_BADF:
                error = EBADF;
                break;
        case LFS_ERR_FBIG:
                error = EFBIG;
                break;
        case LFS_ERR_INVAL:
                error = EINVAL;
                break;
        case LFS_ERR_NOSPC:
                error = ENOSPC;
                break;
        case LFS_ERR_NOMEM:
                error = ENOMEM;
                break;
        case LFS_ERR_NOATTR:
                error = ENOENT; /* XXX */
                break;
        case LFS_ERR_NAMETOOLONG:
                error = ENAMETOOLONG;
                break;
        }
        return error;
}

struct wasi_fdinfo_lfs *
wasi_fdinfo_to_lfs(struct wasi_fdinfo *fdinfo)
{
        assert(fdinfo->type == WASI_FDINFO_USER);
        assert(wasi_fdinfo_is_lfs(fdinfo));
        return (void *)fdinfo;
}

struct wasi_vfs_lfs *
wasi_vfs_to_lfs(struct wasi_vfs *vfs)
{
        return (void *)vfs;
}
