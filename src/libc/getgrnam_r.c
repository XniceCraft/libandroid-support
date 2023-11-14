#include <string.h>
#include <grp.h>
#include <android/api-level.h>

#if __ANDROID_API__ < 24

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

#endif /*__ANDROID_API__*/

#if __ANDROID_API__ < 26

/* These function are dummy function.
   Because they aren't implemented on older android */

struct group* getgrent(void) return NULL;
void setgrent(void) return;
void endgrent(void) return;

#endif /*__ANDROID_API__*/
