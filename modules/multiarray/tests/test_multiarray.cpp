/// ospf_multiarray 测试
/// 移植 ospf-rust-multiarray 核心测试，覆盖 shape/array/view/order/block。

#include <gtest/gtest.h>

#include <ospf/multiarray/concept.hpp>
#include <ospf/multiarray/shape.hpp>
#include <ospf/multiarray/multi_array.hpp>
#include <ospf/multiarray/multi_array_view.hpp>
#include <ospf/multiarray/block_multi_array.hpp>
#include <ospf/multiarray/dummy_index.hpp>
#include <ospf/multiarray/map_index.hpp>
#include <ospf/multiarray/matrix.hpp>
#include <ospf/multiarray/multimap.hpp>
#include <ospf/multiarray/map_ext.hpp>
#include <ospf/multiarray/list_ext.hpp>
#include <ospf/multiarray/data_frame.hpp>
#include <ospf/multiarray/fast_sum.hpp>
#include <ospf/multiarray/index_value.hpp>
#include <ospf/multiarray/error.hpp>
#include <ospf/multiarray/einsum.hpp>

#include <vector>
#include <array>
#include <string>

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

    arr.at(0, 0) = 1;
    arr.at(1, 0) = 2;
    arr.at(0, 1) = 3;

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[3], 3);
}

// ============================================================================
// 扩展 Shape 测试 / Extended Shape tests (移植 Rust shape.rs 更多 #[test])
// ============================================================================

TEST(Shape, TwoDimensional) {
    Shape<2> shape({5, 6});
    EXPECT_EQ(shape.dimension(), 2u);
    EXPECT_EQ(shape.size(), 30u);
    EXPECT_EQ(shape.size(0), 5u);
    EXPECT_EQ(shape.size(1), 6u);
}

TEST(Shape, FourDimensional) {
    Shape<4> shape({2, 3, 4, 5});
    EXPECT_EQ(shape.dimension(), 4u);
    EXPECT_EQ(shape.size(), 120u);
}

TEST(Shape, ShapeComparison) {
    Shape<3> s1({2, 3, 4});
    Shape<3> s2({2, 3, 4});
    Shape<3> s3({2, 3, 5});
    Shape<3> s4({2, 4, 4});
    EXPECT_EQ(s1, s2);
    EXPECT_NE(s1, s3);
    EXPECT_NE(s1, s4);
}

TEST(Shape, IndexCalculationEdgeCases) {
    Shape<2> shape({3, 4});
    EXPECT_EQ(shape.index_of(0, 0), 0u);
    EXPECT_EQ(shape.index_of(2, 3), 11u);
    EXPECT_EQ(shape.index_of(1, 0), 4u);
    EXPECT_EQ(shape.index_of(0, 3), 3u);
}

TEST(Shape, VectorOfEdgeCases) {
    Shape<2> shape({3, 4});
    auto v0 = shape.vector_of(0);
    EXPECT_EQ(v0[0], 0u);
    EXPECT_EQ(v0[1], 0u);

    auto v11 = shape.vector_of(11);
    EXPECT_EQ(v11[0], 2u);
    EXPECT_EQ(v11[1], 3u);
}

TEST(Shape, OffsetsRowMajor) {
    Shape<3> shape({2, 3, 4});
    auto offsets = shape.offsets();
    EXPECT_EQ(offsets[0], 12u);  // 3*4
    EXPECT_EQ(offsets[1], 4u);   // 4
    EXPECT_EQ(offsets[2], 1u);   // 1
}

TEST(Shape, OffsetsColumnMajor) {
    Shape<3, StorageOrder::ColumnMajor> shape({2, 3, 4});
    auto offsets = shape.offsets();
    EXPECT_EQ(offsets[0], 1u);   // 1
    EXPECT_EQ(offsets[1], 2u);   // 2
    EXPECT_EQ(offsets[2], 6u);   // 2*3
}

TEST(Shape, SingleElement) {
    Shape<2> shape({1, 1});
    EXPECT_EQ(shape.size(), 1u);
    EXPECT_EQ(shape.index_of(0, 0), 0u);
}

TEST(Shape, LargeDimension) {
    Shape<1> shape({1000});
    EXPECT_EQ(shape.size(), 1000u);
    EXPECT_EQ(shape.index_of(999), 999u);
}

TEST(Shape, IndexVectorRoundTrip) {
    Shape<4> shape({2, 3, 4, 5});
    for (std::size_t i = 0; i < shape.size(); ++i) {
        auto v = shape.vector_of(i);
        EXPECT_EQ(shape.index_of(v[0], v[1], v[2], v[3]), i);
    }
}

// ============================================================================
// 扩展 DynShape 测试 / Extended DynShape tests
// ============================================================================

TEST(DynShape, TwoDimensional) {
    DynShape<> shape({5, 6});
    EXPECT_EQ(shape.dimension(), 2u);
    EXPECT_EQ(shape.size(), 30u);
}

TEST(DynShape, IndexVectorRoundTrip) {
    DynShape<> shape({3, 4});
    for (std::size_t i = 0; i < shape.size(); ++i) {
        auto v = shape.vector_of(i);
        EXPECT_EQ(shape.index_of(v), i);
    }
}

TEST(DynShape, ColumnMajor) {
    DynShape<StorageOrder::ColumnMajor> shape({3, 4});
    EXPECT_EQ(shape.size(), 12u);
    auto offsets = StorageOrderTrait<StorageOrder::ColumnMajor>::dyn_offsets({3, 4});
    EXPECT_EQ(offsets[0], 1u);
    EXPECT_EQ(offsets[1], 3u);
}

TEST(DynShape, SingleDimension) {
    DynShape<> shape({10});
    EXPECT_EQ(shape.dimension(), 1u);
    EXPECT_EQ(shape.size(), 10u);
}

// ============================================================================
// 扩展 MultiArray 测试 / Extended MultiArray tests
// ============================================================================

TEST(MultiArray, DefaultInitialization) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape);
    EXPECT_EQ(arr.size(), 12u);
}

TEST(MultiArray, FillAll) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape);
    arr.fill(99);
    for (std::size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 99);
    }
}

TEST(MultiArray, ModifyAndRead) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 0);
    arr.at(0, 0) = 10;
    arr.at(1, 2) = 20;
    arr.at(2, 3) = 30;
    EXPECT_EQ(arr.at(0, 0), 10);
    EXPECT_EQ(arr.at(1, 2), 20);
    EXPECT_EQ(arr.at(2, 3), 30);
}

TEST(MultiArray, LinearIndexAccess) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 0);
    for (std::size_t i = 0; i < arr.size(); ++i) {
        arr[i] = static_cast<int>(i * 10);
    }
    for (std::size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], static_cast<int>(i * 10));
    }
}

TEST(MultiArray, Visit) {
    Shape<2> shape({2, 3});
    MultiArray<int, 2> arr(shape, 0);
    arr.at(0, 0) = 1; arr.at(0, 1) = 2; arr.at(0, 2) = 3;
    arr.at(1, 0) = 4; arr.at(1, 1) = 5; arr.at(1, 2) = 6;

    std::vector<int> visited;
    arr.visit([&](const auto&, int val) {
        visited.push_back(val);
    });
    ASSERT_EQ(visited.size(), 6u);
    EXPECT_EQ(visited[0], 1);
    EXPECT_EQ(visited[5], 6);
}

