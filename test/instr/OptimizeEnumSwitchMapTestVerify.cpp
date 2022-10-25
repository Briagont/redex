/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Debug.h"
#include "DexInstruction.h"
#include "Show.h"
#include "verify/OptimizeEnumCommon.h"
#include "verify/VerifyUtil.h"

namespace {

constexpr const char* FOO = "Lcom/facebook/redextest/Foo;";
constexpr const char* FOO_ANONYMOUS = "Lcom/facebook/redextest/Foo$1;";
constexpr const char* ENUM_A = "Lcom/facebook/redextest/EnumA;";
constexpr const char* ENUM_B = "Lcom/facebook/redextest/EnumB;";
constexpr const char* BIG_ENUM = "Lcom/facebook/redextest/BigEnum;";

} // namespace

TEST_F(PreVerify, JavaGeneratedClass) {
  auto enumA = find_class_named(classes, ENUM_A);
  EXPECT_NE(nullptr, enumA);

  auto enumB = find_class_named(classes, ENUM_B);
  EXPECT_NE(nullptr, enumB);

  auto bigEnum = find_class_named(classes, BIG_ENUM);
  EXPECT_NE(nullptr, bigEnum);

  auto foo = find_class_named(classes, FOO);
  EXPECT_NE(nullptr, foo);

  auto foo_anonymous = find_class_named(classes, FOO_ANONYMOUS);
  EXPECT_NE(nullptr, foo_anonymous);

  auto method_use_enumA = DexMethod::get_method(
      "Lcom/facebook/redextest/Foo;.useEnumA:(Lcom/facebook/redextest/"
      "EnumA;)I");
  auto switch_cases_A = collect_const_branch_cases(method_use_enumA);
  std::set<BranchCase> expected_switch_cases_A = {{BranchSource::ArrayGet, 1},
                                                  {BranchSource::ArrayGet, 2}};
  EXPECT_EQ(expected_switch_cases_A, switch_cases_A);

  auto method_use_enumB = DexMethod::get_method(
      "Lcom/facebook/redextest/Foo;.useEnumB:(Lcom/facebook/redextest/"
      "EnumB;)I");
  auto switch_cases_B = collect_const_branch_cases(method_use_enumB);
  std::set<BranchCase> expected_switch_cases_B = {{BranchSource::ArrayGet, 1},
                                                  {BranchSource::ArrayGet, 2}};
  EXPECT_EQ(expected_switch_cases_B, switch_cases_B);

  auto method_use_enumA_again = DexMethod::get_method(
      "Lcom/facebook/redextest/Foo;.useEnumA_again:(Lcom/facebook/redextest/"
      "EnumA;)I");
  auto switch_cases_A_again =
      collect_const_branch_cases(method_use_enumA_again);
  std::set<BranchCase> expected_switch_cases_A_again = {
      {BranchSource::ArrayGet, 1}, {BranchSource::ArrayGet, 3}};
}

TEST_F(PostVerify, JavaGeneratedClass) {
  auto enumA = find_class_named(classes, ENUM_A);
  EXPECT_NE(nullptr, enumA);

  auto enumB = find_class_named(classes, ENUM_B);
  EXPECT_NE(nullptr, enumB);

  auto bigEnum = find_class_named(classes, BIG_ENUM);
  EXPECT_NE(nullptr, bigEnum);

  auto foo = find_class_named(classes, FOO);
  EXPECT_NE(nullptr, foo);

  auto foo_anonymous = find_class_named(classes, FOO_ANONYMOUS);
  EXPECT_EQ(nullptr, foo_anonymous);

  auto method_use_enumA = DexMethod::get_method(
      "Lcom/facebook/redextest/Foo;.useEnumA:(Lcom/facebook/redextest/"
      "EnumA;)I");
  auto switch_cases_A = collect_const_branch_cases(method_use_enumA);
  std::set<BranchCase> expected_switch_cases_A = {
      {BranchSource::VirtualCall, 0},
      {BranchSource::VirtualCall, 1},
      {BranchSource::VirtualCall, 2}};
  EXPECT_EQ(expected_switch_cases_A, switch_cases_A);

  auto method_use_enumB = DexMethod::get_method(
      "Lcom/facebook/redextest/Foo;.useEnumB:(Lcom/facebook/redextest/"
      "EnumB;)I");
  auto switch_cases_B = collect_const_branch_cases(method_use_enumB);
  std::set<BranchCase> expected_switch_cases_B = {
      {BranchSource::VirtualCall, 0},
      {BranchSource::VirtualCall, 1},
      {BranchSource::VirtualCall, 2}};
  EXPECT_EQ(expected_switch_cases_B, switch_cases_B);

  auto method_use_enumA_again = DexMethod::get_method(
      "Lcom/facebook/redextest/Foo;.useEnumA_again:(Lcom/facebook/redextest/"
      "EnumA;)I");
  auto switch_cases_A_again =
      collect_const_branch_cases(method_use_enumA_again);
  std::set<BranchCase> expected_switch_cases_A_again = {
      {BranchSource::VirtualCall, 0}, {BranchSource::VirtualCall, 1}};
  EXPECT_EQ(expected_switch_cases_A_again, switch_cases_A_again);
}
