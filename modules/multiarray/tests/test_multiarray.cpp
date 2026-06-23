/// ospf_multiarray 测试
/// 移植 ospf-rust-multiarray 核心测试，覆盖 shape/array/view/order/block。

#include <gtest/gtest.h>

#include <ospf/multiarray/concept.hpp>
#include <ospf/multiarray/shape.hpp>
#include <ospf/multiarray/array.hpp>
#include <ospf/multiarray/view.hpp>
#include <ospf/multiarray/block.hpp>
#include <ospf/multiarray/dummy_index.hpp>
#include <ospf/multiarray/map_index.hpp>

#include <vector>
#include <array>

using namespace ospf::multiarray;

// ============================================================================
// 存储顺序测试 / Storage order tests
// ============================================================================

// Rust: test_compile_time_shape_row_major
TEST(StorageOrder, RowMajorOffsets) {
    // Shape [2, 3, 4] with RowMajor
    // offsets: [12, 4, 1]
    std::array<std::size_t, 3> shape = {2, 3, 4};
    auto offsets = StorageOrderTrait<StorageOrder::RowMajor>::offsets(shape);
    EXPECT_EQ(offsets[0], 12u);
    EXPECT_EQ(offsets[1], 4u);
    EXPECT_EQ(offsets[2], 1u);

    auto total = StorageOrderTrait<StorageOrder::RowMajor>::total_size(shape);
    EXPECT_EQ(total, 24u);
}

// Rust: test_compile_time_shape_column_major
TEST(StorageOrder, ColumnMajorOffsets) {
    // Shape [2, 3, 4] with ColumnMajor
    // offsets: [1, 2, 6]
    std::array<std::size_t, 3> shape = {2, 3, 4};
    auto offsets = StorageOrderTrait<StorageOrder::ColumnMajor>::offsets(shape);
    EXPECT_EQ(offsets[0], 1u);
    EXPECT_EQ(offsets[1], 2u);
    EXPECT_EQ(offsets[2], 6u);

    auto total = StorageOrderTrait<StorageOrder::ColumnMajor>::total_size(shape);
    EXPECT_EQ(total, 24u);
}

// Rust: test_dyn_shape_row_major
TEST(StorageOrder, DynRowMajorOffsets) {
    std::vector<std::size_t> shape = {2, 3, 4};
    auto offsets = StorageOrderTrait<StorageOrder::RowMajor>::dyn_offsets(shape);
    EXPECT_EQ(offsets[0], 12u);
    EXPECT_EQ(offsets[1], 4u);
    EXPECT_EQ(offsets[2], 1u);
}

// Rust: test_dyn_shape_column_major
TEST(StorageOrder, DynColumnMajorOffsets) {
    std::vector<std::size_t> shape = {2, 3, 4};
    auto offsets = StorageOrderTrait<StorageOrder::ColumnMajor>::dyn_offsets(shape);
    EXPECT_EQ(offsets[0], 1u);
    EXPECT_EQ(offsets[1], 2u);
    EXPECT_EQ(offsets[2], 6u);
}

// ============================================================================
// Shape 测试 / Shape tests
// ============================================================================

// Rust: test_shape_creation
TEST(Shape, Creation) {
    Shape<3> shape({2, 3, 4});
    EXPECT_EQ(shape.dimension(), 3u);
    EXPECT_EQ(shape.size(), 24u);
    EXPECT_EQ(shape.size(0), 2u);
    EXPECT_EQ(shape.size(1), 3u);
    EXPECT_EQ(shape.size(2), 4u);
}

// Rust: test_shape1_new_with
TEST(Shape, OneDimensional) {
    Shape<1> shape({10});
    EXPECT_EQ(shape.dimension(), 1u);
    EXPECT_EQ(shape.size(), 10u);
    EXPECT_EQ(shape.size(0), 10u);
}