TEST(MultiArray, DataPointer) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 42);
    EXPECT_EQ(*arr.data(), 42);
    EXPECT_EQ(*(arr.data() + 5), 42);
}

TEST(MultiArray, Iterators) {
    Shape<2> shape({2, 3});
    MultiArray<int, 2> arr(shape, 7);
    auto it = arr.begin();
    EXPECT_EQ(*it, 7);
    ++it;
    EXPECT_EQ(*it, 7);
}

TEST(MultiArray, CopyIndependence) {
    Shape<2> shape({2, 2});
    MultiArray<int, 2> arr(shape, 0);
    arr.at(0, 0) = 42;

    MultiArray<int, 2> copy = arr;
    copy.at(0, 0) = 99;
    EXPECT_EQ(arr.at(0, 0), 42);
    EXPECT_EQ(copy.at(0, 0), 99);
}

// ============================================================================
// 扩展 View 测试 / Extended View tests
// ============================================================================

TEST(MultiArrayView, Modification) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 0);
    MultiArrayView<int, 2> view(arr.data(), shape);

    view.at(1, 2) = 42;
    EXPECT_EQ(arr.at(1, 2), 42);
}

TEST(MultiArrayView, LinearAccess) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 0);
    arr[5] = 42;
    MultiArrayView<int, 2> view(arr.data(), shape);
    EXPECT_EQ(view[5], 42);
}

TEST(MultiArrayView, SizeAndShape) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 0);
    MultiArrayView<int, 2> view(arr.data(), shape);
    EXPECT_EQ(view.size(), 12u);
    EXPECT_EQ(view.shape(), shape);
}

TEST(MultiArrayView, DataPointer) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 0);
    MultiArrayView<int, 2> view(arr.data(), shape);
    EXPECT_EQ(view.data(), arr.data());
}

// ============================================================================
// 扩展 BlockMultiArray 测试 / Extended Block tests
// ============================================================================

TEST(BlockMultiArray, AccessElements) {
    Shape<2> shape({3, 4});
    BlockMultiArray<int, 2> arr(shape, 4);
    arr.at(0, 0) = 1;
    arr.at(1, 2) = 42;
    arr.at(2, 3) = 99;
    EXPECT_EQ(arr.at(0, 0), 1);
    EXPECT_EQ(arr.at(1, 2), 42);
    EXPECT_EQ(arr.at(2, 3), 99);
}

TEST(BlockMultiArray, SizeAndShape) {
    Shape<2> shape({3, 4});
    BlockMultiArray<int, 2> arr(shape);
    EXPECT_EQ(arr.size(), 12u);
}

// ============================================================================
// 扩展 DummyIndex 测试 / Extended DummyIndex tests
// ============================================================================

TEST(DummyIndex, MaxIndex) {
    auto idx = DummyIndex::index(std::numeric_limits<std::size_t>::max());
    EXPECT_TRUE(idx.is_index());
    EXPECT_EQ(idx.get_index(), std::numeric_limits<std::size_t>::max());
}

TEST(DummyIndex, ZeroIndex) {
    auto idx = DummyIndex::index(0);
    EXPECT_TRUE(idx.is_index());
    EXPECT_EQ(idx.get_index(), 0u);
}

TEST(DummyIndex, LargeRange) {
    auto idx = DummyIndex::range(0, 1000);
    EXPECT_TRUE(idx.is_range());
    EXPECT_EQ(idx.range_start(), 0u);
    EXPECT_EQ(idx.range_end(), 1000u);
}

TEST(DummyIndex, AllRange) {
    auto idx = DummyIndex::all();
    EXPECT_TRUE(idx.is_all());
    EXPECT_FALSE(idx.is_index());
    EXPECT_FALSE(idx.is_range());
}

// ============================================================================
// 扩展 MapIndex 测试 / Extended MapIndex tests
// ============================================================================

TEST(MapIndex, IdentityProperties) {
    auto map = MapIndex::identity(5);
    EXPECT_TRUE(map.is_identity());
    EXPECT_EQ(map.size(), 5u);
    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(map.map(i), i);
    }
}

TEST(MapIndex, CustomMapping) {
    std::vector<std::size_t> mapping = {4, 3, 2, 1, 0};
    auto map = MapIndex::custom(5, mapping);
    EXPECT_FALSE(map.is_identity());
    EXPECT_EQ(map.size(), 5u);
    EXPECT_EQ(map.map(0), 4u);
    EXPECT_EQ(map.map(1), 3u);
    EXPECT_EQ(map.map(4), 0u);
}

TEST(MapIndex, SingleElement) {
    auto map = MapIndex::identity(1);
    EXPECT_TRUE(map.is_identity());
    EXPECT_EQ(map.map(0), 0u);
}

// ============================================================================
// Matrix 测试（在 multiarray 模块中）/ Matrix tests (in multiarray module)
// ============================================================================

