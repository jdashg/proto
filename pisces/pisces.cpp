#include <cstdio>
#include <map>
#include <string>
#include <vector>

typedef uint8_t byte;
constexpr size_t kIOSize = 300*1000;

#define ASSERT(expr) while (!expr) { \
        printf("ASSERT FAILED at %s:%u: %s\n", __FILE__, __LINE__, #expr); \
        break; \
    }

static uint32_t
xorshift32(uint32_t x)
{
    /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

struct TokenView final
{
    size_t mHash;
    const char* const mBegin;
    const size_t mLen;

    TokenView(const char* const begin, const size_t len)
        : mBegin(begin)
        , mLen(len)
    {
        auto hash = uint32_t(0x12345678);
        const auto end = mBegin + mLen;
        for (auto itr = mBegin; itr != end; ++itr) {
            hash = xorshift32(hash ^ *itr);
        }
        mHash = hash;
    }

    bool operator ==(const TokenView& r) const {
        if (mHash != r.mHash ||
            mLen != r.mLen)
        {
            return false;
        }
        const auto endL = mBegin + mLen;
        auto itrL = mBegin;
        auto itrR = r.mBegin;
        while (itrL != endL) {
            if (*itrL != *itrR)
                return false;
            ++itrL;
            ++itrR;
        }
        return true;
    }
};

template<>
struct std::hash<TokenView> final
{
    size_t operator()(const TokenView& x) const { return x.mHash; }
};

void
rebake()
{
}

/*
EXAMPLE_CL_ARGS = [
    'cl.EXE', '-FoUnified_cpp_dom_canvas1.obj', '-c',
    '-Ic:/dev/mozilla/gecko-cinn3-obj/dist/stl_wrappers', '-DDEBUG=1', '-DTRACING=1',
    '-DWIN32_LEAN_AND_MEAN', '-D_WIN32', '-DWIN32', '-D_CRT_RAND_S',
    '-DCERT_CHAIN_PARA_HAS_EXTRA_FIELDS', '-DOS_WIN=1', '-D_UNICODE', '-DCHROMIUM_BUILD',
    '-DU_STATIC_IMPLEMENTATION', '-DUNICODE', '-D_WINDOWS', '-D_SECURE_ATL',
    '-DCOMPILER_MSVC', '-DSTATIC_EXPORTABLE_JS_API', '-DMOZ_HAS_MOZGLUE',
    '-DMOZILLA_INTERNAL_API', '-DIMPL_LIBXUL', '-Ic:/dev/mozilla/gecko-cinn3/dom/canvas',
    '-Ic:/dev/mozilla/gecko-cinn3-obj/dom/canvas',
    '-Ic:/dev/mozilla/gecko-cinn3/js/xpconnect/wrappers',
    '-Ic:/dev/mozilla/gecko-cinn3-obj/ipc/ipdl/_ipdlheaders',
    '-Ic:/dev/mozilla/gecko-cinn3/ipc/chromium/src',
    '-Ic:/dev/mozilla/gecko-cinn3/ipc/glue', '-Ic:/dev/mozilla/gecko-cinn3/dom/workers',
    '-Ic:/dev/mozilla/gecko-cinn3/dom/base', '-Ic:/dev/mozilla/gecko-cinn3/dom/html',
    '-Ic:/dev/mozilla/gecko-cinn3/dom/svg', '-Ic:/dev/mozilla/gecko-cinn3/dom/workers',
    '-Ic:/dev/mozilla/gecko-cinn3/dom/xul', '-Ic:/dev/mozilla/gecko-cinn3/gfx/gl',
    '-Ic:/dev/mozilla/gecko-cinn3/image', '-Ic:/dev/mozilla/gecko-cinn3/js/xpconnect/src',
    '-Ic:/dev/mozilla/gecko-cinn3/layout/generic',
    '-Ic:/dev/mozilla/gecko-cinn3/layout/style',
    '-Ic:/dev/mozilla/gecko-cinn3/layout/xul',
    '-Ic:/dev/mozilla/gecko-cinn3/media/libyuv/include',
    '-Ic:/dev/mozilla/gecko-cinn3/gfx/skia',
    '-Ic:/dev/mozilla/gecko-cinn3/gfx/skia/skia/include/config',
    '-Ic:/dev/mozilla/gecko-cinn3/gfx/skia/skia/include/core',
    '-Ic:/dev/mozilla/gecko-cinn3/gfx/skia/skia/include/gpu',
    '-Ic:/dev/mozilla/gecko-cinn3/gfx/skia/skia/include/utils',
    '-Ic:/dev/mozilla/gecko-cinn3-obj/dist/include',
    '-Ic:/dev/mozilla/gecko-cinn3-obj/dist/include/nspr',
    '-Ic:/dev/mozilla/gecko-cinn3-obj/dist/include/nss', '-MD', '-FI',
    'c:/dev/mozilla/gecko-cinn3-obj/mozilla-config.h', '-DMOZILLA_CLIENT', '-Oy-', '-TP',
    '-nologo', '-wd5026', '-wd5027', '-Zc:sizedDealloc-', '-Zc:threadSafeInit-',
    '-wd4091', '-wd4577', '-D_HAS_EXCEPTIONS=0', '-W3', '-Gy', '-Zc:inline', '-utf-8',
    '-FS', '-Gw', '-wd4251', '-wd4244', '-wd4267', '-wd4345', '-wd4351', '-wd4800',
    '-wd4595', '-we4553', '-GR-', '-Z7', '-Oy-', '-WX',
    '-Ic:/dev/mozilla/gecko-cinn3-obj/dist/include/cairo', '-wd4312',
    'c:/dev/mozilla/gecko-cinn3-obj/dom/canvas/Unified_cpp_dom_canvas1.cpp'
]
*/

class DefineMap final
{

};

class File final
{
public:
    FILE* const mFile;

    operator FILE*() const { return mFile; }
    //operator bool() const { return mFile; }

    ~File() {
        if (mFile) {
            fclose(mFile);
        }
    }
};

byte
Classify(const byte b)
{
    switch (b) {
    case ' ':
    case '\t':
    case '\v':
    case '\r':
    case '\n':
        return ' ';
    case '#':
        return '#';
    case '\\':
        return '\\';
    }
    if (('0' <= b && b <= '9') ||
        ('0' <= b && b <= '9') ||

    default:
        return '?';

}

int
main(const int argc, const char* const argv[])
{
    std::vector<std::string> strings;
    std::vector<const char*> includes;

    const auto envInclude = getenv("INCLUDE");
    printf("envInclude: %s\n", envInclude);

    std::vector<const char*> files;
    for (int i = 1; i < argc; ++i) {
        const auto cur = argv[i];
        const auto len = strlen(cur);
        if (cur[0] != '-') {
            if (cur[0]) {
                files.push_back(cur);
            }
            continue;
        }
        switch (cur[1]) {
        case 'I':
            includes.push_back(cur+2);
            break;
        default:
            return 1;
        }
    }

    byte buffer[kIOSize];

    for (const auto path : files) {
        const File f{fopen(path, "r")};
        ASSERT(f)
        fprintf(stdout, "#line 1 %s\n", path);
        while (true) {
            const auto read = fread(buffer, 1, kIOSize, f);
            const auto written = fwrite(buffer, 1, read, stdout);
            ASSERT(written == read);
            if (read != kIOSize) {
                if (feof(f))
                    break;
                ASSERT(false);
            }
        }
        fputc('\n', stdout);
    }

    return 0;
}
