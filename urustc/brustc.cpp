enum TokenType {
    Unknown,
    Whitespace,
    Word,
    Symbol,
    String,
};

struct Token {
    TokenType type;
    const char* start;
    size_t len;
};

enum TokenizeState {
    Ready,
    In

class Reader final
{
public:
    const uint8_t* const mStart;
    const uint8_t* const mEnd;
protected:
    const uint8_t* mItr;

public:
    Reader(const uint8_t* start, const uint8_t* end)
        : mStart(start)
        , mEnd(end)
        , mItr(start)
    { }

    bool IsDone() const { return mItr == mEnd; }

    uint8_t Peek() const {
        if (IsDone())
            return 0;

        return *mItr;
    }

    uint8_t Read() {
        const auto ret = Peek();

        if (!IsDone()) {
            ++mItr;
        }

        return ret;
    }
};

bool
GatherString(const char*& itr, const char* const end)
{
    assert(*itr == '"');

    while (itr != end) {
    }
}

bool
Tokenize(const char* text, size_t textLen)
{
    auto itr = text;
    const auto itrEnd = text + textLen;

    Token cur;
    cur.type = Unknown;
    cur.start = itr;

    Reader reader(text, text + textLen);

    while (!reader.IsDone()) {
        const auto cur = reader.Peek();
        if (cur == '"') {
            while (true) {
                reader.Read();


    }

    for (; itr != itrEnd; ++itr) {
        switch (cur.type) {
        case TokenType::Unknown:
        case TokenType::Whitespace:
        case TokenType::Word:
        case TokenType::Symbol:
        case TokenType::String:
        }
        if (*itr == '"') {

        }

    }
}