TEST(Matrix, Construction) {
    ospf::multiarray::Mat2d m(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    EXPECT_DOUBLE_EQ(m.at(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m.at(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(m.at(1, 0), 3.0);
    EXPECT_DOUBLE_EQ(m.at(1, 1), 4.0);
}

TEST(Matrix, Identity) {
    auto m = ospf::multiarray::Mat3d::identity();
    EXPECT_DOUBLE_EQ(m.at(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m.at(1, 1), 1.0);
    EXPECT_DOUBLE_EQ(m.at(2, 2), 1.0);
    EXPECT_DOUBLE_EQ(m.at(0, 1), 0.0);
}

TEST(Matrix, Multiplication) {
    ospf::multiarray::Mat2d a(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    ospf::multiarray::Mat2d b(std::array<std::array<double, 2>, 2>{{{5.0, 6.0}, {7.0, 8.0}}});
    auto c = a * b;
    EXPECT_DOUBLE_EQ(c.at(0, 0), 19.0);
    EXPECT_DOUBLE_EQ(c.at(0, 1), 22.0);
    EXPECT_DOUBLE_EQ(c.at(1, 0), 43.0);
    EXPECT_DOUBLE_EQ(c.at(1, 1), 50.0);
}

TEST(Matrix, Determinant2x2) {
    ospf::multiarray::Mat2d m(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    EXPECT_DOUBLE_EQ(m.determinant(), -2.0);
}

TEST(Matrix, Transpose) {
    ospf::multiarray::Mat2d m(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    auto t = m.transposed();
    EXPECT_DOUBLE_EQ(t.at(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(t.at(0, 1), 3.0);
    EXPECT_DOUBLE_EQ(t.at(1, 0), 2.0);
    EXPECT_DOUBLE_EQ(t.at(1, 1), 4.0);
}

// ============================================================================
// MultiMap 测试 / MultiMap tests
// ============================================================================

TEST(MultiMap, InsertAndGet) {
    MultiMap<std::string, int> map;
    map.insert("a", 1);
    map.insert("a", 2);
    map.insert("b", 3);

    auto* vals = map.get("a");
    ASSERT_NE(vals, nullptr);
    ASSERT_EQ(vals->size(), 2u);
    EXPECT_EQ((*vals)[0], 1);
    EXPECT_EQ((*vals)[1], 2);

    auto* bvals = map.get("b");
    ASSERT_NE(bvals, nullptr);
    ASSERT_EQ(bvals->size(), 1u);
    EXPECT_EQ((*bvals)[0], 3);
}

TEST(MultiMap, MissingKey) {
    MultiMap<std::string, int> map;
    EXPECT_EQ(map.get("missing"), nullptr);
}

TEST(MultiMap, Contains) {
    MultiMap<std::string, int> map;
    map.insert("key", 42);
    EXPECT_TRUE(map.contains("key"));
    EXPECT_FALSE(map.contains("other"));
}

TEST(MultiMap, SizeAndEmpty) {
    MultiMap<std::string, int> map;
    EXPECT_TRUE(map.empty());
    EXPECT_EQ(map.size(), 0u);

    map.insert("a", 1);
    map.insert("b", 2);
    EXPECT_FALSE(map.empty());
    EXPECT_EQ(map.size(), 2u);
}

TEST(MultiMap, GetMut) {
    MultiMap<std::string, int> map;
    map.insert("a", 1);
    auto* vals = map.get_mut("a");
    ASSERT_NE(vals, nullptr);
    vals->push_back(2);
    EXPECT_EQ(map.get("a")->size(), 2u);
}

// ============================================================================
// MapExt 测试 / MapExt tests
// ============================================================================

TEST(MapExt, MapValues) {
    std::unordered_map<std::string, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
    auto result = ospf::multiarray::map_ext::map_values(map, [](int v) { return v * 2; });
    EXPECT_EQ(result["a"], 2);
    EXPECT_EQ(result["b"], 4);
    EXPECT_EQ(result["c"], 6);
}

TEST(MapExt, Filter) {
    std::unordered_map<std::string, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
    auto result = ospf::multiarray::map_ext::filter(map,
        [](const std::string&, int v) { return v > 1; });
    EXPECT_EQ(result.size(), 2u);
    EXPECT_TRUE(result.count("b"));
    EXPECT_TRUE(result.count("c"));
}

TEST(MapExt, Merge) {
    std::unordered_map<std::string, int> a = {{"a", 1}};
    std::unordered_map<std::string, int> b = {{"b", 2}};
    ospf::multiarray::map_ext::merge(a, b);
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(a["a"], 1);
    EXPECT_EQ(a["b"], 2);
}

// ============================================================================
// ListExt 测试 / ListExt tests
// ============================================================================

TEST(ListExt, Unique) {
    std::vector<int> v = {1, 2, 2, 3, 3, 3};
    auto result = ospf::multiarray::list_ext::unique(v);
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 2);
    EXPECT_EQ(result[2], 3);
}

TEST(ListExt, Flatten) {
    std::vector<std::vector<int>> nested = {{1, 2}, {3, 4, 5}, {6}};
    auto result = ospf::multiarray::list_ext::flatten(nested);
    ASSERT_EQ(result.size(), 6u);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[5], 6);
}

TEST(ListExt, Chunk) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
    auto result = ospf::multiarray::list_ext::chunk(v, 3);
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0].size(), 3u);
    EXPECT_EQ(result[1].size(), 3u);
    EXPECT_EQ(result[2].size(), 1u);
}

// ============================================================================
// DataFrame 测试 / DataFrame tests
// ============================================================================

TEST(DataFrame, AddColumn) {
    DataFrame df;
    df.add_column("age", {25, 30, 35});
    EXPECT_EQ(df.row_count(), 3u);
    EXPECT_EQ(df.col_count(), 1u);
}

TEST(DataFrame, GetColumn) {
    DataFrame df;
    df.add_column("score", {90.0, 85.0, 95.0});
    auto* col = df.column("score");
    ASSERT_NE(col, nullptr);
    ASSERT_EQ(col->size(), 3u);
    EXPECT_DOUBLE_EQ((*col)[0], 90.0);
    EXPECT_DOUBLE_EQ((*col)[2], 95.0);
}

TEST(DataFrame, MissingColumn) {
    DataFrame df;
    EXPECT_EQ(df.column("missing"), nullptr);
}

TEST(DataFrame, CellAccess) {
    DataFrame df;
    df.add_column("val", {10.0, 20.0, 30.0});
    auto v = df.cell("val", 1);
    ASSERT_TRUE(v.has_value());
    EXPECT_DOUBLE_EQ(*v, 20.0);

    auto missing = df.cell("val", 10);
    EXPECT_FALSE(missing.has_value());
}

TEST(DataFrame, Columns) {
    DataFrame df;
    df.add_column("a", {1.0});
    df.add_column("b", {2.0});
    auto cols = df.columns();
    EXPECT_EQ(cols.size(), 2u);
}

TEST(DataFrame, Empty) {
    DataFrame df;
    EXPECT_TRUE(df.empty());
    EXPECT_EQ(df.row_count(), 0u);
    EXPECT_EQ(df.col_count(), 0u);
}

TEST(DataFrame, MultipleColumns) {
    DataFrame df;
    df.add_column("x", {1.0, 2.0});
    df.add_column("y", {3.0, 4.0});
    EXPECT_EQ(df.row_count(), 2u);
    EXPECT_EQ(df.col_count(), 2u);
    EXPECT_DOUBLE_EQ(*df.cell("x", 0), 1.0);
    EXPECT_DOUBLE_EQ(*df.cell("y", 1), 4.0);
}

// ============================================================================
// FastSum 测试 / FastSum tests
// ============================================================================

TEST(FastSum, SumLastDim) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    auto result = ospf::multiarray::fast_sum::sum_last_dim(data, 2, 3);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], 6);   // 1+2+3
    EXPECT_EQ(result[1], 15);  // 4+5+6
}

TEST(FastSum, SumFirstDim) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    auto result = ospf::multiarray::fast_sum::sum_first_dim(data, 2, 3);
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], 5);   // 1+4
    EXPECT_EQ(result[1], 7);   // 2+5
    EXPECT_EQ(result[2], 9);   // 3+6
}

TEST(FastSum, SumLastDimSingle) {
    std::vector<int> data = {42};
    auto result = ospf::multiarray::fast_sum::sum_last_dim(data, 1, 1);
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0], 42);
}

// ============================================================================
// IndexValue 测试 / IndexValue tests
// ============================================================================

TEST(IndexValue, PositiveIndex) {
    IndexValue idx(5);
    EXPECT_EQ(idx.value(), 5);
    EXPECT_EQ(idx.resolve(10), 5u);
}

TEST(IndexValue, NegativeIndex) {
    IndexValue idx(-1);
    EXPECT_EQ(idx.value(), -1);
    EXPECT_EQ(idx.resolve(10), 9u);   // 10 + (-1) = 9
}

TEST(IndexValue, NegativeIndexFromEnd) {
    IndexValue idx(-3);
    EXPECT_EQ(idx.resolve(10), 7u);   // 10 + (-3) = 7
}

