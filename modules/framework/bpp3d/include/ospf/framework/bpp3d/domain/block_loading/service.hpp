#pragma once
/// Block loading service 聚合头 / Block loading service aggregation header
/// 1:1 对应 Rust bpp3d/domain/block_loading/service.rs

#include <ospf/framework/bpp3d/domain/block_loading/service/simple_block_generator.hpp>
#include <ospf/framework/bpp3d/domain/block_loading/service/complex_block_generator.hpp>
#include <ospf/framework/bpp3d/domain/block_loading/service/depth_first_search_algorithm.hpp>
#include <ospf/framework/bpp3d/domain/block_loading/service/multi_layer_heuristic_search_algorithm.hpp>