// Rust: test_index_calculation
TEST(Shape, IndexCalculation) {
    // RowMajor: [2, 3, 4]
    // index_of(1, 2, 3) = 1*12 + 2*4 + 3*1 = 12 + 8 + 3 = 23
    Shape<3, StorageOrder::RowMajor> shape({2, 3, 4});
    EXPECT_EQ(shape.index_of(0, 0, 0), 0u);
    EXPECT_EQ(shape.index_of(0, 0, 1), 1u);
    EXPECT_EQ(shape.index_of(0, 1, 0), 4u);
    EXPECT_EQ(shape.index_of(1, 0, 0), 12u);
    EXPECT_EQ(shape.index_of(1, 2, 3), 23u);

    // ColumnMajor: [2, 3, 4]
    // index_of(1, 2, 3) = 1*1 + 2*2 + 3*6 = 1 + 4 + 18 = 23
    Shape<3, StorageOrder::ColumnMajor> col_shape({2, 3, 4});
    EXPECT_EQ(col_shape.index_of(0, 0, 0), 0u);
    EXPECT_EQ(col_shape.index_of(1, 0, 0), 1u);
    EXPECT_EQ(col_shape.index_of(0, 1, 0), 2u);
    EXPECT_EQ(col_shape.index_of(0, 0, 1), 6u);
    EXPECT_EQ(col_shape.index_of(1, 2, 3), 23u);
}

// Rust: test_vector_calculation
TEST(Shape, VectorCalculation) {
    Shape<3, StorageOrder::RowMajor> shape({2, 3, 4});

    auto v0 = shape.vector_of(0);
    EXPECT_EQ(v0[0], 0u);
    EXPECT_EQ(v0[1], 0u);
    EXPECT_EQ(v0[2], 0u);

    auto v23 = shape.vector_of(23);
    EXPECT_EQ(v23[0], 1u);
    EXPECT_EQ(v23[1], 2u);
    EXPECT_EQ(v23[2], 3u);
}

// Rust: test_index_vector_inverse
TEST(Shape, IndexVectorInverse) {
    Shape<3, StorageOrder::RowMajor> shape({2, 3, 4});

    // index_of 和 vector_of 互为逆操作
    for (std::size_t i = 0; i < shape.size(); ++i) {
        auto v = shape.vector_of(i);
        EXPECT_EQ(shape.index_of(v[0], v[1], v[2]), i);
    }
}

// Rust: test_len_of_dimension
TEST(Shape, LenOfDimension) {
    Shape<3> shape({2, 3, 4});
    EXPECT_EQ(shape.size(0), 2u);
    EXPECT_EQ(shape.size(1), 3u);
    EXPECT_EQ(shape.size(2), 4u);
}

// Rust: test_offset_of_dimension
TEST(Shape, OffsetOfDimension) {
    Shape<3, StorageOrder::RowMajor> shape({2, 3, 4});
    auto offsets = shape.offsets();
    EXPECT_EQ(offsets[0], 12u);
    EXPECT_EQ(offsets[1], 4u);
    EXPECT_EQ(offsets[2], 1u);
}

// Rust: test_zero_vector
TEST(Shape, ZeroVector) {
    Shape<3> shape({2, 3, 4});
    // 验证形状的基本属性
    EXPECT_EQ(shape.dimension(), 3u);
    EXPECT_EQ(shape.size(), 24u);
}

// Rust: test_type_aliases
TEST(Shape, Equality) {
    Shape<3> s1({2, 3, 4});
    Shape<3> s2({2, 3, 4});
    Shape<3> s3({2, 3, 5});
    EXPECT_EQ(s1, s2);
    EXPECT_NE(s1, s3);
}

// ============================================================================
// DynShape 测试 / Dynamic shape tests
// ============================================================================

// Rust: test_dyn_shape_creation
TEST(DynShape, Creation) {
    DynShape<StorageOrder::RowMajor> shape({2, 3, 4});
    EXPECT_EQ(shape.dimension(), 3u);
    EXPECT_EQ(shape.size(), 24u);
    EXPECT_EQ(shape.size(0), 2u);
    EXPECT_EQ(shape.size(1), 3u);
    EXPECT_EQ(shape.size(2), 4u);
}

TEST(DynShape, IndexCalculation) {
    DynShape<StorageOrder::RowMajor> shape({2, 3, 4});
    EXPECT_EQ(shape.index_of({0, 0, 0}), 0u);
    EXPECT_EQ(shape.index_of({1, 2, 3}), 23u);
}