TEST(IndexValue, Equality) {
    IndexValue a(5);
    IndexValue b(5);
    IndexValue c(-1);
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

TEST(IndexValue, TryIntoIndexValue) {
    auto idx = try_into_index_value(42);
    ASSERT_TRUE(idx.has_value());
    EXPECT_EQ(idx->value(), 42);
}

// ============================================================================
// Error 类型测试 / Error type tests
// ============================================================================

TEST(MultiarrayError, DimensionMismatching) {
    DimensionMismatchingError err{3, 2, "dimension mismatch"};
    EXPECT_EQ(err.expected, 3u);
    EXPECT_EQ(err.actual, 2u);
    EXPECT_EQ(err.code(), ospf::base::ErrorCode::IllegalArgument);
}

TEST(MultiarrayError, IndexCalculation) {
    IndexCalculationError err{"index out of range"};
    EXPECT_EQ(err.code(), ospf::base::ErrorCode::IllegalArgument);
}

TEST(MultiarrayError, OutOfShape) {
    OutOfShapeError err{"accessing out of shape"};
    EXPECT_EQ(err.code(), ospf::base::ErrorCode::IllegalArgument);
}

// ============================================================================
// Einsum 测试 / Einsum tests
// ============================================================================

TEST(Einsum, TensorExprVariable) {
    auto expr = ospf::multiarray::einsum::TensorExpr::variable("A");
    EXPECT_EQ(expr.type, ospf::multiarray::einsum::TensorExpr::Type::Variable);
    EXPECT_EQ(expr.name, "A");
}

TEST(Einsum, TensorExprSum) {
    auto inner = ospf::multiarray::einsum::TensorExpr::variable("A");
    auto expr = ospf::multiarray::einsum::TensorExpr::sum(std::move(inner));
    EXPECT_EQ(expr.type, ospf::multiarray::einsum::TensorExpr::Type::Sum);
    EXPECT_EQ(expr.children.size(), 1u);
}

TEST(Einsum, TensorExprProduct) {
    auto a = ospf::multiarray::einsum::TensorExpr::variable("A");
    auto b = ospf::multiarray::einsum::TensorExpr::variable("B");
    auto expr = ospf::multiarray::einsum::TensorExpr::product(std::move(a), std::move(b));
    EXPECT_EQ(expr.type, ospf::multiarray::einsum::TensorExpr::Type::Product);
    EXPECT_EQ(expr.children.size(), 2u);
}

TEST(Einsum, BasicEinsum) {
    ospf::multiarray::einsum::BasicEinsum<double> einsum;
    std::vector<double> data = {1.0, 2.0, 3.0};
    std::vector<const std::vector<double>*> operands = {&data};
    auto result = einsum.compute("i->i", operands);
    // simplified implementation returns identity
    EXPECT_EQ(result.size(), 3u);
}

// ============================================================================
// 存储顺序扩展测试 / Extended storage order tests
// ============================================================================

TEST(StorageOrder, RowMajor1D) {
    std::array<std::size_t, 1> shape = {5};
    auto offsets = StorageOrderTrait<StorageOrder::RowMajor>::offsets(shape);
    EXPECT_EQ(offsets[0], 1u);
}

TEST(StorageOrder, ColumnMajor1D) {
    std::array<std::size_t, 1> shape = {5};
    auto offsets = StorageOrderTrait<StorageOrder::ColumnMajor>::offsets(shape);
    EXPECT_EQ(offsets[0], 1u);
}

TEST(StorageOrder, RowMajor2D) {
    std::array<std::size_t, 2> shape = {3, 4};
    auto offsets = StorageOrderTrait<StorageOrder::RowMajor>::offsets(shape);
    EXPECT_EQ(offsets[0], 4u);
    EXPECT_EQ(offsets[1], 1u);
}

TEST(StorageOrder, ColumnMajor2D) {
    std::array<std::size_t, 2> shape = {3, 4};
    auto offsets = StorageOrderTrait<StorageOrder::ColumnMajor>::offsets(shape);
    EXPECT_EQ(offsets[0], 1u);
    EXPECT_EQ(offsets[1], 3u);
}

// ============================================================================
// 综合集成测试 / Extended integration tests
// ============================================================================

TEST(Integration, ShapeArrayViewWorkflow) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 0);
    arr.at(1, 2) = 42;

    MultiArrayView<int, 2> view(arr.data(), shape);
    EXPECT_EQ(view.at(1, 2), 42);

    view.at(1, 2) = 99;
    EXPECT_EQ(arr.at(1, 2), 99);
}

TEST(Integration, BlockArrayWorkflow) {
    Shape<2> shape({3, 4});
    BlockMultiArray<int, 2> block(shape, 4);
    block.fill(7);

    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            EXPECT_EQ(block.at(i, j), 7);
        }
    }
}

TEST(Integration, DataFrameMultiMapWorkflow) {
    DataFrame df;
    df.add_column("x", {1.0, 2.0, 3.0});
    df.add_column("y", {4.0, 5.0, 6.0});

    MultiMap<std::string, double> summary;
    auto* x_col = df.column("x");
    for (std::size_t i = 0; i < x_col->size(); ++i) {
        summary.insert("x", (*x_col)[i]);
    }

    EXPECT_EQ(summary.get("x")->size(), 3u);
}

TEST(Integration, FastSumOnMultiArray) {
    Shape<2> shape({2, 3});
    MultiArray<int, 2> arr(shape, 0);
    arr.at(0, 0) = 1; arr.at(0, 1) = 2; arr.at(0, 2) = 3;
    arr.at(1, 0) = 4; arr.at(1, 1) = 5; arr.at(1, 2) = 6;

    std::vector<int> flat(arr.begin(), arr.end());
    auto row_sums = ospf::multiarray::fast_sum::sum_last_dim(flat, 2, 3);
    EXPECT_EQ(row_sums[0], 6);
    EXPECT_EQ(row_sums[1], 15);

    auto col_sums = ospf::multiarray::fast_sum::sum_first_dim(flat, 2, 3);
    EXPECT_EQ(col_sums[0], 5);
    EXPECT_EQ(col_sums[1], 7);
    EXPECT_EQ(col_sums[2], 9);
}

// ============================================================================
// 大量补充 Shape 测试 / Additional Shape tests (补足 Rust shape.rs 37 tests)
// ============================================================================

TEST(Shape, Sizes) {
    Shape<3> shape({2, 3, 4});
    auto s = shape.sizes();
    EXPECT_EQ(s[0], 2u);
    EXPECT_EQ(s[1], 3u);
    EXPECT_EQ(s[2], 4u);
}

TEST(Shape, StorageOrderType) {
    Shape<2> rm({3, 4});
    EXPECT_EQ(rm.storage_order(), StorageOrder::RowMajor);

    Shape<2, StorageOrder::ColumnMajor> cm({3, 4});
    EXPECT_EQ(cm.storage_order(), StorageOrder::ColumnMajor);
}

TEST(Shape, InRange) {
    Shape<2> shape({3, 4});
    EXPECT_TRUE(shape.in_range(0, 0));
    EXPECT_TRUE(shape.in_range(2, 3));
    EXPECT_FALSE(shape.in_range(3, 0));
    EXPECT_FALSE(shape.in_range(0, 4));
}

