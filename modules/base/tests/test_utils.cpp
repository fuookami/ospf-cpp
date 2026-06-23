/// ospf_base 工具与数据结构测试
/// 1:1 移植 ospf-rust-base chunked_collection + indexed_type 全部 #[test]，并补充新模块覆盖。

#include <gtest/gtest.h>

#include <ospf/base/chunked_collection.hpp>
#include <ospf/base/indexed_type.hpp>
#include <ospf/base/collection.hpp>
#include <ospf/base/container.hpp>
#include <ospf/base/string_utils.hpp>
#include <ospf/base/log.hpp>
#include <ospf/base/memory.hpp>
#include <ospf/base/bytes.hpp>
#include <ospf/base/data_structure/data_table.hpp>
#include <ospf/base/data_structure/tagged_map.hpp>
#include <ospf/base/data_structure/optional_array.hpp>
#include <ospf/base/data_structure/pointer_array.hpp>
#include <ospf/base/data_structure/reference_array.hpp>
#include <ospf/base/data_structure/value_or_reference_array.hpp>
#include <ospf/base/serialization/csv.hpp>
#include <ospf/base/serialization/bytes.hpp>
#include <ospf/base/meta_programming.hpp>

#include <string>
#include <vector>

using namespace ospf::base;

// ============================================================================
// ChunkedVec 测试（移植自 Rust chunked_collection.rs 7 条 #[test]）
// ============================================================================

// Rust: test_chunked_vec_basic
TEST(ChunkedVec, Basic) {
    ChunkedVec<int> vec(4);  // chunk_size = 4

    for (int i = 0; i < 10; ++i) {
        vec.push(i);
    }

    EXPECT_EQ(vec.size(), 10u);
    EXPECT_EQ(vec.chunk_count(), 3u);
    EXPECT_EQ(vec.chunk_size(), 4u);

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(vec[i], i);
    }
}

// Rust: test_chunked_vec_get
TEST(ChunkedVec, Get) {
    ChunkedVec<int> vec(4);

    for (int i = 0; i < 10; ++i) {
        vec.push(i);
    }

    for (int i = 0; i < 10; ++i) {
        auto* ptr = vec.get(i);
        ASSERT_NE(ptr, nullptr);
        EXPECT_EQ(*ptr, i);
    }
    EXPECT_EQ(vec.get(10), nullptr);
}

// Rust: test_chunked_vec_iter
TEST(ChunkedVec, Iter) {
    ChunkedVec<int> vec(4);

    for (int i = 0; i < 10; ++i) {
        vec.push(i);
    }

    std::vector<int> collected;
    for (const auto& v : vec) {
        collected.push_back(v);
    }
    std::vector<int> expected = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ(collected, expected);
}

// Rust: test_chunked_vec_iter_mut
TEST(ChunkedVec, IterMut) {
    ChunkedVec<int> vec(4);

    for (int i = 0; i < 10; ++i) {
        vec.push(i);
    }

    for (auto& item : vec) {
        item *= 2;
    }

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(vec[i], i * 2);
    }
}

// Rust: test_chunked_vec_from_iterator (adapted: from vector)
TEST(ChunkedVec, FromVector) {
    std::vector<int> source = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    ChunkedVec<int> vec(4096);
    for (auto v : source) {
        vec.push(v);
    }

    EXPECT_EQ(vec.size(), 10u);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(vec[i], i);
    }
}

// Rust: test_chunked_vec_into_vec
TEST(ChunkedVec, ToVec) {
    ChunkedVec<int> vec(4);

    for (int i = 0; i < 10; ++i) {
        vec.push(i);
    }

    auto flat = vec.to_vec();
    std::vector<int> expected = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ(flat, expected);
}

// Rust: test_chunked_vec_chunks
TEST(ChunkedVec, Chunks) {
    ChunkedVec<int> vec(4);

    for (int i = 0; i < 10; ++i) {
        vec.push(i);
    }

    auto& chunks = vec.chunks();
    EXPECT_EQ(chunks.size(), 3u);
    EXPECT_EQ(chunks[0], (std::vector<int>{0, 1, 2, 3}));
    EXPECT_EQ(chunks[1], (std::vector<int>{4, 5, 6, 7}));
    EXPECT_EQ(chunks[2], (std::vector<int>{8, 9}));
}

// ============================================================================
// IndexedType 测试（移植自 Rust indexed_type.rs 4 条 #[test]）
// ============================================================================

namespace {
    struct TestTag {};
    struct FlushTag {};
    struct FindTag {};

    struct TestAutoItem {
        std::string name;
        std::uint32_t value;
        Index<TestTag> idx;

        [[nodiscard]] std::size_t index() const { return idx.value(); }
    };

    struct TestManualItem {
        std::string name;
        std::uint32_t value;
        ManualIndex<TestTag> idx;