TEST(DynShape, VectorCalculation) {
    DynShape<StorageOrder::RowMajor> shape({2, 3, 4});
    auto v = shape.vector_of(23);
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1u);
    EXPECT_EQ(v[1], 2u);
    EXPECT_EQ(v[2], 3u);
}

TEST(DynShape, Equality) {
    DynShape<> s1({2, 3, 4});
    DynShape<> s2({2, 3, 4});
    DynShape<> s3({2, 3, 5});
    EXPECT_EQ(s1, s2);
    EXPECT_NE(s1, s3);
}

// ============================================================================
// MultiArray 测试 / MultiArray tests
// ============================================================================

// Rust: test_multi_array_creation
TEST(MultiArray, Creation) {
    Shape<3> shape({2, 3, 4});
    MultiArray<int, 3> arr(shape);

    EXPECT_EQ(arr.size(), 24u);
    EXPECT_EQ(arr.dimension(), 3u);
}

// Rust: test_index_with_vector_type
TEST(MultiArray, AccessByIndices) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape);

    arr.at(0, 0) = 1;
    arr.at(1, 2) = 42;
    arr.at(2, 3) = 99;

    EXPECT_EQ(arr.at(0, 0), 1);
    EXPECT_EQ(arr.at(1, 2), 42);
    EXPECT_EQ(arr.at(2, 3), 99);
}

// Rust: test_index_with_usize
TEST(MultiArray, AccessByLinearIndex) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape);

    arr[0] = 1;
    arr[5] = 42;
    arr[11] = 99;

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[5], 42);
    EXPECT_EQ(arr[11], 99);
}

// Rust: test_storage_order
TEST(MultiArray, StorageOrder) {
    // RowMajor
    Shape<2, StorageOrder::RowMajor> rm_shape({3, 4});
    MultiArray<int, 2, StorageOrder::RowMajor> rm_arr(rm_shape);
    rm_arr.at(0, 0) = 1;
    rm_arr.at(0, 1) = 2;
    rm_arr.at(1, 0) = 3;
    // RowMajor: [0,0]=0, [0,1]=1, [1,0]=4
    EXPECT_EQ(rm_arr[0], 1);
    EXPECT_EQ(rm_arr[1], 2);
    EXPECT_EQ(rm_arr[4], 3);

    // ColumnMajor
    Shape<2, StorageOrder::ColumnMajor> cm_shape({3, 4});
    MultiArray<int, 2, StorageOrder::ColumnMajor> cm_arr(cm_shape);
    cm_arr.at(0, 0) = 1;
    cm_arr.at(0, 1) = 2;
    cm_arr.at(1, 0) = 3;
    // ColumnMajor: [0,0]=0, [1,0]=1, [0,1]=3
    EXPECT_EQ(cm_arr[0], 1);
    EXPECT_EQ(cm_arr[1], 3);
    EXPECT_EQ(cm_arr[3], 2);
}

// Rust: test_3d_array
TEST(MultiArray, ThreeDimensional) {
    Shape<3> shape({2, 3, 4});
    MultiArray<int, 3> arr(shape, 0);

    arr.at(1, 2, 3) = 42;
    EXPECT_EQ(arr.at(1, 2, 3), 42);

    // 验证线性索引对应关系
    EXPECT_EQ(arr[shape.index_of(1, 2, 3)], 42);
}

// Rust: test_clone
TEST(MultiArray, Copy) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape);
    arr.at(1, 2) = 42;

    MultiArray<int, 2> copy = arr;
    EXPECT_EQ(copy.at(1, 2), 42);

    // 修改副本不影响原件
    copy.at(1, 2) = 99;
    EXPECT_EQ(arr.at(1, 2), 42);
}

// Rust: test_multi_array_iter_mut
TEST(MultiArray, FillAndIterate) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 0);

    arr.fill(42);
    for (std::size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 42);
    }
}

// Rust: test_multi_array_of_shape
TEST(MultiArray, OfShape) {
    MultiArray<int, 2> arr(Shape<2>({3, 4}), 7);
    for (std::size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 7);
    }
}

// ============================================================================
// MultiArrayView 测试 / View tests
// ============================================================================