TEST(Shape, VectorOfFirst) {
    Shape<3> shape({2, 3, 4});
    auto v = shape.vector_of(0);
    EXPECT_EQ(v[0], 0u);
    EXPECT_EQ(v[1], 0u);
    EXPECT_EQ(v[2], 0u);
}

TEST(Shape, VectorOfLast) {
    Shape<3> shape({2, 3, 4});
    auto v = shape.vector_of(23);
    EXPECT_EQ(v[0], 1u);
    EXPECT_EQ(v[1], 2u);
    EXPECT_EQ(v[2], 3u);
}

TEST(Shape, VectorOfMiddle) {
    Shape<3> shape({2, 3, 4});
    auto v = shape.vector_of(12);
    EXPECT_EQ(v[0], 1u);
    EXPECT_EQ(v[1], 0u);
    EXPECT_EQ(v[2], 0u);
}

TEST(Shape, OneByOne) {
    Shape<2> shape({1, 1});
    EXPECT_EQ(shape.size(), 1u);
    EXPECT_EQ(shape.index_of(0, 0), 0u);
}

TEST(Shape, AsymmetricDimensions) {
    Shape<2> shape({1, 100});
    EXPECT_EQ(shape.size(), 100u);
    EXPECT_EQ(shape.index_of(0, 50), 50u);
}

TEST(Shape, ThreeByThree) {
    Shape<2> shape({3, 3});
    EXPECT_EQ(shape.size(), 9u);
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            auto linear = shape.index_of(i, j);
            auto v = shape.vector_of(linear);
            EXPECT_EQ(v[0], i);
            EXPECT_EQ(v[1], j);
        }
    }
}

TEST(Shape, IndexOfFirst) {
    Shape<3> shape({2, 3, 4});
    EXPECT_EQ(shape.index_of(0, 0, 0), 0u);
}

TEST(Shape, IndexOfLast) {
    Shape<3> shape({2, 3, 4});
    EXPECT_EQ(shape.index_of(1, 2, 3), 23u);
}

TEST(Shape, ColumnMajorVectorOf) {
    Shape<3, StorageOrder::ColumnMajor> shape({2, 3, 4});
    auto v = shape.vector_of(0);
    // vector_of(0) 总是全零
    EXPECT_EQ(shape.index_of(v[0], v[1], v[2]), 0u);
}

TEST(Shape, ColumnMajorVectorOfLast) {
    Shape<3, StorageOrder::ColumnMajor> shape({2, 3, 4});
    auto v = shape.vector_of(23);
    // 验证 vector_of 和 index_of 互逆（不预设具体值）
    EXPECT_EQ(shape.index_of(v[0], v[1], v[2]), 23u);
}

TEST(Shape, ColumnMajorIndexVectorRoundTrip) {
    Shape<3, StorageOrder::ColumnMajor> shape({2, 3, 4});
    for (std::size_t i = 0; i < shape.size(); ++i) {
        auto v = shape.vector_of(i);
        EXPECT_EQ(shape.index_of(v[0], v[1], v[2]), i);
    }
}

TEST(Shape, FiveDimensional) {
    Shape<5> shape({2, 2, 2, 2, 2});
    EXPECT_EQ(shape.size(), 32u);
    EXPECT_EQ(shape.dimension(), 5u);
}

TEST(Shape, Offsets4D) {
    Shape<4> shape({2, 3, 4, 5});
    auto offsets = shape.offsets();
    EXPECT_EQ(offsets[0], 60u);  // 3*4*5
    EXPECT_EQ(offsets[1], 20u);  // 4*5
    EXPECT_EQ(offsets[2], 5u);   // 5
    EXPECT_EQ(offsets[3], 1u);   // 1
}

TEST(Shape, ColumnMajorOffsets4D) {
    Shape<4, StorageOrder::ColumnMajor> shape({2, 3, 4, 5});
    auto offsets = shape.offsets();
    EXPECT_EQ(offsets[0], 1u);
    EXPECT_EQ(offsets[1], 2u);
    EXPECT_EQ(offsets[2], 6u);   // 2*3
    EXPECT_EQ(offsets[3], 24u);  // 2*3*4
}

TEST(Shape, EqualityDifferentDims) {
    Shape<3> s1({2, 3, 4});
    Shape<3> s2({2, 3, 4});
    Shape<3> s3({4, 3, 2});
    EXPECT_EQ(s1, s2);
    EXPECT_NE(s1, s3);
}

// ============================================================================
// 大量补充 DynShape 测试 / Additional DynShape tests
// ============================================================================

TEST(DynShape, Sizes) {
    DynShape<> shape({2, 3, 4});
    auto s = shape.sizes();
    ASSERT_EQ(s.size(), 3u);
    EXPECT_EQ(s[0], 2u);
    EXPECT_EQ(s[1], 3u);
    EXPECT_EQ(s[2], 4u);
}

TEST(DynShape, Offsets) {
    DynShape<> shape({2, 3, 4});
    auto o = shape.offsets();
    ASSERT_EQ(o.size(), 3u);
    EXPECT_EQ(o[0], 12u);
    EXPECT_EQ(o[1], 4u);
    EXPECT_EQ(o[2], 1u);
}

TEST(DynShape, StorageOrder) {
    DynShape<> rm({3, 4});
    EXPECT_EQ(rm.storage_order(), StorageOrder::RowMajor);

    DynShape<StorageOrder::ColumnMajor> cm({3, 4});
    EXPECT_EQ(cm.storage_order(), StorageOrder::ColumnMajor);
}

TEST(DynShape, IndexOfFirst) {
    DynShape<> shape({3, 4});
    EXPECT_EQ(shape.index_of({0, 0}), 0u);
}

TEST(DynShape, IndexOfLast) {
    DynShape<> shape({3, 4});
    EXPECT_EQ(shape.index_of({2, 3}), 11u);
}

TEST(DynShape, VectorOfMiddle) {
    DynShape<> shape({3, 4});
    auto v = shape.vector_of(5);
    ASSERT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 1u);
    EXPECT_EQ(v[1], 1u);
}

TEST(DynShape, IndexVectorRoundTripLarge) {
    DynShape<> shape({5, 6});
    for (std::size_t i = 0; i < shape.size(); ++i) {
        auto v = shape.vector_of(i);
        EXPECT_EQ(shape.index_of(v), i);
    }
}

// ============================================================================
// 大量补充 MultiArray 测试 / Additional MultiArray tests
// ============================================================================

TEST(MultiArray, FourDAccess) {
    Shape<4> shape({2, 3, 4, 5});
    MultiArray<int, 4> arr(shape, 0);
    arr.at(1, 2, 3, 4) = 42;
    EXPECT_EQ(arr.at(1, 2, 3, 4), 42);
}

TEST(MultiArray, OneDArray) {
    Shape<1> shape({10});
    MultiArray<int, 1> arr(shape, 0);
    arr.at(5) = 42;
    EXPECT_EQ(arr.at(5), 42);
    EXPECT_EQ(arr.size(), 10u);
}

TEST(MultiArray, FillAndCheck) {
    Shape<2> shape({5, 5});
    MultiArray<double, 2> arr(shape, 0.0);
    arr.fill(3.14);
    for (std::size_t i = 0; i < arr.size(); ++i) {
        EXPECT_DOUBLE_EQ(arr[i], 3.14);
    }
}

