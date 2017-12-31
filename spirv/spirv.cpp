#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

template<typename T> using sp = std::shared_ptr<T>;
template<typename T> using up = std::unique_ptr<T>;
typedef uint8_t u8;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;

const u32 SPIRV_MAGIC_LENDIAN = 0x07230203;

#define let const auto

#define ASSERT(expr) \
   {\
      if (!(expr)) {\
         printf("Assert failed at %s:%u: `%s`\n", __FILE__, __LINE__, #expr);\
         *(u8*)5 = 3;\
      }\
   }

template<typename T> class Range;

template<typename T>
class Iter {
   Range<T>* mRange;
   T* mPtr;

   friend class Range<T>;

   Iter(Range<T>* const range, T* const ptr)
      : mRange(range)
      , mPtr(ptr)
   {}

public:
   T& operator*() const;

   T& operator[](const i64 n) const {
      const auto x = *this + n;
      return *x;
   }

   auto& operator+=(const i64 n) {
      mPtr += n;
      return *this;
   }
   auto& operator++() { return *this += 1; }
   auto operator+(const i64 n) const {
      auto ret = *this;
      ret += n;
      return ret;
   }
   auto operator==(const Iter<T>& r) const {
      return mPtr == r.mPtr;
   }
   auto operator!=(const Iter<T>& r) const { return !(*this == r); }
};

template<typename T>
class Range {
public:
   const Iter<T> begin;
   const Iter<T> end;

   Range(T* const _begin, T* const _end)
      : begin(this, _begin)
      , end(this, _end)
   {
      ASSERT(begin.mPtr <= end.mPtr);
   }

   auto Subrange(const Iter<T>& sbegin, const Iter<T>& send) const {
      ASSERT(begin.mPtr <= sbegin.mPtr);
      ASSERT(begin.mPtr <= send.mPtr);
      ASSERT(sbegin.mPtr <= end.mPtr);
      ASSERT(send.mPtr <= end.mPtr);
      return Range<T>(sbegin.mPtr, send.mPtr);
   }

   uint64_t Size() const { return end.mPtr - begin.mPtr; }
};

template<typename T>
auto
MakeRange(T* const begin, T* const end)
{
   return Range<T>(begin, end);
}

template<typename T>
auto
MakeRange(T* const begin, const uint64_t size)
{
   return Range<T>(begin, begin+size);
}

template<typename T>
T&
Iter<T>::operator*() const
{
   ASSERT(mRange->begin.mPtr <= mPtr)
   ASSERT(mPtr < mRange->end.mPtr)
   return *mPtr;
}

// --

struct SpirvOp
{
   const u32 opcode;

   static sp<SpirvOp> Parse(u32 opcode, const Range<const u32>& data);

   virtual bool Validate() const = 0;
};

/*static*/ sp<SpirvOp>
SpirvOp::Parse(const u32 opcode, const Range<const u32>& data)
{
   return nullptr;
}

// --

class SpirvModule final
{
   const std::vector<sp<SpirvOp>> mOpList;

   SpirvModule(std::vector<sp<SpirvOp>>&& opList)
      : mOpList(opList)
   {}

public:
   static sp<SpirvModule> Parse(u64 size, const u32* data);
};

/*static*/ sp<SpirvModule>
SpirvModule::Parse(const u64 size, const u32* const data)
{
   let range = MakeRange(data, size);
   auto itr = range.begin;
   let spirvMagic = itr[0];
   ASSERT(spirvMagic == SPIRV_MAGIC_LENDIAN);
   let spirvVersion = itr[1];
   let generatorMagic = itr[2];
   let idBounds = itr[3];
   let schema = itr[4];
   itr += 5;

   std::vector<sp<SpirvOp>> opList;
   while (itr != range.end) {
      let opHead = *itr;
      //printf("opHead: %x\n", opHead);
      let opcode = opHead & 0xffff;
      let opWords = opHead >> 16;
      ASSERT(opWords);

      let itrArgsEnd = itr + opWords;
      ++itr;
      let argRange = range.Subrange(itr, itrArgsEnd);
      opList.push_back(SpirvOp::Parse(opcode, argRange));

      itr = itrArgsEnd;
   }
   return sp<SpirvModule>(new SpirvModule(std::move(opList)));
}

std::vector<u8>
ReadFile(FILE* const f)
{
   std::vector<std::vector<u8>> chunks;
   u64 total = 0;
   while (!feof(f)) {
      chunks.emplace_back(1000);
      auto& last = *chunks.rbegin();
      let readCount = fread(last.data(), 1, last.size(), f);
      total += readCount;
      ASSERT(!ferror(f));
      last.resize(readCount);
      if (!readCount) {
         chunks.pop_back();
      }
   }

   std::vector<u8> ret(total);
   u8* dest = ret.data();
   for (let& x : chunks) {
      memcpy(dest, x.data(), x.size());
      dest += x.size();
   }

   return ret;
}

int
main(const int argc, const char* const argv[])
{
   let bytes = ReadFile(stdin);
   ASSERT(bytes.size() % 4 == 0);
   (void)SpirvModule::Parse(bytes.size()/4, (const u32*)bytes.data());
   return 0;
}
