#include <string.h>
#include <grp.h>

// Credit: https://github.com/chombourger/android-udev/blob/c78aaf59cdc4378346eb084e9cdc02ca46bc389a/android/missing.c
int getgrnam_r(const char *name, struct group *grp, char *buf, size_t buflen, struct group **result)
{
	struct group *g = getgrnam(name);

	if (g != NULL) {
		grp->gr_gid = g->gr_gid;
		strncpy(buf, g->gr_name, buflen);
		grp->gr_name = buf;
		grp->gr_passwd = NULL;
		grp->gr_mem = NULL;
		*result = grp;
		return 0;
	}
	*result = NULL;
	return -1;
}
