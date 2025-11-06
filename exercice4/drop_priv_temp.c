/*
 * drop_priv.c
 *
 * Compile: gcc -Wall -Wextra -o drop_priv drop_priv.c
 * Run: sudo ./drop_priv
 *
 * But read the explanations below before exécution.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

static void print_id(const char *prefix) {
    uid_t ruid = getuid();      /* real UID */
    uid_t euid = geteuid();     /* effective UID */
    gid_t rgid = getgid();      /* real GID */
    gid_t egid = getegid();     /* effective GID */
    printf("%s: ruid=%u euid=%u rgid=%u egid=%u\n",
           prefix, (unsigned)ruid, (unsigned)euid, (unsigned)rgid, (unsigned)egid);
}

/* Try to drop privileges permanently to target uid/gid.
 * Return 0 on success, -1 on failure (errno set).
 *
 * Strategy:
 * 1) initgroups(name, gid)  -> set supplementary groups for the target user
 * 2) setgid(gid)            -> change group ID (so FS and other checks use new gid)
 * 3) setuid(uid)            -> drop uid (real & effective)
 *
 * If available, prefer setresuid to clear real/effective/saved in one call.
 */
int drop_privs_permanently(uid_t target_uid, gid_t target_gid, const char *username) {
    /* 1) initgroups -> set supplementary groups for username */
    if (initgroups(username, target_gid) != 0) {
        fprintf(stderr, "initgroups(%s,%u) failed: %s\n", username, (unsigned)target_gid, strerror(errno));
        return -1;
    }

    /* 2) setgid -> change GID */
    if (setgid(target_gid) != 0) {
        fprintf(stderr, "setgid(%u) failed: %s\n", (unsigned)target_gid, strerror(errno));
        return -1;
    }

    /* 3) setuid -> change UID (this typically drops effective and real uid for good) */
    if (setuid(target_uid) != 0) {
        fprintf(stderr, "setuid(%u) failed: %s\n", (unsigned)target_uid, strerror(errno));
        return -1;
    }

    /* As a stronger guarantee, try to set the saved uid/gid as well (if available). */
#ifdef __linux__
    /* On Linux prefer setresuid/setresgid if present to clear real/effective/saved */
    if (setresgid((gid_t)target_gid, (gid_t)target_gid, (gid_t)target_gid) != 0) {
        /* Not fatal in all environments; we already called setgid, so print but continue */
        fprintf(stderr, "setresgid(%u,%u,%u) warning: %s\n",
                (unsigned)target_gid, (unsigned)target_gid, (unsigned)target_gid, strerror(errno));
    }
    if (setresuid((uid_t)target_uid, (uid_t)target_uid, (uid_t)target_uid) != 0) {
        fprintf(stderr, "setresuid(%u,%u,%u) warning: %s\n",
                (unsigned)target_uid, (unsigned)target_uid, (unsigned)target_uid, strerror(errno));
    }
#endif

    /* Final verification: try to regain root (should fail) */
    if (seteuid(0) == 0) {
        fprintf(stderr, "Security warning: process could regain euid 0 after drop!\n");
        /* try to drop again to remain safe */
        seteuid(target_uid);
        return -1;
    }

    return 0;
}

int main(int argc, char **argv) {
    const char *target_name = "nobody";     /* username to drop to */
    struct passwd *pw;

    printf("PID %d starting\n", getpid());
    print_id("Initial");

    /* Must be root to drop privileges from root -> nobody. If not root, warn but continue attempt. */
    if (geteuid() != 0) {
        fprintf(stderr, "Warning: not running as root (euid=%u). To get expected behavior run with sudo.\n", (unsigned)geteuid());
    }

    /* Lookup the nobody user */
    pw = getpwnam(target_name);
    if (!pw) {
        fprintf(stderr, "getpwnam(%s) failed: user not found\n", target_name);
        return EXIT_FAILURE;
    }
    uid_t target_uid = pw->pw_uid;
    gid_t target_gid = pw->pw_gid;

    printf("Will sleep 5 seconds as root (or current identity) before dropping to %s:%u\n", target_name, (unsigned)target_gid);
    sleep(5);

    printf("Attempting to drop privileges to %s (uid=%u gid=%u)\n", target_name, (unsigned)target_uid, (unsigned)target_gid);

    if (drop_privs_permanently(target_uid, target_gid, target_name) != 0) {
        fprintf(stderr, "Failed to drop privileges permanently. Exiting.\n");
        return EXIT_FAILURE;
    }

    print_id("After drop");

    /* Now sleep 60 seconds as required */
    printf("Privileges dropped. Sleeping 60 seconds before exit...\n");
    sleep(60);

    print_id("Before exit");
    printf("Exiting.\n");
    return EXIT_SUCCESS;
}