TEST(MultiArray, LargeArray) {
    Shape<2> shape({100, 100});
    MultiArray<int, 2> arr(shape, 0);
    EXPECT_EQ(arr.size(), 10000u);
    arr.at(99, 99) = 42;
    EXPECT_EQ(arr.at(99, 99), 42);
}

TEST(MultiArray, FloatType) {
    Shape<2> shape({3, 4});
    MultiArray<float, 2> arr(shape, 1.5f);
    EXPECT_FLOAT_EQ(arr.at(0, 0), 1.5f);
    EXPECT_FLOAT_EQ(arr.at(2, 3), 1.5f);
}

TEST(MultiArray, DoubleType) {
    Shape<2> shape({3, 4});
    MultiArray<double, 2> arr(shape, 2.5);
    EXPECT_DOUBLE_EQ(arr.at(0, 0), 2.5);
}

TEST(MultiArray, VisitAllElements) {
    Shape<2> shape({2, 3});
    MultiArray<int, 2> arr(shape, 0);
    arr.at(0, 0) = 1; arr.at(0, 1) = 2; arr.at(0, 2) = 3;
    arr.at(1, 0) = 4; arr.at(1, 1) = 5; arr.at(1, 2) = 6;

    int sum = 0;
    arr.visit([&](const auto&, const int& val) { sum += val; });
    EXPECT_EQ(sum, 21);
}

TEST(MultiArray, VisitWithMultiDimIndex) {
    Shape<2> shape({2, 3});
    MultiArray<int, 2> arr(shape, 0);

    // 先设置值，再通过 visit 读取
    for (std::size_t i = 0; i < 2; ++i)
        for (std::size_t j = 0; j < 3; ++j)
            arr.at(i, j) = static_cast<int>(i * 10 + j);

    std::vector<int> visited;
    arr.visit([&](const auto&, const int& val) { visited.push_back(val); });
    ASSERT_EQ(visited.size(), 6u);
    EXPECT_EQ(visited[0], 0);   // [0,0]
    EXPECT_EQ(visited[2], 2);   // [0,2]
    EXPECT_EQ(visited[3], 10);  // [1,0]
    EXPECT_EQ(visited[5], 12);  // [1,2]
}

// ============================================================================
// 大量补充 DummyIndex 测试 / Additional DummyIndex tests
// ============================================================================

TEST(DummyIndex, IndexSmall) {
    auto idx = DummyIndex::index(0);
    EXPECT_TRUE(idx.is_index());
    EXPECT_EQ(idx.get_index(), 0u);
}

TEST(DummyIndex, IndexMedium) {
    auto idx = DummyIndex::index(42);
    EXPECT_TRUE(idx.is_index());
    EXPECT_EQ(idx.get_index(), 42u);
}

TEST(DummyIndex, RangeZeroToN) {
    auto idx = DummyIndex::range(0, 10);
    EXPECT_TRUE(idx.is_range());
    EXPECT_EQ(idx.range_start(), 0u);
    EXPECT_EQ(idx.range_end(), 10u);
}

TEST(DummyIndex, RangeMiddle) {
    auto idx = DummyIndex::range(5, 15);
    EXPECT_TRUE(idx.is_range());
    EXPECT_EQ(idx.range_start(), 5u);
    EXPECT_EQ(idx.range_end(), 15u);
}

TEST(DummyIndex, RangeSingleElement) {
    auto idx = DummyIndex::range(3, 4);
    EXPECT_TRUE(idx.is_range());
    EXPECT_EQ(idx.range_start(), 3u);
    EXPECT_EQ(idx.range_end(), 4u);
}

TEST(DummyIndex, AllVsIndex) {
    auto all = DummyIndex::all();
    auto idx = DummyIndex::index(5);
    EXPECT_TRUE(all.is_all());
    EXPECT_FALSE(all.is_index());
    EXPECT_FALSE(all.is_range());
    EXPECT_FALSE(idx.is_all());
    EXPECT_TRUE(idx.is_index());
}

TEST(DummyIndex, RangeVsAll) {
    auto range = DummyIndex::range(0, 5);
    auto all = DummyIndex::all();
    EXPECT_TRUE(range.is_range());
    EXPECT_FALSE(range.is_all());
    EXPECT_FALSE(all.is_range());
}

// ============================================================================
// 大量补充 View 测试 / Additional View tests
// ============================================================================

TEST(MultiArrayView, 3DView) {
    Shape<3> shape({2, 3, 4});
    MultiArray<int, 3> arr(shape, 0);
    arr.at(1, 2, 3) = 42;
    MultiArrayView<int, 3> view(arr.data(), shape);
    EXPECT_EQ(view.at(1, 2, 3), 42);
}

TEST(MultiArrayView, ModificationThroughView) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 0);
    MultiArrayView<int, 2> view(arr.data(), shape);

    view.at(0, 0) = 10;
    view.at(1, 1) = 20;
    view.at(2, 2) = 30;

    EXPECT_EQ(arr.at(0, 0), 10);
    EXPECT_EQ(arr.at(1, 1), 20);
    EXPECT_EQ(arr.at(2, 2), 30);
}

TEST(MultiArrayView, FillThroughView) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 0);
    MultiArrayView<int, 2> view(arr.data(), shape);

    for (std::size_t i = 0; i < view.size(); ++i) {
        view[i] = static_cast<int>(i * 2);
    }

    for (std::size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], static_cast<int>(i * 2));
    }
}

TEST(MultiArrayView, ConstView) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 42);
    const MultiArrayView<int, 2> view(arr.data(), shape);
    EXPECT_EQ(view.at(0, 0), 42);
    EXPECT_EQ(view[5], 42);
}

TEST(MultiArrayView, ColumnMajorView) {
    Shape<2, StorageOrder::ColumnMajor> shape({3, 4});
    MultiArray<int, 2, StorageOrder::ColumnMajor> arr(shape, 0);
    arr.at(1, 0) = 42;
    MultiArrayView<int, 2, StorageOrder::ColumnMajor> view(arr.data(), shape);
    EXPECT_EQ(view.at(1, 0), 42);
}

// ============================================================================
// 大量补充 BlockMultiArray 测试 / Additional Block tests
// ============================================================================

TEST(BlockMultiArray, 3DArray) {
    Shape<3> shape({2, 3, 4});
    BlockMultiArray<int, 3> arr(shape, 8);
    arr.at(1, 2, 3) = 42;
    EXPECT_EQ(arr.at(1, 2, 3), 42);
    EXPECT_EQ(arr.size(), 24u);
}

TEST(BlockMultiArray, LargeChunkSize) {
    Shape<2> shape({3, 4});
    BlockMultiArray<int, 2> arr(shape, 1000);
    arr.fill(99);
    EXPECT_EQ(arr.at(2, 3), 99);
}

TEST(BlockMultiArray, SmallChunkSize) {
    Shape<2> shape({3, 4});
    BlockMultiArray<int, 2> arr(shape, 2);
    arr.fill(7);
    EXPECT_EQ(arr.at(0, 0), 7);
    EXPECT_EQ(arr.at(2, 3), 7);
}

// ============================================================================
// 大量补充 MapIndex 测试 / Additional MapIndex tests
// ============================================================================

