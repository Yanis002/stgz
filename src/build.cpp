#define LITTLEENDIANIFY2(x) L ## x
#define LITTLEENDIANIFY(x) LITTLEENDIANIFY2(x)

#define STRINGIFY(s) LITTLEENDIANIFY(#s)
#define EXPAND_AND_STRINGIFY(s) STRINGIFY(s)

#ifdef PACKAGE_AUTHOR
const wchar_t gBuildAuthor[] = EXPAND_AND_STRINGIFY(PACKAGE_AUTHOR);
#else
const wchar_t gBuildAuthor[] = L"???";
#endif

#ifdef PACKAGE_COMMIT_AUTHOR
const wchar_t gCommitAuthor[] = EXPAND_AND_STRINGIFY(PACKAGE_COMMIT_AUTHOR);
#else
const wchar_t gCommitAuthor[] = L"???";
#endif

const wchar_t gBuildDate[] = LITTLEENDIANIFY(__DATE__) L" " LITTLEENDIANIFY(__TIME__);

#ifdef PACKAGE_VERSION
const wchar_t gBuildGitVersion[] = EXPAND_AND_STRINGIFY(PACKAGE_VERSION);
#else
const wchar_t gBuildGitVersion[] = L"???";
#endif

#ifdef PACKAGE_NAME
const wchar_t gCommitGitString[] = EXPAND_AND_STRINGIFY(PACKAGE_NAME);
#else
const wchar_t gCommitGitString[] = L"???";
#endif
