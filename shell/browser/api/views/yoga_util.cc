// Copyright (c) 2020 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "shell/browser/api/views/yoga_util.h"

#include <algorithm>
#include <map>
#include <tuple>
#include <utility>

#include "base/strings/string_number_conversions.h"
#include "base/strings/string_util.h"
#include "third_party/yoga/yoga/Yoga.h"

namespace electron {

namespace {

// Converters to convert string to integer.
using IntConverter = bool (*)(const std::string&, int*);

bool AlignValue(const std::string& value, int* out) {
  if (value == "auto")
    *out = static_cast<int>(YGAlignAuto);
  else if (value == "base-line")
    *out = static_cast<int>(YGAlignBaseline);
  else if (value == "space-between")
    *out = static_cast<int>(YGAlignSpaceBetween);
  else if (value == "space-around")
    *out = static_cast<int>(YGAlignSpaceAround);
  else if (value == "center")
    *out = static_cast<int>(YGAlignCenter);
  else if (value == "stretch")
    *out = static_cast<int>(YGAlignStretch);
  else if (value == "flex-start")
    *out = static_cast<int>(YGAlignFlexStart);
  else if (value == "flex-end")
    *out = static_cast<int>(YGAlignFlexEnd);
  else
    return false;
  return true;
}

bool DirectionValue(const std::string& value, int* out) {
  if (value == "ltr")
    *out = static_cast<int>(YGDirectionLTR);
  else if (value == "rtl")
    *out = static_cast<int>(YGDirectionRTL);
  else if (value == "inherit")
    *out = static_cast<int>(YGDirectionInherit);
  else
    return false;
  return true;
}

bool DisplayValue(const std::string& value, int* out) {
  if (value == "flex")
    *out = static_cast<int>(YGDisplayFlex);
  else if (value == "none")
    *out = static_cast<int>(YGDisplayNone);
  else
    return false;
  return true;
}

bool FlexDirectionValue(const std::string& value, int* out) {
  if (value == "row")
    *out = static_cast<int>(YGFlexDirectionRow);
  else if (value == "row-reverse")
    *out = static_cast<int>(YGFlexDirectionRowReverse);
  else if (value == "column")
    *out = static_cast<int>(YGFlexDirectionColumn);
  else if (value == "column-reverse")
    *out = static_cast<int>(YGFlexDirectionColumnReverse);
  else
    return false;
  return true;
}

bool JustifyValue(const std::string& value, int* out) {
  if (value == "center")
    *out = static_cast<int>(YGJustifyCenter);
  else if (value == "space-around")
    *out = static_cast<int>(YGJustifySpaceAround);
  else if (value == "space-between")
    *out = static_cast<int>(YGJustifySpaceBetween);
  else if (value == "space-evenly")
    *out = static_cast<int>(YGJustifySpaceEvenly);
  else if (value == "flex-start")
    *out = static_cast<int>(YGJustifyFlexStart);
  else if (value == "flex-end")
    *out = static_cast<int>(YGJustifyFlexEnd);
  else
    return false;
  return true;
}

bool OverflowValue(const std::string& value, int* out) {
  if (value == "visible")
    *out = static_cast<int>(YGOverflowVisible);
  else if (value == "hidden")
    *out = static_cast<int>(YGOverflowHidden);
  else
    return false;
  return true;
}

bool PositionValue(const std::string& value, int* out) {
  if (value == "absolute")
    *out = static_cast<int>(YGPositionTypeAbsolute);
  else if (value == "relative")
    *out = static_cast<int>(YGPositionTypeRelative);
  else
    return false;
  return true;
}

bool WrapValue(const std::string& value, int* out) {
  if (value == "wrap")
    *out = static_cast<int>(YGWrapWrap);
  else if (value == "nowrap")
    *out = static_cast<int>(YGWrapNoWrap);
  else if (value == "wrap-reverse")
    *out = static_cast<int>(YGWrapWrapReverse);
  else
    return false;
  return true;
}

// Convert the value to pixel value.
float PixelValue(std::string value) {
  if (base::EndsWith(value, "px", base::CompareCase::SENSITIVE))
    value = value.substr(0, value.length() - 2);
  double out;
  if (!base::StringToDouble(value, &out)) {
    LOG(WARNING) << "Invalid pixel value: " << value;
    return 0;
  }
  return out;
}

// Convert the value to percent value.
int PercentValue(const std::string& value) {
  int integer;
  if (!base::StringToInt(value.substr(0, value.length() - 1), &integer)) {
    LOG(WARNING) << "Invalid percent value: " << value;
    return 0;
  }
  return integer;
}

// We use int to represent enums.
using IntSetter = void (*)(const YGNodeRef, int);
using FloatSetter = void (*)(const YGNodeRef, float);
using AutoSetter = void (*)(const YGNodeRef);
using EdgeSetter = void (*)(const YGNodeRef, const YGEdge, float);

// Sorted list of CSS node properties.
const std::tuple<const char*, IntConverter, IntSetter> int_setters[] = {
    {"alignContent", AlignValue,
     reinterpret_cast<IntSetter>(YGNodeStyleSetAlignContent)},
    {"alignItems", AlignValue,
     reinterpret_cast<IntSetter>(YGNodeStyleSetAlignItems)},
    {"alignSelf", AlignValue,
     reinterpret_cast<IntSetter>(YGNodeStyleSetAlignSelf)},
    {"direction", DirectionValue,
     reinterpret_cast<IntSetter>(YGNodeStyleSetDirection)},
    {"display", DisplayValue,
     reinterpret_cast<IntSetter>(YGNodeStyleSetDisplay)},
    {"flexDirection", FlexDirectionValue,
     reinterpret_cast<IntSetter>(YGNodeStyleSetFlexDirection)},
    {"flexWrap", WrapValue,
     reinterpret_cast<IntSetter>(YGNodeStyleSetFlexWrap)},
    {"justifyContent", JustifyValue,
     reinterpret_cast<IntSetter>(YGNodeStyleSetJustifyContent)},
    {"overflow", OverflowValue,
     reinterpret_cast<IntSetter>(YGNodeStyleSetOverflow)},
    {"position", PositionValue,
     reinterpret_cast<IntSetter>(YGNodeStyleSetPositionType)},
};
const std::pair<const char*, FloatSetter> float_setters[] = {
    {"aspectRatio", YGNodeStyleSetAspectRatio},
    {"flex", YGNodeStyleSetFlex},
    {"flexBasis", YGNodeStyleSetFlexBasis},
    {"flexGrow", YGNodeStyleSetFlexGrow},
    {"flexShrink", YGNodeStyleSetFlexShrink},
    {"height", YGNodeStyleSetHeight},
    {"maxHeight", YGNodeStyleSetMaxHeight},
    {"maxWidth", YGNodeStyleSetMaxWidth},
    {"minHeight", YGNodeStyleSetMinHeight},
    {"minWidth", YGNodeStyleSetMinWidth},
    {"width", YGNodeStyleSetWidth},
};
const std::pair<const char*, AutoSetter> auto_setters[] = {
    {"flexBasis", YGNodeStyleSetFlexBasisAuto},
    {"height", YGNodeStyleSetHeightAuto},
    {"width", YGNodeStyleSetWidthAuto},
};
const std::pair<const char*, FloatSetter> percent_setters[] = {
    {"flexBasis", YGNodeStyleSetFlexBasisPercent},
    {"height", YGNodeStyleSetHeightPercent},
    {"maxHeight", YGNodeStyleSetMaxHeightPercent},
    {"maxWidth", YGNodeStyleSetMaxWidthPercent},
    {"minHeight", YGNodeStyleSetMinHeightPercent},
    {"minWidth", YGNodeStyleSetMinWidthPercent},
    {"width", YGNodeStyleSetWidthPercent},
};
const std::tuple<const char*, YGEdge, EdgeSetter> edge_setters[] = {
    {"border", YGEdgeAll, YGNodeStyleSetBorder},
    {"borderBottom", YGEdgeBottom, YGNodeStyleSetBorder},
    {"borderLeft", YGEdgeLeft, YGNodeStyleSetBorder},
    {"borderRight", YGEdgeRight, YGNodeStyleSetBorder},
    {"borderTop", YGEdgeTop, YGNodeStyleSetBorder},
    {"bottom", YGEdgeBottom, YGNodeStyleSetPosition},
    {"left", YGEdgeLeft, YGNodeStyleSetPosition},
    {"margin", YGEdgeAll, YGNodeStyleSetMargin},
    {"marginBottom", YGEdgeBottom, YGNodeStyleSetMargin},
    {"marginLeft", YGEdgeLeft, YGNodeStyleSetMargin},
    {"marginRight", YGEdgeRight, YGNodeStyleSetMargin},
    {"marginTop", YGEdgeTop, YGNodeStyleSetMargin},
    {"padding", YGEdgeAll, YGNodeStyleSetPadding},
    {"paddingBottom", YGEdgeBottom, YGNodeStyleSetPadding},
    {"paddingLeft", YGEdgeLeft, YGNodeStyleSetPadding},
    {"paddingRight", YGEdgeRight, YGNodeStyleSetPadding},
    {"paddingTop", YGEdgeTop, YGNodeStyleSetPadding},
    {"right", YGEdgeRight, YGNodeStyleSetPosition},
    {"top", YGEdgeTop, YGNodeStyleSetPosition},
};
const std::tuple<const char*, YGEdge, EdgeSetter> edge_percent_setters[] = {
    {"bottom", YGEdgeBottom, YGNodeStyleSetPositionPercent},
    {"left", YGEdgeLeft, YGNodeStyleSetPositionPercent},
    {"margin", YGEdgeAll, YGNodeStyleSetMarginPercent},
    {"marginBottom", YGEdgeBottom, YGNodeStyleSetMarginPercent},
    {"marginLeft", YGEdgeLeft, YGNodeStyleSetMarginPercent},
    {"marginRight", YGEdgeRight, YGNodeStyleSetMarginPercent},
    {"marginTop", YGEdgeTop, YGNodeStyleSetMarginPercent},
    {"padding", YGEdgeAll, YGNodeStyleSetPaddingPercent},
    {"paddingBottom", YGEdgeBottom, YGNodeStyleSetPaddingPercent},
    {"paddingLeft", YGEdgeLeft, YGNodeStyleSetPaddingPercent},
    {"paddingRight", YGEdgeRight, YGNodeStyleSetPaddingPercent},
    {"paddingTop", YGEdgeTop, YGNodeStyleSetPaddingPercent},
    {"right", YGEdgeRight, YGNodeStyleSetPositionPercent},
    {"top", YGEdgeTop, YGNodeStyleSetPositionPercent},
};

// Compare function to compare elements.
template <typename T>
bool ElementCompare(const T& e1, const T& e2) {
  return base::StringPiece(std::get<0>(e1)) < std::get<0>(e2);
}

// Check if the array is sorted.
template <typename T, size_t n>
bool IsSorted(T (&setters)[n]) {
  return std::is_sorted(std::begin(setters), std::end(setters),
                        ElementCompare<T>);
}

// Compare function to compare between elements and keys.
template <typename T>
bool FirstCompare(const T& element, const std::string& key) {
  return std::get<0>(element) < key;
}

// Find out the setter from array.
template <typename T, size_t n>
T* Find(T (&setters)[n], const std::string& name) {
  auto iter = std::lower_bound(std::begin(setters), std::end(setters), name,
                               FirstCompare<T>);
  if (iter == std::end(setters) || name != std::get<0>(*iter))
    return nullptr;
  return &(*iter);
}

// Set style for int properties.
bool SetIntStyle(YGNodeRef node,
                 const std::string& name,
                 const std::string& value) {
  auto* tup = Find(int_setters, name);
  if (!tup)
    return false;
  int converted;
  if (!std::get<1>(*tup)(value, &converted)) {
    LOG(WARNING) << "Invalid value " << value << " for property " << name;
    return false;
  }
  std::get<2> (*tup)(node, converted);
  return true;
}

// Set style for float properties.
bool SetFloatStyle(YGNodeRef node, const std::string& name, float value) {
  auto* tup = Find(float_setters, name);
  if (!tup)
    return false;
  std::get<1> (*tup)(node, value);
  return true;
}

// Set "auto" property for styles.
bool SetAutoStyle(YGNodeRef node, const std::string& name) {
  auto* tup = Find(auto_setters, name);
  if (!tup)
    return false;
  std::get<1> (*tup)(node);
  return true;
}

// Dispatch to float for auto depending on the value.
bool SetUnitStyle(YGNodeRef node,
                  const std::string& name,
                  const std::string& value) {
  if (value == "auto")
    return SetAutoStyle(node, name);
  else
    return SetFloatStyle(node, name, PixelValue(value));
}

// Set style for percent properties.
bool SetPercentStyle(YGNodeRef node,
                     const std::string& name,
                     const std::string& value) {
  auto* tup = Find(percent_setters, name);
  if (!tup)
    return false;
  std::get<1> (*tup)(node, PercentValue(value));
  return true;
}

// Set style for edge properties.
bool SetEdgeStyle(YGNodeRef node, const std::string& name, float value) {
  auto* tup = Find(edge_setters, name);
  if (!tup)
    return false;
  std::get<2> (*tup)(node, std::get<1>(*tup), value);
  return true;
}

bool SetEdgeStyle(YGNodeRef node,
                  const std::string& name,
                  const std::string& value) {
  return SetEdgeStyle(node, name, PixelValue(value));
}

// Set style for edge percent properties.
bool SetEdgePercentStyle(YGNodeRef node,
                         const std::string& name,
                         const std::string& value) {
  auto* tup = Find(edge_percent_setters, name);
  if (!tup)
    return false;
  std::get<2> (*tup)(node, std::get<1>(*tup), PercentValue(value));
  return true;
}

// Check whether the value is xx%.
bool IsPercentValue(const std::string& value) {
  if (value.size() < 2 || value.size() > 4)
    return false;
  return value.back() == '%';
}

}  // namespace

void SetYogaProperty(YGNodeRef node, const std::string& name, float value) {
  SetFloatStyle(node, name, value) || SetEdgeStyle(node, name, value);
}

void SetYogaProperty(YGNodeRef node,
                     const std::string& name,
                     const std::string& value) {
  DCHECK(IsSorted(int_setters) && IsSorted(float_setters) &&
         IsSorted(auto_setters) && IsSorted(percent_setters) &&
         IsSorted(edge_setters) && IsSorted(edge_percent_setters))
      << "Property setters must be sorted";
  if (IsPercentValue(value)) {
    SetPercentStyle(node, name, value) ||
        SetEdgePercentStyle(node, name, value);
  } else {
    SetIntStyle(node, name, value) || SetUnitStyle(node, name, value) ||
        SetEdgeStyle(node, name, value);
  }
}

namespace {

std::map<const views::View*, YGNodeRef> g_yoga_nodes;

}  // namespace

void AttachYogaNode(views::View* view, YGNodeRef node) {
  g_yoga_nodes[view] = node;
}

YGNodeRef GetYogaNode(const views::View* view) {
  auto it = g_yoga_nodes.find(view);
  if (it != g_yoga_nodes.end())
    return it->second;
  else
    return nullptr;
}

}  // namespace electron
