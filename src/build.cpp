#define STRINGIFY(s) #s
#define EXPAND_AND_STRINGIFY(s) STRINGIFY(s)

#ifdef PACKAGE_AUTHOR
extern "C" const char gBuildAuthor[] = EXPAND_AND_STRINGIFY(PACKAGE_AUTHOR);
#else
extern "C" const char gBuildAuthor[] = "???";
#endif

#ifdef PACKAGE_COMMIT_AUTHOR
extern "C" const char gCommitAuthor[] = EXPAND_AND_STRINGIFY(PACKAGE_COMMIT_AUTHOR);
#else
extern "C" const char gCommitAuthor[] = "???";
#endif

extern "C" const char gBuildDate[] = __DATE__ " " __TIME__;

#ifdef PACKAGE_VERSION
extern "C" const char gBuildGitVersion[] = EXPAND_AND_STRINGIFY(PACKAGE_VERSION);
#else
extern "C" const char gBuildGitVersion[] = "???";
#endif

#ifdef PACKAGE_NAME
extern "C" const char gCommitGitString[] = EXPAND_AND_STRINGIFY(PACKAGE_NAME);
#else
extern "C" const char gCommitGitString[] = "???";
#endif