// Rust: test_multi_array_view_conversion
TEST(MultiArrayView, Basic) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape);
    arr.at(1, 2) = 42;

    MultiArrayView<int, 2> view(arr.data(), shape);
    EXPECT_EQ(view.at(1, 2), 42);

    // 通过视图修改
    view.at(1, 2) = 99;
    EXPECT_EQ(arr.at(1, 2), 99);
}

// ============================================================================
// BlockMultiArray 测试 / Block array tests
// ============================================================================

TEST(BlockMultiArray, Basic) {
    Shape<2> shape({3, 4});
    BlockMultiArray<int, 2> arr(shape, 8);  // chunk_size = 8

    arr.at(1, 2) = 42;
    EXPECT_EQ(arr.at(1, 2), 42);
    EXPECT_EQ(arr.size(), 12u);
}

TEST(BlockMultiArray, Fill) {
    Shape<2> shape({3, 4});
    BlockMultiArray<int, 2> arr(shape, 8);
    arr.fill(7);

    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            EXPECT_EQ(arr.at(i, j), 7);
        }
    }
}

// ============================================================================
// DummyIndex 测试 / Dummy index tests
// ============================================================================

TEST(DummyIndex, ConcreteIndex) {
    auto idx = DummyIndex::index(5);
    EXPECT_TRUE(idx.is_index());
    EXPECT_FALSE(idx.is_all());
    EXPECT_FALSE(idx.is_range());
    EXPECT_EQ(idx.get_index(), 5u);
}

TEST(DummyIndex, All) {
    auto idx = DummyIndex::all();
    EXPECT_FALSE(idx.is_index());
    EXPECT_TRUE(idx.is_all());
    EXPECT_FALSE(idx.is_range());
}

TEST(DummyIndex, Range) {
    auto idx = DummyIndex::range(2, 8);
    EXPECT_FALSE(idx.is_index());
    EXPECT_FALSE(idx.is_all());
    EXPECT_TRUE(idx.is_range());
    EXPECT_EQ(idx.range_start(), 2u);
    EXPECT_EQ(idx.range_end(), 8u);
}

// ============================================================================
// MapIndex 测试 / Map index tests
// ============================================================================

TEST(MapIndex, Identity) {
    auto map = MapIndex::identity(10);
    EXPECT_TRUE(map.is_identity());
    EXPECT_EQ(map.size(), 10u);
    EXPECT_EQ(map.map(5), 5u);
}

TEST(MapIndex, Custom) {
    std::vector<std::size_t> mapping = {2, 0, 1};
    auto map = MapIndex::custom(3, mapping);
    EXPECT_FALSE(map.is_identity());
    EXPECT_EQ(map.map(0), 2u);
    EXPECT_EQ(map.map(1), 0u);
    EXPECT_EQ(map.map(2), 1u);
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, ShapeAndArrayWorkflow) {
    // 创建 3D 形状
    Shape<3> shape({2, 3, 4});

    // 创建数组并填充
    MultiArray<double, 3> arr(shape, 0.0);

    // 按多维索引设置值
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            for (std::size_t k = 0; k < 4; ++k) {
                arr.at(i, j, k) = static_cast<double>(i * 12 + j * 4 + k);
            }
        }
    }

    // 验证线性索引对应关系
    for (std::size_t linear = 0; linear < arr.size(); ++linear) {
        auto v = shape.vector_of(linear);
        EXPECT_DOUBLE_EQ(arr[linear], arr.at(v[0], v[1], v[2]));
    }
}

TEST(Integration, ColumnMajorWorkflow) {
    Shape<2, StorageOrder::ColumnMajor> shape({3, 4});
    MultiArray<int, 2, StorageOrder::ColumnMajor> arr(shape, 0);

    // 设置值
    arr.at(0, 0) = 1;
    arr.at(1, 0) = 2;
    arr.at(0, 1) = 3;

    // ColumnMajor: 内存布局是列连续的
    // [0,0]=0, [1,0]=1, [2,0]=2, [0,1]=3, ...
    EXPECT_EQ(arr[0], 1);   // [0,0]
    EXPECT_EQ(arr[1], 2);   // [1,0]
    EXPECT_EQ(arr[3], 3);   // [0,1]
}
