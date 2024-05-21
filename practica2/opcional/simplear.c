#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <utime.h>
#include <errno.h>

#define AR_MAGIC "<arspl>\n"
#define MAGSIZE 8

struct ar_ghdr {
    char ar_magic[MAGSIZE];
    int nfiles;
};

struct ar_lhdr {
    char ar_memname[16];
    long ar_size;
    char ar_date[12];
    int ar_uid;
    int ar_gid;
    char ar_mode[12];
};

void create_ar(const char *arname) {
    struct ar_ghdr ghdr;
    int fd = open(arname, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error creating archive");
        exit(EXIT_FAILURE);
    }

    strncpy(ghdr.ar_magic, AR_MAGIC, MAGSIZE);
    ghdr.nfiles = 0;

    if (write(fd, &ghdr, sizeof(ghdr)) != sizeof(ghdr)) {
        perror("Error writing global header");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

void insert_file(const char *arname, const char *filename) {
    struct ar_ghdr ghdr;
    struct ar_lhdr lhdr;
    struct stat st;
    int fd_ar, fd_file;
    char buffer[1024];
    ssize_t bytes_read;

    fd_ar = open(arname, O_RDWR);
    if (fd_ar == -1) {
        perror("Error opening archive");
        exit(EXIT_FAILURE);
    }

    if (read(fd_ar, &ghdr, sizeof(ghdr)) != sizeof(ghdr)) {
        perror("Error reading global header");
        exit(EXIT_FAILURE);
    }

    if (stat(filename, &st) == -1) {
        perror("Error getting file stats");
        exit(EXIT_FAILURE);
    }

    strncpy(lhdr.ar_memname, filename, sizeof(lhdr.ar_memname));
    lhdr.ar_size = st.st_size;
    snprintf(lhdr.ar_date, sizeof(lhdr.ar_date), "%ld", st.st_mtime);
    lhdr.ar_uid = st.st_uid;
    lhdr.ar_gid = st.st_gid;
    snprintf(lhdr.ar_mode, sizeof(lhdr.ar_mode), "%o", st.st_mode);

    lseek(fd_ar, 0, SEEK_END);

    if (write(fd_ar, &lhdr, sizeof(lhdr)) != sizeof(lhdr)) {
        perror("Error writing local header");
        exit(EXIT_FAILURE);
    }

    fd_file = open(filename, O_RDONLY);
    if (fd_file == -1) {
        perror("Error opening file to insert");
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(fd_file, buffer, sizeof(buffer))) > 0) {
        if (write(fd_ar, buffer, bytes_read) != bytes_read) {
            perror("Error writing file content");
            exit(EXIT_FAILURE);
        }
    }

    close(fd_file);

    ghdr.nfiles += 1;
    lseek(fd_ar, 0, SEEK_SET);
    if (write(fd_ar, &ghdr, sizeof(ghdr)) != sizeof(ghdr)) {
        perror("Error updating global header");
        exit(EXIT_FAILURE);
    }

    close(fd_ar);
}


void extract_files(const char *arname) {
    struct ar_ghdr ghdr;
    struct ar_lhdr lhdr;
    int fd_ar, fd_file;
    char buffer[1024];
    ssize_t bytes_read, bytes_to_read;

    fd_ar = open(arname, O_RDONLY);
    if (fd_ar == -1) {
        perror("Error opening archive");
        exit(EXIT_FAILURE);
    }

    if (read(fd_ar, &ghdr, sizeof(ghdr)) != sizeof(ghdr)) {
        perror("Error reading global header");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ghdr.nfiles; i++) {
        if (read(fd_ar, &lhdr, sizeof(lhdr)) != sizeof(lhdr)) {
            perror("Error reading local header");
            exit(EXIT_FAILURE);
        }

        fd_file = open(lhdr.ar_memname, O_CREAT | O_WRONLY | O_TRUNC, strtol(lhdr.ar_mode, NULL, 8));
        if (fd_file == -1) {
            perror("Error creating file");
            exit(EXIT_FAILURE);
        }

        bytes_to_read = lhdr.ar_size;
        while (bytes_to_read > 0) {
            bytes_read = read(fd_ar, buffer, sizeof(buffer) < bytes_to_read ? sizeof(buffer) : bytes_to_read);
            if (bytes_read == -1) {
                perror("Error reading file content");
                exit(EXIT_FAILURE);
            }

            if (write(fd_file, buffer, bytes_read) != bytes_read) {
                perror("Error writing file content");
                exit(EXIT_FAILURE);
            }

            bytes_to_read -= bytes_read;
        }

        close(fd_file);

        struct utimbuf new_times;
        new_times.actime = strtol(lhdr.ar_date, NULL, 10);
        new_times.modtime = strtol(lhdr.ar_date, NULL, 10);
        utime(lhdr.ar_memname, &new_times);
    }

    close(fd_ar);
}


// Función principal
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [options] archive [files...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *option = argv[1];
    char *arname = argv[2];

    if (strcmp(option, "-c") == 0) {
        create_ar(arname);
    }

    if (strcmp(option, "-r") == 0 || (strcmp(option, "-cr") == 0 && argc > 3)) {
        if (strcmp(option, "-cr") == 0) {
            create_ar(arname);
        }

        for (int i = 3; i < argc; i++) {
            insert_file(arname, argv[i]);
        }
    }

    if (strcmp(option, "-x") == 0) {
        extract_files(arname);
    }

    return 0;
}