        [[nodiscard]] bool is_indexed() const { return idx.indexed(); }
        void set_idx(std::size_t v) { idx.set_index(v); }
        [[nodiscard]] std::size_t index() const { return idx.value(); }
    };

    struct FlushItem {
        std::string name;
        Index<FlushTag> idx;

        [[nodiscard]] std::size_t index() const { return idx.value(); }
    };

    struct FindItem {
        std::string name;
        Index<FindTag> idx;

        [[nodiscard]] std::size_t index() const { return idx.value(); }
    };
}

// Rust: test_auto_indexed_type
TEST(IndexedType, AutoIndexed) {
    // Reset generator for deterministic test
    TypedIndexGenerator<TestTag>::instance().flush();

    TestAutoItem item1{"test1", 1, {}};
    TestAutoItem item2{"test2", 2, {}};

    EXPECT_EQ(item1.index(), 0u);
    EXPECT_EQ(item2.index(), 1u);
}

// Rust: test_manual_indexed_type
TEST(IndexedType, ManualIndexed) {
    TypedIndexGenerator<TestTag>::instance().flush();

    TestManualItem item1{"test1", 1, {}};
    TestManualItem item2{"test2", 2, {}};

    EXPECT_FALSE(item1.is_indexed());
    EXPECT_FALSE(item2.is_indexed());

    item1.set_idx(10);
    // item2 uses auto-generated index
    item2.set_idx(TypedIndexGenerator<TestTag>::instance().next());

    EXPECT_TRUE(item1.is_indexed());
    EXPECT_TRUE(item2.is_indexed());
    EXPECT_EQ(item1.index(), 10u);
    EXPECT_EQ(item2.index(), 0u);  // generator was flushed, so next is 0
}

// Rust: test_flush
TEST(IndexedType, Flush) {
    TypedIndexGenerator<FlushTag>::instance().flush();

    FlushItem item1{"test1", {}};
    EXPECT_EQ(item1.index(), 0u);

    TypedIndexGenerator<FlushTag>::instance().flush();
    FlushItem item2{"test2", {}};
    EXPECT_EQ(item2.index(), 0u);
}

// Rust: test_find_or_get
TEST(IndexedType, FindOrGet) {
    TypedIndexGenerator<FindTag>::instance().flush();

    FindItem item0{"test0", {}};
    FindItem item1{"test1", {}};
    std::vector<FindItem> items;
    items.push_back(std::move(item1));
    items.push_back(std::move(item0));

    // item0 has index 0, item1 has index 1
    // items[0] is item1 (index 1), items[1] is item0 (index 0)
    auto* found0 = find_or_get(items, 0u);
    ASSERT_NE(found0, nullptr);
    EXPECT_EQ(found0->name, "test0");

    auto* found1 = find_or_get(items, 1u);
    ASSERT_NE(found1, nullptr);
    EXPECT_EQ(found1->name, "test1");

    EXPECT_EQ(find_or_get(items, 2u), nullptr);
}

// ============================================================================
// 补充测试：DataTaggedMap
// ============================================================================

TEST(TaggedMap, Basic) {
    TaggedMap map;
    map.set<int>("count", 42);
    map.set<std::string>("name", "hello");

    auto* count = map.get<int>("count");
    ASSERT_NE(count, nullptr);
    EXPECT_EQ(*count, 42);

    auto* name = map.get<std::string>("name");
    ASSERT_NE(name, nullptr);
    EXPECT_EQ(*name, "hello");

    EXPECT_EQ(map.get<double>("count"), nullptr);  // wrong type
    EXPECT_EQ(map.get<int>("missing"), nullptr);    // missing key
}

// ============================================================================
// 补充测试：DataTable
// ============================================================================

TEST(DataTable, Basic) {
    DataTable table;
    table.add_column("name");
    table.add_column("age");

    table.add_row({{"name", "Alice"}, {"age", "30"}});
    table.add_row({{"name", "Bob"}, {"age", "25"}});

    EXPECT_EQ(table.row_count(), 2u);
    EXPECT_EQ(table.col_count(), 2u);
    EXPECT_EQ(table.cell(0, "name"), "Alice");
    EXPECT_EQ(table.cell(1, "age"), "25");
}

// ============================================================================
// 补充测试：OptionalArray
// ============================================================================

TEST(OptionalArray, Basic) {
    OptionalArray<int> arr(5);
    EXPECT_EQ(arr.size(), 5u);

    arr[0] = 42;
    arr[3] = 99;

    EXPECT_EQ(arr[0].value(), 42);
    EXPECT_FALSE(arr[1].has_value());
    EXPECT_EQ(arr[3].value(), 99);
}

// ============================================================================
// 补充测试：PointerArray
// ============================================================================

