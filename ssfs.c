#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/xattr.h>

static const char *dirpath = "/home/ridlo28/Documents";

void Generator(char *desc, char *path, int check, int check2, char *path2){
        char result[1024];
        char flag[10];
        FILE *demo;
        if(check==0) {
                strcpy(flag,"WARNING");
        }
        else{
                strcpy(flag,"INFO");
        }
        time_t t = time(NULL); 
        struct tm tm = *localtime(&t); //nampilin waktu sekarang
        if(check2==0) {
                snprintf(result,1024,"%s::%02d%02d%02d%02d:%02d:%02d::%s::%s", flag, tm.tm_year%100, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, desc, path);
        }
        else{
                snprintf(result,1024,"%s::%02d%02d%02d%02d:%02d:%02d::%s::%s::%s", flag, tm.tm_year%100, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, desc, path, path2);
        }
        demo=fopen("/home/ridlo28/fs.log", "a");
        fprintf(demo, "%s\n", result);
        fclose(demo);
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
        int res;
        // char fpath[1000];
        char fpath[1000];

        sprintf(fpath, "%s%s", dirpath, path);
        // sprintf(fpath, "%s/%s", dirpath, path);
        res = lstat(fpath, stbuf);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_access(const char *path, int mask)
{
        int res;
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        res = access(fpath, mask);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
        int res;
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        Generator("READLINK",fpath,1,0,NULL);
        res = readlink(fpath, buf, size - 1);
        if (res == -1)
                return -errno;

        buf[res] = '\0';
        return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        //Generator("LS",fpath,1,0,NULL);
        DIR *dp;
        struct dirent *de;

        (void) offset;
        (void) fi;

        dp = opendir(fpath);
        if (dp == NULL)
                return -errno;

        while ((de = readdir(dp)) != NULL) {
                struct stat st;
                memset(&st, 0, sizeof(st));
                st.st_ino = de->d_ino;
                st.st_mode = de->d_type << 12;
                if (filler(buf, de->d_name, &st, 0))
                        break;
        }

        closedir(dp);
        return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        int res;
        Generator("MKNOD",fpath,1,0,NULL);
        /* On Linux this could just be 'mknod(path, mode, rdev)' but this
           is more portable */
        if (S_ISREG(mode)) {
                res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
                if (res >= 0)
                        res = close(res);
        } else if (S_ISFIFO(mode))
                res = mkfifo(fpath, mode);
        else
                res = mknod(fpath, mode, rdev);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
        char fpath[1000];
        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        Generator("MKDIR",fpath,1,0,NULL);
        int res;

        res = mkdir(fpath, mode);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_unlink(const char *path)
{
        char fpath[1000];
        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        Generator("RM",fpath,0,0,NULL);
        int res;

        res = unlink(fpath);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_rmdir(const char *path)
{
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        int res;
        Generator("RMDIR",fpath,0,0,NULL);
        res = rmdir(fpath);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_symlink(const char *from, const char *to)
{
        int res;
        char new_from[1000];
        char new_to[1000];
        sprintf(new_from,"%s%s",dirpath,from);
        sprintf(new_to,"%s%s",dirpath,to);
        Generator("SYMLINK",new_from,1,1,new_to);
        res = symlink(new_from, new_to);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_rename(const char *from, const char *to)
{
        int res;
        char new_from[1000];
        char new_to[1000];
        sprintf(new_from,"%s%s",dirpath,from);
        sprintf(new_to,"%s%s",dirpath,to);
        Generator("RENAME",new_from,1,1,new_to);
        res = rename(new_from, new_to);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_link(const char *from, const char *to)
{
        int res;
        char new_from[1000];
        char new_to[1000];
        sprintf(new_from,"%s%s",dirpath,from);
        sprintf(new_to,"%s%s",dirpath,to);
        Generator("LINK",new_from,1,1,new_to);
        res = link(new_from, new_to);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
        int res;
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        Generator("CHMOD",fpath,1,0,NULL);
        res = chmod(fpath, mode);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
        int res;
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        Generator("CHOWN",fpath,1,0,NULL);
        res = lchown(fpath, uid, gid);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
        int res;
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        Generator("TRUNCATE",fpath,1,0,NULL);
        res = truncate(fpath, size);
        if (res == -1)
                return -errno;

        return 0;
}
static int xmp_utimens(const char *path, const struct timespec ts[2])
{
        int res;
        struct timeval tv[2];
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        tv[0].tv_sec = ts[0].tv_sec;
        tv[0].tv_usec = ts[0].tv_nsec / 1000;
        tv[1].tv_sec = ts[1].tv_sec;
        tv[1].tv_usec = ts[1].tv_nsec / 1000;
        Generator("UTIMENS",fpath,1,0,NULL);
        res = utimes(fpath, tv);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
        int res;
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        Generator("CAT",fpath,1,0,NULL);
        res = open(fpath, fi->flags);
        if (res == -1)
                return -errno;

        close(res);
        return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi)
{
        int fd;
        int res;
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        (void) fi;
        fd = open(fpath, O_RDONLY);
        if (fd == -1)
                return -errno;
        Generator("READ",fpath,1,0,NULL);
        res = pread(fd, buf, size, offset);
        if (res == -1)
                res = -errno;

        close(fd);
        return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi)
{
        int fd;
        int res;
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        (void) fi;
        fd = open(fpath, O_WRONLY);
        if (fd == -1)
                return -errno;
        Generator("WRITE",fpath,1,0,NULL);
        res = pwrite(fd, buf, size, offset);
        if (res == -1)
                res = -errno;

        close(fd);
        return res;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
        int res;
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        res = statvfs(fpath, stbuf);
        if (res == -1)
                return -errno;

        return 0;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {

        (void) fi;
        char fpath[1000];

        if (!strcmp(path, "/"))
        {
                path = dirpath;
                sprintf(fpath, "%s", path);
        }
        else
        {
                sprintf(fpath, "%s%s", dirpath, path);
        }
        Generator("CREATE",fpath,1,0,NULL);
        int res;
        res = creat(fpath, mode);
        if(res == -1)
                return -errno;

        close(res);

        return 0;
}


static struct fuse_operations xmp_oper = {
        .getattr        = xmp_getattr,
        .access         = xmp_access,
        .readlink       = xmp_readlink,
        .readdir        = xmp_readdir,
        .mknod          = xmp_mknod,
        .mkdir          = xmp_mkdir,
        .symlink        = xmp_symlink,
        .unlink         = xmp_unlink,
        .rmdir          = xmp_rmdir,
        .rename         = xmp_rename,
        .link           = xmp_link,
        .chmod          = xmp_chmod,
        .chown          = xmp_chown,
        .truncate       = xmp_truncate,
        .utimens        = xmp_utimens,
        .open           = xmp_open,
        .read           = xmp_read,
        .write          = xmp_write,
        .statfs         = xmp_statfs,
        .create         = xmp_create,
};

int main(int argc, char *argv[])
{
        umask(0);
        return fuse_main(argc, argv, &xmp_oper, NULL);
}