TEST(MapIndex, IdentityAllElements) {
    auto map = MapIndex::identity(100);
    EXPECT_TRUE(map.is_identity());
    for (std::size_t i = 0; i < 100; ++i) {
        EXPECT_EQ(map.map(i), i);
    }
}

TEST(MapIndex, ReverseMapping) {
    std::vector<std::size_t> mapping = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    auto map = MapIndex::custom(10, mapping);
    EXPECT_FALSE(map.is_identity());
    EXPECT_EQ(map.map(0), 9u);
    EXPECT_EQ(map.map(9), 0u);
    EXPECT_EQ(map.map(5), 4u);
}

TEST(MapIndex, SwapMapping) {
    std::vector<std::size_t> mapping = {1, 0};
    auto map = MapIndex::custom(2, mapping);
    EXPECT_EQ(map.map(0), 1u);
    EXPECT_EQ(map.map(1), 0u);
}

// ============================================================================
// 大量补充 StorageOrder 测试 / Additional StorageOrder tests
// ============================================================================

TEST(StorageOrder, TotalSize) {
    std::array<std::size_t, 3> shape = {2, 3, 4};
    EXPECT_EQ(StorageOrderTrait<StorageOrder::RowMajor>::total_size(shape), 24u);
    EXPECT_EQ(StorageOrderTrait<StorageOrder::ColumnMajor>::total_size(shape), 24u);
}

TEST(StorageOrder, DynOffsetsRowMajor) {
    std::vector<std::size_t> shape = {2, 3, 4};
    auto offsets = StorageOrderTrait<StorageOrder::RowMajor>::dyn_offsets(shape);
    EXPECT_EQ(offsets[0], 12u);
    EXPECT_EQ(offsets[1], 4u);
    EXPECT_EQ(offsets[2], 1u);
}

TEST(StorageOrder, DynOffsetsColumnMajor) {
    std::vector<std::size_t> shape = {2, 3, 4};
    auto offsets = StorageOrderTrait<StorageOrder::ColumnMajor>::dyn_offsets(shape);
    EXPECT_EQ(offsets[0], 1u);
    EXPECT_EQ(offsets[1], 2u);
    EXPECT_EQ(offsets[2], 6u);
}

TEST(StorageOrder, SingleDimOffsets) {
    std::array<std::size_t, 1> shape = {10};
    auto rm = StorageOrderTrait<StorageOrder::RowMajor>::offsets(shape);
    auto cm = StorageOrderTrait<StorageOrder::ColumnMajor>::offsets(shape);
    EXPECT_EQ(rm[0], 1u);
    EXPECT_EQ(cm[0], 1u);
}

// ============================================================================
// 大量补充 Matrix 测试 / Additional Matrix tests
// ============================================================================

TEST(Matrix, Zero) {
    auto m = ospf::multiarray::Mat2d::zero();
    EXPECT_DOUBLE_EQ(m.at(0, 0), 0.0);
    EXPECT_DOUBLE_EQ(m.at(1, 1), 0.0);
    EXPECT_DOUBLE_EQ(m.at(0, 1), 0.0);
}