TEST(PointerArray, Basic) {
    PointerArray<int> arr(3);
    EXPECT_EQ(arr.size(), 3u);

    arr.emplace(0, 10);
    arr.emplace(2, 30);

    EXPECT_EQ(*arr.get(0), 10);
    EXPECT_EQ(arr.get(1), nullptr);
    EXPECT_EQ(*arr.get(2), 30);
}

// ============================================================================
// 补充测试：ReferenceArray
// ============================================================================

TEST(ReferenceArray, Basic) {
    int a = 1, b = 2, c = 3;
    ReferenceArray<int> arr;
    arr.add(std::ref(a));
    arr.add(std::ref(b));
    arr.add(std::ref(c));

    EXPECT_EQ(arr.get(0), 1);
    EXPECT_EQ(arr.get(1), 2);
    EXPECT_EQ(arr.get(2), 3);

    arr.get(0) = 10;
    EXPECT_EQ(a, 10);
}

// ============================================================================
// 补充测试：ValueOrReferenceArray
// ============================================================================

TEST(ValueOrReferenceArray, Basic) {
    int external = 42;
    ValueOrReferenceArray<int> arr;
    arr.add_value(10);
    arr.add_ref(&external);

    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 42);

    arr.get(1) = 99;
    EXPECT_EQ(external, 99);
}

// ============================================================================
// 补充测试：StringUtils
// ============================================================================

TEST(StringUtils, Split) {
    auto parts = split("a,b,c", ',');
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "a");
    EXPECT_EQ(parts[1], "b");
    EXPECT_EQ(parts[2], "c");
}

TEST(StringUtils, Join) {
    std::vector<std::string> parts = {"a", "b", "c"};
    EXPECT_EQ(join(parts, "-"), "a-b-c");
}

TEST(StringUtils, Trim) {
    EXPECT_EQ(trim("  hello  "), "hello");
    EXPECT_EQ(trim("\t\n"), "");
}

TEST(StringUtils, ToLower) {
    EXPECT_EQ(to_lower("Hello"), "hello");
}

TEST(StringUtils, StartsWith) {
    EXPECT_TRUE(starts_with("hello world", "hello"));
    EXPECT_FALSE(starts_with("hello world", "world"));
}

TEST(StringUtils, EndsWith) {
    EXPECT_TRUE(ends_with("hello world", "world"));
    EXPECT_FALSE(ends_with("hello world", "hello"));
}

// ============================================================================
// 补充测试：Bytes
// ============================================================================

TEST(Bytes, BitOperations) {
    EXPECT_TRUE(bit_is_set(0b1010, 1));
    EXPECT_FALSE(bit_is_set(0b1010, 0));
    EXPECT_EQ(bit_set(0b0000, 3), 0b1000);
    EXPECT_EQ(bit_clear(0b1111, 2), 0b1011);
    EXPECT_EQ(bit_toggle(0b1010, 0), 0b1011);
    EXPECT_EQ(popcount(0b1010), 2);
}

TEST(Bytes, HexEncoding) {
    std::vector<std::byte> bytes = {
        static_cast<std::byte>(0xde),
        static_cast<std::byte>(0xad),
        static_cast<std::byte>(0xbe),
        static_cast<std::byte>(0xef)
    };
    auto hex = to_hex(bytes);
    EXPECT_EQ(hex, "deadbeef");

    auto decoded = from_hex("deadbeef");
    EXPECT_EQ(decoded, bytes);
}

// ============================================================================
// 补充测试：CSV serialization
// ============================================================================

TEST(Csv, RoundTrip) {
    std::vector<std::string> fields = {"Alice", "30", "Engineer"};
    auto row = to_csv_row(fields);
    auto parsed = parse_csv_row(row);
    EXPECT_EQ(parsed, fields);
}

// ============================================================================
// 补充测试：Byte serialization
// ============================================================================

TEST(ByteSerialization, PodRoundTrip) {
    int original = 42;
    auto bytes = to_bytes(original);
    auto restored = from_bytes<int>(bytes);
    EXPECT_EQ(restored, original);
}

TEST(ByteSerialization, VectorRoundTrip) {
    std::vector<int> original = {1, 2, 3, 4, 5};
    auto bytes = to_bytes(original);
    auto restored = vector_from_bytes<int>(bytes);
    EXPECT_EQ(restored, original);
}

// ============================================================================
// 补充测试：MetaProgramming
// ============================================================================

TEST(MetaProgramming, TypeListSize) {
    using List = TypeList<int, double, char>;
    EXPECT_EQ((TypeListSize_v<List>), 3u);
}

// ============================================================================
// 补充测试：Log
// ============================================================================

TEST(Log, LevelSetting) {
    auto old = g_log_level;
    set_log_level(LogLevel::Warn);
    EXPECT_EQ(g_log_level, LogLevel::Warn);
    set_log_level(old);
}