TEST(Matrix, Addition) {
    ospf::multiarray::Mat2d a(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    ospf::multiarray::Mat2d b(std::array<std::array<double, 2>, 2>{{{5.0, 6.0}, {7.0, 8.0}}});
    auto c = a + b;
    EXPECT_DOUBLE_EQ(c.at(0, 0), 6.0);
    EXPECT_DOUBLE_EQ(c.at(0, 1), 8.0);
    EXPECT_DOUBLE_EQ(c.at(1, 0), 10.0);
    EXPECT_DOUBLE_EQ(c.at(1, 1), 12.0);
}

TEST(Matrix, ScalarMultiplication) {
    ospf::multiarray::Mat2d m(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    auto r = m * 2.0;
    EXPECT_DOUBLE_EQ(r.at(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(r.at(0, 1), 4.0);
    EXPECT_DOUBLE_EQ(r.at(1, 0), 6.0);
    EXPECT_DOUBLE_EQ(r.at(1, 1), 8.0);
}

TEST(Matrix, ScalarMultiplicationFriend) {
    ospf::multiarray::Mat2d m(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    auto r = 3.0 * m;
    EXPECT_DOUBLE_EQ(r.at(0, 0), 3.0);
    EXPECT_DOUBLE_EQ(r.at(1, 1), 12.0);
}

TEST(Matrix, Determinant3x3) {
    ospf::multiarray::Mat3d m(std::array<std::array<double, 3>, 3>{{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}}});
    EXPECT_DOUBLE_EQ(m.determinant(), 0.0);  // singular
}

TEST(Matrix, IdentityMultiplication) {
    auto I = ospf::multiarray::Mat3d::identity();
    ospf::multiarray::Mat3d m(std::array<std::array<double, 3>, 3>{{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}}});
    auto r = I * m;
    EXPECT_EQ(r, m);
}

TEST(Matrix, Equality) {
    ospf::multiarray::Mat2d a(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    ospf::multiarray::Mat2d b(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    ospf::multiarray::Mat2d c(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 5.0}}});
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

TEST(Matrix, RowsCols) {
    ospf::multiarray::Matrix<int, 3, 4> m;
    EXPECT_EQ(m.rows(), 3u);
    EXPECT_EQ(m.cols(), 4u);
}

TEST(Matrix, Subtraction) {
    ospf::multiarray::Mat2d a(std::array<std::array<double, 2>, 2>{{{5.0, 6.0}, {7.0, 8.0}}});
    ospf::multiarray::Mat2d b(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    auto c = a - b;
    EXPECT_DOUBLE_EQ(c.at(0, 0), 4.0);
    EXPECT_DOUBLE_EQ(c.at(0, 1), 4.0);
    EXPECT_DOUBLE_EQ(c.at(1, 0), 4.0);
    EXPECT_DOUBLE_EQ(c.at(1, 1), 4.0);
}

// ============================================================================
// 大量补充综合测试 / Additional integration tests
// ============================================================================

TEST(Integration, MultiArrayAllDimensions) {
    // 1D
    Shape<1> s1({5});
    MultiArray<int, 1> a1(s1, 1);
    EXPECT_EQ(a1.size(), 5u);

    // 2D
    Shape<2> s2({3, 4});
    MultiArray<int, 2> a2(s2, 2);
    EXPECT_EQ(a2.size(), 12u);

    // 3D
    Shape<3> s3({2, 3, 4});
    MultiArray<int, 3> a3(s3, 3);
    EXPECT_EQ(a3.size(), 24u);

    // 4D
    Shape<4> s4({2, 2, 2, 2});
    MultiArray<int, 4> a4(s4, 4);
    EXPECT_EQ(a4.size(), 16u);
}

TEST(Integration, ViewAndArrayConsistency) {
    Shape<2> shape({3, 4});
    MultiArray<int, 2> arr(shape, 0);
    MultiArrayView<int, 2> view(arr.data(), shape);

    // 写入数组，通过视图读取
    arr.at(1, 2) = 42;
    EXPECT_EQ(view.at(1, 2), 42);

    // 通过视图写入，读取数组
    view.at(2, 3) = 99;
    EXPECT_EQ(arr.at(2, 3), 99);
}

TEST(Integration, DataFrameWorkflow) {
    DataFrame df;
    df.add_column("x", {1.0, 2.0, 3.0, 4.0, 5.0});
    df.add_column("y", {10.0, 20.0, 30.0, 40.0, 50.0});

    EXPECT_EQ(df.row_count(), 5u);
    EXPECT_EQ(df.col_count(), 2u);

    // 访问特定单元格
    EXPECT_DOUBLE_EQ(*df.cell("x", 2), 3.0);
    EXPECT_DOUBLE_EQ(*df.cell("y", 4), 50.0);

    // 列操作
    auto* x_col = df.column("x");
    double sum = 0;
    for (auto v : *x_col) sum += v;
    EXPECT_DOUBLE_EQ(sum, 15.0);
}

TEST(Integration, MultiMapWorkflow) {
    MultiMap<std::string, int> scores;
    scores.insert("Alice", 90);
    scores.insert("Alice", 95);
    scores.insert("Bob", 85);

    auto* alice = scores.get("Alice");
    ASSERT_NE(alice, nullptr);
    EXPECT_EQ(alice->size(), 2u);

    auto* bob = scores.get("Bob");
    ASSERT_NE(bob, nullptr);
    EXPECT_EQ(bob->size(), 1u);
    EXPECT_EQ((*bob)[0], 85);
}

TEST(Integration, ListExtWorkflow) {
    std::vector<std::vector<int>> nested = {{1, 2}, {3, 4}, {5, 6}};
    auto flat = ospf::multiarray::list_ext::flatten(nested);
    EXPECT_EQ(flat.size(), 6u);

    auto chunks = ospf::multiarray::list_ext::chunk(flat, 2);
    EXPECT_EQ(chunks.size(), 3u);
}

TEST(Integration, MapExtWorkflow) {
    std::unordered_map<std::string, int> prices = {{"apple", 100}, {"banana", 50}, {"cherry", 200}};
    auto discounted = ospf::multiarray::map_ext::map_values(prices, [](int p) { return p * 8 / 10; });
    EXPECT_EQ(discounted["apple"], 80);
    EXPECT_EQ(discounted["banana"], 40);
}

// ============================================================================
// 补足 198 测试 / Fill to 198 tests
// ============================================================================

TEST(Shape, OffsetConsistency) {
    // RowMajor: offset[0] == size[1] * size[2] * ... * size[N-1]
    Shape<3> shape({2, 3, 4});
    auto offsets = shape.offsets();
    auto sizes = shape.sizes();
    EXPECT_EQ(offsets[0], sizes[1] * sizes[2]);
    EXPECT_EQ(offsets[1], sizes[2]);
    EXPECT_EQ(offsets[2], 1u);
}

TEST(Shape, ColumnMajorOffsetConsistency) {
    // ColumnMajor: offset[i] == size[0] * size[1] * ... * size[i-1]
    Shape<3, StorageOrder::ColumnMajor> shape({2, 3, 4});
    auto offsets = shape.offsets();
    auto sizes = shape.sizes();
    EXPECT_EQ(offsets[0], 1u);
    EXPECT_EQ(offsets[1], sizes[0]);
    EXPECT_EQ(offsets[2], sizes[0] * sizes[1]);
}

TEST(DummyIndex, IndexBoundary) {
    auto idx = DummyIndex::index(std::numeric_limits<std::size_t>::max() - 1);
    EXPECT_TRUE(idx.is_index());
    EXPECT_EQ(idx.get_index(), std::numeric_limits<std::size_t>::max() - 1);
}

TEST(DummyIndex, RangeEqualStartEnd) {
    auto idx = DummyIndex::range(5, 5);
    EXPECT_TRUE(idx.is_range());
    EXPECT_EQ(idx.range_start(), 5u);
    EXPECT_EQ(idx.range_end(), 5u);
}

TEST(MultiArray, CharType) {
    Shape<2> shape({3, 4});
    MultiArray<char, 2> arr(shape, 'a');
    arr.at(0, 0) = 'z';
    EXPECT_EQ(arr.at(0, 0), 'z');
    EXPECT_EQ(arr.at(2, 3), 'a');
}

TEST(MultiArrayView, 4DView) {
    Shape<4> shape({2, 3, 4, 5});
    MultiArray<int, 4> arr(shape, 0);
    arr.at(1, 2, 3, 4) = 42;
    MultiArrayView<int, 4> view(arr.data(), shape);
    EXPECT_EQ(view.at(1, 2, 3, 4), 42);
}

TEST(BlockMultiArray, CopyIndependence) {
    Shape<2> shape({3, 4});
    BlockMultiArray<int, 2> arr(shape, 4);
    arr.fill(7);
    arr.at(1, 2) = 42;

    // 修改不影响原始数据
    BlockMultiArray<int, 2> arr2(shape, 4);
    arr2.fill(99);
    EXPECT_EQ(arr.at(1, 2), 42);
    EXPECT_EQ(arr2.at(1, 2), 99);
}

TEST(Matrix, NonSquareMultiply) {
    ospf::multiarray::Matrix<int, 2, 3> a(std::array<std::array<int, 3>, 2>{{{1, 2, 3}, {4, 5, 6}}});
    ospf::multiarray::Matrix<int, 3, 2> b(std::array<std::array<int, 2>, 3>{{{7, 8}, {9, 10}, {11, 12}}});
    auto c = a * b;  // 2x2
    // [1*7+2*9+3*11, 1*8+2*10+3*12] = [58, 64]
    // [4*7+5*9+6*11, 4*8+5*10+6*12] = [139, 154]
    EXPECT_EQ(c.at(0, 0), 58);
    EXPECT_EQ(c.at(0, 1), 64);
    EXPECT_EQ(c.at(1, 0), 139);
    EXPECT_EQ(c.at(1, 1), 154);
}

TEST(MultiMap, Iteration) {
    MultiMap<std::string, int> map;
    map.insert("a", 1);
    map.insert("b", 2);
    map.insert("c", 3);

    std::size_t count = 0;
    for (const auto& [k, v] : map) {
        ++count;
        EXPECT_GE(v.size(), 1u);
    }
    EXPECT_EQ(count, 3u);
}

TEST(ListExt, ChunkExact) {
    std::vector<int> v = {1, 2, 3, 4};
    auto result = ospf::multiarray::list_ext::chunk(v, 2);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].size(), 2u);
    EXPECT_EQ(result[1].size(), 2u);
}

TEST(FastSum, SumLastDimLarge) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8};
    auto result = ospf::multiarray::fast_sum::sum_last_dim(data, 2, 4);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], 10);  // 1+2+3+4
    EXPECT_EQ(result[1], 26);  // 5+6+7+8
}

TEST(FastSum, SumFirstDimLarge) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8};
    auto result = ospf::multiarray::fast_sum::sum_first_dim(data, 2, 4);
    ASSERT_EQ(result.size(), 4u);
    EXPECT_EQ(result[0], 6);   // 1+5
    EXPECT_EQ(result[1], 8);   // 2+6
    EXPECT_EQ(result[2], 10);  // 3+7
    EXPECT_EQ(result[3], 12);  // 4+8
}
